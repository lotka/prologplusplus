#include "prolog++.h"

string cropString(string &input_string, int n)
{
	string crop;
	int i = 0;
	for(i = 0; i < input_string.size(); ++i)
	{
		if(i == n)
		{
			break;
		}
		else
		{
			crop.push_back(input_string[i]);
		}
	}
	input_string.erase(0,i+1);
	return crop;
}

string cropString(string &input_string, char delimiter)
{
	string crop;
	int i = 0;
	for(i = 0;i < input_string.size(); ++i)
	{
		if(input_string[i] == delimiter)
		{
			break;
		}
		else
		{
			crop.push_back(input_string[i]);
		}
	}
	input_string.erase(0,i+1);
	return crop;
}

int countChar(string s, char c)
{
	int count = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]==c)
		{
			++count;
		}
	}
	return count;
}

int countChar(string s, char c, char delimiter)
{
	int count = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]==delimiter)
		{
			return count;
		}
		if(s[i]==c)
		{
			++count;
		}
	}
	return count;
}

string strip_ws(string s)
{
	string new_s;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]!=' ')
		{
			new_s.push_back(s[i]);
		}
	}
	return new_s;
}
