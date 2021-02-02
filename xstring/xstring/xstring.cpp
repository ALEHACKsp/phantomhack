#define _CRT_SECURE_NO_WARNINGS
#include "../../shared/xstring/xstring.h"
#include <ctime>
#include <random>
#include <iomanip>
#include <sstream>

#define stime (UINT)time(NULL)

namespace Utils
{
	enum
	{
		INT_TYPE_DECIMAL,
		INT_TYPE_HEX,
		INT_TYPE_BIN
	};

	template <typename T>
	T RandomBetween(T min, T max)
	{
		static bool rinit = false;
		static std::default_random_engine generator;
		if (!rinit)
		{
			generator.seed(stime);
			rinit = true;
		}
		std::uniform_int_distribution<T> distribution(min, max);
		T dice_roll = distribution(generator);
		return dice_roll;
	}

	template <typename T>
	size_t __fastcall IntLength(T num, int type = INT_TYPE_DECIMAL)
	{
		size_t len = 0;
		if (num == 0)
			num = 1;
		switch (type)
		{
		case INT_TYPE_DECIMAL:
			if (num < 0)
				len++;
			while (num != 0)
			{
				num /= 10;
				len++;
			}
			break;
		case INT_TYPE_BIN:
			len = sizeof(num) * 8;
			break;
		case INT_TYPE_HEX:
			while (num != 0)
			{
				num /= 16;
				len++;
			}
			break;
		default:
			__assume(0);
		}
		return len;
	}

	inline int ipow(int a, unsigned int b)
	{
		if (b == 0)
			return 1;

		int p = a;
		for (unsigned int i = 1; i < b; i++)
		{
			p *= a;
		}

		return p;
	}

	template <typename fT>
	size_t __fastcall FloatLength(fT num)
	{
		size_t len = 1;
		if (num == 0.0)
			return 3;
		if (num < 0.0)
			len++;

		fT sig = 0, mant = 0, tmant = 0;
		mant = modf(num, &sig);
		tmant = mant;
		unsigned __int64 imant = 0, isig = (unsigned __int64)sig;

		len += IntLength<unsigned __int64>(isig);

		int x = 1;
		while (tmant != 0.0)
		{
			tmant = mant * ipow(10, x);
			tmant = modf(tmant, &sig);
			x++;
		}
		mant *= ipow(10, x);
		imant = (unsigned __int64)mant;

		len += IntLength<unsigned __int64>(imant);

		return len;
	}

	size_t __fastcall BoolLength(bool b)
	{
		if (b)
			return 4;
		else
			return 5;
	}

	int __fastcall CharToDig(char c)
	{
		switch (c)
		{
		case '0':
			return 0;
			break;
		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;
		case '3':
			return 3;
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
		default:
			return 0;
		}
	}

	char __fastcall DigToChar(int d)
	{
		if (IntLength<int>(d) > 1)
			return 0;

		switch (d)
		{
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
			break;
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		default:
			__assume(0);
		}
	}

	bool __fastcall IsDigit(char c)
	{
		switch (c)
		{
		case '0':
			return true;
			break;
		case '1':
			return true;
			break;
		case '2':
			return true;
			break;
		case '3':
			return true;
			break;
		case '4':
			return true;
			break;
		case '5':
			return true;
			break;
		case '6':
			return true;
			break;
		case '7':
			return true;
			break;
		case '8':
			return true;
			break;
		case '9':
			return true;
			break;
		default:
			return false;
		}
	}

	void __fastcall ToLower(char* str)
	{
		size_t len = strlen(str);
		for (size_t i = 0; i < len; i++)
			str[i] = tolower(str[i]);
	}

	void __fastcall ToUpper(char* str)
	{
		size_t len = strlen(str);
		for (size_t i = 0; i < len; i++)
			str[i] = toupper(str[i]);
	}

	void __fastcall wToLower(wchar_t* str)
	{
		size_t len = wcslen(str);
		for (size_t i = 0; i < len; i++)
			str[i] = towlower(str[i]);
	}

	void __fastcall wToUpper(wchar_t* str)
	{
		size_t len = wcslen(str);
		for (size_t i = 0; i < len; i++)
			str[i] = towupper(str[i]);
	}

	size_t ftoa_s(char* buf, size_t bufSize, float f, short int pre = -1)
	{
		size_t len = FloatLength<float>(f);
		if (len + 1 > bufSize)
			return 0;

		std::ostringstream ss;
		if (pre < 0)
			ss << f;
		else
			ss << std::fixed << std::setprecision(pre) << f;
		strcpy_s(buf, bufSize, ss.str().c_str());
		return strlen(buf);
	}

	size_t dtoa_s(char* buf, size_t bufSize, double d, short int pre = -1)
	{
		size_t len = FloatLength<double>(d);
		if (len + 1 > bufSize)
			return 0;

		std::ostringstream ss;
		if (pre < 0)
			ss << d;
		else
			ss << std::fixed << std::setprecision(pre) << d;
		strcpy_s(buf, bufSize, ss.str().c_str());
		return strlen(buf);
	}

	size_t ldtoa_s(char* buf, size_t bufSize, long double d, short int pre = -1)
	{
		size_t len = FloatLength<double>(d);
		if (len + 1 > bufSize)
			return 0;

		std::ostringstream ss;
		if (pre < 0)
			ss << d;
		else
			ss << std::fixed << std::setprecision(pre) << d;
		strcpy_s(buf, bufSize, ss.str().c_str());
		return strlen(buf);
	}

	size_t ftow_s(wchar_t* buf, size_t bufSize, float f, short int pre = -1)
	{
		size_t len = FloatLength<float>(f);
		if (len + 1 > bufSize)
			return 0;

		std::wostringstream ss;
		if (pre < 0)
			ss << f;
		else
			ss << std::fixed << std::setprecision(pre) << f;
		wcscpy_s(buf, bufSize, ss.str().c_str());
		return wcslen(buf);
	}

	size_t dtow_s(wchar_t* buf, size_t bufSize, double d, short int pre = -1)
	{
		size_t len = FloatLength<double>(d);
		if (len + 1 > bufSize)
			return 0;

		std::wostringstream ss;
		if (pre < 0)
			ss << d;
		else
			ss << std::fixed << std::setprecision(pre) << d;
		wcscpy_s(buf, bufSize, ss.str().c_str());
		return wcslen(buf);
	}

	size_t ldtow_s(wchar_t* buf, size_t bufSize, long double d, short int pre = -1)
	{
		size_t len = FloatLength<double>(d);
		if (len + 1 > bufSize)
			return 0;

		std::wostringstream ss;
		if (pre < 0)
			ss << d;
		else
			ss << std::fixed << std::setprecision(pre) << d;
		wcscpy_s(buf, bufSize, ss.str().c_str());
		return wcslen(buf);
	}

}


// *******************
// ******* ANSI ******
// *******************
/*
xstringA::Iterator::Iterator(xstringA &str)
{
	m_pstr = &str;
	m_i = 0;
	m_from = 0;
	m_maxcount = 0;
	m_looped = false;
	m_reversed = false;
	m_reached = false;
	m_count = 0;
}

bool xstringA::Iterator::Go()
{
	if (m_reached)
		return false;

	if (m_reversed)
	{
		if (m_looped)
		{
			
			if (m_count == m_maxcount)
				m_reached = true;
			else
			{
				m_i--;
				m_count++;
			}
		}
		else
		{
			if (m_count == m_maxcount)
				m_reached = true;
			else
			{
				m_i--;
			}
			if (m_i == 0)
				m_reached = true;
		}
	}
	else
	{
		if (m_looped)
		{
			if(m_count == m_maxcount)
				m_reached = true;
		}
		else
		{
			if (m_count == m_maxcount)
				m_reached = true;
			if (m_i == m_pstr->m_stLen - 1)
				m_reached = true;
		}
	}

	return true;
}

void xstringA::Iterator::Reset(xstringA &str)
{
	m_pstr = &str;
	m_i = 0;
	m_from = 0;
	m_maxcount = 0;
	m_looped = false;
	m_reversed = false;
	m_reached = false;
	m_count = 0;
}


void xstringA::Iterator::Reversed(bool IsReversed)
{
	m_reversed = IsReversed;
}

void xstringA::Iterator::Looped(bool IsLooped)
{
	m_looped = IsLooped;
}

void xstringA::Iterator::SetBounds(size_t start, size_t count)
{
	m_i = 0;
	m_from = start;
	m_maxcount = count;
	m_reached = false;
}

char xstringA::Iterator::Get()
{
	return m_pstr->operator[](m_i);
}

void __fastcall xstringA::Iterator::Set(char c)
{
	m_pstr->SetChar(c, m_i);
}*/


_xstringA::_xstringA()
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_lpcStr = NULL;
	m_bXorStr = true;
	m_lpXorKey = NULL;
	m_stxKeySize = 0;
}

_xstringA::_xstringA(bool xorstr)
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_lpcStr = NULL;
	m_bXorStr = xorstr;
	m_lpXorKey = NULL;
	m_stxKeySize = 0;
}

_xstringA::_xstringA(volatile const char* str, bool xor)
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_bXorStr = xor;
	m_stxKeySize = 0;
	_setStr(const_cast<const char*>(str), true);
}

_xstringA::_xstringA(const char* str, bool xor)
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_bXorStr = xor;
	m_stxKeySize = 0;
	_setStr(str, true);
}

_xstringA::_xstringA(std::string str, bool xor)
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_bXorStr = xor;
	m_stxKeySize = 0;
	_setStr(str.c_str(), true);
}

/*_xstringA::_xstringA(const xstringA &str)
{
m_hMutex = CreateMutex(NULL, FALSE, NULL);
m_stLen = 0;
m_stxKeySize = 0;
m_bXorStr = str.m_bXorStr;
this->_setStr(str.GetStr().c_str(), true);
}*/

_xstringA::_xstringA(xstringA &str)
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_stLen = 0;
	m_stxKeySize = 0;
	m_bXorStr = str.m_bXorStr;
	this->_setStr(str.GetStr().c_str(), true);
}

_xstringA::~_xstringA()
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen > 0)
		delete[] m_lpcStr;
	if (m_stxKeySize > 0)
		delete[] m_lpXorKey;
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

