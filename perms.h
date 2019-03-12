#ifndef FILE_H
#define FILE_H

# include <iostream>

# include "Membership_Database.h"
# include "File_Sys_Obj.h"
z
//#include "Dir.h"
//#include "File_Sys_Obj.h"
//#include "utils.h"
//
//#include<string>
//#include<vector>


using namespace std;



// returns true/false if user has perms
bool user_has_perms(const string perm_type, const File_Sys_Obj * fso, const string cur_username, Membership_Database md)
{
	cout << "in user has perms: " << fso->m_name << endl;//``````````````````````````````````````````````````````````````````````````
}

// turns 9 char perm string into 3 char, or'ing together everything that should be
string true_perm_str(const string perm_string, const string cur_username, Membership_Database md)
{

}















#endif
