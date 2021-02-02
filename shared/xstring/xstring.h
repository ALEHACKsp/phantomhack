#pragma once
/*
	******************************************************
	***     Created by Fullmetal Alcoholic in 2018     ***
	***		Subject: xstring (xstringA/xstringW)       ***
	*** Description: xor encrypted thread-safe  string ***
	******************************************************

	Formatting Guide:

	Both of strings (xstringA/xstringW) has .Format and .AppendFormat methods.
	It is similar with sprintf and wsprintf formatting, but it has a unique implementation.

	**************************************************************************************
	*	Pattern: %<#><[Width/Precision]><Modifier><Data type><(Delimiter for raw data)>	 *
	**************************************************************************************

	0. Percent character
	If you need to put percent character in your string ('%') - just write it twice: '%%'. Single percent means format pattern.

	1. Prefix '#' [optional]
	# - put it after format character to use prefix "0x" (or "0b") for hexadecimal or binary numbers.
		
	2. Width/Precision '[...]' [optional]
	[width (or precision if float, double or decimal value)] - size limit for string or raw buffer to be presented. Must be inside of []. 
	If float, double or digital data types are chosen (exepting 'B' and 'c') - this value will mean precision of it.

	3. Modifier [optional]
	Modifier - 'l' or 'll' (long / long long). Size prefix for numbers. For example lli us long long int. 
	type - type of data to format.
	Modifiers only for raw data types ('rx', 'rX', 'ru', 'rb'): 'n' for no space delimiters. Cannot be used with '#' prefix.
	And 't' for custom delimiter. When using t after data type comes '()'. You can define any delimiter inside. 
	For example '(;\r\n)', '(_)', '(lol)', etc. 
	And modifier 'z' to specify string or char type. Only for 's' and 'c' types.

	4. Data type [required]
	You must to select one of next data types to interpret input data according to your needs.
	You can choose only one of data type for every '%' format instance. But you can modify some of them with 'l' or 'll'.
	For example '%u', '%lu' or '%llu'.

	5. Delimiter for raw data '()' [optional]
	Any characters or words to separate bytes. Usable only with 't' modifier. Not neccessary even with 't'.
	Can also be empty: '()'

	Data types:
	*********************************************************************************************************

		d, i	-		signed int.
		u		-		unsigned int.
		f		-		double (can also present float) ("lf" is a long double).
		x		-		Lower-case unsigned hexadecimal int.
		X		-		Upper-case unsigned hexadecimal int.
		a, p	-		Lower-case address (or pointer). Same as x.
		A, P	-		Upper-case address (or pointer). Same as X.
		b		-		Any type as binary.
		c		-		Single character (char or wchar_t).
		s		-		String (char*).
		B		-		Boolean (bool) (as text).
		rx		-		Raw bytes as HEX pairs (lower-case). 
						If no width defined - next function parameter must be size of bytes to read. 
		rX		-		Raw bytes as HEX pairs (upper-case). 
						If no width defined - next function parameter must be size of bytes to read.
		ru	    -		Raw bytes as unsigned decimal bytes. 
						If no width defined - next function parameter must be size of bytes to read.
		rb		-		Raw bytes as binary sequence. 
						If no width defined - next function parameter must be size of bytes to read.

	*********************************************************************************************************

	Modifiers:
	*********************************************************************************************************

		l		-		Means 'long'. Application: Only for integer and floating point values.
							Supported data types: 'd', 'i', 'f', 'x', 'X', 'a', 'p', 'A', 'P', 'b'

		ll		-		Means 'long long'. Maximum size. Can be applied only to integer values.
							Supported data types: 'd', 'i', 'x', 'X', 'a', 'p', 'A', 'P', 'b'

		n		-		No delimiter for raw data. Bytes will be presented without any delimiters. 
						Can be applied only to raw data. Not works with '#' prefix.
							Supported data types: 'rx', 'rX', 'ru', 'rb'.

		t		-		Custom delimiter for raw data. Can be applied only to raw data.
						When used: You able to put '()' on the end of pattern to specify the delimiter.
						Example of usage: '%trx(;)' - All raw data bytes will be delimited by ';'.
						If brackets are not used with this modifier - all data will be delimited by spaces.
							Supported data types: 'rx', 'rX', 'ru', 'rb'.

		z		-		Explicitly specify string type. Widechar or multibyte. 
						Can be applied only to string or character.
						When used: You able to put '.' on the end of pattern to specify the type of string.
						'.a' Means ANSI string (or multibyte). '.w' Means wide-char string.
						Example of usage: '%zc.a' or '%zs.w'.
							Supported data types: 's', 'c'.

	*********************************************************************************************************

	All possible combinations of modificators and data types:
	ld, li, lld, lli
	lu, llu
	lb, llb
	lf
	lx, llx, lX, llX
	la, lla, lA, llA
	lp, llp, lP, llP
	lb, llb
	nr, nR, nru, nrb		(Works only without '#' prefix)
	trx, trX, tru, trb
	zs, zc

	Exapmles:
	xstr.Format("Int some = %i", i);									//Int some = 28
	xstr.Format("Progress: %i%%", prog);								//Progress: 95%
	xstr.Format("Length of str %s is %u", buf, len);					//Length of str Tokyo is 5
	xstr.Format("Raw bytes: %#r", buf, size);							//Raw bytes: 0x1F 0x56 0x12 0xA7 0x84 0xD1
	xstr.Format("Raw bytes: %#[4]r", buf);								//Raw bytes: 0x1F 0x56 0x12 0xA7
	xstr.Format("Raw bytes: %[4]r", buf);								//Raw bytes: 1F 56 12 A7
	xstr.Format("Very long number as HEX: %#llX", some);				//Very long number as HEX: 0x12415FX225
	xstr.Format("First 3 chars of str is %[3]s", buf);					//First 3 chars of str is Tok
	
	double d = 2.842146;
	xstr.Format("d with precision 3 is %[3]f", d);						//d with precision 3 is 2.842
*/

