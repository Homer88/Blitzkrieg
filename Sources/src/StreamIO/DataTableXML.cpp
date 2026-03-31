#include <algorithm>
#include "StdAfx.h"
#include "DataTableXML.h"
#include "tinyxml2.h"

using namespace tinyxml2;


// Helper function to add name to buffer
static int AddToBuffer(const std::string& szName, char*& pszBuffer, int nBufferSize, int nTotalSize)
{
    int nLen = szName.size();
    if (nTotalSize + nLen + 2 >= nBufferSize)
        return 0;

    strcpy(pszBuffer, szName.c_str());
    pszBuffer += nLen;
    *pszBuffer++ = '\0';
    return nLen + 1;
}

CDataTableXML::CDataTableXML()
    : xmlDocument(NULL)
    , xmlRootNode(NULL)
    , bModified(false)
{
}

CDataTableXML::~CDataTableXML()
{
    try
    {
        if (bModified && pStream.GetPtr() && xmlDocument && xmlRootNode)
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

bool CDataTableXML::Open(IDataStream* _pStream, const char* pszBaseNode)
{
    try
    {
        pStream = _pStream;

        // Read entire stream into memory
        int nSize = pStream->GetSize();
        if (nSize <= 0)
            return false;

        char* pBuffer = new char[nSize + 1];
        pStream->Read(pBuffer, nSize);
        pBuffer[nSize] = '\0';

        // Create XML document
        xmlDocument = new tinyxml2::XMLDocument();
        XMLError err = xmlDocument->Parse(pBuffer, nSize);
        delete[] pBuffer;

        if (err != XML_SUCCESS)
        {
            // Create new document if parsing fails
            delete xmlDocument;
            xmlDocument = new tinyxml2::XMLDocument();

            // Add XML declaration
            XMLDeclaration* pDecl = xmlDocument->NewDeclaration("xml version=\"1.0\"");
            xmlDocument->InsertFirstChild(pDecl);

            // Create root element
            xmlRootNode = xmlDocument->NewElement(pszBaseNode);
            xmlDocument->InsertEndChild(xmlRootNode);
        }
        else
        {
            // Find root node
            xmlRootNode = xmlDocument->FirstChildElement(pszBaseNode);
            if (!xmlRootNode)
                xmlRootNode = xmlDocument->RootElement();
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string CDataTableXML::MakeName(const char* pszRow, const char* pszEntry)
{
    std::string szName = std::string(pszRow) + "/" + std::string(pszEntry);
    // Replace '.' with '/' in the path
    for (size_t i = 0; i < szName.size(); ++i)
    {
        if (szName[i] == '.')
            szName[i] = '/';
    }
    return szName;
}

XMLElement* CDataTableXML::GetNode(const std::string& szName)
{
    if (!xmlRootNode)
        return NULL;

    XMLElement* pCurrent = xmlRootNode;
    size_t start = 0;
    size_t end = szName.find('/');

    // Navigate through path
    while (end != std::string::npos)
    {
        std::string tag = szName.substr(start, end - start);
        XMLElement* pChild = pCurrent->FirstChildElement(tag.c_str());
        if (!pChild)
            return NULL;
        pCurrent = pChild;
        start = end + 1;
        end = szName.find('/', start);
    }

    // Last part - the entry name
    std::string entry = szName.substr(start);

    // Try to find attribute first
    const XMLAttribute* pAttr = pCurrent->FindAttribute(entry.c_str());
    if (pAttr)
    {
        // Return as element wrapper (hack: create temporary element)
        // For simplicity, we'll return the parent element and handle specially
        return pCurrent;
    }

    // Then try child element
    return pCurrent->FirstChildElement(entry.c_str());
}

XMLNode* CDataTableXML::GetTextNode(const char* pszRow, const char* pszEntry)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);
    return pElement;
}

int CDataTableXML::GetRowNames(char* pszBuffer, int nBufferSize)
{
    if (!xmlRootNode || !pszBuffer || nBufferSize <= 0)
        return 0;

    try
    {
        int nTotalSize = 0;
        char* pCurrent = pszBuffer;

        for (XMLElement* pChild = xmlRootNode->FirstChildElement();
            pChild;
            pChild = pChild->NextSiblingElement())
        {
            std::string szName = pChild->Name();
            int nAdded = AddToBuffer(szName, pCurrent, nBufferSize, nTotalSize);
            if (nAdded == 0)
                break;
            nTotalSize += nAdded;
        }

        // Add double null termination
        if (nTotalSize + 1 <= nBufferSize)
        {
            *pCurrent++ = '\0';
            nTotalSize++;
        }

        return nTotalSize;
    }
    catch (...)
    {
        return 0;
    }
}

int CDataTableXML::GetEntryNames(const char* pszRow, char* pszBuffer, int nBufferSize)
{
    if (!xmlRootNode || !pszBuffer || nBufferSize <= 0)
        return 0;

    try
    {
        // Find row element
        std::string rowPath = pszRow;
        XMLElement* pRowElement = GetNode(rowPath);
        if (!pRowElement)
            return 0;

        int nTotalSize = 0;
        char* pCurrent = pszBuffer;

        // Process attributes
        for (const XMLAttribute* pAttr = pRowElement->FirstAttribute();
            pAttr;
            pAttr = pAttr->Next())
        {
            std::string szName = pAttr->Name();
            int nAdded = AddToBuffer(szName, pCurrent, nBufferSize, nTotalSize);
            if (nAdded == 0)
                break;
            nTotalSize += nAdded;
        }

        // Process child elements
        for (XMLElement* pChild = pRowElement->FirstChildElement();
            pChild;
            pChild = pChild->NextSiblingElement())
        {
            std::string szName = pChild->Name();

            // Check if this element has children
            bool hasChildren = (pChild->FirstChildElement() != NULL);

            if (hasChildren)
            {
                // Recursively get nested entry names
                std::string fullPath = rowPath + "/" + szName;
                char buffer[65536];
                int nSize = GetEntryNames(fullPath.c_str(), buffer, 65536);

                if (nSize > 1)
                {
                    char* pos = buffer;   // ← исправлено
                    while (*pos != 0 && (pos - buffer <= nSize))
                    {
                        std::string szNewName = szName + "/" + pos;
                        // Replace '/' with '.' in the final name
                        for (size_t i = 0; i < szNewName.size(); ++i)
                        {
                            if (szNewName[i] == '/')
                                szNewName[i] = '.';
                        }
                        int nAdded = AddToBuffer(szNewName, pCurrent, nBufferSize, nTotalSize);
                        if (nAdded == 0) break;
                        nTotalSize += nAdded;
                        pos = std::find(pos, buffer + nSize, '\0') + 1;
                    }
                }
            }
            else
            {
                int nAdded = AddToBuffer(szName, pCurrent, nBufferSize, nTotalSize);
                if (nAdded == 0)
                    break;
                nTotalSize += nAdded;
            }
        }

        // Add double null termination
        if (nTotalSize + 1 <= nBufferSize)
        {
            *pCurrent++ = '\0';
            nTotalSize++;
        }

        return nTotalSize;
    }
    catch (...)
    {
        return 0;
    }
}

int CDataTableXML::GetInt(const char* pszRow, const char* pszEntry, int defval)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement)
        return defval;

    // Check if it's an attribute
    const XMLAttribute* pAttr = pElement->FindAttribute(pszEntry);
    if (pAttr)
        return pAttr->IntValue();

    // Check if it's a child element
    XMLElement* pChild = pElement->FirstChildElement(pszEntry);
    if (pChild && pChild->GetText())
        return atoi(pChild->GetText());

    return defval;
}

double CDataTableXML::GetDouble(const char* pszRow, const char* pszEntry, double defval)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement)
        return defval;

    // Check if it's an attribute
    const XMLAttribute* pAttr = pElement->FindAttribute(pszEntry);
    if (pAttr)
        return pAttr->DoubleValue();

    // Check if it's a child element
    XMLElement* pChild = pElement->FirstChildElement(pszEntry);
    if (pChild && pChild->GetText())
        return atof(pChild->GetText());

    return defval;
}