void __fastcall _xstringA::_setStr(const char* str, bool HoldMutex) volatile
{
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen > 0)
		delete[] m_lpcStr;
	size_t strl = strlen(str);
	m_lpcStr = new volatile char[strl + 1];

	if (m_bXorStr)
		_generateKey(strl);

	if (m_bXorStr)
	{
		size_t j = 0;
		for (UINT i = 0; i < strl; i++)
		{
			m_lpcStr[i] = str[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
	}
	else
	{
		for (UINT i = 0; i < strl; i++)
			m_lpcStr[i] = str[i];
	}
	m_lpcStr[strl] = '\0';

	m_stLen = strl;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

const std::string _xstringA::GetStr() const volatile
{
	return GetStr_a();
}

const std::string _xstringA::GetStr_a() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	std::string tmp;
	char* decrypted;
	if (m_bXorStr)
	{
		decrypted = new char[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<char*>(m_lpcStr);

	if (m_stLen > 0)
		tmp = const_cast<const char*>(decrypted);
	else
		tmp = "";

	if (m_bXorStr)
		delete[] decrypted;
	ReleaseMutex(m_hMutex);
	return tmp.c_str();
}

const std::wstring _xstringA::GetStr_w() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	std::string tmp;
	std::wstring wtmp;
	char* decrypted;
	if (m_bXorStr)
	{
		decrypted = new char[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<char*>(m_lpcStr);
	if (m_stLen > 0)
	{
		tmp = const_cast<const char*>(decrypted);
		size_t size = tmp.length() + 1;
		wchar_t* buf = new wchar_t[size];
		std::mbstowcs(buf, tmp.c_str(), size);
		wtmp = buf;
		delete[] buf;
	}
	else
		wtmp = L"";

	if (m_bXorStr)
		delete[] decrypted;
	ReleaseMutex(m_hMutex);
	return wtmp.c_str();
}

size_t _xstringA::GetLen() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	size_t len = m_stLen;
	ReleaseMutex(m_hMutex);
	return len;
}

std::string _xstringA::GetReversed() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	char* decrypted;
	char* reversed;
	std::string res;
	if (m_bXorStr)
	{
		decrypted = new char[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<char*>(m_lpcStr);
	reversed = new char[m_stLen + 1];

	size_t last_index = m_stLen - 1;
	for (size_t i = last_index; true; i--)
	{
		reversed[last_index - i] = decrypted[i];
		if (i == 0)
			break;
	}

	res = reversed;
	if (m_bXorStr)
		delete[] decrypted;
	delete[] reversed;
	ReleaseMutex(m_hMutex);
	return res;
}

void __fastcall _xstringA::Append(const xstringA& str) volatile
{
	Append(str.GetStr().c_str());
}

void __fastcall _xstringA::Append(const char* str) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	volatile char* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile char[m_stLen + 1];
			_decrypt(const_cast<char*>(tmp));
			delete[] m_lpcStr;
		}
		else
			tmp = m_lpcStr;
	}

	size_t oldstrl = m_stLen;
	size_t strl = strlen(str);
	size_t newstrl = m_stLen + strl;
	m_lpcStr = new volatile char[newstrl + 1];

	if (m_bXorStr)
		_generateKey(strl);

	UINT i = 0, j = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = tmp[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = tmp[i];
		i++;
	}

	UINT k = 0;
	while (k < strl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = str[k] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = str[k];
		i++; k++;
	}

	m_lpcStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::_append(const char* str, bool HoldMutex) volatile
{
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	volatile char* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile char[m_stLen + 1];
			_decrypt(const_cast<char*>(tmp));
			delete[] m_lpcStr;
		}
		else
			tmp = m_lpcStr;
	}

	size_t oldstrl = m_stLen;
	size_t strl = strlen(str);
	size_t newstrl = m_stLen + strl;
	m_lpcStr = new volatile char[newstrl + 1];

	if (m_bXorStr)
		_generateKey(strl);

	UINT i = 0, j = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = tmp[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = tmp[i];
		i++;
	}

	UINT k = 0;
	while (k < strl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = str[k] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = str[k];
		i++; k++;
	}

	m_lpcStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::Append(const std::string str) volatile
{
	Append(str.c_str());
}

void __fastcall _xstringA::Push(const char c) volatile
{
	if (c == '\0')
		return;
	WaitForSingleObject(m_hMutex, INFINITE);
	volatile char* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile char[m_stLen + 1];
			_decrypt(const_cast<char*>(tmp));
			delete[] m_lpcStr;
		}
		else
			tmp = m_lpcStr;
	}

	size_t oldstrl = m_stLen;
	size_t newstrl = m_stLen + 1;
	m_lpcStr = new volatile char[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl);

	UINT i = 0, j = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = tmp[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = tmp[i];
		i++;
	}

	if (m_bXorStr)
		m_lpcStr[i] = c ^ m_lpXorKey[j];
	else
		m_lpcStr[i] = c;
	i++;
	m_lpcStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::_push(const char c, bool HoldMutex) volatile
{
	if (c == '\0')
		return;
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	volatile char* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile char[m_stLen + 1];
			_decrypt(const_cast<char*>(tmp));
			delete[] m_lpcStr;
		}
		else
			tmp = m_lpcStr;
	}

	size_t oldstrl = m_stLen;
	size_t newstrl = m_stLen + 1;
	m_lpcStr = new volatile char[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl);

	UINT i = 0, j = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
		{
			m_lpcStr[i] = tmp[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
		else
			m_lpcStr[i] = tmp[i];
		i++;
	}

	if (m_bXorStr)
		m_lpcStr[i] = c ^ m_lpXorKey[j];
	else
		m_lpcStr[i] = c;
	i++;
	m_lpcStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

bool __fastcall _xstringA::SetChar(const char c, size_t n) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen == 0 || n < 0 || n > m_stLen)
	{
		ReleaseMutex(m_hMutex);
		return false;
	}
	if (m_bXorStr)
		m_lpcStr[n] = c ^ m_lpXorKey[_getKeyIndexForIndex(n)];
	else
		m_lpcStr[n] = c;
	ReleaseMutex(m_hMutex);
	return true;
}

