#include "StdAfx.h"
#include "DataTreeXML.h"
#include "tinyxml2.h"

using namespace tinyxml2;

CDataTreeXML::CDataTreeXML(IDataTree::EAccessMode _eMode)
    : xmlDocument(NULL)
    , eMode(_eMode)
    , xmlCurrNode(NULL)
    , xmlCurrElement(NULL)
{
}

CDataTreeXML::~CDataTreeXML()
{
    try
    {
        FinishChunk();

        if (pStream && xmlDocument && !IsReading())
        {
            // Save XML document to stream
            XMLPrinter printer;
            xmlDocument->Print(&printer);

            const char* pXML = printer.CStr();
            int nSize = strlen(pXML);

            pStream->Seek(0, STREAM_SEEK_SET);
            pStream->Write((void*)pXML, nSize);
        }
    }
    catch (...)
    {
    }

    if (xmlDocument)
        delete xmlDocument;
}

bool CDataTreeXML::Open(IDataStream* _pStream, const char* idBaseNode)
{
    pStream = _pStream;
    xmlDocument = new tinyxml2::XMLDocument();

    if (IsReading())
    {
        if (!pStream)
            return false;

        // Read entire stream into memory
        int nSize = pStream->GetSize();
        if (nSize <= 0)
            return false;

        char* pBuffer = new char[nSize + 1];
        pStream->Read(pBuffer, nSize);
        pBuffer[nSize] = '\0';

        // Parse XML
        XMLError err = xmlDocument->Parse(pBuffer, nSize);
        delete[] pBuffer;

        if (err != XML_SUCCESS)
            return false;

        nodes.push_back(xmlDocument->RootElement());
        xmlCurrNode = nodes.back();

        return StartChunk(idBaseNode);
    }
    else
    {
        // Add XML declaration
        XMLDeclaration* pDecl = xmlDocument->NewDeclaration("xml version=\"1.0\"");
        xmlDocument->InsertFirstChild(pDecl);

        return StartChunk(idBaseNode);
    }
}

int CDataTreeXML::StartChunk(const char* idChunk)
{
    if (idChunk[0] == '\0')
        return -1;

    if (IsReading())
    {
        if (xmlCurrNode)
        {
            nodes.push_back(xmlCurrNode);
        }

        if (!xmlCurrNode)
            return 0;

        // Find child element
        XMLElement* pElement = NULL;

        if (xmlCurrNode->ToElement())
        {
            pElement = xmlCurrNode->ToElement()->FirstChildElement(idChunk);
        }

        if (!pElement)
        {
            FinishChunk();
            return 0;
        }

        xmlCurrNode = pElement;
        return 1;
    }
    else
    {
        if (xmlCurrElement)
        {
            elements.push_back(xmlCurrElement);
        }

        XMLElement* pElement = xmlDocument->NewElement(idChunk);

        if (elements.empty())
            xmlDocument->InsertEndChild(pElement);
        else
            elements.back()->InsertEndChild(pElement);

        xmlCurrElement = pElement;

        return xmlCurrElement ? 1 : 0;
    }
}

void CDataTreeXML::FinishChunk()
{
    if (IsReading())
    {
        if (nodes.empty())
            xmlCurrNode = NULL;
        else
        {
            xmlCurrNode = nodes.back();
            nodes.pop_back();
        }
    }
    else
    {
        if (elements.empty())
            xmlCurrElement = NULL;
        else
        {
            xmlCurrElement = elements.back();
            elements.pop_back();
        }
    }
}

int CDataTreeXML::StartContainerChunk(const char* idChunk)
{
    const char* szChunkName = (idChunk[0] == '\0') ? "data" : idChunk;

    if (IsReading())
    {
        if (!xmlCurrNode || !xmlCurrNode->ToElement())
            return 0;

        XMLElement* pParent = xmlCurrNode->ToElement();
        XMLElement* pContainer = pParent->FirstChildElement(szChunkName);

        if (!pContainer)
            return 0;

        nodes.push_back(xmlCurrNode);

        SNodesList nodeList;
        nodeList.parentElement = pContainer;

        // Collect all item elements
        for (XMLElement* pItem = pContainer->FirstChildElement("item");
            pItem;
            pItem = pItem->NextSiblingElement("item"))
        {
            nodeList.items.push_back(pItem);
        }

        nodelists.push_back(nodeList);
        nodelists.back().nCurrElement = -1;

        return 1;
    }
    else
    {
        if (xmlCurrElement)
        {
            elements.push_back(xmlCurrElement);
        }

        XMLElement* pArrayBase = xmlDocument->NewElement(szChunkName);
        arrbases.push_back(pArrayBase);

        if (xmlCurrElement)
            xmlCurrElement->InsertEndChild(pArrayBase);
        else
            xmlDocument->InsertEndChild(pArrayBase);

        return pArrayBase ? 1 : 0;
    }
}

void CDataTreeXML::FinishContainerChunk()
{
    FinishChunk();

    if (IsReading())
        nodelists.pop_back();
    else
        arrbases.pop_back();
}

bool CDataTreeXML::SetChunkCounter(int nCount)
{
    if (IsReading())
    {
        if (nodelists.empty())
            return false;

        SNodesList& list = nodelists.back();
        list.nCurrElement = nCount;

        if (list.nCurrElement >= 0 && list.nCurrElement < (int)list.items.size())
            xmlCurrNode = list.items[list.nCurrElement];
        else
            xmlCurrNode = NULL;

        return xmlCurrNode != NULL;
    }
    else
    {
        if (arrbases.empty())
            return false;

        XMLElement* pItem = xmlDocument->NewElement("item");
        arrbases.back()->InsertEndChild(pItem);
        xmlCurrElement = pItem;

        return xmlCurrElement != NULL;
    }
}