const char* CDataTableXML::GetString(const char* pszRow, const char* pszEntry,
    const char* defval, char* pszBuffer, int nBufferSize)
{
    if (!pszBuffer || nBufferSize <= 0)
        return defval;

    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (pElement)
    {
        // Check if it's an attribute
        const XMLAttribute* pAttr = pElement->FindAttribute(pszEntry);
        if (pAttr && pAttr->Value())
        {
            strncpy(pszBuffer, pAttr->Value(), nBufferSize - 1);
            pszBuffer[nBufferSize - 1] = '\0';
            return pszBuffer;
        }

        // Check if it's a child element
        XMLElement* pChild = pElement->FirstChildElement(pszEntry);
        if (pChild && pChild->GetText())
        {
            strncpy(pszBuffer, pChild->GetText(), nBufferSize - 1);
            pszBuffer[nBufferSize - 1] = '\0';
            return pszBuffer;
        }
    }

    strncpy(pszBuffer, defval, nBufferSize - 1);
    pszBuffer[nBufferSize - 1] = '\0';
    return pszBuffer;
}

int CDataTableXML::GetRawData(const char* pszRow, const char* pszEntry,
    void* pBuffer, int nBufferSize)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement || !pBuffer || nBufferSize <= 0)
        return 0;

    const char* pText = NULL;

    // Check if it's an attribute
    const XMLAttribute* pAttr = pElement->FindAttribute(pszEntry);
    if (pAttr && pAttr->Value())
        pText = pAttr->Value();

    // Check if it's a child element
    if (!pText)
    {
        XMLElement* pChild = pElement->FirstChildElement(pszEntry);
        if (pChild && pChild->GetText())
            pText = pChild->GetText();
    }

    if (!pText)
        return 0;

    // Convert hex string to binary
    int nLen = strlen(pText);
    int nOutSize = nBufferSize;
    NStr::StringToBin(pText, pBuffer, &nOutSize);
    return nOutSize;
}