bool operator==(const volatile _xstringA left, const volatile _xstringA& right)
{
	if (left.m_hMutex == right.m_hMutex)
		return true;

	if (strcmp(left.GetStr().c_str(), right.GetStr().c_str()) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringA left, const volatile _xstringA& right)
{
	if (left.m_hMutex == right.m_hMutex)
		return false;

	if (strcmp(left.GetStr().c_str(), right.GetStr().c_str()) == 0)
		return false;
	else
		return true;
}


bool operator==(const volatile _xstringA left, const char* right)
{
	if (strcmp(left.GetStr().c_str(), right) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringA left, const char* right)
{
	if (strcmp(left.GetStr().c_str(), right) == 0)
		return false;
	else
		return true;
}

bool operator==(const volatile _xstringA left, const std::string& right)
{
	if (strcmp(left.GetStr().c_str(), right.c_str()) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringA left, const std::string& right)
{
	if (strcmp(left.GetStr().c_str(), right.c_str()) == 0)
		return false;
	else
		return true;
}

char _xstringA::operator [] (size_t n) const volatile
{
	char c;
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen == 0 || n < 0 || n > m_stLen)
		c = '\0';
	else
	{
		if (m_bXorStr)
			c = m_lpcStr[n] ^ m_lpXorKey[_getKeyIndexForIndex(n)];
		else
			c = m_lpcStr[n];
	}
	ReleaseMutex(m_hMutex);
	return c;
}

volatile _xstringA& _xstringA::operator=(const volatile _xstringA& right) volatile
{
	if (this == &right)
		return *this;

	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(const_cast<const char*>(right.GetStr().c_str()), false);
	ReleaseMutex(m_hMutex);
	return *this;
}

volatile _xstringA& _xstringA::operator=(const char* right) volatile
{
	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(right, false);
	ReleaseMutex(m_hMutex);
	return *this;
}

volatile _xstringA& _xstringA::operator=(const std::string right) volatile
{
	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(right.c_str(), false);
	ReleaseMutex(m_hMutex);
	return *this;
}

void __fastcall _xstringA::ReInit(const char* str, bool xor) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	m_bXorStr = xor;
	_setStr(str, false);
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::Encryption(bool enabled) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_bXorStr == enabled)
	{
		ReleaseMutex(m_hMutex);
		return;
	}

	if (m_stLen == 0)
	{
		m_bXorStr = enabled;
		ReleaseMutex(m_hMutex);
		return;
	}

	char* tmp;
	m_bXorStr = enabled;
	if (enabled)
	{
		tmp = new char[m_stLen + 1];
		strcpy_s(tmp, m_stLen + 1, const_cast<char*>(m_lpcStr));
		_setStr(tmp, false);
		delete[] tmp;
	}
	else
	{
		tmp = new char[m_stLen + 1];
		_decrypt(tmp);
		_setStr(tmp, false);
		delete[] tmp;
	}
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::_decrypt(char* output) const volatile
{
	size_t j = 0;
	if (m_stLen != 0)
	{
		for (size_t i = 0; i < m_stLen; i++)
		{
			output[i] = m_lpcStr[i] ^ m_lpXorKey[j];
			j++;

			if (j >= m_stxKeySize)
				j = 0;
		}
	}
	output[m_stLen] = '\0';
}

size_t __fastcall _xstringA::_getKeyIndexForIndex(size_t i) const volatile
{
	if (i < m_stxKeySize)
		return i;

	size_t t = i / m_stxKeySize;
	t = m_stxKeySize * t;
	t = i - t;
	return t;
}

void _xstringA::_generateKey(size_t strl) volatile
{
	if (m_stxKeySize > 0)
		delete[] m_lpXorKey;

	if (strl <= 25)
		m_stxKeySize = strl;
	else
		m_stxKeySize = Utils::RandomBetween<size_t>(11, 25);

	if (m_stxKeySize > 0)
		m_lpXorKey = new volatile char[m_stxKeySize];
	else
		m_lpXorKey = NULL;

	UCHAR uc = 0;
	for (size_t i = 0; i < m_stxKeySize; i++)
	{
		uc = (UCHAR)Utils::RandomBetween<unsigned short>(0, 255);
		m_lpXorKey[i] = *(reinterpret_cast<char*>(&uc));
	}
}

volatile _xstringA& _xstringA::operator+=(const volatile _xstringA& right) volatile
{
	Append(right);
	return *this;
}

volatile _xstringA& _xstringA::operator+=(const char* right) volatile
{
	Append(right);
	return *this;
}

volatile _xstringA& _xstringA::operator+=(char right) volatile
{
	Push(right);
	return *this;
}

volatile _xstringA& _xstringA::operator+=(const std::string right) volatile
{
	Append(right.c_str());
	return *this;
}

xstringA operator+(xstringA left, const xstringA& right)
{
	xstringA xs = left;
	xs += right;
	return xs;
}

xstringA operator+(xstringA left, const char* right)
{
	xstringA xs = left;
	xs += right;
	return xs;
}

xstringA operator+(xstringA left, char right)
{
	xstringA xs = left;
	xs += right;
	return xs;
}

xstringA operator+(xstringA left, std::string right)
{
	xstringA xs = left;
	xs += right;
	return xs;
}

_xstringA::operator const std::string() const volatile
{
	return GetStr_a();
}

void __fastcall _xstringA::Format(const char* fmt, ...) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	va_list ap;
	size_t newlen = 0;
	size_t flen = strlen(fmt);
	va_start(ap, fmt);
	char tmp[65]; tmp[0] = 0;
	char delim[64]; delim[0] = 0;
	size_t j = 0, bs = 0, itmp = 0, m = 0;
	bool limited = false;
	bool prefix = false;
	bool nodelim = false;
	bool zm = false; char zc = 0;
	unsigned long long int ulli;
	unsigned long int uli;
	unsigned int ui;
	long long int lli;
	long int li;
	int _i;
	char c;
	wchar_t wc;

	//float fl;
	double doub;
	long double ldoub;
	bool _b;
	char* astrptr;
	wchar_t* wstrptr;
	BYTE* bptr;

	_setStr("", false);
	for (size_t i = 0; i < flen; i++)
	{
		if (fmt[i] == '%')
		{
			i++; limited = false; prefix = false; nodelim = false; bs = 0; itmp = 0;  zm = false;
			if (fmt[i] == '\0')
				break;

			switch (fmt[i])
			{
			case '%':
				_push(fmt[i], false);
				break;

			case '#':
				prefix = true;
				i++;
				switch (fmt[i])
				{
				case '[':
					i++; j = 0;
					while (fmt[i] != ']')
					{
						tmp[j] = fmt[i];
						j++; i++;
					}
					tmp[j] = '\0';
					bs = (size_t)(atoi(tmp));
					limited = true;

					i++;
					if (fmt[i] == 't')
					{
						itmp = i + 3; j = 0;
						if (fmt[itmp] == '(')
						{
							itmp++;
							while (fmt[itmp] != ')')
							{
								delim[j] = fmt[itmp];
								j++; itmp++;
							}
							if (j == 0)
								nodelim = true;
							else
								delim[j] = '\0';
						}
						else
						{
							delim[0] = ' '; delim[1] = 0;
							itmp = i + 1;
						}
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp = i + 1;
					}

					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								_append("0b", false);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}


					switch (fmt[i])
					{
					case 'l':
						i++;
						if (fmt[i] == 'l')
						{
							i++;
							switch (fmt[i])
							{
							case 'b':
								ulli = va_arg(ap, unsigned long long int);
								_ui64toa_s(ulli, tmp, 65, 2);
								itmp = strlen(tmp);
								_append("0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64toa_s(ulli, tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64toa_s(ulli, tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						else
						{
							switch (fmt[i])
							{
							case 'b':
								uli = va_arg(ap, unsigned long int);
								_ui64toa_s(uli, tmp, 65, 2);
								itmp = strlen(tmp);
								_append("0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64toa_s(uli, tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64toa_s(uli, tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						break;

					case 'b':
						ui = va_arg(ap, unsigned int);
						_ui64toa_s(ui, tmp, 65, 2);
						itmp = strlen(tmp);
						_append("0b", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64toa_s(ui, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append("0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64toa_s(ui, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append("0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
						break;
					}
					break;


				case 'l':
					i++;
					if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;

						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 2);
							_append("0b", false);
							_append(tmp, false);
							break;

						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;

						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 2);
							_append("0b", false);
							_append(tmp, false);
							break;
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp--;
					}
					i++;


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								_append("0b", false);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'r':

					i++;
					switch (fmt[i])
					{
					case 'b':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							Utils::ToUpper(tmp);
							_append("0b", false);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToLower(tmp);
					_append("0x", false);
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToUpper(tmp);
					_append("0x", false);
					_append(tmp, false);
					break;

				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 2);
					_append("0b", false);
					_append(tmp, false);
					break;

				}
				break;

			case '[':
				i++; j = 0;
				while (fmt[i] != ']')
				{
					tmp[j] = fmt[i];
					j++; i++;
				}
				tmp[j] = '\0';
				bs = (size_t)(atoi(tmp));
				limited = true;

				i++;
				switch (fmt[i])
				{
				case 'z':
					if (fmt[i + 1] == 's')
					{
						zm = true;
						if (fmt[i + 2] == '.')
						{
							zc = fmt[i + 3];
							m = 3;
						}
						else
						{
							zc = 'a';
							m = 1;
						}
							

						if(zc != 'w')
							zc = 'a';
					}
					else
						break;
				case 's':
					if (zm)
					{
						switch (zc)
						{
							case 'w':
								wstrptr = va_arg(ap, wchar_t*);
								astrptr = new char[bs+1];
								std::wcstombs(astrptr, wstrptr, bs);
								astrptr[bs] = '\0';
								break;
							case 'a':
							default:
								astrptr = va_arg(ap, char*);
						}

						for (size_t q = 0; q < bs; q++)
							_push(astrptr[q], false);

						if (zm && zc == 'w')
							delete[] astrptr;

						i += m;
					}
					else
					{
						astrptr = va_arg(ap, char*);
						for (size_t q = 0; q < bs; q++)
						{
							_push(astrptr[q], false);
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						i++;
						itmp = i + 1;
					}


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'u':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 10);
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'n':
					nodelim = true;
					i++;
				case 'r':
					i++;
					switch (fmt[i])
					{
					case 'u':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'b':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'l':

					i++;
					if (fmt[i] == 'f')
					{
						ldoub = va_arg(ap, long double);
						Utils::ldtoa_s(tmp, 65, ldoub, (short)bs);
						_append(tmp, false);
					}
					else if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							lli = va_arg(ap, long long int);
							itmp = Utils::IntLength<long long int>(lli, Utils::INT_TYPE_DECIMAL);
							_i64toa_s(lli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_DECIMAL);
							_ui64toa_s(ulli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 10);
							itmp = strlen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							li = va_arg(ap, long int);
							itmp = Utils::IntLength<long int>(li, Utils::INT_TYPE_DECIMAL);
							_i64toa_s(li, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_DECIMAL);
							_ui64toa_s(uli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 10);
							itmp = strlen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}

					break;
				case 'f':
					doub = va_arg(ap, double);
					Utils::dtoa_s(tmp, 65, doub, (short)bs);
					_append(tmp, false);
					break;

				case 'd':
				case 'i':
					_i = va_arg(ap, int);
					itmp = Utils::IntLength<int>(_i, Utils::INT_TYPE_DECIMAL);
					_i64toa_s(_i, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToLower(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToUpper(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'u':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_DECIMAL);
					_ui64toa_s(ui, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 10);
					itmp = strlen(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				}

				break;


			case 'l':
				i++;
				if (fmt[i] == 'l')
				{
					i++;
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						lli = va_arg(ap, long long int);
						_i64toa_s(lli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 10);
						_append(tmp, false);
						break;
					case  'b':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 2);
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append(tmp, false);
						break;

					}
				}
				else
				{
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						li = va_arg(ap, long int);
						_itoa_s(li, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case 'f':
						ldoub = va_arg(ap, long double);
						Utils::ldtoa_s(tmp, 65, ldoub);
						_append(tmp, false);
						break;

					case 'a':
					case 'p':
					case 'x':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append(tmp, false);
						break;

					case 'A':
					case 'P':
					case 'X':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append(tmp, false);
						break;

						break;
						break;
					case 'b':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 2);
						_append(tmp, false);
						break;

					}
				}
				break;

			case 't':

				itmp = i + 3; j = 0;
				if (fmt[itmp] == '(')
				{
					itmp++;
					while (fmt[itmp] != ')')
					{
						delim[j] = fmt[itmp];
						j++; itmp++;
					}
					if (j == 0)
						nodelim = true;
					else
						delim[j] = '\0';
				}
				else
				{
					delim[0] = ' '; delim[1] = 0;
					itmp--;
				}
				i++;


				if (fmt[i] == 'r')
				{
					i++;
					switch (fmt[i])
					{
					case 'u':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'b':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;
					case 'x':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'X':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					default:
						__assume(0);
					}


				}
				break;

			case 'i':
			case 'd':
				_i = va_arg(ap, int);
				_itoa_s(_i, tmp, 65, 10);
				_append(tmp, false);
				break;

			case  'u':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 10);
				_append(tmp, false);
				break;

			case 'f':
				doub = va_arg(ap, double);
				Utils::dtoa_s(tmp, 65, doub);
				_append(tmp, false);
				break;

			case 'x':
			case 'a':
			case 'p':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 16);
				Utils::ToLower(tmp);
				_append(tmp, false);
				break;

			case 'X':
			case 'A':
			case 'P':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 16);
				Utils::ToUpper(tmp);
				_append(tmp, false);
				break;

			case 'z':
				if (fmt[i + 1] == 's' || fmt[i + 1] == 'c')
				{
					zm = true;
					if (fmt[i + 2] == '.')
					{
						zc = fmt[i + 3];
						m = 3;
					}
					else 
					{
						zc = 'a';
						m = 1;
					}
						
					if (zc != 'w')
						zc = 'a';
				}
				else
					break;

			case 's':
				if (!(zm && fmt[i + 1] == 'c'))
				{
					if (zm)
					{
						switch (zc)
						{
						case 'w':
							wstrptr = va_arg(ap, wchar_t*);
							itmp = wcslen(wstrptr);
							astrptr = new char[itmp + 1];
							std::wcstombs(astrptr, wstrptr, itmp);
							astrptr[itmp] = '\0';
							break;
						case 'a':
						default:
							astrptr = va_arg(ap, char*);
						}

						_append(astrptr, false);

						if (zm && zc == 'w')
							delete[] astrptr;

						i += m;
					}
					else
					{
						astrptr = va_arg(ap, char*);
						_append(astrptr, false);
					}
					break;
				}

			case 'c':
				if (zm)
				{
					switch (zc)
					{
					case 'w':
						wc = va_arg(ap, wchar_t);
						wstrptr = new wchar_t[2];
						astrptr = new char[2];
						wstrptr[0] = wc; wstrptr[1] = '\0';
						std::wcstombs(astrptr, wstrptr, 1);
						c = astrptr[0];
						delete[] wstrptr;
						delete[] astrptr;
						break;
					case 'a':
					default:
						c = va_arg(ap, char);
					}

					_push(c, false);

					i += m;
				}	
				else
				{
					c = va_arg(ap, char);
					_push(c, false);
				}
				break;
			case 'b':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 2);
				Utils::ToUpper(tmp);
				_append(tmp, false);
				break;

			case 'B':
				_b = va_arg(ap, bool);
				if (_b)
					_append("true", false);
				else
					_append("false", false);
				break;

			case 'n':
				nodelim = true;
				i++;
			case 'r':
				i++;
				switch (fmt[i])
				{
				case 'd':
				case 'i':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 10);
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				case 'b':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 2);
						itmp = strlen(tmp);
						for (m = 0; m < 8 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;
				case 'x':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 16);
						Utils::ToLower(tmp);
						itmp = strlen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				case 'X':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 16);
						Utils::ToUpper(tmp);
						itmp = strlen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				default:
					__assume(0);
				}
				break;
			}
		}
		else
			_push(fmt[i], false);
	}

	va_end(ap);
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringA::AppendFormat(const char* fmt, ...) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	va_list ap;
	size_t newlen = 0;
	size_t flen = strlen(fmt);
	va_start(ap, fmt);
	char tmp[65]; tmp[0] = 0;
	char delim[64]; delim[0] = 0;
	size_t j = 0, bs = 0, itmp = 0, m = 0;
	bool limited = false;
	bool prefix = false;
	bool nodelim = false;
	bool zm = false; char zc = 0;
	unsigned long long int ulli;
	unsigned long int uli;
	unsigned int ui;
	long long int lli;
	long int li;
	int _i;
	char c;
	wchar_t wc;

	//float fl;
	double doub;
	long double ldoub;
	bool _b;
	char* astrptr;
	wchar_t* wstrptr;
	BYTE* bptr;

	for (size_t i = 0; i < flen; i++)
	{
		if (fmt[i] == '%')
		{
			i++; limited = false; prefix = false; nodelim = false; bs = 0; itmp = 0;  zm = false;
			if (fmt[i] == '\0')
				break;

			switch (fmt[i])
			{
			case '%':
				_push(fmt[i], false);
				break;

			case '#':
				prefix = true;
				i++;
				switch (fmt[i])
				{
				case '[':
					i++; j = 0;
					while (fmt[i] != ']')
					{
						tmp[j] = fmt[i];
						j++; i++;
					}
					tmp[j] = '\0';
					bs = (size_t)(atoi(tmp));
					limited = true;

					i++;
					if (fmt[i] == 't')
					{
						itmp = i + 3; j = 0;
						if (fmt[itmp] == '(')
						{
							itmp++;
							while (fmt[itmp] != ')')
							{
								delim[j] = fmt[itmp];
								j++; itmp++;
							}
							if (j == 0)
								nodelim = true;
							else
								delim[j] = '\0';
						}
						else
						{
							delim[0] = ' '; delim[1] = 0;
							itmp = i + 1;
						}
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp = i + 1;
					}

					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								_append("0b", false);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}


					switch (fmt[i])
					{
					case 'l':
						i++;
						if (fmt[i] == 'l')
						{
							i++;
							switch (fmt[i])
							{
							case 'b':
								ulli = va_arg(ap, unsigned long long int);
								_ui64toa_s(ulli, tmp, 65, 2);
								itmp = strlen(tmp);
								_append("0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64toa_s(ulli, tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64toa_s(ulli, tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						else
						{
							switch (fmt[i])
							{
							case 'b':
								uli = va_arg(ap, unsigned long int);
								_ui64toa_s(uli, tmp, 65, 2);
								itmp = strlen(tmp);
								_append("0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64toa_s(uli, tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64toa_s(uli, tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						break;

					case 'b':
						ui = va_arg(ap, unsigned int);
						_ui64toa_s(ui, tmp, 65, 2);
						itmp = strlen(tmp);
						_append("0b", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64toa_s(ui, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append("0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64toa_s(ui, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append("0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
						break;
					}
					break;


				case 'l':
					i++;
					if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;

						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 2);
							_append("0b", false);
							_append(tmp, false);
							break;

						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							_append(tmp, false);
							break;

						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 2);
							_append("0b", false);
							_append(tmp, false);
							break;
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp--;
					}
					i++;


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								_append("0b", false);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								_append("0x", false);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'r':

					i++;
					switch (fmt[i])
					{
					case 'b':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							Utils::ToUpper(tmp);
							_append("0b", false);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							_append("0x", false);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							_append("0x", false);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToLower(tmp);
					_append("0x", false);
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToUpper(tmp);
					_append("0x", false);
					_append(tmp, false);
					break;

				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 2);
					_append("0b", false);
					_append(tmp, false);
					break;

				}
				break;

			case '[':
				i++; j = 0;
				while (fmt[i] != ']')
				{
					tmp[j] = fmt[i];
					j++; i++;
				}
				tmp[j] = '\0';
				bs = (size_t)(atoi(tmp));
				limited = true;

				i++;
				switch (fmt[i])
				{
				case 'z':
					if (fmt[i + 1] == 's')
					{
						zm = true;
						if (fmt[i + 2] == '.')
						{
							zc = fmt[i + 3];
							m = 3;
						}
						else
						{
							zc = 'a';
							m = 1;
						}


						if (zc != 'w')
							zc = 'a';
					}
					else
						break;
				case 's':
					if (zm)
					{
						switch (zc)
						{
						case 'w':
							wstrptr = va_arg(ap, wchar_t*);
							astrptr = new char[bs + 1];
							std::wcstombs(astrptr, wstrptr, bs);
							astrptr[bs] = '\0';
							break;
						case 'a':
						default:
							astrptr = va_arg(ap, char*);
						}

						for (size_t q = 0; q < bs; q++)
							_push(astrptr[q], false);

						if (zm && zc == 'w')
							delete[] astrptr;

						i += m;
					}
					else
					{
						astrptr = va_arg(ap, char*);
						for (size_t q = 0; q < bs; q++)
						{
							_push(astrptr[q], false);
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						i++;
						itmp = i + 1;
					}


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'u':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 10);
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 2);
								itmp = strlen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToLower(tmp);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itoa_s(bptr[q], tmp, 65, 16);
								Utils::ToUpper(tmp);
								itmp = strlen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'n':
					nodelim = true;
					i++;
				case 'r':
					i++;
					switch (fmt[i])
					{
					case 'u':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'b':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'l':

					i++;
					if (fmt[i] == 'f')
					{
						ldoub = va_arg(ap, long double);
						Utils::ldtoa_s(tmp, 65, ldoub, (short)bs);
						_append(tmp, false);
					}
					else if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							lli = va_arg(ap, long long int);
							itmp = Utils::IntLength<long long int>(lli, Utils::INT_TYPE_DECIMAL);
							_i64toa_s(lli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64toa_s(ulli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_DECIMAL);
							_ui64toa_s(ulli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64toa_s(ulli, tmp, 65, 10);
							itmp = strlen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							li = va_arg(ap, long int);
							itmp = Utils::IntLength<long int>(li, Utils::INT_TYPE_DECIMAL);
							_i64toa_s(li, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64toa_s(uli, tmp, 65, 16);
							Utils::ToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_DECIMAL);
							_ui64toa_s(uli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64toa_s(uli, tmp, 65, 10);
							itmp = strlen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}

					break;
				case 'f':
					doub = va_arg(ap, double);
					Utils::dtoa_s(tmp, 65, doub, (short)bs);
					_append(tmp, false);
					break;

				case 'd':
				case 'i':
					_i = va_arg(ap, int);
					itmp = Utils::IntLength<int>(_i, Utils::INT_TYPE_DECIMAL);
					_i64toa_s(_i, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToLower(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64toa_s(ui, tmp, 65, 16);
					Utils::ToUpper(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'u':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_DECIMAL);
					_ui64toa_s(ui, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64toa_s(ui, tmp, 65, 10);
					itmp = strlen(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				}

				break;


			case 'l':
				i++;
				if (fmt[i] == 'l')
				{
					i++;
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						lli = va_arg(ap, long long int);
						_i64toa_s(lli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 10);
						_append(tmp, false);
						break;
					case  'b':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 2);
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ulli = va_arg(ap, unsigned long long int);
						_ui64toa_s(ulli, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append(tmp, false);
						break;

					}
				}
				else
				{
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						li = va_arg(ap, long int);
						_itoa_s(li, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case 'f':
						ldoub = va_arg(ap, long double);
						Utils::ldtoa_s(tmp, 65, ldoub);
						_append(tmp, false);
						break;

					case 'a':
					case 'p':
					case 'x':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 16);
						Utils::ToLower(tmp);
						_append(tmp, false);
						break;

					case 'A':
					case 'P':
					case 'X':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 16);
						Utils::ToUpper(tmp);
						_append(tmp, false);
						break;

						break;
						break;
					case 'b':
						uli = va_arg(ap, unsigned long int);
						_ui64toa_s(uli, tmp, 65, 2);
						_append(tmp, false);
						break;

					}
				}
				break;

			case 't':

				itmp = i + 3; j = 0;
				if (fmt[itmp] == '(')
				{
					itmp++;
					while (fmt[itmp] != ')')
					{
						delim[j] = fmt[itmp];
						j++; itmp++;
					}
					if (j == 0)
						nodelim = true;
					else
						delim[j] = '\0';
				}
				else
				{
					delim[0] = ' '; delim[1] = 0;
					itmp--;
				}
				i++;


				if (fmt[i] == 'r')
				{
					i++;
					switch (fmt[i])
					{
					case 'u':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'b':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 2);
							itmp = strlen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;
					case 'x':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToLower(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'X':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itoa_s(bptr[q], tmp, 65, 16);
							Utils::ToUpper(tmp);
							itmp = strlen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					default:
						__assume(0);
					}


				}
				break;

			case 'i':
			case 'd':
				_i = va_arg(ap, int);
				_itoa_s(_i, tmp, 65, 10);
				_append(tmp, false);
				break;

			case  'u':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 10);
				_append(tmp, false);
				break;

			case 'f':
				doub = va_arg(ap, double);
				Utils::dtoa_s(tmp, 65, doub);
				_append(tmp, false);
				break;

			case 'x':
			case 'a':
			case 'p':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 16);
				Utils::ToLower(tmp);
				_append(tmp, false);
				break;

			case 'X':
			case 'A':
			case 'P':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 16);
				Utils::ToUpper(tmp);
				_append(tmp, false);
				break;

			case 'z':
				if (fmt[i + 1] == 's' || fmt[i + 1] == 'c')
				{
					zm = true;
					if (fmt[i + 2] == '.')
					{
						zc = fmt[i + 3];
						m = 3;
					}
					else
					{
						zc = 'a';
						m = 1;
					}

					if (zc != 'w')
						zc = 'a';
				}
				else
					break;

			case 's':
				if (!(zm && fmt[i + 1] == 'c'))
				{
					if (zm)
					{
						switch (zc)
						{
						case 'w':
							wstrptr = va_arg(ap, wchar_t*);
							itmp = wcslen(wstrptr);
							astrptr = new char[itmp + 1];
							std::wcstombs(astrptr, wstrptr, itmp);
							astrptr[itmp] = '\0';
							break;
						case 'a':
						default:
							astrptr = va_arg(ap, char*);
						}

						_append(astrptr, false);

						if (zm && zc == 'w')
							delete[] astrptr;

						i += m;
					}
					else
					{
						astrptr = va_arg(ap, char*);
						_append(astrptr, false);
					}
					break;
				}

			case 'c':
				if (zm)
				{
					switch (zc)
					{
					case 'w':
						wc = va_arg(ap, wchar_t);
						wstrptr = new wchar_t[2];
						astrptr = new char[2];
						wstrptr[0] = wc; wstrptr[1] = '\0';
						std::wcstombs(astrptr, wstrptr, 1);
						c = astrptr[0];
						delete[] wstrptr;
						delete[] astrptr;
						break;
					case 'a':
					default:
						c = va_arg(ap, char);
					}

					_push(c, false);

					i += m;
				}
				else
				{
					c = va_arg(ap, char);
					_push(c, false);
				}
				break;
			case 'b':
				ui = va_arg(ap, unsigned int);
				_ui64toa_s(ui, tmp, 65, 2);
				Utils::ToUpper(tmp);
				_append(tmp, false);
				break;

			case 'B':
				_b = va_arg(ap, bool);
				if (_b)
					_append("true", false);
				else
					_append("false", false);
				break;

			case 'n':
				nodelim = true;
				i++;
			case 'r':
				i++;
				switch (fmt[i])
				{
				case 'd':
				case 'i':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 10);
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				case 'b':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 2);
						itmp = strlen(tmp);
						for (m = 0; m < 8 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;
				case 'x':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 16);
						Utils::ToLower(tmp);
						itmp = strlen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				case 'X':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itoa_s(bptr[q], tmp, 65, 16);
						Utils::ToUpper(tmp);
						itmp = strlen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(" ", false);
					}
					break;

				default:
					__assume(0);
				}
				break;
			}
		}
		else
			_push(fmt[i], false);
	}

	va_end(ap);
	ReleaseMutex(m_hMutex);
}

// *******************
// ***** UNICODE *****
// *******************
_xstringW::_xstringW()
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_lpcwStr = NULL;
	m_bXorStr = true;
	m_stxKeySize = 0;
	m_lpXorKey = NULL;
}

