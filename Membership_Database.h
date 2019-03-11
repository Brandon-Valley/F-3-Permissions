#ifndef MEMBERSHIP_DATABASE_H
#define MEMBERSHIP_DATABASE_H


# include <iostream>

//#include "Dir.h"+
//#include "File_Sys_Obj.h"
#include "utils.h"

#include<string>
#include<vector>

using namespace std;


const string ROOT_GROUP_NAME = "users";
const string ROOT_USER_NAME  = "root_user";




struct group
{
	string name;
	vector<string> users_vec = {};
};




//Database of groups/users
class Membership_Database
{
public:
	vector<group> m_group_vec = {};
	string m_curr_user;

	//assign defaults
	Membership_Database()
	{
		groupadd(ROOT_GROUP_NAME);
		useradd_G({ROOT_GROUP_NAME}, ROOT_USER_NAME);
		m_curr_user = ROOT_USER_NAME;
	}


	void groupadd(const string group_name)
	{
		if (group_exists(group_name))
			throw "groupadd: Could not add group " + group_name + ": group already exists";
		else
		{
			group new_group;
			new_group.name = group_name;
			m_group_vec.push_back(new_group);
		}
	}

	// test that you cant have empty group names and that it doesnt work if you use a grop name that doesnt exist !!!!!!!!!!
	void useradd_G (const vector<string> group_names_vec, const string new_user_name)
	{
		if (user_exists(new_user_name))
			throw "useradd: Could not add user " + new_user_name + ": user already exists";
		else
		{
			//check if all groups from vec exist
			for (int i = 0 ; i < group_names_vec.size() ; i++)
			{
				if (group_exists(group_names_vec[i]) == false)
				{
					throw "useradd: could not add user " + new_user_name + " one or more of the given groups do not exist";
					return;
				}
			}

			//continue if all groups exist, add new_user_name to all groups in vec
			for (int i = 0 ; i < group_names_vec.size() ; i++)
			{
				int g_pos = group_pos(group_names_vec[i]);
				m_group_vec[g_pos].users_vec.push_back(group_names_vec[i]);
			}
		}
	}


	void whoami() { cout << m_curr_user << endl; }


private:

	bool group_exists(const string group_name)
	{
		for (int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (m_group_vec[i].name == group_name)
				return true;
		}
		return false;
	}


	bool user_exists(const string username)
	{
		for (int i = 0 ; i < m_group_vec.size() ; i++)
		{
			for (int k = 0 ; k < m_group_vec[i].users_vec.size() ; k++)
			{
				if (m_group_vec[i].users_vec[k] == username)
					return true;
			}
		}
		return false;
	}


	int group_pos(const string group_name)
	{
		for(int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (m_group_vec[i].name == group_name)
				return i;
		}
	}

};











#endif
