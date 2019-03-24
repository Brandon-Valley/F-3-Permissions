#ifndef DIR_H
#define DIR_H

# include <iostream>

#include "File.h"
#include "utils.h"
#include "Membership_Database.h"
#include "perms.h"


#include<string>
#include<vector>

using namespace std;


const string ROOT_M_NAME = "";
const Membership_Database NULL_MEMBERSHIP_DATABASE = Membership_Database(true);



// ????????????????????????????????????????????????????????????????????????????????????????????



//----------------------------------------------------------------------------------------------------

// what should happen when the owning user of a file is deleted? should ls -l still show the deleted user as the owner?




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// check that you can do stuff to files you cant see


class Dir : public File_Sys_Obj
{
private:

	bool in_children(string name)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == name)
				return true;
		}
		return false;
	}



	File_Sys_Obj * get_p_from_children(string name)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == name)
				return m_child_p_vec[i];
		}
		return NULL;
	}


	File_Sys_Obj * get_dir_p_from_children(string dir_name)
	{
		File_Sys_Obj * dir_p = get_p_from_children(dir_name);

		if (dir_p == NULL or dir_p->is_dir() == false)
			return NULL;
		else
			return dir_p;
	}


	File_Sys_Obj * get_file_p_from_children(string filename)
	{
		File_Sys_Obj * file_p = get_p_from_children(filename);

		if (file_p == NULL or file_p->is_file() == false)
			return NULL;
		else
			return file_p;
	}


