#ifndef __DATATABLEXML_H__
#define __DATATABLEXML_H__

#include "tinyxml2.h"

class CDataTableXML : public IDataTable
{
    OBJECT_MINIMAL_METHODS(CDataTableXML);

    CPtr<IDataStream> pStream;                      // stream, this table was open with
    tinyxml2::XMLDocument* xmlDocument;             // XML document
    tinyxml2::XMLElement* xmlRootNode;              // root node
    bool bModified;

    // Get text node by name (attribute or child element)
    tinyxml2::XMLNode* GetTextNode(const char* pszRow, const char* pszEntry);

    inline void SetModified() { bModified = true; }

    template <class TYPE>
    inline void SetValue(const char* pszRow, const char* pszEntry, const TYPE& val);

    tinyxml2::XMLElement* GetNode(const std::string& szName);
    const std::string MakeName(const char* pszRow, const char* pszEntry);

public:
    CDataTableXML();
    virtual ~CDataTableXML();

    bool Open(IDataStream* pStream, const char* pszBaseNode);

    // Get row names
    virtual int STDCALL GetRowNames(char* pszBuffer, int nBufferSize);

    // Get entry names
    virtual int STDCALL GetEntryNames(const char* pszRow, char* pszBuffer, int nBufferSize);

    // Clear section
    virtual void STDCALL ClearRow(const char* pszRowName) {}

    // Get methods
    virtual int STDCALL GetInt(const char* pszRow, const char* pszEntry, int defval);
    virtual double STDCALL GetDouble(const char* pszRow, const char* pszEntry, double defval);
    virtual const char* STDCALL GetString(const char* pszRow, const char* pszEntry,
        const char* defval, char* pszBuffer, int nBufferSize);
    virtual int STDCALL GetRawData(const char* pszRow, const char* pszEntry,
        void* pBuffer, int nBufferSize);

    // Set methods
    virtual void STDCALL SetInt(const char* pszRow, const char* pszEntry, int val);
    virtual void STDCALL SetDouble(const char* pszRow, const char* pszEntry, double val);
    virtual void STDCALL SetString(const char* pszRow, const char* pszEntry, const char* val);
    virtual void STDCALL SetRawData(const char* pszRow, const char* pszEntry,
        const void* pBuffer, int nBufferSize);
};

#endif // __DATATABLEXML_H__