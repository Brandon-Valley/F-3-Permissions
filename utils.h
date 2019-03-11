#ifndef UTILS_H
#define UTILS_H

# include <iostream>
#include <stdio.h>
#include <time.h>

#include<string>
#include<vector>

# include "Dir.h"

using namespace std;

const string FULL_PERM_STR = "rwxrwxrwx";


//takes int and returns binary str
string single_digit_int_2_binary_str(const int a)
{
    string binary  ("");
	  int mask = 1;
	  for(int i = 0; i < 3; i++)
	  {
	  if((mask&a) >= 1)
		  binary = "1"+binary;
	  else
		  binary = "0"+binary;
	  mask<<=1;
	  }
	  return binary;
}


int char_2_int(const char in_char)
{
	return int(in_char - 48);
}


bool valid_perm_num_str(const string perm_num_str)
{
	if (perm_num_str.size() != 3)
		return false;

	for (int i = 0 ; i < perm_num_str.size() ; i++)
	{
		if (isdigit(perm_num_str[i]) == false)
			return false;

		int char_num = char_2_int(perm_num_str[i]);

		if (char_num > 7)
			return false;

	}
	return true;
}

//takes in perm_num, retruns perm_num_str
string perm_num_2_str(const string perm_num_str)
{
	if (valid_perm_num_str(perm_num_str) == false)
		throw "chmod: invalid mode: ‘" + perm_num_str + "’";

	//fill binary_perm_str
	string binary_perm_str;
	for (int i = 0 ; i < perm_num_str.size() ; i++)
		binary_perm_str += single_digit_int_2_binary_str(char_2_int(perm_num_str[i]));


	//build final_perm_str
	string final_perm_str = FULL_PERM_STR;
	for (int i = 0 ; i < binary_perm_str.size() ; i++)
	{
		if (binary_perm_str[i] == '0')
			final_perm_str[i] = '-';
	}

	return final_perm_str;
}



// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



//splits string into vector of strings using space as delim.
vector<string> split(string str, string token = " ")
{
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}



//Dir * parse()
//{
//
//}





#endif
