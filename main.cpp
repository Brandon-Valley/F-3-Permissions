
# include "Dir.h"
# include "Membership_Database.h"

# include <iostream>

using namespace std;




//parses input commands
Dir * parse(const string command, Dir * cur_dir, Membership_Database & md)
{
	Dir * dp_cur_dir = static_cast<Dir*>(cur_dir);

	vector<string> cv = split(command, " ");

//	cout << "cv: " << str_vec_2_str(cv) << endl;//``````````````````````````````````````````````````````````````````````````


	if      (cv.size() == 1 and   cv[0] == "quit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "exit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "pwd")   { cout << dp_cur_dir->pwd() << endl; }
	else if (cv.size() == 1 and   cv[0] == "ls")    { dp_cur_dir->ls(md); }
	else if (cv.size() == 1 and   cv[0] == "groups"){ md.groups(); }


	else if (cv.size() == 1 and   cv[0].rfind("./", 0) == 0)    { dp_cur_dir->dot_slash(cv[0].substr(2), md); } // if ./ at beginning of str, do dot_slash on str[2:-1]


	else if (cv.size() == 2 and   cv[0] == "cd")       { return cur_dir->cd   (cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "mkdir")    {        cur_dir->mkdir(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "rmdir")    {        cur_dir->rmdir(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "touch")    {        cur_dir->touch(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "rm")       {        cur_dir->rm   (cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "groupadd") {        md.groupadd   (cv[1]    ); }
	else if (cv.size() == 2 and   cv[0] == "useradd")  {        md.useradd    (cv[1]    ); }
	else if (cv.size() == 2 and   cv[0] == "userdel")  {        md.userdel    (cv[1]    ); }
	else if (cv.size() == 2 and   cv[0] == "switchto") {        md.switchto   (cv[1]    ); }

	else if (cv.size() == 2 and   cv[0] == "ls" and cv[1] == "-l") { cur_dir->ls_l(md); }


	else if (cv.size() == 3 and   cv[0] == "chmod")           { cur_dir->chmod(cv[2], cv[1], md); }
	else if (cv.size() == 3 and   cv[0] == "chown")           { cur_dir->chown(cv[1], cv[2], md); }
	else if (cv.size() == 3 and   cv[0] == "chgrp")           { cur_dir->chgrp(cv[1], cv[2], md); }


	else if (cv.size() == 4 and   cv[0] == "userdel" and cv[1] == "-G")  { md.userdel_G(cv[2], cv[3]); }
	else if (cv.size() == 4 and   cv[0] == "useradd" and cv[1] == "-G")  { md.useradd_G(cv[2], cv[3]); }

	else if (cv.size() == 4 and   cv[0] == "usermod" and cv[1] == "-a" and cv[2] == "-G")  { md.usermod_a_G(cv[3]); }


	else { throw "No command '" + command + "' found"; }

	return cur_dir;
}



int main()
{
	//make membership database
	Membership_Database md = Membership_Database();
//	md.print();

	//root is the top level directory
	Dir root = Dir(ROOT_M_NAME, md.m_curr_username, md.owning_group_name());
	Dir *cur_dir = &root;
	cur_dir->m_is_root = true;



	while (true)
	{
		try
		{
			try
			{
				try
				{
					cout << cur_dir->pwd() << "  >> ";
					string command;
					getline(cin, command);
					cur_dir = parse(command, cur_dir, md);
				}
				catch (string error_msg) { cout << error_msg << endl; }
			}
			catch(char const* error_msg) { cout << error_msg << endl; } //need this for stuff like: throw "cd: invalid input";
		}
		catch (...)   { cout << "ERROR" << endl; }
	}
	exit(0);
	return 0;
}
