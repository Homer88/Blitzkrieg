// ExcelExporter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <afxdb.h>
#include <odbcinst.h>
#include <iostream>
#include <set>

#include "ExcelExporter.h"
#include "XmlReader.h"
#include "..\misc\strproc.h"

const int STRING_SIZE = 65536;

CString CExcelExporter::GetExcelDriverName()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString szDriver;

	// Get the names of the installed drivers ("odbcinst.h" has to be included )
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return "";

	// Search for the driver...
	do
	{
		if( strstr( pszBuf, "Excel" ) != 0 )
		{
			// Found !
			szDriver = CString( pszBuf );
			break;
		}
		pszBuf = strchr( pszBuf, '\0' ) + 1;
	}
	while( pszBuf[1] != '\0' );

	return szDriver;
}

class CSpaceSeparator
{
public:
		bool operator() ( const char cSymbol ) const { return isspace( cSymbol ); }
};

void CExcelExporter::ConvertFilesToExcel( const vector<string> &files, const char *pszExcelFileName, const char *pszCrapFile, const char *pszNodeName, bool bIgnoreFields )
{
	if ( files.empty() )
		return;

	//читаем поля из crap файла
	std::vector< std::string > crapFields;
	if ( strlen(pszCrapFile) > 0 )
	{
		FILE *pFile = fopen( pszCrapFile, "r" );
		if ( pFile != 0 )
		{
			char szString[STRING_SIZE];
			while ( fgets( szString, STRING_SIZE, pFile ) != 0 )
			{
				int nLen = strlen( szString );
				if ( szString[nLen-1] == '\n' )
					szString[ nLen - 1 ] = '\0';
				NStr::CStringIterator<CSpaceSeparator> it( szString, CSpaceSeparator() );
				if ( it.IsEnd() )
					continue;

				crapFields.push_back( *it );
				++it;
			}
		}
	}

	if ( !bIgnoreFields && crapFields.empty() )
		bIgnoreFields = true;

	//парсим RPG данные из всех файлов
	CXMLReadVector filesValuesVector;
	// Для каждого файла читаем данные через CXMLReader (теперь на tinyxml2)
	std::vector<CXMLReadVector> allFilesValues( files.size() );
	for ( int i=0; i<files.size(); i++ )
	{
		CXMLReader xmlReader;
		xmlReader.ReadRPGInformationFromFile( files[i].c_str(), allFilesValues[i], crapFields, bIgnoreFields, pszNodeName );
	}

	// Create table structure - собираем все уникальные ключи
	std::set<SXMLValue> mySet;
	for ( int i=0; i<allFilesValues.size(); i++ )
	{
		for ( int k=0; k<allFilesValues[i].size(); k++ )
		{
			mySet.insert( allFilesValues[i][k] );
		}
	}

	std::string szStrToSave = "FileName\t";
	for ( std::set<SXMLValue>::iterator it=mySet.begin(); it!=mySet.end(); ++it )
	{
		szStrToSave += it->szName;
		szStrToSave += "\t";
	}

	if ( szStrToSave.size() == 0 )
	{
		return;
	}

	szStrToSave = szStrToSave.substr( 0, szStrToSave.size() - 1 );
	szStrToSave += "\n";
	CPtr<IDataStream> pStream = OpenFileStream( pszExcelFileName, STREAM_ACCESS_WRITE );
	if ( pStream == 0 )
	{
		std::string szErr = NStr::Format( "Error: The file %s can not be opened", pszExcelFileName );
		AfxMessageBox( szErr.c_str() );
		return;
	}

	pStream->Write( szStrToSave.c_str(), szStrToSave.size() );


	//formatting output
	for ( int i=0; i<allFilesValues.size(); i++ )
	{
		std::cout << files[i].c_str() << std::flush;

		szStrToSave = "";
		CXMLReadVector cur;
		for ( std::set<SXMLValue>::iterator it=mySet.begin(); it!=mySet.end(); ++it )
		{
			int k = 0;
			for ( ; k<allFilesValues[i].size(); k++ )
			{
				if ( allFilesValues[i][k].szName == it->szName )
					break;
			}

			if ( k == allFilesValues[i].size() )
			{
				SXMLValue val;
				val.bString = it->bString;
				val.szName = it->szName;
				val.szVal = "";
				cur.push_back( val );
			}
			else
			{
				cur.push_back( allFilesValues[i][k] );
			}
		}

		{
			// Insert data
			szStrToSave += files[i];
			szStrToSave += "\t";

			for ( int k=0; k<cur.size(); k++ )
			{
				szStrToSave += cur[k].szVal;
				szStrToSave += "\t";
			}
			if ( szStrToSave.size() > 0 )
				szStrToSave = szStrToSave.substr( 0, szStrToSave.size() - 1 );
			szStrToSave += "\n";
			pStream->Write( szStrToSave.c_str(), szStrToSave.size() );
		}

		std::cout << "     -done" << endl;
	}
}

