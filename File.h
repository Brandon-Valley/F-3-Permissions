#ifndef FILE_H
#define FILE_H


# include <iostream>

//#include "Dir.h"
#include "File_Sys_Obj.h"
#include "utils.h"

#include<string>
#include<vector>





class File : public File_Sys_Obj
{

public:
	// default constructor
	File(const string filename, const string owning_username, const string owning_group_name)
	{
		m_name = filename;
		m_last_date_modified = currentDateTime();
		m_file_sys_obj_type = "file";
		m_owning_username = owning_username;
		m_owning_group_name = owning_group_name;
	}






};






#endif

