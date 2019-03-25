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
const string ROOT_USERNAME  = "root_user";
const int GROUP_NAMES_PER_LINE = 5;




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
	string m_curr_username;
	bool m_database_null; // used to give ~Dir() full access to delete whatever it wants

	//assign defaults
	Membership_Database(bool database_null = false)
	{
		m_database_null = database_null;
		m_curr_username = ROOT_USERNAME;
		groupadd(ROOT_GROUP_NAME);
		useradd(ROOT_USERNAME);
	}

	void print()
	{
		cout << "md: {" << endl;
		for (int i = 0 ; i < m_group_vec.size() ; i++)
			cout << "      " << m_group_vec[i].name << ":  " << str_vec_2_str(m_group_vec[i].users_vec) << endl;
		cout << "    }" << endl;
	}

	void whoami() { cout << m_curr_username << endl; }

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

	bool group_exists(const string group_name)
	{
		for (int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (m_group_vec[i].name == group_name)
				return true;
		}
		return false;
	}

	bool user_in_group(const string username, const string group_name)
	{
		int g_pos = group_pos(group_name);

		for (int i = 0 ; i < m_group_vec[g_pos].users_vec.size() ; i++)
		{
			if (m_group_vec[g_pos].users_vec[i] == username)
				return true;
		}
		return false;
	}

	//returns name of first group in m_group_vec that contains user
	string owning_group_name()
	{
		for (int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (user_in_group(m_curr_username, m_group_vec[i].name))
			{
				return m_group_vec[i].name;
			}

		}
	}


	// assignment functions VVV


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


	// make new user and add it to all groups given in vec
	void useradd_G (const string group_names_str, const string new_username)
		{
			vector<string> group_names_vec = split(group_names_str, ",");

			if (user_exists(new_username))
				throw "useradd: Could not add user " + new_username + ": user already exists";
			else
			{
				//check if all groups from vec exist
				for (int i = 0 ; i < group_names_vec.size() ; i++)
				{
					if (group_exists(group_names_vec[i]) == false)
					{
						throw "useradd: could not add user " + new_username + " one or more of the given groups do not exist";
						return;
					}
				}

				//continue if all groups exist, add new_username to all groups in vec
				for (int i = 0 ; i < group_names_vec.size() ; i++)
				{
					int g_pos = group_pos(group_names_vec[i]);
					m_group_vec[g_pos].users_vec.push_back(new_username);
				}
			}
		}

	// make new user and add it to ROOT_GROUP_NAME
	void useradd (const string new_username) { useradd_G(ROOT_GROUP_NAME, new_username); }

	// add m_curr_username to given group
	void usermod_a_G (const string group_name)
	{
		if      (group_exists(group_name) == false)
			throw "usermod: Could not add user " + m_curr_username + ": to group " + group_name + ": group does not exist";
		else if (user_in_group(m_curr_username, group_name))
			throw "usermod: Could not add user " + m_curr_username + ": to group " + group_name + ": user already in group";
		else
		{
			int g_pos = group_pos(group_name);
			m_group_vec[g_pos].users_vec.push_back(m_curr_username);
		}
	}

	//print out list of groups m_curr_username is member of
	void groups()
	{
		vector<string> group_names_2_print = groups_vec(m_curr_username);

		vector<string> output_vec;
		string line;

		//fill output_vec
		for(int i = 0 ; i < group_names_2_print.size() ; i++)
		{
			if(i % GROUP_NAMES_PER_LINE != 0 or i == 0)
				line += group_names_2_print[i] + "\t";
			else
			{
				output_vec.push_back(line);
				line = group_names_2_print[i] + "\t";
			}
		}
		output_vec.push_back(line);

		//print out lines in output_vec
		for (int i = 0 ; i < output_vec.size() ; i++)
			cout << output_vec[i] << endl;
	}

	//switch to given user
	void switchto(const string username)
	{
		if (user_exists(username) == false)
			throw "switchto: Could not switch to user " + username + ": user does not exist";
//		else if (m_curr_username == username)
//			throw "switchto: " + username + " is already the current user";
		else
			m_curr_username = username;
	}

	//deletes user from given group
	void userdel_G(const string group_name, const string username)
	{
		//check that group and user exist, and that user exists in given group if not, throw error
		if      (ROOT_USERNAME == username)
			throw "userdel: Could not delete user " + username + ": cannot delete root user";
		else if (m_curr_username == username)
			throw "userdel: Could not delete user " + username + ": cannot delete current user";
		else if (group_exists(group_name) == false)
			throw "userdel: Could not delete user " + username + ": from group " + group_name + ": group does not exist";
		else if (user_exists(username) == false)
			throw "userdel: Could not delete user " + username + ": user does not exist";
		else if (user_in_group(username, group_name) == false)
			throw "userdel: Could not delete user " + username + ": from group " + group_name + ": user does not exist in group";
		else
		{
			int g_pos = group_pos(group_name);
			int u_pos = user_pos_in_group(username, group_name);

			m_group_vec[g_pos].users_vec.erase(m_group_vec[g_pos].users_vec.begin() + u_pos); //erase user from group
		}
	}

	//delets user from all groups
	void userdel(const string username)
	{
		if (user_exists(username) == false)
		{
			throw "userdel: Could not delete user " + username + ": user does not exist";
			return;
		}

		vector<string> group_names_2_del_user_from = groups_vec(username);

		for (int i = 0 ; i < group_names_2_del_user_from.size() ; i++)
		{
//			int g_pos = group_pos(group_names_2_del_user_from[i]);
			userdel_G(group_names_2_del_user_from[i], username);
		}

	}




private:

	int group_pos(const string group_name)
	{
		for(int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (m_group_vec[i].name == group_name)
				return i;
		}
	}


	int user_pos_in_group(const string username, const string group_name)
	{
		int g_pos = group_pos(group_name);

		for (int i = 0 ; i < m_group_vec[g_pos].users_vec.size() ; i++)
		{
			if (m_group_vec[g_pos].users_vec[i] == username)
				return i;
		}
	}


	vector<string> groups_vec(const string username)
	{
		vector<string> group_names = {};

		//fill group_names_2_print
		for (int i = 0 ; i < m_group_vec.size() ; i++)
		{
			if (user_in_group(username, m_group_vec[i].name))
				group_names.push_back(m_group_vec[i].name);
		}

		return group_names;
	}



};











#endif
