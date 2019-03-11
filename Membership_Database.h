#ifndef MEMBERSHIP_DATABASE_H
#define MEMBERSHIP_DATABASE_H


# include <iostream>

//#include "Dir.h"
//#include "File_Sys_Obj.h"
//#include "utils.h"

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


	Membership_Database()
	{
		cout << "in constructor for md" << endl; //```````````````````````````````````````````````````````````````````````
		groupadd(ROOT_GROUP_NAME);
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

};











#endif
