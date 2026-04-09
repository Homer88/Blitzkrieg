#ifndef __STRING_PROCESSING_H__
#define __STRING_PROCESSING_H__
////////////////////////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stack>
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NStr
{
	// ��� �������� �� �������� ��������� ��������� ������: (), {}, [], ""
	//
	// �������� ����������� ������ �� �����������
	// ������ 'cOpenBracket' ������ ����������� ���� ����������� �������
	const char GetCloseBracket( const char cOpenBracket );
	// �������� �� ������ ����������� �������
	bool IsOpenBracket( const char cSymbol );
	// �������� ����� ���� ������
	void AddBrackets( const char cOpenBracket, const char cCloseBracket );
	// ������� ���� ������
	void RemoveBrackets( const char cOpenBracket, const char cCloseBracket );
	// ��������� ������ �� ������ ����� �� ��������� �����������
	void SplitString( const std::string &szString, std::vector<std::string> &szVector, const char cSeparator );
	void SplitString( const std::wstring &szString, std::vector<std::wstring> &szVector, const wchar_t cSeparator );
	// ��������� ������ �� ������ ����� �� ��������� ����������� � ������ ������ ����� �����������
	void SplitStringWithBrackets( const std::string &szString, std::vector<std::string> &szVector, const char cSeparator );
	// ��������� ������ �� ������ ����� �� ��������� ����������� � ������ ������ ����� �����������
	void SplitStringWithMultipleBrackets( const std::string &szString, std::vector<std::string> &szVector, const char cSeparator );
	// ����� ����������� ������ ��� ����� ���������� ������
	int FindCloseBracket( const std::string &szString, int nPos, const char cOpenBracket );
	// ����� ����������� ������ � ������ ���������� ������
	int FindMultipleCloseBracket( const std::string &szString, int nPos, const char cOpenBracket );
	// ��������� ����� ������
	template <class TYPE>
		inline int GetStrLen( const TYPE *pszString )
		{
			int nLength = 0;
			while ( pszString[nLength++] != 0 ) ;
			return nLength - 1;
		}
	template <class T1, class T2, class T3>
		inline int GetStrLen( const std::basic_string<T1, T2, T3> &szString )
		{
			return szString.length();
		}
	// �������� ��� ������� 'cTrim'
	// �������� ��� 'cTrim' �����
	inline void TrimLeft( std::string &szString, const char cTrim ) { szString.erase( 0, szString.find_first_not_of( cTrim ) ); }
	// �������� ��� 'pszTrim' �����
	inline void TrimLeft( std::string &szString, const char *pszTrim ) { szString.erase( 0, szString.find_first_not_of( pszTrim ) ); }
	// �������� ��� whitespaces �����
  inline void TrimLeft( std::string &szString ) { TrimLeft(szString, " \t\n\r"); } 
	// �������� ��� 'pszTrim' ������
	void TrimRight( std::string &szString, const char *pszTrim );
	// �������� ��� 'cTrim' ������
	void TrimRight( std::string &szString, const char cTrim );   
	// �������� ��� whitespaces ������
  inline void TrimRight( std::string &szString ) { TrimRight(szString, " \t\n\r"); }
	// �������� ��� 'pszTrim' � ����� ������
	inline void TrimBoth( std::string &szString, const char *pszTrim ) { TrimLeft( szString, pszTrim ); TrimRight( szString, pszTrim ); }
	// �������� ��� 'cTrim' � ����� ������
	inline void TrimBoth( std::string &szString, const char cTrim ) { TrimLeft( szString, cTrim ); TrimRight( szString, cTrim ); }
	// �������� ��� whitespaces � ����� ������
  inline void TrimBoth( std::string &szString ) { TrimBoth(szString, " \t\n\r"); }
	// �������� ��� ������� 'cTrim' �� ������
	void TrimInside( std::string &szString, const char *pszTrim );
	inline void TrimInside( std::string &szString, const char cTrim ) { szString.erase( std::remove(szString.begin(), szString.end(), cTrim), szString.end() ); }
  inline void TrimInside( std::string &szString ) { TrimInside(szString, " \t\n\r"); }
	// �������� � �������� ��� ������� ��������
	void ToLower( std::string &szString );
	void ToUpper( std::string &szString );
  // ������������� ����� � ������, �������� ������ ��� ����� (��� �������) ����������� ������������ (default = '.')
  void ToDotString( std::string *pDst, int nVal, const char cSeparator = '.' );
	// �������� �� ������ �������������� �����
	inline bool IsBinDigit( const char cChar ) { return ( (cChar == '0') && (cChar == '1') ); }
	inline bool IsOctDigit( const char cChar ) { return ( (cChar >= '0') && (cChar <= '7') ); }
	inline bool IsDecDigit( const char cChar ) { return ( (cChar >= '0') && (cChar <= '9') ); }
	inline bool IsHexDigit( const char cChar ) { return ( (cChar >= '0') && (cChar <= '9') ) || ( (cChar >= 'a') && (cChar <= 'f') ) || ( (cChar >= 'A') && (cChar <= 'F') ); }
	inline bool IsSign( const char cChar ) { return ( (cChar == '-') || (cChar == '+') ); }
	bool IsDecNumber( const std::string &szString );
	bool IsOctNumber( const std::string &szString );
	bool IsHexNumber( const std::string &szString );
	// convert 'string', which represents integer value in any radix (oct, dec, hex) to 'int'
	int ToInt( const char *pszString );
	inline int ToInt( const std::string &szString ) { return ToInt( szString.c_str() ); }
	unsigned long ToULong( const char *pszString );
	inline unsigned long ToULong( const std::string &szString ) { return ToULong( szString.c_str() ); }
	// convert 'string', which represents FP value to 'float' and 'double'
	float ToFloat( const char *pszString );
	inline float ToFloat( const std::string &szString ) { return ToFloat( szString.c_str() ); }
	double ToDouble( const char *pszString );
	inline double ToDouble( const std::string &szString ) { return ToDouble( szString.c_str() ); }
	// 
	void SetCodePage( int nCodePage );
	void ToAscii( std::string *pRes, const std::wstring &szSrc );
	inline std::string ToAscii( const std::wstring &szSrc )
	{
		std::string szDst;
		ToAscii( &szDst, szSrc );
		return szDst;
	}
	void ToUnicode( std::wstring *pRes, const std::string &szSrc );
	inline std::wstring ToUnicode( const std::string &szSrc )
	{
		std::wstring szDst;
		ToUnicode( &szDst, szSrc );
		return szDst;
	}
	// �������������� ������ ��� � sprintf. 
	// NON-REENTRANT!!! Uses internal static buffer. Max string length = 2048 chars
	// � ����, ��� non-reentrant ������� ��� �����, �� ��� ������� ������ ��� ����� ������
	const char* __cdecl Format( const char *pszFormat, ... );
	// ��������������� ����� �/� OutputDebugString
	void __cdecl DebugTrace( const char *pszFormat, ... );
	//
	// default separator functor for CStringIterator
	class CCharSeparator
	{
		const char cSeparator;
	public:
		explicit CCharSeparator( const char _cSeparator )	: cSeparator( _cSeparator ) {  }
		bool operator()( const char cSymbol ) const { return cSymbol == cSeparator; }
	};
	// separator with recursive bracket functor
	class CBracketCharSeparator
	{
		const char cSeparator;
		std::stack<char> stackBrackets;
	public:
		explicit CBracketCharSeparator( const char _cSeparator )	: cSeparator( _cSeparator ) {  }
		bool operator()( const char cSymbol );
	};
	// std::string iteration class
	template <class TSeparator = CCharSeparator>
	class CStringIterator
	{
	private:
		TSeparator tSeparator;               // functor, which returns true, if next char is separator
		std::string szInput;									// input string
		std::string szString;                 // result string
		int nLastPos;                         // current lexeme begin position
		int nPos;                             // current position
	public:
		CStringIterator( const char *pszInput, TSeparator &_tSeparator, int _nPos = 0 )
			: szInput( pszInput ), tSeparator( _tSeparator ), nPos( _nPos ), nLastPos( _nPos ) { Next(); }
		CStringIterator( const std::string &_szInput, TSeparator &_tSeparator, int _nPos = 0 )
			: szInput( _szInput ), tSeparator( _tSeparator ), nPos( _nPos ), nLastPos( _nPos ) { Next(); }
		// iteration
		// extract next lexem
		const CStringIterator& Next()
		{
			nLastPos = nPos;
			for ( ; nPos<szInput.size(); ++nPos )
			{
				if ( tSeparator(szInput[nPos]) )
				{
					szString = szInput.substr( nLastPos, nPos - nLastPos );
					++nPos;
					return *this;
				}
			}
			szString = szInput.substr( nLastPos, nPos - nLastPos );
			return *this;
		}
		const CStringIterator& operator++() { return Next(); }
		// is 'nPos' at the end or at the begining of the string
		bool IsBegin() const { return nPos == 0; }
		bool IsEnd() const { return nLastPos == szInput.size(); }
		// positions
		int GetLastPos() const { return nLastPos; }
		int GetPos() const { return nPos; }
		// access lexem (const and non-const)
		operator const std::string*() const { return &szString; }
		operator std::string*() { return &szString; }
		const std::string* operator->() const { return &szString; }
		std::string* operator->() { return &szString; }
		const std::string& operator*() const { return szString; }
		std::string& operator*() { return szString; }
		// access lexem as decimal or floating-point values
		operator const double() const { return ToDouble( szString ); }
		operator const float() const { return ToFloat( szString ); }
		operator const int() const { return ToInt( szString ); }
	};
};
////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __STRING_PROCESSING_H__