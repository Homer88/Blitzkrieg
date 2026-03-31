#ifndef __DATATREEXML_H__
#define __DATATREEXML_H__

#include "tinyxml2.h"

class CDataTreeXML : public IDataTree
{
    OBJECT_MINIMAL_METHODS(CDataTreeXML);

    CPtr<IDataStream> pStream;                      // stream, this table was open with
    tinyxml2::XMLDocument* xmlDocument;             // XML document
    IDataTree::EAccessMode eMode;

    // Node stack for hierarchy
    std::list<tinyxml2::XMLNode*> nodes;
    tinyxml2::XMLNode* xmlCurrNode;

    // Element stack for writing
    std::list<tinyxml2::XMLElement*> elements;
    std::list<tinyxml2::XMLElement*> arrbases;      // array base elements
    tinyxml2::XMLElement* xmlCurrElement;

    // For array reading
    struct SNodesList
    {
        tinyxml2::XMLElement* parentElement;
        std::vector<tinyxml2::XMLElement*> items;
        int nCurrElement;

        SNodesList() : parentElement(NULL), nCurrElement(-1) {}
    };
    std::list<SNodesList> nodelists;

    // Get attribute from current node
    const tinyxml2::XMLAttribute* GetAttribute(const char* idChunk);

    // Get text node (attribute or child element)
    tinyxml2::XMLNode* GetTextNode(const char* idChunk);

public:
    CDataTreeXML(IDataTree::EAccessMode eMode);
    virtual ~CDataTreeXML();

    bool Open(IDataStream* pStream, const char* idBaseNode);

    // Is opened in the READ mode?
    virtual bool STDCALL IsReading() const { return eMode == IDataTree::READ; }

    // Start new complex chunk
    virtual int STDCALL StartChunk(const char* idChunk);

    // Finish complex chunk
    virtual void STDCALL FinishChunk();

    // Simply data chunk: text, integer, fp
    virtual int STDCALL GetChunkSize();
    virtual bool STDCALL RawData(void* pData, int nSize);
    virtual bool STDCALL StringData(char* pData);
    virtual bool STDCALL StringData(WORD* pData);
    virtual bool STDCALL DataChunk(const char* idChunk, int* pData);
    virtual bool STDCALL DataChunk(const char* idChunk, double* pData);

    // Array data serialization (special case)
    virtual int STDCALL CountChunks(const char* idChunk);
    virtual bool STDCALL SetChunkCounter(int nCount);
    virtual int STDCALL StartContainerChunk(const char* idChunk);
    virtual void STDCALL FinishContainerChunk();
};

#endif // __DATATREEXML_H__