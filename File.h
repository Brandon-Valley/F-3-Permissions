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
	File(const string name)
	{
		m_name = name;
		m_last_date_modified = currentDateTime();
		m_file_sys_obj_type = "file";
	}






};






#endif