_xstringW::_xstringW(bool xorstr)
{
	m_stLen = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_lpcwStr = NULL;
	m_bXorStr = xorstr;
	m_stxKeySize = 0;
	m_lpXorKey = NULL;
}

_xstringW::_xstringW(volatile const wchar_t* str, bool xor)
{
	m_stLen = 0;
	m_bXorStr = xor;
	m_stxKeySize = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	_setStr(const_cast<const wchar_t*>(str));
}

_xstringW::_xstringW(const wchar_t* str, bool xor)
{
	m_stLen = 0;
	m_bXorStr = xor;
	m_stxKeySize = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	_setStr(str);
}

_xstringW::_xstringW(const std::wstring str, bool xor)
{
	m_stLen = 0;
	m_bXorStr = xor;
	m_stxKeySize = 0;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	_setStr(str.c_str());
}

/*_xstringW::_xstringW(const xstringW &str)
{
m_hMutex = CreateMutex(NULL, FALSE, NULL);
m_stLen = 0;
m_stxKeySize = 0;
m_bXorStr = str.m_bXorStr;
this->_setStr(str.GetStr().c_str());
}*/

_xstringW::_xstringW(xstringW &str)
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_stLen = 0;
	m_stxKeySize = 0;
	m_bXorStr = str.m_bXorStr;
	this->_setStr(str.GetStr().c_str());
}