#include <Windows.h>
#include <string>

class _xstringA;
class _xstringW;

typedef volatile _xstringW xstringW;
typedef volatile _xstringA xstringA;

#ifdef UNICODE

typedef xstringW xstring;

#else

typedef xstringA xstring;

#endif

class _xstringA
{
public:
	/*class Iterator
	{
	private:
		xstringA* m_pstr;
		size_t m_i;
		size_t m_from;
		size_t m_maxcount;
		size_t m_count;
		bool m_looped;
		bool m_reversed;
		bool m_reached;

	public:
		Iterator(xstringA &str);
		bool Go();
		void Reversed(bool IsReversed);
		void Looped(bool IsLooped);
		void SetBounds(size_t start, size_t count = 0);
		char Get();
		void Reset(xstringA &str);
		void __fastcall Set(char c);

	};*/

private:
	//friend Iterator;
	volatile char* m_lpcStr;
	volatile char* m_lpXorKey;
	volatile size_t m_stLen;
	volatile size_t m_stxKeySize;
	volatile HANDLE m_hMutex;
	volatile bool m_bXorStr;

	void __fastcall _setStr(const char* str, bool HoldMutex = true) volatile;
	void __fastcall _decrypt(char* output) const volatile;
	void _generateKey(size_t strl) volatile;
	size_t __fastcall _getKeyIndexForIndex(size_t i) const volatile;
	void __fastcall _append(const char* str, bool HoldMutex) volatile;
	void __fastcall _push(const char c, bool HoldMutex) volatile;

public:
	_xstringA();
	_xstringA(bool xorstr);
	_xstringA(volatile const char* str, bool xor = true);
	_xstringA(const char* str, bool xor = true);
	_xstringA(std::string str, bool xor = true);
	//_xstringA(const xstringA &str);
	_xstringA(xstringA &str);
	~_xstringA();

	const std::string GetStr() const volatile;
	const std::string GetStr_a() const volatile;
	const std::wstring GetStr_w() const volatile;
	size_t GetLen() const volatile;
	std::string GetReversed() const volatile;

	void __fastcall Append(const xstringA& str) volatile;
	void __fastcall Append(const char* str) volatile;
	void __fastcall Append(const std::string str) volatile;
	void __fastcall Push(const char c) volatile;

	void __fastcall Format(const char* fmt, ...) volatile;
	void __fastcall AppendFormat(const char* fmt, ...) volatile;

	void __fastcall Encryption(bool enabled) volatile;

	void __fastcall ReInit(const char* str, bool xor = true) volatile;

	bool __fastcall SetChar(const char c, size_t n) volatile;

	volatile _xstringA& operator=(const volatile _xstringA& right) volatile;
	volatile _xstringA& operator=(const char* right) volatile;
	volatile _xstringA& operator=(const std::string right) volatile;