int CDataTreeXML::CountChunks(const char* idChunk)
{
    if (IsReading())
    {
        if (!xmlCurrNode || !xmlCurrNode->ToElement())
            return 0;

        const char* szChunkName = (idChunk[0] == '\0') ? "data" : idChunk;
        XMLElement* pParent = xmlCurrNode->ToElement();
        XMLElement* pContainer = pParent->FirstChildElement(szChunkName);

        if (!pContainer)
            return 0;

        int nCount = 0;
        for (XMLElement* pItem = pContainer->FirstChildElement("item");
            pItem;
            pItem = pItem->NextSiblingElement("item"))
        {
            nCount++;
        }

        return nCount;
    }

    return 0;
}

const XMLAttribute* CDataTreeXML::GetAttribute(const char* idChunk)
{
    if (!xmlCurrNode || !xmlCurrNode->ToElement())
        return NULL;

    return xmlCurrNode->ToElement()->FindAttribute(idChunk);
}

XMLNode* CDataTreeXML::GetTextNode(const char* idChunk)
{
    if (!xmlCurrNode || !xmlCurrNode->ToElement())
        return NULL;

    XMLElement* pElement = xmlCurrNode->ToElement();

    // Try to find attribute
    const XMLAttribute* pAttr = pElement->FindAttribute(idChunk);
    if (pAttr)
    {
        // Create a dummy node to return
        return xmlCurrNode;
    }

    // Try to find child element
    XMLElement* pChild = pElement->FirstChildElement(idChunk);
    if (pChild)
        return pChild;

    return NULL;
}

int CDataTreeXML::GetChunkSize()
{
    if (IsReading() && xmlCurrNode && xmlCurrNode->ToElement())
    {
        const char* pText = xmlCurrNode->ToElement()->GetText();
        if (pText)
            return strlen(pText);
    }
    return 0;
}

bool CDataTreeXML::RawData(void* pData, int nSize)
{
    if (IsReading())
    {
        int nStrSize = GetChunkSize();
        if (nStrSize <= 0)
            return false;

        char* pBuffer = new char[nStrSize + 1];
        if (!StringData(pBuffer))
        {
            delete[] pBuffer;
            return false;
        }

        int nCheck = 0;
        NStr::StringToBin(pBuffer, pData, &nCheck);
        delete[] pBuffer;

        return (nCheck == nSize);
    }
    else
    {
        std::string szBuffer;
        szBuffer.resize(nSize * 2 + 1);
        NStr::BinToString(pData, nSize, const_cast<char*>(szBuffer.c_str()));
        return StringData(const_cast<char*>(szBuffer.c_str()));
    }
}

bool CDataTreeXML::StringData(char* pData)
{
    if (IsReading())
    {
        if (xmlCurrNode && xmlCurrNode->ToElement())
        {
            const char* pText = xmlCurrNode->ToElement()->GetText();
            if (pText)
            {
                strcpy(pData, pText);
                return true;
            }
        }
        return false;
    }
    else
    {
        if (xmlCurrElement)
        {
            XMLText* pText = xmlDocument->NewText(pData);
            xmlCurrElement->InsertEndChild(pText);
            return true;
        }
        return false;
    }
}

bool CDataTreeXML::StringData(WORD* pData)
{
    if (IsReading())
    {
        if (xmlCurrNode && xmlCurrNode->ToElement())
        {
            const char* pText = xmlCurrNode->ToElement()->GetText();
            if (pText)
            {
                // Convert char to wide char (simplified)
                for (int i = 0; pText[i] && i < 1024; i++)
                    pData[i] = pText[i];
                return true;
            }
        }
        return false;
    }
    else
    {
        if (xmlCurrElement)
        {
            // Convert wide char to char (simplified)
            char szBuffer[1024];
            for (int i = 0; pData[i] && i < 1023; i++)
                szBuffer[i] = (char)pData[i];
            szBuffer[1023] = '\0';

            XMLText* pText = xmlDocument->NewText(szBuffer);
            xmlCurrElement->InsertEndChild(pText);
            return true;
        }
        return false;
    }
}

bool CDataTreeXML::DataChunk(const char* idChunk, int* pData)
{
    if (IsReading())
    {
        const XMLAttribute* pAttr = GetAttribute(idChunk);
        if (pAttr && pAttr->Value())
        {
            sscanf(pAttr->Value(), "%d", pData);
            return true;
        }
        return false;
    }
    else
    {
        if (xmlCurrElement)
        {
            char buffer[64];
            sprintf(buffer, "%d", *pData);
            xmlCurrElement->SetAttribute(idChunk, buffer);
            return true;
        }
        return false;
    }
}

bool CDataTreeXML::DataChunk(const char* idChunk, double* pData)
{
    if (IsReading())
    {
        const XMLAttribute* pAttr = GetAttribute(idChunk);
        if (pAttr && pAttr->Value())
        {
            sscanf(pAttr->Value(), "%lf", pData);
            return true;
        }
        return false;
    }
    else
    {
        if (xmlCurrElement)
        {
            char buffer[64];
            sprintf(buffer, "%g", *pData);
            xmlCurrElement->SetAttribute(idChunk, buffer);
            return true;
        }
        return false;
    }
}