_xstringW::~_xstringW()
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen > 0)
		delete[] m_lpcwStr;
	if (m_stxKeySize)
		delete[] m_lpXorKey;
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

void __fastcall _xstringW::_setStr(const wchar_t* str, bool HoldMutex) volatile
{
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen > 0)
		delete[] m_lpcwStr;
	size_t strl = wcslen(str);
	m_lpcwStr = new volatile wchar_t[strl + 1];

	if (m_bXorStr)
		_generateKey(strl * sizeof(wchar_t));

	if (m_bXorStr)
	{
		size_t j = 0;
		for (UINT i = 0; i < strl; i++)
			m_lpcwStr[i] = _cryptWchar(i, str[i], true);
	}
	else
	{
		for (UINT i = 0; i < strl; i++)
			m_lpcwStr[i] = str[i];
	}
	m_lpcwStr[strl] = '\0';

	m_stLen = strl;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

const std::wstring _xstringW::GetStr_w() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	std::wstring tmp;
	wchar_t* decrypted;
	if (m_bXorStr)
	{
		decrypted = new wchar_t[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<wchar_t*>(m_lpcwStr);

	if (m_stLen > 0)
		tmp = const_cast<const wchar_t*>(decrypted);
	else
		tmp = L"";

	if (m_bXorStr)
		delete[] decrypted;
	ReleaseMutex(m_hMutex);
	return tmp.c_str();
}

const std::wstring _xstringW::GetStr() const volatile
{
	return GetStr_w();
}

const std::string _xstringW::GetStr_a() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	std::wstring tmp;
	std::string atmp;
	wchar_t* decrypted;
	if (m_bXorStr)
	{
		decrypted = new wchar_t[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<wchar_t*>(m_lpcwStr);
	if (m_stLen > 0)
	{
		tmp = const_cast<const wchar_t*>(decrypted);
		size_t size = tmp.length() + 1;
		char* buf = new char[size];
		std::wcstombs(buf, tmp.c_str(), size);
		atmp = buf;
		delete[] buf;
	}
	else
		atmp = "";

	if (m_bXorStr)
		delete[] decrypted;
	ReleaseMutex(m_hMutex);
	return atmp.c_str();
}


size_t _xstringW::GetLen() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	size_t len = m_stLen;
	ReleaseMutex(m_hMutex);
	return len;
}

std::wstring _xstringW::GetReversed() const volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	wchar_t* decrypted;
	wchar_t* reversed;
	std::wstring res;
	if (m_bXorStr)
	{
		decrypted = new wchar_t[m_stLen + 1];
		_decrypt(decrypted);
	}
	else
		decrypted = const_cast<wchar_t*>(m_lpcwStr);
	reversed = new wchar_t[m_stLen + 1];

	size_t last_index = m_stLen - 1;
	for (size_t i = last_index; true; i--)
	{
		reversed[last_index - i] = decrypted[i];
		if (i == 0)
			break;
	}

	res = reversed;
	if (m_bXorStr)
		delete[] decrypted;
	delete[] reversed;
	ReleaseMutex(m_hMutex);
	return res;
}

void __fastcall _xstringW::Append(const xstringW& str) volatile
{
	Append(str.GetStr().c_str());
}

void __fastcall _xstringW::Append(const std::wstring str) volatile
{
	Append(str.c_str());
}

void __fastcall _xstringW::Append(const wchar_t* str) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	volatile wchar_t* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile wchar_t[m_stLen + 1];
			_decrypt(const_cast<wchar_t*>(tmp));
			delete[] m_lpcwStr;
		}
		else
			tmp = m_lpcwStr;
	}

	size_t oldstrl = m_stLen;
	size_t strl = wcslen(str);
	size_t newstrl = m_stLen + strl;
	m_lpcwStr = new volatile wchar_t[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl * sizeof(wchar_t));

	UINT i = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, tmp[i], true);
		else
			m_lpcwStr[i] = tmp[i];
		i++;
	}

	UINT j = 0;
	while (j < strl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, str[j], true);
		else
			m_lpcwStr[i] = str[j];
		i++; j++;
	}

	m_lpcwStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringW::_append(const wchar_t* str, bool HoldMutex) volatile
{
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	volatile wchar_t* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile wchar_t[m_stLen + 1];
			_decrypt(const_cast<wchar_t*>(tmp));
			delete[] m_lpcwStr;
		}
		else
			tmp = m_lpcwStr;
	}

	size_t oldstrl = m_stLen;
	size_t strl = wcslen(str);
	size_t newstrl = m_stLen + strl;
	m_lpcwStr = new volatile wchar_t[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl * sizeof(wchar_t));

	UINT i = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, tmp[i], true);
		else
			m_lpcwStr[i] = tmp[i];
		i++;
	}

	UINT j = 0;
	while (j < strl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, str[j], true);
		else
			m_lpcwStr[i] = str[j];
		i++; j++;
	}

	m_lpcwStr[i] = '\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

void __fastcall _xstringW::Push(const wchar_t c) volatile
{
	if (c == '\0')
		return;
	WaitForSingleObject(m_hMutex, INFINITE);
	volatile wchar_t* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile wchar_t[m_stLen + 1];
			_decrypt(const_cast<wchar_t*>(tmp));
			delete[] m_lpcwStr;
		}
		else
			tmp = m_lpcwStr;
	}

	size_t oldstrl = m_stLen;
	size_t newstrl = m_stLen + 1;
	m_lpcwStr = new volatile wchar_t[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl * sizeof(wchar_t));

	UINT i = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, tmp[i], true);
		else
			m_lpcwStr[i] = tmp[i];
		i++;
	}

	if (m_bXorStr)
		m_lpcwStr[i] = _cryptWchar(i, c, true);
	else
		m_lpcwStr[i] = c;
	i++;
	m_lpcwStr[i] = (wchar_t)'\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringW::_push(const wchar_t c, bool HoldMutex) volatile
{
	if (c == '\0')
		return;
	if (HoldMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
	volatile wchar_t* tmp = nullptr;
	if (m_stLen > 0)
	{
		if (m_bXorStr)
		{
			tmp = new volatile wchar_t[m_stLen + 1];
			_decrypt(const_cast<wchar_t*>(tmp));
			delete[] m_lpcwStr;
		}
		else
			tmp = m_lpcwStr;
	}

	size_t oldstrl = m_stLen;
	size_t newstrl = m_stLen + 1;
	m_lpcwStr = new volatile wchar_t[newstrl + 1];

	if (m_bXorStr)
		_generateKey(newstrl * sizeof(wchar_t));

	UINT i = 0;
	while (i < oldstrl)
	{
		if (m_bXorStr)
			m_lpcwStr[i] = _cryptWchar(i, tmp[i], true);
		else
			m_lpcwStr[i] = tmp[i];
		i++;
	}

	if (m_bXorStr)
		m_lpcwStr[i] = _cryptWchar(i, c, true);
	else
		m_lpcwStr[i] = c;
	i++;
	m_lpcwStr[i] = (wchar_t)'\0';

	m_stLen = newstrl;
	if (oldstrl > 0)
		delete[] tmp;
	if (HoldMutex)
		ReleaseMutex(m_hMutex);
}

bool __fastcall _xstringW::SetChar(const wchar_t c, size_t n) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen == 0 || n < 0 || n > m_stLen)
	{
		ReleaseMutex(m_hMutex);
		return false;
	}
	if (m_bXorStr)
		m_lpcwStr[n] = _cryptWchar(n, c, true);
	else
		m_lpcwStr[n] = c;
	ReleaseMutex(m_hMutex);
	return true;
}

