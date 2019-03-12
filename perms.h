#ifndef PERMS_H
#define PERMS_H

# include <iostream>

# include "Membership_Database.h"
# include "File_Sys_Obj.h"

//#include "Dir.h"
//#include "File_Sys_Obj.h"
//#include "utils.h"
//
//#include<string>
//#include<vector>


using namespace std;

const string FULL_TRUE_PERM_STR = "rwx";

string or_perm_strings(const string a, const string b)
{
	string output_perm_str = "---";

	for (int i = 0 ; i < FULL_TRUE_PERM_STR.size() ; i++)
	{
		if (a[i] == FULL_TRUE_PERM_STR[i] or b[i] == FULL_TRUE_PERM_STR[i])
			output_perm_str[i] = FULL_TRUE_PERM_STR[i];
	}
	return output_perm_str;
}

// owner  group  public
string owner_perms (const string perm_str) { return perm_str.substr(0,2); }
string group_perms (const string perm_str) { return perm_str.substr(3,5); }
string public_perms(const string perm_str) { return perm_str.substr(6,8); }


// turns 9 char perm string into 3 char, or'ing together everything that should be
string build_true_perm_str(const string perm_str, const string owning_username,
		                   const string owning_group_name, Membership_Database md)
{
	string true_perm_str = public_perms(perm_str);
//	cout << "in build true perm str, or: " << or_perm_strings("rw-", "-w-") << endl;//````````````````````````

	if (owning_username == md.m_curr_username)
		true_perm_str = or_perm_strings(true_perm_str, owner_perms(perm_str));

	if (md.user_in_group(owning_username, owning_group_name))
		true_perm_str = or_perm_strings(true_perm_str, group_perms(perm_str));

	return true_perm_str;
}


// returns true/false if user has perms
bool user_has_perms(const char perm_type, const File_Sys_Obj * fso, Membership_Database md)
{
	cout << "in user has perms: " << fso->m_name << endl;//``````````````````````````````````````````````````````````````````````````
	string true_perm_str = build_true_perm_str(fso->m_perm_str, fso->m_owning_username, fso->m_owning_group_name, md);
}














#endif