public:
	File_Sys_Obj * m_parent_dir_p;//	Dir * m_parent_dir_p;
	vector<File_Sys_Obj*> m_child_p_vec = {};



	// default constructor
	Dir(const string name, const string owning_username, const string owning_group_name)
	{
		m_name = name;
		m_last_date_modified = currentDateTime();
		m_file_sys_obj_type = "dir";
		m_owning_username = owning_username;
		m_owning_group_name = owning_group_name;
	}


	//deconstructor
	~Dir()
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->is_dir())
			{
				Dir * dir_2_delete = static_cast<Dir*>(m_child_p_vec[i]);

				for (int i = 0 ; i < dir_2_delete->m_child_p_vec.size() ; i++)
				{
					if (m_child_p_vec[i]->is_dir())
						dir_2_delete->rmdir(dir_2_delete->m_child_p_vec[i]->m_name, NULL_MEMBERSHIP_DATABASE);
					else
						dir_2_delete->rm(dir_2_delete->m_child_p_vec[i]->m_name, NULL_MEMBERSHIP_DATABASE); //set m_database null = true to give full access to delete);
				}

				delete dir_2_delete;
				m_child_p_vec.erase(m_child_p_vec.begin() + i);
				return;
			}
			else // if its a file
				rm(m_child_p_vec[i]->m_name, NULL_MEMBERSHIP_DATABASE);
		}
	}

	// perms broken !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// should you be able to do this inside a dir without write perms????????????????????????????????????????????????????????????????
	//makes new dir inside current dir and adds a pointer to it to m_dir_child_p_vec
	void mkdir(const string new_dir_name, Membership_Database md)
	{

		if (m_name != ROOT_M_NAME) //`````````````````````````````````````````````````````````````````````````````````````````````````
		{
			cout << "parent perm str: " << m_parent_dir_p->m_perm_str << endl;//````````````````````````````````````````````````````````````
			cout << "in mkdir, user_has_perms('w', m_parent_dir_p, md) = " << user_has_perms('w', m_parent_dir_p, md) << endl;//```````````````````````````````````````````````
		}

		//check if parent dir has write perms or is root
		if (m_name != ROOT_M_NAME and user_has_perms('w', m_parent_dir_p, md) == false )
			throw "mkdir: cannot create directory ‘" + new_dir_name + "’: Permission Denied";
		else
		{
			if (in_children(new_dir_name) == true)
				throw "mkdir: cannot create directory ‘" + new_dir_name + "’: Directory already exists";
			else
			{
				Dir * new_dir = new Dir(new_dir_name, md.m_curr_username, md.owning_group_name());
				new_dir->m_parent_dir_p = this;
				m_child_p_vec.push_back(new_dir);
			}
		}
	}


	// removes directory and it's children
	void rmdir(const string dir_name, Membership_Database md)
	{
		File_Sys_Obj * dir_p = get_dir_p_from_children(dir_name);

		if (dir_p == NULL)
			throw "rmdir: failed: No such directory";
		else
		{
			if (user_has_perms('w', dir_p, md) == false)
				throw "rmdir: failed to remove " + dir_name + ":  Permission Denied";
//			else if (dir_p->)
			else
			{
				for (int i = 0 ; i < m_child_p_vec.size() ; i++)
				{
					if (m_child_p_vec[i]->m_name == dir_name and m_child_p_vec[i]->is_dir())
					{
						Dir * dir_2_delete = static_cast<Dir*>(m_child_p_vec[i]);

						if(dir_2_delete->m_child_p_vec.size() > 0)
						{
							throw "rmdir: failed to remove " + dir_name + ":  Directory is not empty";
							return;
						}

						for (int i = 0 ; i < dir_2_delete->m_child_p_vec.size() ; i++)
						{
							if (m_child_p_vec[i]->is_dir())
								dir_2_delete->rmdir(dir_2_delete->m_child_p_vec[i]->m_name, NULL_MEMBERSHIP_DATABASE);
							else
								dir_2_delete->rm(dir_2_delete->m_child_p_vec[i]->m_name, NULL_MEMBERSHIP_DATABASE);
						}

						delete dir_2_delete;
						m_child_p_vec.erase(m_child_p_vec.begin() + i);
						return;
					}
				}
			}
		}
	}


	// removes file
	void rm(const string filename, Membership_Database md)
	{
		File_Sys_Obj * file_p = get_file_p_from_children(filename);

		if (file_p == NULL)
			throw "rm: failed to remove " + filename + ":  No such file";
		else
		{
			if (user_has_perms('w', file_p, md) == false)
				throw "rm: failed to remove " + filename + ":  Permission Denied";
			else
			{
				for (int i = 0 ; i < m_child_p_vec.size() ; i++)
				{
					if (m_child_p_vec[i]->m_name == filename and m_child_p_vec[i]->is_file())
					{
						delete m_child_p_vec[i];
						m_child_p_vec.erase(m_child_p_vec.begin() + i);
						return;
					}
				}
			}
		}
	}


	//lists all dirs and files in current dir
	void ls(Membership_Database md)
	{
		vector<string> output_vec;
		string line;

		//fill output_vec
		for(int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if(user_has_perms('r', m_child_p_vec[i], md)) // if current child has read permissions
			{
				if(i % 5 != 0 or i == 0)
					line += m_child_p_vec[i]->m_name + "\t";
				else
				{
					output_vec.push_back(line);
					line = m_child_p_vec[i]->m_name + "\t";
				}
			}
		}
		output_vec.push_back(line);

		//print out lines in output_vec
		for (int i = 0 ; i < output_vec.size() ; i++)
			cout << output_vec[i] << endl;
	}


	//list all files and dir's in directory with all info
	void ls_l(Membership_Database md)
	{
		for(int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if(user_has_perms('r', m_child_p_vec[i], md)) // if current child has read permissions
			{
				cout << m_child_p_vec[i]->m_perm_str << "\t" << m_child_p_vec[i]->m_owning_username << "\t" << m_child_p_vec[i]->m_owning_group_name << "\t" << m_child_p_vec[i]->m_size << "\t" << m_child_p_vec[i]->m_last_date_modified << "\t" << m_child_p_vec[i]->m_name;

				if (m_child_p_vec[i]->is_dir() == true)
					cout << "/";
				cout << endl;
			}
		}

	}

	//waiting on email - should this need read or execute perms ?????????????????????????????????????????????????????????????????????????????????
	//returns pointer to named dir or to m_parent_dict_p if cd ..
	Dir * cd(const string dir_name, Membership_Database md)
	{
		if (dir_name == "..")
		{
			if (m_is_root == true)
				throw "cd: invalid input";
			else
				return m_parent_dir_p;
		}
		else
		{

			for (int i = 0 ; i < m_child_p_vec.size() ; i++)
			{
				if (m_child_p_vec[i]->m_name == dir_name and m_child_p_vec[i]->is_dir())
				{
					if (user_has_perms('x', m_child_p_vec[i], md))
						return static_cast<Dir*>(m_child_p_vec[i]);
					else
						throw "cd: cannot access " + dir_name + ": Permission Denied";
				}
			}

			throw "cd: " + dir_name + ": No such directory";
		}
	}


	// returns string showing path from root to this dir
	string pwd()
	{
		string final_str = "";
		vector<string> parent_dir_names;
		Dir * current_dir_p = this;

		//fill parent_dir_names
		parent_dir_names.push_back(current_dir_p->m_name);

		while (current_dir_p->m_name != ROOT_M_NAME)
		{
			current_dir_p = current_dir_p->m_parent_dir_p;
			parent_dir_names.push_back(current_dir_p->m_name);
		}

		//make final_str from parent_dir_names
		for (int i = 0 ; i < parent_dir_names.size() ; i++)
			final_str = parent_dir_names[i] + '/' + final_str;

		return final_str;
	}

	//make this not work on a dir to update date without write perms!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// should you be able to do this inside a dir without write perms????????????????????????????????????????????????????????????????
	// if file already exists, update m_last_date_modified, if not, make new file
	void touch(const string name, Membership_Database md)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == name)
			{
				m_child_p_vec[i]->update_last_date_modified();
				return;
			}
		}

		File *new_file = new File(name, md.m_curr_username, md.owning_group_name());

		m_child_p_vec.push_back(new_file);
	}

	// check perms !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//preforms chmod on the file or dir given which exists inside cur_dir, not on cur_dir itself
	void chmod(const string name, const string perm_num_str)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == name)
			{
				string new_perm_str = perm_num_2_str(perm_num_str);

				m_child_p_vec[i]->m_perm_str = perm_num_2_str(perm_num_str);
				return;
			}
		}
		throw "chmod: cannot access " + name +": No such file or directory";
	}


	//"executes" file if it exists, throws exception otherwise
	void dot_slash(const string filename, Membership_Database md)
	{
		File_Sys_Obj * file_p = get_file_p_from_children(filename);

		if (file_p == NULL)
			throw "./: cannot access " + filename + ": No such file";
		else
		{
			if (user_has_perms('x', file_p, md))
				cout << filename << " executed!" << endl;
			else
				throw "./: cannot access " + filename + ": Permission Denied";
		}
	}


	// check perms !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Change the indicated file to be owned by the indicated user  fso_name = file_sys_obj_name
	void chown(const string username, const string fso_name, Membership_Database md)
	{
		if      (in_children(fso_name) == false)
			throw "chown: cannot access " + fso_name + ": does not exist";
		else if (md.user_exists(username) == false)
			throw "chown: cannot access user " + username + ": user does not exist";
		else
		{
			File_Sys_Obj * fso_p = get_p_from_children(fso_name);
			fso_p->m_owning_username = username;
		}
	}


	// check perms !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Change the indicated file to be owned by the indicated group
	void chgrp(const string group_name, const string fso_name, Membership_Database md)
	{
		if      (in_children(fso_name) == false)
			throw "chgrp: cannot access " + fso_name + ": does not exist";
		else if (md.group_exists(group_name) == false)
			throw "chgrp: cannot access group " + group_name + ": group does not exist";
		else
		{
			File_Sys_Obj * fso_p = get_p_from_children(fso_name);
			fso_p->m_owning_group_name = group_name;
		}
	}
};
















#endif
