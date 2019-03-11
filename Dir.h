#ifndef DIR_H
#define DIR_H

# include <iostream>

#include "File.h"
#include "utils.h"

#include<string>
#include<vector>

using namespace std;


const string ROOT_M_NAME = "";



// ????????????????????????????????????????????????????????????????????????????????????????????
// what is 1 in front of pbg thing in ls -l ?
// if you try to ./ a file that exists but that you cannot see, should it say "no file of that name exists" or "access denied"?


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//fix error msg from last time



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



	File_Sys_Obj * get_file_p_from_children(string filename)
	{
//		File_Sys_Obj * file_p = get_file_p_from_children(filename);
//
//	    cout << file_p->m_name << endl;//````````````````````````````````````````````````````````````
//
//
//		if (file_p == NULL or file_p->is_file() == false)
//			return NULL;
//		else
//			return file_p;


		File_Sys_Obj * file_p = get_p_from_children(filename);

		if (file_p == NULL or file_p->is_file() == false)
			return NULL;
		else
			return file_p;

	}


public:
	Dir * m_parent_dir_p;
	vector<File_Sys_Obj*> m_child_p_vec = {};



	// default constructor
	Dir(const string name)
	{
		m_name = name;
		m_last_date_modified = currentDateTime();
		m_file_sys_obj_type = "dir";
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
						dir_2_delete->rmdir(dir_2_delete->m_child_p_vec[i]->m_name);
					else
						dir_2_delete->rm(dir_2_delete->m_child_p_vec[i]->m_name);
				}

				delete dir_2_delete;
				m_child_p_vec.erase(m_child_p_vec.begin() + i);
				return;
			}
			else // if its a file
				rm(m_child_p_vec[i]->m_name);
		}
	}


	//makes new dir inside current dir and adds a pointer to it to m_dir_child_p_vec
	void mkdir(const string new_dir_name)
	{
		if (in_children(new_dir_name) == true)
			throw "mkdir: cannot create directory ‘" + new_dir_name + "’: File exists";
		else
		{
			Dir * new_dir = new Dir(new_dir_name);
			new_dir->m_parent_dir_p = this;
			m_child_p_vec.push_back(new_dir);
		}
	}


	// removes directory and it's children
	void rmdir(const string dir_name)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == dir_name and m_child_p_vec[i]->is_dir())
			{
				Dir * dir_2_delete = static_cast<Dir*>(m_child_p_vec[i]);

				for (int i = 0 ; i < dir_2_delete->m_child_p_vec.size() ; i++)
				{
					if (m_child_p_vec[i]->is_dir())
						dir_2_delete->rmdir(dir_2_delete->m_child_p_vec[i]->m_name);
					else
						dir_2_delete->rm(dir_2_delete->m_child_p_vec[i]->m_name);
				}

				delete dir_2_delete;
				m_child_p_vec.erase(m_child_p_vec.begin() + i);
				return;
			}
		}
		throw "rmdir: failed to remove " + dir_name + ":  No such directory";
	}


	// removes file
	void rm(const string file_name)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == file_name and m_child_p_vec[i]->is_file())
			{
				delete m_child_p_vec[i];
				m_child_p_vec.erase(m_child_p_vec.begin() + i);
				return;
			}
		}
		throw "rm: failed to remove " + file_name + ":  No such file";
	}


	//lists all dirs and files in current dir
	void ls()
	{
		vector<string> output_vec;
		string line;

		//fill output_vec
		for(int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if(i % 5 != 0 or i == 0)
				line += m_child_p_vec[i]->m_name + "\t";
			else
			{
				output_vec.push_back(line);
				line = m_child_p_vec[i]->m_name + "\t";
			}
		}
		output_vec.push_back(line);

		//print out lines in output_vec
		for (int i = 0 ; i < output_vec.size() ; i++)
			cout << output_vec[i] << endl;
	}


	//list all files and dir's in directory with all info
	void ls_l()
	{
		for(int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			cout << m_child_p_vec[i]->m_perm_str << "\t" << m_child_p_vec[i]->m_1_pbg_thing << "\t" << m_child_p_vec[i]->m_owning_user << "\t" << m_child_p_vec[i]->m_size << "\t" << m_child_p_vec[i]->m_last_date_modified << "\t" << m_child_p_vec[i]->m_name;

			if (m_child_p_vec[i]->is_dir() == true)
				cout << "/";
			cout << endl;
		}

	}


	//returns pointer to named dir or to m_parent_dict_p if cd ..
	Dir * cd(const string dir_name)
	{
		if (dir_name == "..")
		{
			if (m_is_root == true)
				throw "cd: invalid input"; //this wont print, it will just say ERROR b/c its not returning anything but thats fine, im lazy
			else
				return m_parent_dir_p;
		}


		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == dir_name and m_child_p_vec[i]->is_dir())
				return static_cast<Dir*>(m_child_p_vec[i]);
		}

		throw "cd: " + dir_name + ": No such directory";
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


	// if file already exists, update m_last_date_modified, if not, make new file
	void touch(const string name)
	{
		for (int i = 0 ; i < m_child_p_vec.size() ; i++)
		{
			if (m_child_p_vec[i]->m_name == name)
			{
				m_child_p_vec[i]->update_last_date_modified();
				return;
			}
		}
		File *new_file = new File(name);
		m_child_p_vec.push_back(new_file);
	}


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


	//"executes" file if it exists, trows execption otherwise
	void dot_slash(const string filename)
	{
		File_Sys_Obj * file_p = get_file_p_from_children(filename);


		if (file_p == NULL)
			throw "./: cannot access " + filename +": No such file";
		else
		    cout << filename << " executed!" << endl;
	}



};
















#endif