void CDataTableXML::SetInt(const char* pszRow, const char* pszEntry, int val)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement)
    {
        // Create node hierarchy
        XMLElement* pCurrent = xmlRootNode;
        size_t start = 0;
        size_t end = path.find('/');

        while (end != std::string::npos)
        {
            std::string tag = path.substr(start, end - start);
            XMLElement* pChild = pCurrent->FirstChildElement(tag.c_str());
            if (!pChild)
            {
                pChild = xmlDocument->NewElement(tag.c_str());
                pCurrent->InsertEndChild(pChild);
            }
            pCurrent = pChild;
            start = end + 1;
            end = path.find('/', start);
        }

        pElement = pCurrent;
    }

    // Set as attribute
    pElement->SetAttribute(pszEntry, val);
    SetModified();
}

void CDataTableXML::SetDouble(const char* pszRow, const char* pszEntry, double val)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement)
    {
        // Create node hierarchy
        XMLElement* pCurrent = xmlRootNode;
        size_t start = 0;
        size_t end = path.find('/');

        while (end != std::string::npos)
        {
            std::string tag = path.substr(start, end - start);
            XMLElement* pChild = pCurrent->FirstChildElement(tag.c_str());
            if (!pChild)
            {
                pChild = xmlDocument->NewElement(tag.c_str());
                pCurrent->InsertEndChild(pChild);
            }
            pCurrent = pChild;
            start = end + 1;
            end = path.find('/', start);
        }

        pElement = pCurrent;
    }

    // Set as attribute
    pElement->SetAttribute(pszEntry, val);
    SetModified();
}

void CDataTableXML::SetString(const char* pszRow, const char* pszEntry, const char* val)
{
    std::string path = MakeName(pszRow, pszEntry);
    XMLElement* pElement = GetNode(path);

    if (!pElement)
    {
        // Create node hierarchy
        XMLElement* pCurrent = xmlRootNode;
        size_t start = 0;
        size_t end = path.find('/');

        while (end != std::string::npos)
        {
            std::string tag = path.substr(start, end - start);
            XMLElement* pChild = pCurrent->FirstChildElement(tag.c_str());
            if (!pChild)
            {
                pChild = xmlDocument->NewElement(tag.c_str());
                pCurrent->InsertEndChild(pChild);
            }
            pCurrent = pChild;
            start = end + 1;
            end = path.find('/', start);
        }

        pElement = pCurrent;
    }

    // Create text element
    XMLElement* pTextElement = xmlDocument->NewElement(pszEntry);
    pTextElement->SetText(val);
    pElement->InsertEndChild(pTextElement);
    SetModified();
}

void CDataTableXML::SetRawData(const char* pszRow, const char* pszEntry,
    const void* pBuffer, int nBufferSize)
{
    std::string szString;
    szString.resize(nBufferSize * 2 + 1);
    NStr::BinToString(pBuffer, nBufferSize, const_cast<char*>(szString.c_str()));
    SetString(pszRow, pszEntry, szString.c_str());
    SetModified();
}