void CExcelExporter::ConvertExcelToXMLFiles( const char *pszExcelFileName, const char *pszNodeName )
{
	FILE *pFile = fopen( pszExcelFileName, "r" );
	if ( !pFile )
	{
		std::cout << "Error: Cannot open " << pszExcelFileName << std::endl;
		return;
	}

	std::vector< std::string > headers;
	char szCur[STRING_SIZE];
	if ( fgets( szCur, STRING_SIZE, pFile ) == 0 )
	{
		std::cout << "Error: The file is empty" << std::endl;
		fclose( pFile );
		return;
	}
	szCur[ strlen( szCur ) - 1 ] = '\0';

	std::string szBaseNodeName = pszNodeName;		//"RPG"
	{
		NStr::CStringIterator<> it( szCur, NStr::CCharSeparator('\t') );
		for ( ; !it.IsEnd(); ++it )
		{
			std::string szCur = *it;
			int nBegin = szCur.find( '\"' );
			if ( nBegin != std::string::npos )
			{
				szCur = szCur.substr( nBegin+1, szCur.size()-2 );
			}
			headers.push_back( szCur );
		}
	}

	while ( fgets( szCur, STRING_SIZE, pFile ) != 0 )
	{
		szCur[ strlen( szCur ) - 1 ] = '\0';
		NStr::CStringIterator<> it( szCur, NStr::CCharSeparator('\t') );
		if ( it.IsEnd() )
			continue;

		//read the file name
		std::string szFileName = *it;

		//определяем тип узла по расширению. по умолчанию - "RPG"
		{
			std::string szExtension = szFileName.substr( szFileName.rfind( '.' ) );
			for ( int i=0; i<extensions.size(); i++ )
			{
				if ( !strcmpi( extensions[i].szExtension.c_str()+1, szExtension.c_str() ) )
				{
					szBaseNodeName = extensions[i].szBaseNode;
					break;
				}
			}
		}

		++it;
		if ( it.IsEnd() )
			continue;

		CXMLValuesVector valuesVector;
		if ( it.IsEnd() )
			continue;
		CXMLValue val;
		int i = 1;

		for ( ; !it.IsEnd(); ++it )
		{
			val.first = headers[i];
			std::string szCur = *it;
			int nBegin = szCur.find( '\"' );
			if ( nBegin != std::string::npos )
			{
				szCur = szCur.substr( nBegin+1, szCur.rfind('\"') );
			}
			val.second = szCur;
			valuesVector.push_back( val );
			i++;
		}

		CXMLWriter xmlWriter;
		if ( xmlWriter.SaveRPGInformationToXML( szFileName.c_str(), valuesVector, szBaseNodeName.c_str() ) )
			std::cout << szFileName.c_str() << "    -done" << std::endl;
		else
			std::cout << szFileName.c_str() << "    -FAILED" << std::endl;
	}

	fclose( pFile );
}
