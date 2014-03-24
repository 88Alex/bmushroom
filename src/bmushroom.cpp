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
#include <cstdlib> // for exit(), etc.

using namespace std;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Usage: bmushroom program.bf" << endl;
		exit(0);
	}

	string filename = argv[1];
	ifstream programFile(filename.c_str());
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
		if((*i).size() < longestLength) longestLength = (*i).size();
	}

	// create the char matrix
	int sizeX = longestLength;
	int sizeY = lines.size();
	char** program = new char*[sizeX];
	for(int i = 0; i < sizeX; i++)
	{
		program[i] = new char[sizeY];
	} // amazingly this is the only way to do this

	for(int i = 0; i < lines.size(); i++)
	{
		string line = lines.at(i);
		for(int j = 0; j < line.size(); j++)
		{
			program[i][j] = line.at(j);
		}
	}
}
