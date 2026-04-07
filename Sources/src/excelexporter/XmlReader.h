#ifndef __XML_READER_H__
#define __XML_READER_H__

#include "tinyxml2.h"

struct SXMLValue
{
	bool bString;			// 0 = number, 1 = string
	string szName;
	string szVal;

	SXMLValue() : bString( true ) {}
};

inline bool operator < ( const SXMLValue &a, const SXMLValue &b ) { return a.szName < b.szName; }

typedef vector< SXMLValue > CXMLReadVector;
typedef pair<string, string> CXMLValue;
typedef vector< CXMLValue > CXMLValuesVector;

class CXMLReader
{
protected:
	tinyxml2::XMLElement* FindRPGNode( tinyxml2::XMLElement* startNode, const char *pszNodeName );
	void ReadInformation( tinyxml2::XMLElement* node, const string &szPrefix, CXMLReadVector &result, vector<string> &crapFields, bool bIgnoreFields );
	bool IsCrappedValue( const std::string &szValName, const vector<string> &crapFields, bool bIgnoreFields, bool bCompareOnlyFirstSymbols );

public:
	CXMLReader() {}
	~CXMLReader() {}

	bool ReadRPGInformationFromFile( const char *pszFileName, CXMLReadVector &result, vector<string> &crapFields, bool bIgnoreFields, const char *pszNodeName );
};

class CXMLWriter
{
protected:
	tinyxml2::XMLElement* FindRPGNode( tinyxml2::XMLElement* startNode, const char *pszNodeName );
	void FindNodeAndSetAttribute( tinyxml2::XMLElement* startNode, const string &szName, const string &szAttributeValue );

public:
	CXMLWriter() {}
	~CXMLWriter() {}

	bool SaveRPGInformationToXML( const char *pszFileName, const CXMLValuesVector &valuesVector, const char *pszNodeName );
};

#endif		//__XML_READER_H__
