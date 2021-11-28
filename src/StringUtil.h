#pragma once
#include "Macros.h"

string		stringTrimRight		(const  string& s, const    char* t =  " \t\r\n");
wstring		stringTrimRight		(const wstring& s, const wchar_t* t = L" \t\r\n");
string		stringTrimLeft		(const  string& s, const    char* t =  " \t\r\n");
wstring		stringTrimLeft		(const wstring& s, const wchar_t* t = L" \t\r\n");
string		stringTrim			(const  string& s, const    char* t =  " \t\r\n");
wstring		stringTrim			(const wstring& s, const wchar_t* t = L" \t\r\n");
string		stringAfterFirst	(const  string& s, const string& marker);
string		stringAfterLast		(const  string& s, const string& marker);
string		stringBeforeFirst	(const  string& s, const string& marker);
string		stringBeforeLast	(const  string& s, const string& marker);
size_t		stringFind			(const  string& s,    char c, size_t offset=0);
size_t		stringFind			(const wstring& s, wchar_t c, size_t offset=0);
size_t		stringFind			(const  string& s, const  string& token, size_t offset=0);
size_t		stringFind			(const wstring& s, const wstring& token, size_t offset=0);
size_t		stringFindOneOf		(const  string& s, const  string& charsList, size_t offset=0);
size_t		stringFindOneOf		(const wstring& s, const wstring& charsList, size_t offset=0);
size_t		stringReverseFind	(const  string& s,    char c, size_t offset=0);
size_t		stringReverseFind	(const wstring& s, wchar_t c, size_t offset=0);
void		stringReplaceChar	( string& str, char    OldChar, char    NewChar);
void		stringReplaceChar	(wstring& str, wchar_t OldChar, wchar_t NewChar);
void		stringReplaceChar	(char*    str, char    OldChar, char    NewChar);
void		stringReplaceChar	(wchar_t* str, wchar_t OldChar, wchar_t NewChar);
void		stringRemoveChar	( string& str, char    c);
void		stringRemoveChar	(wstring& str, wchar_t c);
string		stringToLower		(const  string& s);
wstring		stringToLower		(const wstring& s);
string		stringToUpper		(const  string& s);
wstring		stringToUpper		(const wstring& s);
bool		isStringEndsWith	(const string& s, const string& token);
bool		isStringStartsWith	(const string& s, const string& token);
bool		isStringContains	(const string& s, const string& token);
bool		isStringInList		(const string& s, const vector<string>& list, int& outIndex);
bool		isStringInList		(const char*   s, const vector<string>& list, int& outIndex);
string		stringLeft			(const  string& s, size_t nbChar);
wstring		stringLeft			(const wstring& s, size_t nbChar);
string		stringRight			(const  string& s, size_t nbChar);
wstring		stringRight			(const wstring& s, size_t nbChar);
string		getStringBetween	(const  string& s, const  string& tokenStart, const  string& tokenEnd);
wstring		getStringBetween	(const wstring& s, const wstring& tokenStart, const wstring& tokenEnd);
string		stringReplaceBetween(const  string& s, const  string& tokenStart, const  string& tokenEnd, const  string& newStr);
wstring		stringReplaceBetween(const wstring& s, const wstring& tokenStart, const wstring& tokenEnd, const wstring& newStr);
string		stringReplaceToken	(const  string& s, const  string& token, const  string& newStr);
wstring		stringReplaceToken	(const wstring& s, const wstring& token, const wstring& newStr);
string		stringReplaceAllTokens( string s, const  string& token, const  string& newStr);
wstring		stringReplaceAllTokens(wstring s, const wstring& token, const wstring& newStr);
size_t		stringCountToken	(const  string& str, const  string& token);
size_t		stringCountToken	(const wstring& str, const wstring& token);
string		vectorToString		(const vector< string>& v, const  string& spacer);
wstring		vectorToString		(const vector<wstring>& v, const wstring& spacer);
void		stringToVector		( string s, vector< string> & v, char    delim =  ' ', bool trim_spaces = true);
void		stringToVector		(wstring s, vector<wstring> & v, wchar_t delim = L' ', bool trim_spaces = true);
float		stringToFloat		(const  string& s, bool& outIsOk);
float		stringToFloat		(const wstring& s, bool& outIsOk);
double		stringToDouble		(const  string& s, bool& outIsOk);
double		stringToDouble		(const wstring& s, bool& outIsOk);
int			stringToInt			(const  string& s, bool& outIsOk);
int			stringToInt			(const wstring& s, bool& outIsOk);
int64_t		stringToInt64		(const  string& s, bool& outIsOk);
int64_t		stringToInt64		(const wstring& s, bool& outIsOk);
uint64_t	stringToUInt64		(const  string& s, bool& outIsOk);
uint64_t	stringToUInt64		(const wstring& s, bool& outIsOk);
uint64_t	stringHexToUInt64	(const  string& s, bool& outIsOk);
uint64_t	stringHexToUInt64	(const wstring& s, bool& outIsOk);
bool		stringToBool		(const  string& s, bool& outIsOk);
bool		stringToBool		(const wstring& s, bool& outIsOk);
uint32_t	stringToHexRGB		(const  string& s, bool& outIsOk);
uint32_t	stringToHexRGB		(const wstring& s, bool& outIsOk);
int64_t		hexToInt64			(const  string& s, bool& outIsOk);
int64_t		hexToInt64			(const wstring& s, bool& outIsOk);
uint64_t	hexToUInt64			(const  string& s, bool& outIsOk);
uint64_t	hexToUInt64			(const wstring& s, bool& outIsOk);
string		stringFormat		(const char*    fmt, ...);
wstring		stringFormat		(const wchar_t* fmt, ...);
string		toString			(double	d, int nbDecimal=7);
string		toString			(float	 f);
string		toString			(int	 i);
string		toString			(int64_t i);
string		toString			(size_t	 i);
string		toString			(bool	 b);
string		toStringHex			(int i, int width);
string		toStringHexRGB		(      u8 r, u8 g, u8 b);
string		toStringHexARGB		(u8 a, u8 r, u8 g, u8 b);
wstring		toWString			(double	 d, int nbDecimal=7);
wstring		toWString			(float	 f);
wstring		toWString			(int	 i);
wstring		toWString			(int64_t i);
wstring		toWString			(size_t	 i);
wstring		toWString			(bool	 b);
wstring		toWStringHex		(int i, int width);
wstring		toWStringHexRGB		(      u8 r, u8 g, u8 b);
wstring		toWStringHexARGB	(u8 a, u8 r, u8 g, u8 b);
char*		__itoa				(int32_t  value, char* str, int radix);
char*		__i64toa			(int64_t  value, char* str, int radix);
char*		__uitoa				(uint32_t value, char* str, int radix);
char*		__ui64toa			(uint64_t value, char* str, int radix);
string		WStringToString		(const wstring& wstr);
wstring		StringToWString		(const  string&  str);
//char		CharToAsciiChar		(char    c);
//wchar_t	WCharToAsciiChar	(wchar_t c);
string		stringEscapeC		(const string& str);
string		stringUnescapeC		(const string& str);