	volatile _xstringA& operator+=(const volatile _xstringA& right) volatile;
	volatile _xstringA& operator+=(const char* right) volatile;
	volatile _xstringA& operator+=(char right) volatile;
	volatile _xstringA& operator+=(const std::string right) volatile;

	friend bool operator==(const volatile _xstringA left, const volatile _xstringA& right);
	friend bool operator!=(const volatile _xstringA left, const volatile _xstringA& right);

	friend bool operator==(const volatile _xstringA left, const std::string& right);
	friend bool operator!=(const volatile _xstringA left, const std::string& right);

	friend bool operator==(const volatile _xstringA left, const char* right);
	friend bool operator!=(const volatile _xstringA left, const char* right);

	friend xstringA operator+(xstringA left, const xstringA& right);
	friend xstringA operator+(xstringA left, const char* right);
	friend xstringA operator+(xstringA left, char right);
	friend xstringA operator+(xstringA left, std::string right);

	operator const std::string() const volatile;
	char operator [] (size_t n) const volatile;

};

class _xstringW
{
private:
	volatile wchar_t* m_lpcwStr;
	volatile char* m_lpXorKey;
	volatile size_t m_stLen;
	volatile size_t m_stxKeySize;
	volatile HANDLE m_hMutex;
	volatile bool m_bXorStr;

	void __fastcall _setStr(const wchar_t* str, bool HoldMutex = true) volatile;
	void __fastcall _decrypt(wchar_t* output) const volatile;
	void _generateKey(size_t strl) volatile;
	size_t __fastcall _getKeyIndexForIndex(size_t i) const volatile;
	wchar_t __fastcall _cryptWchar(size_t index, wchar_t _wc = 0, bool custom = false) const volatile;
	void __fastcall _append(const wchar_t* str, bool HoldMutex) volatile;
	void __fastcall _push(const wchar_t c, bool HoldMutex) volatile;

public:
	_xstringW();
	_xstringW(bool xorstr);
	_xstringW(volatile const wchar_t* str, bool xor = true);
	_xstringW(const wchar_t* str, bool xor = true);
	_xstringW(const std::wstring str, bool xor = true);
	//_xstringW(const xstringW &str);
	_xstringW(xstringW &str);
	~_xstringW();

	const std::wstring GetStr() const volatile;
	const std::wstring GetStr_w() const volatile;
	const std::string GetStr_a() const volatile;
	size_t GetLen() const volatile;
	std::wstring GetReversed() const volatile;

	void __fastcall Append(const xstringW& str) volatile;
	void __fastcall Append(const wchar_t* str) volatile;
	void __fastcall Append(const std::wstring str) volatile;
	void __fastcall Push(const wchar_t c) volatile;

	void __fastcall Format(const wchar_t* fmt, ...) volatile;
	void __fastcall AppendFormat(const wchar_t* fmt, ...) volatile;

	void __fastcall Encryption(bool enabled) volatile;

	void __fastcall ReInit(const wchar_t* str, bool xor = true) volatile;

	bool __fastcall SetChar(const wchar_t c, size_t n) volatile;

	volatile _xstringW& operator=(const volatile _xstringW& right) volatile;
	volatile _xstringW& operator=(const wchar_t* right) volatile;
	volatile _xstringW& operator=(const std::wstring right) volatile;

	volatile _xstringW& operator+=(const volatile _xstringW& right) volatile;
	volatile _xstringW& operator+=(const wchar_t* right) volatile;
	volatile _xstringW& operator+=(wchar_t right) volatile;
	volatile _xstringW& operator+=(const std::wstring right) volatile;

	friend bool operator==(const volatile _xstringW left, const volatile _xstringW& right);
	friend bool operator!=(const volatile _xstringW left, const volatile _xstringW& right);

	friend bool operator==(const volatile _xstringW left, const std::wstring& right);
	friend bool operator!=(const volatile _xstringW left, const std::wstring& right);

	friend bool operator==(const volatile _xstringW left, const wchar_t* right);
	friend bool operator!=(const volatile _xstringW left, const wchar_t* right);

	friend xstringW operator+(xstringW left, const xstringW& right);
	friend xstringW operator+(xstringW left, const wchar_t* right);
	friend xstringW operator+(xstringW left, wchar_t right);
	friend xstringW operator+(xstringW left, const std::wstring& right);

	operator const std::wstring() const volatile;
	wchar_t operator [] (size_t n) const volatile;
};