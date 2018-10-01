#include "stsclasses.h"

/*
   _____ _                        _____           _       _   
  / ____| |                      / ____|         (_)     | |  
 | (___ | |_ ___  _ __ _ __ ___ | (___   ___ _ __ _ _ __ | |_ 
  \___ \| __/ _ \| '__| '_ ` _ \ \___ \ / __| '__| | '_ \| __|
  ____) | || (_) | |  | | | | | |____) | (__| |  | | |_) | |_ 
 |_____/ \__\___/|_|  |_| |_| |_|_____/ \___|_|  |_| .__/ \__|
                                                   | |        
                                                   |_|              
Overview:
	These files are for the official stormscript compiler
	Locations:
		sts class: stsclasses.h
		read and main: here
		out: stsstream.cpp
		parser (readline and errors): compile.cpp
		variable and function declarations: stsdec.cpp
		Executable(not in repo): bin directory
	Example files (example.sts) are stored in the root directory.
*/

void sts::read(char *argv[], int argc)
{
	string filename = argv[1];
	std::ifstream file;
	string contents;
	file.open(filename.c_str());
	char c = file.get();
	
	while (file.good())
	{
		contents += c;
		c = file.get();
	}
	file.close();

	for (int x = 0; x <= contents.size(); x++)
	{
		if (contents[x] == '\n')
		{
			sizeoff++;
		}
	} //create sizeof in lines

	sizeoff++;
	prg.resize(sizeoff); //create vector for lines
	int loc = 0;
	int a = 0;
	for (int x = 0; x <= contents.size(); x++)
	{
		if (contents[x] == '\n')
		{
			for (int y = loc; y < x; y++)
			{
				prg[a] += contents[y]; //add lines to vector
			}
			a++;
			loc = x + 1;
		}
	}
	for (int x = loc; x <= contents.size(); x++)
	{
		prg[prg.size() - 1] += contents[x];
	} //add last line to vector

	interp(filename, prg, sizeoff, argv, argc);
}

void sts::error(int num, string issue)
{
	if (num == 0)
	{
		cout << "Error: No input files" << endl;
	}
	else if (num == 1)
	{
		cout << "Error: \"" << issue << "\" is not a recognised command" << endl;
	}
	else if (num == 2)
	{
		cout << "Error: \"" << issue << "\" is not a recognised type" << endl;
	}
	else if (num == 3){
		cout << "Error: \"" << issue << "\" is not a valid expression" << endl;
	}
	else if (num == 4){
		cout << "Error: \"" << issue << "\" is an unsupported type" << endl;
	}
	else if (num == 5){
		cout << "Error: \"" << issue << "\" is an array and must use a subscript on print" << endl;
	}
	else if (num == 6){
		cout << "Error: \"" << issue << "\" is out of range" << endl; 
	}
	else if (num == 7){
		cout << "Error: return is not allowed in do{ function." << endl;
	}
	else if (num == 8){
		cout << "Error: \"" << issue << "\" is not a value." << endl;
	}
	else if (num == 9){
		cout << "Error: \"" << issue << "\" is not the correct type." << endl;
	}
	exit(0);
}

int main(int argc, char *argv[])
{
	sts script;

	if (argc != 1)
	{
		script.read(argv, argc);
	}
	else
	{
		script.error(0, "");
	}

	return 0;
}