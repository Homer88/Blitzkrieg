#include "StdAfx.h"
#include "XmlReader.h"
#include "io.h"

#include "..\StreamIO\StreamAdaptor.h"
#include "..\Misc\FileUtils.h"
#include "..\Misc\StrProc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tinyxml2::XMLElement* CXMLReader::FindRPGNode( tinyxml2::XMLElement* startNode, const char *pszNodeName )
{
	if ( !startNode )
		return 0;

	for ( tinyxml2::XMLElement* child = startNode->FirstChildElement(); child; child = child->NextSiblingElement() )
	{
		if ( strcmp( child->Name(), pszNodeName ) == 0 )
			return child;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tinyxml2::XMLElement* CXMLWriter::FindRPGNode( tinyxml2::XMLElement* startNode, const char *pszNodeName )
{
	if ( !startNode )
		return 0;

	for ( tinyxml2::XMLElement* child = startNode->FirstChildElement(); child; child = child->NextSiblingElement() )
	{
		if ( strcmp( child->Name(), pszNodeName ) == 0 )
			return child;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CXMLReader::IsCrappedValue( const std::string &szValName, const vector<string> &crapFields, bool bIgnoreFields, bool bCompareOnlyFirstSymbols )
{
	for ( int i=0; i<crapFields.size(); i++ )
	{
		string szCur = szValName;
		{
			// remove item(**) suffixes for comparison
			int nPos = -1;
			const char *pTemp = szCur.c_str();
			do
			{
				pTemp = strstr( pTemp, "item" );
				if ( pTemp == 0 )
					break;

				if ( szCur.size() < pTemp - szCur.c_str() + 4 + 4 )
					break;			// 4 == item, 4 == (**)
				if ( pTemp[4] != '(' || pTemp[7] != ')' )
				{
					pTemp = pTemp + 8;
					continue;
				}

				if ( pTemp[5] < '0' || pTemp[5] > '9' || pTemp[6] < '0' || pTemp[6] > '9' )
				{
					pTemp = pTemp + 8;
					continue;
				}

				// found pattern, remove it
				szCur.erase( pTemp - szCur.c_str() + 4, 4 );
				pTemp += 4;
			} while( pTemp );
		}
		szCur = szCur.substr( 0, crapFields[i].size() );

		string szCompare = crapFields[i];
		if ( bCompareOnlyFirstSymbols )
			szCompare = szCompare.substr( 0, szCur.size() );
		if ( szCur == szCompare )
		{
			if ( bIgnoreFields )
				return true;
			else
				return false;
		}
	}

	if ( bIgnoreFields )
		return false;
	else
		return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CXMLReader::ReadInformation( tinyxml2::XMLElement* node, const string &szPrefix, CXMLReadVector &result, vector<string> &crapFields, bool bIgnoreFields )
{
	if ( !node )
		return;

	// read attributes first
	for ( const tinyxml2::XMLAttribute* attr = node->FirstAttribute(); attr; attr = attr->Next() )
	{
		string szNodeName = attr->Name();
		string szNodeValue = attr->Value() ? attr->Value() : "";

		SXMLValue val;
		val.bString = false;
		val.szName = szPrefix + szNodeName;
		val.szVal = szNodeValue;
		if ( !IsCrappedValue(val.szName, crapFields, bIgnoreFields, false) )
			result.push_back( val );
	}

	int nItemIndex = 0;

	// iterate through all child elements
	for ( tinyxml2::XMLElement* child = node->FirstChildElement(); child; child = child->NextSiblingElement() )
	{
		string szNewPrefix = szPrefix;

		bool bString = false;
		{
			// check if this is a leaf node (no children and no attributes)
			bool hasChildren = (child->FirstChildElement() != 0);
			bool hasAttributes = (child->FirstAttribute() != 0);
			const char* pText = child->GetText();

			if ( !hasChildren && !hasAttributes && pText && strlen(pText) > 0 )
			{
				string szNodeName = child->Name();
				string szVal = pText;

				SXMLValue val;
				val.bString = true;
				val.szName = szPrefix + szNodeName;
				val.szVal = szVal;
				if ( !IsCrappedValue(val.szName, crapFields, bIgnoreFields, false) )
					result.push_back( val );
				bString = true;
			}
		}

		if ( !bString )
		{
			string szNodeName = child->Name();
			if ( szNodeName == "item" )
			{
				szNodeName = NStr::Format( "item(%.2d)", nItemIndex );
				nItemIndex++;
			}
			szNewPrefix += szNodeName + ';';
			if ( !IsCrappedValue(szNewPrefix, crapFields, bIgnoreFields, true) )
				ReadInformation( child, szNewPrefix, result, crapFields, bIgnoreFields );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CXMLReader::ReadRPGInformationFromFile( const char *pszFileName, CXMLReadVector &result, vector<string> &crapFields, bool bIgnoreFields, const char *pszNodeName )
{
	tinyxml2::XMLDocument doc;
	if ( doc.LoadFile( pszFileName ) != tinyxml2::XML_SUCCESS )
		return false;

	// find RPG node
	tinyxml2::XMLElement* xmlRoot = doc.RootElement();
	if ( !xmlRoot )
		return false;

	tinyxml2::XMLElement* xmlCurrNode = FindRPGNode( xmlRoot, pszNodeName );
	if ( xmlCurrNode == 0 )
		return false;

	ReadInformation( xmlCurrNode, "", result, crapFields, bIgnoreFields );
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CXMLWriter::FindNodeAndSetAttribute( tinyxml2::XMLElement* startNode, const string &szName, const string &szAttributeValue )
{
	if ( !startNode )
		return;

	int nPos = szName.find(';');
	if ( nPos != -1 )
	{
		string szCurrentFindNodeName = szName.substr( 0, nPos );
		// find child with that name
		tinyxml2::XMLElement* found = 0;
		int nItemIndex = 0;

		for ( tinyxml2::XMLElement* child = startNode->FirstChildElement(); child; child = child->NextSiblingElement() )
		{
			string szNodeName = child->Name();
			if ( szNodeName == szCurrentFindNodeName )
			{
				found = child;
				break;
			}

			if ( szNodeName == "item" )
			{
				string szTempNodeName = NStr::Format( "item(%.2d)", nItemIndex );
				if ( szTempNodeName == szCurrentFindNodeName )
				{
					found = child;
					szCurrentFindNodeName = "item";
					break;
				}
				nItemIndex++;
			}
		}

		string szNextNodeName = szName.substr( nPos + 1 );
		if ( !found )
		{
			// create new element
			tinyxml2::XMLDocument* pDoc = startNode->GetDocument();
			tinyxml2::XMLElement* newNode = pDoc->NewElement( szCurrentFindNodeName.c_str() );
			startNode->InsertEndChild( newNode );
			FindNodeAndSetAttribute( newNode, szNextNodeName, szAttributeValue );
		}
		else
			FindNodeAndSetAttribute( found, szNextNodeName, szAttributeValue );
		return;
	}
	else
	{
		// no ';' - this is an attribute or text
		if ( szName == "#text" )
		{
			tinyxml2::XMLDocument* pDoc = startNode->GetDocument();
			tinyxml2::XMLText* newText = pDoc->NewText( szAttributeValue.c_str() );
			// replace existing text
			if ( startNode->FirstChild() )
				startNode->DeleteChild( startNode->FirstChild() );
			startNode->InsertEndChild( newText );
		}
		else
			startNode->SetAttribute( szName.c_str(), szAttributeValue.c_str() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CXMLWriter::SaveRPGInformationToXML( const char *pszFileName, const CXMLValuesVector &valuesVector, const char *pszNodeName )
{
	if ( _access( pszFileName, 02 ) )
	{
		return false;
	}

	tinyxml2::XMLDocument doc;
	if ( doc.LoadFile( pszFileName ) != tinyxml2::XML_SUCCESS )
		return false;

	// find RPG node
	tinyxml2::XMLElement* xmlRoot = doc.RootElement();
	if ( !xmlRoot )
		return false;

	tinyxml2::XMLElement* xmlStartNode = FindRPGNode( xmlRoot, pszNodeName );
	if ( xmlStartNode == 0 )
		return false;

	for ( CXMLValuesVector::const_iterator it=valuesVector.begin(); it!=valuesVector.end(); ++it )
	{
		if ( it->second.size() > 0 )
			FindNodeAndSetAttribute( xmlStartNode, it->first, it->second );
	}

	// save file
	if ( doc.SaveFile( pszFileName ) != tinyxml2::XML_SUCCESS )
		return false;

	return true;
}
