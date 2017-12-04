#pragma once
#include<iostream>  
#include<stdio.h>   
#include<string>
#include <string.h> 
#include <assert.h>

using namespace std;

unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
std::string UrlEncode(const std::string& str)
{
	cout << "in the function: 'UrlEncode' " << endl;
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}
std::string UrlDecode(const std::string& str)
{
	cout << "in the function: 'UrlDecode' " << endl;
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}


/*old.version*/
/*static unsigned char char_to_hex(unsigned char x)
{
	return (unsigned char)(x > 9 ? x + 55 : x + 48);
}

static int is_alpha_number_char(unsigned char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return 1;
	return 0;
}

//url编码实现   

void UrlEncode(unsigned char * src, int  src_len, unsigned char * dest, int  dest_len)
{
	unsigned char ch;
	int  len = 0;

	while (len < (dest_len - 4) && *src)
	{
		ch = (unsigned char)*src;
		if (*src == ' ')
		{
			*dest++ = '+';
		}
		else if (is_alpha_number_char(ch) || strchr("=!~*'()", ch))
		{
			*dest++ = *src;
		}
		else
		{
			*dest++ = '%';
			*dest++ = char_to_hex((unsigned char)(ch >> 4));
			*dest++ = char_to_hex((unsigned char)(ch % 16));
		}
		++src;
		++len;
	}
	*dest = 0;
	return;
}



//解url编码实现   

string UrlDecode(unsigned char* encd, string decd)//unsigned char* decd
{
	
	int j, i;
	char *cd = (char*)encd;
	char p[2];
	unsigned int num;
	j = 0;
	decd.clear();
	for (i = 0; i < strlen(cd); i++)
	{
		memset(p, '/0', 2);
		if (cd[i] != '%')
		{
			decd += cd[i];
			//decd[j++] = cd[i];
			continue;
		}

		p[0] = cd[++i];
		p[1] = cd[++i];

		p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);
		p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);
		decd += (unsigned char)(p[0] * 16 + p[1]);
		//decd[j++] = (unsigned char)(p[0] * 16 + p[1]);

	}
	//decd[j] = '/0';
	return decd;
	//return decd;
}
*/