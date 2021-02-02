#include "dcsa1.h"

xstringA DCSA1::HashString(xstringA str)
{
	size_t size = str.GetLen() + 1;
	size_t looplast = size;
	BYTE hash[32];

	if (size < 32)
		looplast = 32;

	size_t i = 0, j = 0, k = 0;
	bool _base = true;

	while (k < looplast)
	{
		if (_base)
			hash[j] = str[i] ^ c_base[j];
		else
			hash[j] ^= str[i];

		i++;  j++; k++;
		if (j >= 32)
		{
			j = 0;
			_base = false;
		}

		if (i >= size)
			i = 0;
	}

	i = 0; j = 0; k = 0;
	while (i < 32)
	{
		while (k < looplast)
		{
			if (j % 2 == 0)
				hash[i] ^= str[j];
			else
				hash[i] ^= ~str[j];

			j++; k++;
			if (j >= size)
				j = 0;
		}

		i++;
	}

	char rbuf[65]; rbuf[0] = '\0';
	i = 0;
	while (i < 32)
	{
		sprintf_s(rbuf, 65, "%s%x", rbuf, hash[i]);
		i++;
	}

	return rbuf;
}




xstringA DCSA1::HashBinary(LPVOID* ptr, size_t size)
{
	return "";
}