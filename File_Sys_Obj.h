#ifndef FILE_SYS_OBJ_H
#define FILE_SYS_OBJ_H


# include <iostream>

#include "utils.h"

#include<string>
#include<vector>

using namespace std;


//can be a file or a dir
class File_Sys_Obj
{

public:
	string m_owning_user = "owning_user";
	int m_size = 888;
	string m_perm_str = FULL_PERM_STR;  // owner  group  public
	string m_1_pbg_thing = "1	pbg";
	bool m_is_root = false;

	string m_last_date_modified;
	string m_name;
	string m_file_sys_obj_type;


	bool is_dir()  {return is_file_sys_obj_type("dir");}
	bool is_file() {return is_file_sys_obj_type("file");}


	void update_last_date_modified()
	{
		m_last_date_modified = currentDateTime();
	}


	string owner_perm_str() { return m_perm_str.substr(0,3); }

private:

	//returns t/f if this file_sys_obj is the provided type
	bool is_file_sys_obj_type(const string type_str)
	{
		if (m_file_sys_obj_type == type_str)
			return true;
		else
			return false;
	}



};






#endif