bool operator==(const volatile _xstringW left, const volatile _xstringW& right)
{
	if (left.m_hMutex == right.m_hMutex)
		return true;

	if (wcscmp(left.GetStr().c_str(), right.GetStr().c_str()) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringW left, const volatile _xstringW& right)
{
	if (left.m_hMutex == right.m_hMutex)
		return false;

	if (wcscmp(left.GetStr().c_str(), right.GetStr().c_str()) == 0)
		return false;
	else
		return true;
}


bool operator==(const volatile _xstringW left, const wchar_t* right)
{
	if (wcscmp(left.GetStr().c_str(), right) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringW left, const wchar_t* right)
{
	if (wcscmp(left.GetStr().c_str(), right) == 0)
		return false;
	else
		return true;
}

bool operator==(const volatile _xstringW left, const std::wstring& right)
{
	if (wcscmp(left.GetStr().c_str(), right.c_str()) == 0)
		return true;
	else
		return false;
}

bool operator!=(const volatile _xstringW left, const std::wstring& right)
{
	if (wcscmp(left.GetStr().c_str(), right.c_str()) == 0)
		return false;
	else
		return true;
}

wchar_t _xstringW::operator [] (size_t n) const volatile
{
	wchar_t c;
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_stLen == 0 || n < 0 || n > m_stLen)
		c = '\0';
	else
	{
		if (m_bXorStr)
			c = _cryptWchar(n);
		else
			c = m_lpcwStr[n];
	}
	ReleaseMutex(m_hMutex);
	return c;
}

volatile _xstringW& _xstringW::operator=(const volatile _xstringW& right) volatile
{
	if (this == &right)
		return *this;

	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(const_cast<const wchar_t*>(right.GetStr().c_str()), false);
	ReleaseMutex(m_hMutex);
	return *this;
}

volatile _xstringW& _xstringW::operator=(const wchar_t* right) volatile
{
	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(right, false);
	ReleaseMutex(m_hMutex);
	return *this;
}

volatile _xstringW& _xstringW::operator=(const std::wstring right) volatile
{
	if (!m_hMutex)
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(m_hMutex, INFINITE);
	if (!m_stLen)
		m_stLen = 0;
	_setStr(right.c_str(), false);
	ReleaseMutex(m_hMutex);
	return *this;
}

void __fastcall _xstringW::ReInit(const wchar_t* str, bool xor) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	m_bXorStr = xor;
	_setStr(str, false);
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringW::_decrypt(wchar_t* output) const volatile
{
	if (m_stLen != 0)
	{
		for (size_t i = 0; i < m_stLen; i++)
			output[i] = _cryptWchar(i);
	}
	output[m_stLen] = (wchar_t)'\0';
}

void _xstringW::_generateKey(size_t strl) volatile
{
	if (m_stxKeySize > 0)
		delete[] m_lpXorKey;

	if (strl <= 25)
		m_stxKeySize = strl;
	else
		m_stxKeySize = Utils::RandomBetween<size_t>(11, 25);

	if (m_stxKeySize > 0)
		m_lpXorKey = new volatile char[m_stxKeySize];
	else
		m_lpXorKey = NULL;

	UCHAR uc = 0;
	for (size_t i = 0; i < m_stxKeySize; i++)
	{
		uc = (UCHAR)Utils::RandomBetween<unsigned short>(0, 255);
		m_lpXorKey[i] = *(reinterpret_cast<char*>(&uc));
	}
}

size_t __fastcall _xstringW::_getKeyIndexForIndex(size_t i) const volatile
{
	if (i < m_stxKeySize)
		return i;

	size_t t = i / m_stxKeySize;
	t = m_stxKeySize * t;
	t = i - t;
	return t;
}

void __fastcall _xstringW::Encryption(bool enabled) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_bXorStr == enabled)
	{
		ReleaseMutex(m_hMutex);
		return;
	}

	if (m_stLen == 0)
	{
		m_bXorStr = enabled;
		ReleaseMutex(m_hMutex);
		return;
	}

	wchar_t* tmp;
	m_bXorStr = enabled;
	if (enabled)
	{
		tmp = new wchar_t[m_stLen + 1];
		wcscpy_s(tmp, m_stLen + 1, const_cast<wchar_t*>(m_lpcwStr));
		_setStr(tmp, false);
		delete[] tmp;
	}
	else
	{
		tmp = new wchar_t[m_stLen + 1];
		_decrypt(tmp);
		_setStr(tmp, false);
		delete[] tmp;
	}
	ReleaseMutex(m_hMutex);
}

wchar_t __fastcall _xstringW::_cryptWchar(size_t index, wchar_t _wc, bool custom) const volatile
{
	size_t wcsize = sizeof(wchar_t);
	wchar_t wc = m_lpcwStr[index];
	if (custom)
		wc = _wc;
	char* tmp = reinterpret_cast<char*>(&wc);

	for (size_t i = 0; i < wcsize; i++)
		tmp[i] = tmp[i] ^ m_lpXorKey[_getKeyIndexForIndex((index * wcsize) + i)];

	return wc;
}

volatile _xstringW& _xstringW::operator+=(const volatile _xstringW& right) volatile
{
	Append(right);
	return *this;
}

volatile _xstringW& _xstringW::operator+=(const wchar_t* right) volatile
{
	Append(right);
	return *this;
}

volatile _xstringW& _xstringW::operator+=(wchar_t right) volatile
{
	Push(right);
	return *this;
}

volatile _xstringW& _xstringW::operator+=(const std::wstring right) volatile
{
	Append(right.c_str());
	return *this;
}

xstringW operator+(xstringW left, const xstringW& right)
{
	xstringW xs = left;
	xs += right;
	return left;
}

xstringW operator+(xstringW left, const wchar_t* right)
{
	xstringW xs = left;
	xs += right;
	return left;
}

xstringW operator+(xstringW left, wchar_t right)
{
	xstringW xs = left;
	xs += right;
	return left;
}

xstringW operator+(xstringW left, const std::wstring& right)
{
	xstringW xs = left;
	xs += right;
	return left;
}

_xstringW::operator const std::wstring() const volatile
{
	return GetStr_w();
}

