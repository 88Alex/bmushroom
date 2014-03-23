/*
* BMushroom Befunge Interpreter
* Main File
* Developed by Alexander Kitaev
* Correspondence: alex.kitaev58@gmail.com
*/

#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Usage: bmushroom program.bf" << endl;
		exit(0);
	}

	string filename = argv[1];
	ifstream programFile(filename);
	vector<string> lines = vector<string>();
	string temp;

	// load all the lines into the vector
	while(!programFile.eof())
	{
		getline(programFile, temp);
		lines.push_back(temp);
	}

	// find the length of the longest line
	int longestLength = 0;
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); ++i)
	{
		if((*i).length() < longestLength) longestLength = (*i).length();
	}

	// create the char matrix
	char** program = new char[longestLength][lines.length()];
	for(int i = 0; i < lines.size(); i++)
	{
		string line = lines.at(i);
		for(j = 0; j < line.length(); j++)
		{
			program[i][j] = line.at(j);
		}
	}
}
