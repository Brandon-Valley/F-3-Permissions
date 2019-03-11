
# include "Dir.h"

# include <iostream>

using namespace std;




//parses input commands
Dir * parse(const string command, Dir * cur_dir)
{
	Dir * dp_cur_dir = static_cast<Dir*>(cur_dir);

	vector<string> cv = split(command);


	if      (cv.size() == 1 and   cv[0] == "quit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "exit")  { exit(0); } //command to stop loop
	else if (cv.size() == 1 and   cv[0] == "pwd")   { cout << dp_cur_dir->pwd() << endl; }
	else if (cv.size() == 1 and   cv[0] == "ls")    { dp_cur_dir->ls(); }

	else if (cv.size() == 1 and   cv[0].rfind("./", 0) == 0)    { dp_cur_dir->dot_slash(cv[0].substr(2)); } // if ./ at beginning of str, do dot_slash on str[2:-1]


	else if (cv.size() == 2 and   cv[0] == "cd")     { return cur_dir->cd(cv[1]); }
	else if (cv.size() == 2 and   cv[0] == "mkdir")  { cur_dir->mkdir(cv[1]); }
	else if (cv.size() == 2 and   cv[0] == "rmdir")  { cur_dir->rmdir(cv[1]); }
	else if (cv.size() == 2 and   cv[0] == "touch")  { cur_dir->touch(cv[1]); }
	else if (cv.size() == 2 and   cv[0] == "rm")     { cur_dir->rm(cv[1]); }

	else if (cv.size() == 2 and   cv[0] == "ls" and cv[1] == "-l") { cur_dir->ls_l(); }

	else if (cv.size() == 3 and   cv[0] == "chmod")                { cur_dir->chmod(cv[2], cv[1]); }


	else { throw "No command '" + command + "' found"; }

	return cur_dir;
}




int main()
{
	//root is the top level directory
	Dir root = Dir(ROOT_M_NAME);
	Dir *cur_dir = &root;
	cur_dir->m_is_root = true;

	cur_dir->touch("f1");
	cur_dir->mkdir("stan");
	cur_dir->dot_slash("f1");
//	cur_dir->dot_slash("f2");
//	cur_dir->dot_slash("stan");

//	cout << cur_dir->owner_perm_str() << endl;


//	while (true)
//	{
//		try
//		{
//			try
//			{
//				cout << cur_dir->pwd() << "  >> ";
//				string command;
//				getline(cin, command);
////				cout << " !!!DELETE THIS!!! command recieved:  " << command << endl;//`````````````````````````````````````````````````````````````````````````````
//				cur_dir = parse(command, cur_dir);
//			}
//			catch (string error_msg) { cout << error_msg << endl; }
//		}
//		catch (...)   { cout << "ERROR" << endl; }
//	}
	exit(0);
	return 0;
}