void __fastcall _xstringW::Format(const wchar_t* fmt, ...) volatile
{
	WaitForSingleObject(m_hMutex, INFINITE);
	va_list ap;
	size_t newlen = 0;
	size_t flen = wcslen(fmt);
	va_start(ap, fmt);
	wchar_t tmp[65]; tmp[0] = 0;
	wchar_t delim[64]; delim[0] = 0;
	size_t j = 0, bs = 0, itmp = 0, m = 0;
	bool limited = false;
	bool prefix = false;
	bool nodelim = false;
	bool zm = false; wchar_t zc = 0;
	unsigned long long int ulli;
	unsigned long int uli;
	unsigned int ui;
	long long int lli;
	long int li;
	int _i;
	char c;
	wchar_t wc;

	//float fl;
	double doub;
	long double ldoub;
	bool _b;
	char* astrptr;
	wchar_t* wstrptr;
	BYTE* bptr;

	_setStr(L"", false);
	for (size_t i = 0; i < flen; i++)
	{
		if (fmt[i] == '%')
		{
			i++; limited = false; prefix = false; nodelim = false; bs = 0; itmp = 0;  zm = false;
			if (fmt[i] == '\0')
				break;

			switch (fmt[i])
			{
			case '%':
				_push(fmt[i], false);
				break;

			case '#':
				prefix = true;
				i++;
				switch (fmt[i])
				{
				case '[':
					i++; j = 0;
					while (fmt[i] != ']')
					{
						tmp[j] = fmt[i];
						j++; i++;
					}
					tmp[j] = '\0';
					bs = (size_t)(_wtoi(tmp));
					limited = true;

					i++;
					if (fmt[i] == 't')
					{
						itmp = i + 3; j = 0;
						if (fmt[itmp] == '(')
						{
							itmp++;
							while (fmt[itmp] != ')')
							{
								delim[j] = fmt[itmp];
								j++; itmp++;
							}
							if (j == 0)
								nodelim = true;
							else
								delim[j] = '\0';
						}
						else
						{
							delim[0] = ' '; delim[1] = 0;
							itmp = i + 1;
						}
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp = i + 1;
					}

					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								_append(L"0b", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}


					switch (fmt[i])
					{
					case 'l':
						i++;
						if (fmt[i] == 'l')
						{
							i++;
							switch (fmt[i])
							{
							case 'b':
								ulli = va_arg(ap, unsigned long long int);
								_ui64tow_s(ulli, tmp, 65, 2);
								itmp = wcslen(tmp);
								_append(L"0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64tow_s(ulli, tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64tow_s(ulli, tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						else
						{
							switch (fmt[i])
							{
							case 'b':
								uli = va_arg(ap, unsigned long int);
								_ui64tow_s(uli, tmp, 65, 2);
								itmp = wcslen(tmp);
								_append(L"0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64tow_s(uli, tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64tow_s(uli, tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						break;

					case 'b':
						ui = va_arg(ap, unsigned int);
						_ui64tow_s(ui, tmp, 65, 2);
						itmp = wcslen(tmp);
						_append(L"0b", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64tow_s(ui, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(L"0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64tow_s(ui, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(L"0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
						break;
					}
					break;


				case 'l':
					i++;
					if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;

						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 2);
							_append(L"0b", false);
							_append(tmp, false);
							break;

						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;

						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 2);
							_append(L"0b", false);
							_append(tmp, false);
							break;
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp--;
					}
					i++;


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								_append(L"0b", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'r':

					i++;
					switch (fmt[i])
					{
					case 'b':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							Utils::wToUpper(tmp);
							_append(L"0b", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToLower(tmp);
					_append(L"0x", false);
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToUpper(tmp);
					_append(L"0x", false);
					_append(tmp, false);
					break;

				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 2);
					_append(L"0b", false);
					_append(tmp, false);
					break;

				}
				break;

			case '[':
				i++; j = 0;
				while (fmt[i] != ']')
				{
					tmp[j] = fmt[i];
					j++; i++;
				}
				tmp[j] = '\0';
				bs = (size_t)(_wtoi(tmp));
				limited = true;

				i++;
				switch (fmt[i])
				{
				case 'z':
					if (fmt[i + 1] == 's')
					{
						zm = true;
						if (fmt[i + 2] == '.')
						{
							zc = fmt[i + 3];
							m = 3;
						}
						else
						{
							zc = 'w';
							m = 1;
						}


						if (zc != 'a')
							zc = 'w';
					}
					else
						break;

				case 's':
					if (zm)
					{
						switch (zc)
						{
						case 'a':
							astrptr = va_arg(ap, char*);
							wstrptr = new wchar_t[bs + 1];
							std::mbstowcs(wstrptr, astrptr, bs);
							wstrptr[bs] = '\0';
							break;
						case 'w':
						default:
							wstrptr = va_arg(ap, wchar_t*);
						}

						for (size_t q = 0; q < bs; q++)
							_push(wstrptr[q], false);

						if (zm && zc == 'a')
							delete[] wstrptr;

						i += m;
					}
					else
					{
						wstrptr = va_arg(ap, wchar_t*);
						for (size_t q = 0; q < bs; q++)
						{
							_push(wstrptr[q], false);
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						i++;
						itmp = i + 1;
					}


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'u':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 10);
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'n':
					nodelim = true;
					i++;
				case 'r':
					i++;
					switch (fmt[i])
					{
					case 'u':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'b':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'l':

					i++;
					if (fmt[i] == 'f')
					{
						ldoub = va_arg(ap, long double);
						Utils::ldtow_s(tmp, 65, ldoub, (short)bs);
						_append(tmp, false);
					}
					else if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							lli = va_arg(ap, long long int);
							itmp = Utils::IntLength<long long int>(lli, Utils::INT_TYPE_DECIMAL);
							_i64tow_s(lli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_DECIMAL);
							_ui64tow_s(ulli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 10);
							itmp = wcslen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							li = va_arg(ap, long int);
							itmp = Utils::IntLength<long int>(li, Utils::INT_TYPE_DECIMAL);
							_i64tow_s(li, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_DECIMAL);
							_ui64tow_s(uli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 10);
							itmp = wcslen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}

					break;
				case 'f':
					doub = va_arg(ap, double);
					Utils::dtow_s(tmp, 65, doub, (short)bs);
					_append(tmp, false);
					break;

				case 'd':
				case 'i':
					_i = va_arg(ap, int);
					itmp = Utils::IntLength<int>(_i, Utils::INT_TYPE_DECIMAL);
					_i64tow_s(_i, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToLower(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToUpper(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'u':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_DECIMAL);
					_ui64tow_s(ui, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 10);
					itmp = wcslen(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				}

				break;


			case 'l':
				i++;
				if (fmt[i] == 'l')
				{
					i++;
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						lli = va_arg(ap, long long int);
						_i64tow_s(lli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 10);
						_append(tmp, false);
						break;
					case  'b':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 2);
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(tmp, false);
						break;

					}
				}
				else
				{
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						li = va_arg(ap, long int);
						_itow_s(li, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case 'f':
						ldoub = va_arg(ap, long double);
						Utils::ldtow_s(tmp, 65, ldoub);
						_append(tmp, false);
						break;

					case 'a':
					case 'p':
					case 'x':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(tmp, false);
						break;

					case 'A':
					case 'P':
					case 'X':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(tmp, false);
						break;

						break;
						break;
					case 'b':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 2);
						_append(tmp, false);
						break;

					}
				}
				break;

			case 't':

				itmp = i + 3; j = 0;
				if (fmt[itmp] == '(')
				{
					itmp++;
					while (fmt[itmp] != ')')
					{
						delim[j] = fmt[itmp];
						j++; itmp++;
					}
					if (j == 0)
						nodelim = true;
					else
						delim[j] = '\0';
				}
				else
				{
					delim[0] = ' '; delim[1] = 0;
					itmp--;
				}
				i++;


				if (fmt[i] == 'r')
				{
					i++;
					switch (fmt[i])
					{
					case 'u':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'b':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;
					case 'x':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'X':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					default:
						__assume(0);
					}


				}
				break;

			case 'i':
			case 'd':
				_i = va_arg(ap, int);
				_itow_s(_i, tmp, 65, 10);
				_append(tmp, false);
				break;

			case  'u':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 10);
				_append(tmp, false);
				break;

			case 'f':
				doub = va_arg(ap, double);
				Utils::dtow_s(tmp, 65, doub);
				_append(tmp, false);
				break;

			case 'x':
			case 'a':
			case 'p':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 16);
				Utils::wToLower(tmp);
				_append(tmp, false);
				break;

			case 'X':
			case 'A':
			case 'P':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 16);
				Utils::wToUpper(tmp);
				_append(tmp, false);
				break;

			case 'z':
				if (fmt[i + 1] == 's' || fmt[i + 1] == 'c')
				{
					zm = true;
					if (fmt[i + 2] == '.')
					{
						zc = fmt[i + 3];
						m = 3;
					}
					else
					{
						zc = 'w';
						m = 1;
					}

					if (zc != 'a')
						zc = 'w';
				}
				else
					break;

			case 's':
				if (!(zm && fmt[i + 1] == 'c'))
				{
					if (zm)
					{
						switch (zc)
						{
						case 'a':
							astrptr = va_arg(ap, char*);
							itmp = strlen(astrptr);
							wstrptr = new wchar_t[itmp + 1];
							std::mbstowcs(wstrptr, astrptr, itmp);
							wstrptr[itmp] = '\0';
							break;
						case 'w':
						default:
							wstrptr = va_arg(ap, wchar_t*);
						}

						_append(wstrptr, false);

						if (zm && zc == 'a')
							delete[] wstrptr;

						i += m;
					}
					else
					{
						wstrptr = va_arg(ap, wchar_t*);
						_append(wstrptr, false);
					}
					break;
				}

			case 'c':
				if (zm)
				{
					switch (zc)
					{
					case 'a':
						c = va_arg(ap, char);
						wstrptr = new wchar_t[2];
						astrptr = new char[2];
						astrptr[0] = c; astrptr[1] = '\0';
						std::mbstowcs(wstrptr, astrptr, 1);
						wc = wstrptr[0];
						delete[] wstrptr;
						delete[] astrptr;
						break;
					case 'w':
					default:
						wc = va_arg(ap, wchar_t);
					}

					_push(wc, false);

					i += m;
				}
				else
				{
					wc = va_arg(ap, wchar_t);
					_push(wc, false);
				}
				break;
			case 'b':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 2);
				Utils::wToUpper(tmp);
				_append(tmp, false);
				break;

			case 'B':
				_b = va_arg(ap, bool);
				if (_b)
					_append(L"true", false);
				else
					_append(L"false", false);
				break;

			case 'n':
				nodelim = true;
				i++;
			case 'r':
				i++;
				switch (fmt[i])
				{
				case 'd':
				case 'i':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 10);
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				case 'b':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 2);
						itmp = wcslen(tmp);
						for (m = 0; m < 8 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;
				case 'x':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 16);
						Utils::wToLower(tmp);
						itmp = wcslen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				case 'X':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 16);
						Utils::wToUpper(tmp);
						itmp = wcslen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				default:
					__assume(0);
				}
				break;
			}
		}
		else
			_push(fmt[i], false);
	}

	va_end(ap);
	ReleaseMutex(m_hMutex);
}

void __fastcall _xstringW::AppendFormat(const wchar_t* fmt, ...) volatile

{
	WaitForSingleObject(m_hMutex, INFINITE);
	va_list ap;
	size_t newlen = 0;
	size_t flen = wcslen(fmt);
	va_start(ap, fmt);
	wchar_t tmp[65]; tmp[0] = 0;
	wchar_t delim[64]; delim[0] = 0;
	size_t j = 0, bs = 0, itmp = 0, m = 0;
	bool limited = false;
	bool prefix = false;
	bool nodelim = false;
	bool zm = false; wchar_t zc = 0;
	unsigned long long int ulli;
	unsigned long int uli;
	unsigned int ui;
	long long int lli;
	long int li;
	int _i;
	char c;
	wchar_t wc;

	//float fl;
	double doub;
	long double ldoub;
	bool _b;
	char* astrptr;
	wchar_t* wstrptr;
	BYTE* bptr;

	for (size_t i = 0; i < flen; i++)
	{
		if (fmt[i] == '%')
		{
			i++; limited = false; prefix = false; nodelim = false; bs = 0; itmp = 0;  zm = false;
			if (fmt[i] == '\0')
				break;

			switch (fmt[i])
			{
			case '%':
				_push(fmt[i], false);
				break;

			case '#':
				prefix = true;
				i++;
				switch (fmt[i])
				{
				case '[':
					i++; j = 0;
					while (fmt[i] != ']')
					{
						tmp[j] = fmt[i];
						j++; i++;
					}
					tmp[j] = '\0';
					bs = (size_t)(_wtoi(tmp));
					limited = true;

					i++;
					if (fmt[i] == 't')
					{
						itmp = i + 3; j = 0;
						if (fmt[itmp] == '(')
						{
							itmp++;
							while (fmt[itmp] != ')')
							{
								delim[j] = fmt[itmp];
								j++; itmp++;
							}
							if (j == 0)
								nodelim = true;
							else
								delim[j] = '\0';
						}
						else
						{
							delim[0] = ' '; delim[1] = 0;
							itmp = i + 1;
						}
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp = i + 1;
					}

					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								_append(L"0b", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}


					switch (fmt[i])
					{
					case 'l':
						i++;
						if (fmt[i] == 'l')
						{
							i++;
							switch (fmt[i])
							{
							case 'b':
								ulli = va_arg(ap, unsigned long long int);
								_ui64tow_s(ulli, tmp, 65, 2);
								itmp = wcslen(tmp);
								_append(L"0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64tow_s(ulli, tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								ulli = va_arg(ap, unsigned long long int);
								itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
								_ui64tow_s(ulli, tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						else
						{
							switch (fmt[i])
							{
							case 'b':
								uli = va_arg(ap, unsigned long int);
								_ui64tow_s(uli, tmp, 65, 2);
								itmp = wcslen(tmp);
								_append(L"0b", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'a':
							case 'p':
							case 'x':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64tow_s(uli, tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							case 'A':
							case 'P':
							case 'X':
								uli = va_arg(ap, unsigned long int);
								itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
								_ui64tow_s(uli, tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								if (bs >= itmp)
									for (size_t m = 0; m < bs - itmp; m++)
									{
										_push('0', false);
									}
								_append(tmp, false);
								break;
							}
						}
						break;

					case 'b':
						ui = va_arg(ap, unsigned int);
						_ui64tow_s(ui, tmp, 65, 2);
						itmp = wcslen(tmp);
						_append(L"0b", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64tow_s(ui, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(L"0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ui = va_arg(ap, unsigned int);
						itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
						_ui64tow_s(ui, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(L"0x", false);
						if (bs >= itmp)
							for (size_t m = 0; m < bs - itmp; m++)
							{
								_push('0', false);
							}
						_append(tmp, false);
						break;
						break;
					}
					break;


				case 'l':
					i++;
					if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;

						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 2);
							_append(L"0b", false);
							_append(tmp, false);
							break;

						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							_append(tmp, false);
							break;

						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 2);
							_append(L"0b", false);
							_append(tmp, false);
							break;
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						itmp--;
					}
					i++;


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								_append(L"0b", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							bs = va_arg(ap, size_t);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								_append(L"0x", false);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'r':

					i++;
					switch (fmt[i])
					{
					case 'b':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							Utils::wToUpper(tmp);
							_append(L"0b", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							_append(L"0x", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							_append(L"0x", false);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToLower(tmp);
					_append(L"0x", false);
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToUpper(tmp);
					_append(L"0x", false);
					_append(tmp, false);
					break;

				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 2);
					_append(L"0b", false);
					_append(tmp, false);
					break;

				}
				break;

			case '[':
				i++; j = 0;
				while (fmt[i] != ']')
				{
					tmp[j] = fmt[i];
					j++; i++;
				}
				tmp[j] = '\0';
				bs = (size_t)(_wtoi(tmp));
				limited = true;

				i++;
				switch (fmt[i])
				{
				case 'z':
					if (fmt[i + 1] == 's')
					{
						zm = true;
						if (fmt[i + 2] == '.')
						{
							zc = fmt[i + 3];
							m = 3;
						}
						else
						{
							zc = 'w';
							m = 1;
						}


						if (zc != 'a')
							zc = 'w';
					}
					else
						break;

				case 's':
					if (zm)
					{
						switch (zc)
						{
						case 'a':
							astrptr = va_arg(ap, char*);
							wstrptr = new wchar_t[bs + 1];
							std::mbstowcs(wstrptr, astrptr, bs);
							wstrptr[bs] = '\0';
							break;
						case 'w':
						default:
							wstrptr = va_arg(ap, wchar_t*);
						}

						for (size_t q = 0; q < bs; q++)
							_push(wstrptr[q], false);

						if (zm && zc == 'a')
							delete[] wstrptr;

						i += m;
					}
					else
					{
						wstrptr = va_arg(ap, wchar_t*);
						for (size_t q = 0; q < bs; q++)
						{
							_push(wstrptr[q], false);
						}
					}
					break;

				case 't':

					itmp = i + 3; j = 0;
					if (fmt[itmp] == '(')
					{
						itmp++;
						while (fmt[itmp] != ')')
						{
							delim[j] = fmt[itmp];
							j++; itmp++;
						}
						if (j == 0)
							nodelim = true;
						else
							delim[j] = '\0';
						i++;
					}
					else
					{
						delim[0] = ' '; delim[1] = 0;
						i++;
						itmp = i + 1;
					}


					if (fmt[i] == 'r')
					{
						i++;
						switch (fmt[i])
						{
						case 'u':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 10);
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'b':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 2);
								itmp = wcslen(tmp);
								for (m = 0; m < 8 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;
						case 'x':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToLower(tmp);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						case 'X':
							i = itmp;
							bptr = va_arg(ap, BYTE*);
							for (size_t q = 0; q < bs; q++)
							{
								_itow_s(bptr[q], tmp, 65, 16);
								Utils::wToUpper(tmp);
								itmp = wcslen(tmp);
								for (m = 0; m < 2 - itmp; m++)
								{
									_push('0', false);
								}
								_append(tmp, false);

								if (q != (bs - 1) && !nodelim)
									_append(delim, false);
							}
							break;

						default:
							__assume(0);
						}


					}
					break;

				case 'n':
					nodelim = true;
					i++;
				case 'r':
					i++;
					switch (fmt[i])
					{
					case 'u':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'b':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;
					case 'x':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					case 'X':
						bptr = va_arg(ap, BYTE*);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(L" ", false);
						}
						break;

					default:
						__assume(0);
					}
					break;

				case 'l':

					i++;
					if (fmt[i] == 'f')
					{
						ldoub = va_arg(ap, long double);
						Utils::ldtow_s(tmp, 65, ldoub, (short)bs);
						_append(tmp, false);
					}
					else if (fmt[i] == 'l')
					{
						i++;
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							lli = va_arg(ap, long long int);
							itmp = Utils::IntLength<long long int>(lli, Utils::INT_TYPE_DECIMAL);
							_i64tow_s(lli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_HEX);
							_ui64tow_s(ulli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							ulli = va_arg(ap, unsigned long long int);
							itmp = Utils::IntLength<unsigned long long int>(ulli, Utils::INT_TYPE_DECIMAL);
							_ui64tow_s(ulli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							ulli = va_arg(ap, unsigned long long int);
							_ui64tow_s(ulli, tmp, 65, 10);
							itmp = wcslen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}
					else
					{
						switch (fmt[i])
						{
						case 'd':
						case 'i':
							li = va_arg(ap, long int);
							itmp = Utils::IntLength<long int>(li, Utils::INT_TYPE_DECIMAL);
							_i64tow_s(li, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'a':
						case 'p':
						case 'x':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToLower(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'A':
						case 'P':
						case 'X':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_HEX);
							_ui64tow_s(uli, tmp, 65, 16);
							Utils::wToUpper(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'u':
							uli = va_arg(ap, unsigned long int);
							itmp = Utils::IntLength<unsigned long int>(uli, Utils::INT_TYPE_DECIMAL);
							_ui64tow_s(uli, tmp, 65, 10);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						case 'b':
							uli = va_arg(ap, unsigned long int);
							_ui64tow_s(uli, tmp, 65, 10);
							itmp = wcslen(tmp);
							if (bs >= itmp)
								for (size_t m = 0; m < bs - itmp; m++)
								{
									_push('0', false);
								}
							_append(tmp, false);
							break;
						}
					}

					break;
				case 'f':
					doub = va_arg(ap, double);
					Utils::dtow_s(tmp, 65, doub, (short)bs);
					_append(tmp, false);
					break;

				case 'd':
				case 'i':
					_i = va_arg(ap, int);
					itmp = Utils::IntLength<int>(_i, Utils::INT_TYPE_DECIMAL);
					_i64tow_s(_i, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'a':
				case 'p':
				case 'x':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToLower(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'A':
				case 'P':
				case 'X':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_HEX);
					_ui64tow_s(ui, tmp, 65, 16);
					Utils::wToUpper(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'u':
					ui = va_arg(ap, unsigned int);
					itmp = Utils::IntLength<unsigned int>(ui, Utils::INT_TYPE_DECIMAL);
					_ui64tow_s(ui, tmp, 65, 10);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				case 'b':
					ui = va_arg(ap, unsigned int);
					_ui64tow_s(ui, tmp, 65, 10);
					itmp = wcslen(tmp);
					if (bs >= itmp)
						for (size_t m = 0; m < bs - itmp; m++)
						{
							_push('0', false);
						}
					_append(tmp, false);
					break;
				}

				break;


			case 'l':
				i++;
				if (fmt[i] == 'l')
				{
					i++;
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						lli = va_arg(ap, long long int);
						_i64tow_s(lli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 10);
						_append(tmp, false);
						break;
					case  'b':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 2);
						_append(tmp, false);
						break;
					case 'a':
					case 'p':
					case 'x':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(tmp, false);
						break;
					case 'A':
					case 'P':
					case 'X':
						ulli = va_arg(ap, unsigned long long int);
						_ui64tow_s(ulli, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(tmp, false);
						break;

					}
				}
				else
				{
					switch (fmt[i])
					{
					case 'i':
					case 'd':
						li = va_arg(ap, long int);
						_itow_s(li, tmp, 65, 10);
						_append(tmp, false);
						break;

					case  'u':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 10);
						_append(tmp, false);
						break;

					case 'f':
						ldoub = va_arg(ap, long double);
						Utils::ldtow_s(tmp, 65, ldoub);
						_append(tmp, false);
						break;

					case 'a':
					case 'p':
					case 'x':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 16);
						Utils::wToLower(tmp);
						_append(tmp, false);
						break;

					case 'A':
					case 'P':
					case 'X':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 16);
						Utils::wToUpper(tmp);
						_append(tmp, false);
						break;

						break;
						break;
					case 'b':
						uli = va_arg(ap, unsigned long int);
						_ui64tow_s(uli, tmp, 65, 2);
						_append(tmp, false);
						break;

					}
				}
				break;

			case 't':

				itmp = i + 3; j = 0;
				if (fmt[itmp] == '(')
				{
					itmp++;
					while (fmt[itmp] != ')')
					{
						delim[j] = fmt[itmp];
						j++; itmp++;
					}
					if (j == 0)
						nodelim = true;
					else
						delim[j] = '\0';
				}
				else
				{
					delim[0] = ' '; delim[1] = 0;
					itmp--;
				}
				i++;


				if (fmt[i] == 'r')
				{
					i++;
					switch (fmt[i])
					{
					case 'u':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 10);
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'b':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 2);
							itmp = wcslen(tmp);
							for (m = 0; m < 8 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;
					case 'x':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToLower(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					case 'X':
						i = itmp;
						bptr = va_arg(ap, BYTE*);
						bs = va_arg(ap, size_t);
						for (size_t q = 0; q < bs; q++)
						{
							_itow_s(bptr[q], tmp, 65, 16);
							Utils::wToUpper(tmp);
							itmp = wcslen(tmp);
							for (m = 0; m < 2 - itmp; m++)
							{
								_push('0', false);
							}
							_append(tmp, false);

							if (q != (bs - 1) && !nodelim)
								_append(delim, false);
						}
						break;

					default:
						__assume(0);
					}


				}
				break;

			case 'i':
			case 'd':
				_i = va_arg(ap, int);
				_itow_s(_i, tmp, 65, 10);
				_append(tmp, false);
				break;

			case  'u':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 10);
				_append(tmp, false);
				break;

			case 'f':
				doub = va_arg(ap, double);
				Utils::dtow_s(tmp, 65, doub);
				_append(tmp, false);
				break;

			case 'x':
			case 'a':
			case 'p':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 16);
				Utils::wToLower(tmp);
				_append(tmp, false);
				break;

			case 'X':
			case 'A':
			case 'P':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 16);
				Utils::wToUpper(tmp);
				_append(tmp, false);
				break;

			case 'z':
				if (fmt[i + 1] == 's' || fmt[i + 1] == 'c')
				{
					zm = true;
					if (fmt[i + 2] == '.')
					{
						zc = fmt[i + 3];
						m = 3;
					}
					else
					{
						zc = 'w';
						m = 1;
					}

					if (zc != 'a')
						zc = 'w';
				}
				else
					break;

			case 's':
				if (!(zm && fmt[i + 1] == 'c'))
				{
					if (zm)
					{
						switch (zc)
						{
						case 'a':
							astrptr = va_arg(ap, char*);
							itmp = strlen(astrptr);
							wstrptr = new wchar_t[itmp + 1];
							std::mbstowcs(wstrptr, astrptr, itmp);
							wstrptr[itmp] = '\0';
							break;
						case 'w':
						default:
							wstrptr = va_arg(ap, wchar_t*);
						}

						_append(wstrptr, false);

						if (zm && zc == 'a')
							delete[] wstrptr;

						i += m;
					}
					else
					{
						wstrptr = va_arg(ap, wchar_t*);
						_append(wstrptr, false);
					}
					break;
				}

			case 'c':
				if (zm)
				{
					switch (zc)
					{
					case 'a':
						c = va_arg(ap, char);
						wstrptr = new wchar_t[2];
						astrptr = new char[2];
						astrptr[0] = c; astrptr[1] = '\0';
						std::mbstowcs(wstrptr, astrptr, 1);
						wc = wstrptr[0];
						delete[] wstrptr;
						delete[] astrptr;
						break;
					case 'w':
					default:
						wc = va_arg(ap, wchar_t);
					}

					_push(wc, false);

					i += m;
				}
				else
				{
					wc = va_arg(ap, wchar_t);
					_push(wc, false);
				}
				break;
			case 'b':
				ui = va_arg(ap, unsigned int);
				_ui64tow_s(ui, tmp, 65, 2);
				Utils::wToUpper(tmp);
				_append(tmp, false);
				break;

			case 'B':
				_b = va_arg(ap, bool);
				if (_b)
					_append(L"true", false);
				else
					_append(L"false", false);
				break;

			case 'n':
				nodelim = true;
				i++;
			case 'r':
				i++;
				switch (fmt[i])
				{
				case 'd':
				case 'i':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 10);
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				case 'b':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 2);
						itmp = wcslen(tmp);
						for (m = 0; m < 8 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;
				case 'x':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 16);
						Utils::wToLower(tmp);
						itmp = wcslen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				case 'X':
					bptr = va_arg(ap, BYTE*);
					bs = va_arg(ap, size_t);
					for (size_t q = 0; q < bs; q++)
					{
						_itow_s(bptr[q], tmp, 65, 16);
						Utils::wToUpper(tmp);
						itmp = wcslen(tmp);
						for (m = 0; m < 2 - itmp; m++)
						{
							_push('0', false);
						}
						_append(tmp, false);

						if (q != (bs - 1) && !nodelim)
							_append(L" ", false);
					}
					break;

				default:
					__assume(0);
				}
				break;
			}
		}
		else
			_push(fmt[i], false);
	}

	va_end(ap);
	ReleaseMutex(m_hMutex);
}