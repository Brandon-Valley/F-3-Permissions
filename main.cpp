
# include "Dir.h"
# include "Membership_Database.h"

# include <iostream>

using namespace std;




//parses input commands
Dir * parse(const string command, Dir * cur_dir, Membership_Database md)
{
	Dir * dp_cur_dir = static_cast<Dir*>(cur_dir);

	vector<string> cv = split(command);


	if      (cv.size() == 1 and   cv[0] == "quit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "exit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "pwd")   { cout << dp_cur_dir->pwd() << endl; }
	else if (cv.size() == 1 and   cv[0] == "ls")    { dp_cur_dir->ls(md); }

	else if (cv.size() == 1 and   cv[0].rfind("./", 0) == 0)    { dp_cur_dir->dot_slash(cv[0].substr(2), md); } // if ./ at beginning of str, do dot_slash on str[2:-1]


	else if (cv.size() == 2 and   cv[0] == "cd")     { return cur_dir->cd   (cv[1]); }
	else if (cv.size() == 2 and   cv[0] == "mkdir")  {        cur_dir->mkdir(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "rmdir")  {        cur_dir->rmdir(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "touch")  {        cur_dir->touch(cv[1], md); }
	else if (cv.size() == 2 and   cv[0] == "rm")     {        cur_dir->rm   (cv[1], md); }

	else if (cv.size() == 2 and   cv[0] == "ls" and cv[1] == "-l") { cur_dir->ls_l(md); }

	else if (cv.size() == 3 and   cv[0] == "chmod")                { cur_dir->chmod(cv[2], cv[1]); }


	else { throw "No command '" + command + "' found"; }

	return cur_dir;
}




int main()
{
	//make membership database
	Membership_Database md = Membership_Database();
	md.print();

	//root is the top level directory
	Dir root = Dir(ROOT_M_NAME, md.m_curr_username, md.owning_group_name());
	Dir *cur_dir = &root;
	cur_dir->m_is_root = true;






	//testing

//	try
//	{
//
//
//		cout << md.m_group_vec.size() << endl;
//
//
//		md.whoami();
//		md.print();
//
//		cur_dir->touch("f1"  , md);
//		cur_dir->ls_l(md);
//
//		cur_dir->mkdir("stan", md);
//		cur_dir->dot_slash("f1", md);
//	//	cur_dir->dot_slash("f2");
//	//	cur_dir->dot_slash("stan");
//
//		md.groupadd("u2");
//		md.groupadd("u3");
//		md.groupadd("u4");
//		md.groupadd("u5");
//		md.groups();
//		cout << "here" << endl;
//
//		md.print();
//
//		md.useradd_G({"users", "u2", "u3", "u4", "u5"}, "joe");
//		md.print();
//
//		md.switchto("joe");
//		md.groups();
//
//		md.useradd_G({"users", "u2", "u3", "u4", "u5"}, "bob");
//		md.switchto("bob");
//		md.groups();
//		md.print();
//
//		md.userdel_G("u3", "joe");
//		md.print();
//
////		md.userdel_G("u2", "bob");
////		md.groups();
//
////		md.userdel("bob");
////		md.groups();
////		md.print();
//
////			md.switchto("jo2");
//
//		cur_dir->ls_l(md);
//
//		cout << "SPACER" << endl;
//		//make user own file when make it !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//		cout << "md.m_curr_username:  " << md.m_curr_username << endl;
//		cout << "md.owning_group_name():  " << md.owning_group_name() << endl;
//
//		cur_dir->touch("file_1", md);
//		cur_dir->ls_l(md);
//		cur_dir->chown("joe", "file_1", md);
//		cur_dir->chgrp("u4", "file_1", md);
//		cur_dir->chown("joe", "stan", md);
//		cur_dir->chgrp("u4", "stan", md);
//		cout << "SPACER" << endl;
//		cur_dir->ls_l(md);
//
//		cur_dir->dot_slash("f1", md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
//		md.print();
//		md.whoami();
//		cur_dir->chmod("f1", "654");
//		cur_dir->ls_l(md);
//		cur_dir->dot_slash("f1", md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
//		md.switchto("root_user");
//		md.print();
//		md.whoami();
//		cur_dir->chmod("f1", "654");
//		cur_dir->ls_l(md);
//		cur_dir->dot_slash("f1", md);
//
////		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
////		md.userdel_G("users", "joe");
////		md.switchto("joe");
////		md.print();
////		cur_dir->chmod("f1", "654");
////		cur_dir->ls_l(md);
////		cout << "whoami: ";
////		md.whoami();
////		cur_dir->dot_slash("f1", md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
////		md.userdel_G("users", "joe");
//		md.switchto("root_user");
//		md.print();
//		cur_dir->chmod("f1", "654");
//		cur_dir->ls_l(md);
//		cout << "whoami: ";
//		md.whoami();
//		cur_dir->rm("f1", md);
//		cur_dir->ls_l(md);
////		cur_dir->rm("stan", md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
////		md.userdel_G("users", "joe");
//		md.switchto("joe");
//		md.print();
//		cur_dir->chmod("stan", "654");
//		cur_dir->ls_l(md);
//		cout << "whoami: ";
//		md.whoami();
//		cur_dir->rmdir("stan", md);
//		cur_dir->ls_l(md);
//
////		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
////		md.userdel_G("users", "bob");
////		cur_dir->mkdir("d3", md);
////		cur_dir->touch("f3", md);
////		cur_dir->ls_l(md);
////		md.switchto("joe");
////		md.print();
////		cur_dir->chmod("d3", "052");
////		cur_dir->chmod("f3", "052");
////		cur_dir->ls_l(md);
////		cout << "whoami: ";
////		md.whoami();
////		cur_dir->ls(md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
//		md.switchto("root_user");
//		md.userdel_G("users", "bob");
//		cur_dir->mkdir("d3", md);
//		cur_dir->touch("f3", md);
//		cur_dir->ls_l(md);
//		md.switchto("joe");
//		md.print();
//		cur_dir->chmod("d3", "052");
//		cur_dir->chmod("f3", "052");
////		cur_dir->ls_l(md);
//		cout << "whoami: ";
//		md.whoami();
//		cout << "  FINAL LS_L() VVV" << endl;
//		cur_dir->ls_l(md);
//
//		cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
//		cur_dir->mkdir("wow", md);
//		cur_dir->touch("lol", md);
//		cur_dir->cd("wow");
////		cur_dir->ls(md);
////		cur_dir->mkdir("wow", md);
////		cur_dir->cd("..");
//		cur_dir->cd("wow");
////		cur_dir->rm("wow", md);
//		cur_dir->rmdir("wow", md);
////		cur_dir->cd("..");
//		cur_dir->touch("wow", md);
//		cur_dir->touch("lol", md);
////		cur_dir->rmdir("lol", md);
//		cur_dir->rm("wow", md);
//		cur_dir->rm("lol", md);
//
//
//
//	}
//	catch (string error_msg) { cout << error_msg << endl; }






//	cout << cur_dir->owner_perm_str() << endl;


	while (true)
	{
		try
		{
			try
			{
				cout << cur_dir->pwd() << "  >> ";
				string command;
				getline(cin, command);
//				cout << " !!!DELETE THIS!!! command recieved:  " << command << endl;//`````````````````````````````````````````````````````````````````````````````
				cur_dir = parse(command, cur_dir, md);
			}
			catch (string error_msg) { cout << error_msg << endl; }
		}
		catch (...)   { cout << "ERROR" << endl; }
	}
	exit(0);
	return 0;
}
