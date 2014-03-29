/*
* BMushroom Befunge Interpreter
* Main File
* Developed by Alexander Kitaev
* Correspondence: alex.kitaev58@gmail.com
*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib> // for exit(), etc.
#include "stack.cpp"

using namespace std;

enum Direction
{
	RIGHT, LEFT, UP, DOWN
};

void displayHelp()
{
	cout << "Usage: bmushroom [options] program.bf" << endl;
	cout << "Options:" << endl;
	cout << "  -a, immediately abort if an erroneous instruction is given"
		<< endl;
	cout << "  -w: if an erroneous instruction is given, print a warning "
		<< "but do not shut down the program" << endl;
	cout << "  (note: default action on erroneous instructions "
		<< "is to do nothing)" << endl;
	cout << "  (note: -a overrides -w if both are set)" << endl;
	cout << "  -l: print a newline after outputting something" << endl;
	cout << "    (except if the value is whitespace)" << endl;
	cout << "  -s: print a space after outputting anything" << endl;
	cout << "    (except if the value is whitespace)" << endl;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		displayHelp();
		exit(0);
	}

	bool abortOn = false;
	bool warningsOn = false;
	bool newlinesOn = false;
	bool spacesOn = false;

	string arg;
	for(int i = 1; i < argc; i++) // search all except for first and last
	{
		arg = argv[i];
		if(arg.find("-h") != string::npos
			|| arg.find("--help") != string::npos)
		{
			displayHelp();
			exit(0);
		}
		if(arg.find("-a") != string::npos && !abortOn)
		{
			abortOn = true;
		}
		if(arg.find("-w") != string::npos && !warningsOn)
		{
			warningsOn = true;
		}
		if(arg.find("-n") != string::npos && !newlinesOn)
		{
			newlinesOn = true;
		}
		if(arg.find("-s") != string::npos && !spacesOn)
		{
			spacesOn = true;
		}
	}

	string filename = argv[argc]; // the last of the arguments
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

	// execute the program
	int xPos = 0;
	int yPos = 0;
	int a = 0; // temporary variable 1
	int b = 0; // temporary variable 2
	bool isStringMode = false;
	bool skipNextCell = false;
	bool endOfProgram = false;
	Direction currDirection = RIGHT;
	Stack<short> programStack = Stack<short>(); 

	while(!endOfProgram)
	{
		char currChar = program[xPos][yPos];
		if(isStringMode)
		{
			if(currChar == '"') isStringMode = false;
			else programStack.push(currChar);
		}
		else if(skipNextCell)
		{
			skipNextCell = false; // take no action other than this
		}
		else
		{
			switch(currChar)
			{
				case '0':
					programStack.push(0);
					break;
				case '1':
					programStack.push(1);
					break;
				case '2':
					programStack.push(2);
					break;
				case '3':
					programStack.push(3);
					break;
				case '4':
					programStack.push(4);
					break;
				case '5':
					programStack.push(5);
					break;
				case '6':
					programStack.push(6);
					break;
				case '7':
					programStack.push(7);
					break;
				case '8':
					programStack.push(8);
					break;
				case '9':
					programStack.push(9);
					break;
				case '+':
					a = programStack.pop();
					b = programStack.pop();
					programStack.push(a + b);
					break;
				case '-':
					a = programStack.pop();
					b = programStack.pop();
					programStack.push(b - a);
					break;
				case '*':
					a = programStack.pop();
					b = programStack.pop();
					programStack.push(a * b);
					break;
				case '/':
					a = programStack.pop();
					b = programStack.pop();
					if(a == 0)
					{
						if(abortOn)
						{
							cerr << "Error: " << filename << " @ " << xPos
								<< ", " << yPos << ":" << endl;
							cerr << "Attempted division by 0" << endl;
							exit(1);
						}
						else if(warningsOn)
						{
							cerr << "Warning: " << filename << " @ " << xPos
								<< ", " << yPos << ":" << endl;
							cerr << "Attempted division by 0" << endl;
						}
						// this code gets executed no matter what warningsOn is
						// because if abortOn is off, 0 is pushed
						programStack.push(0);
					}
					programStack.push(b / a);
					break;
				case '%':
					a = programStack.pop();
					b = programStack.pop();
					if(a == 0)
					{
						if(abortOn)
						{
							cerr << "Error: " << filename << " @ " << xPos
								<< ", " << yPos << ":" << endl;
							cerr << "Attempted modular division by 0" << endl;
							exit(1);
						}
						else if(warningsOn)
						{
							cerr << "Warning: " << filename << " @ " << xPos
								<< ", " << yPos << ":" << endl;
							cerr << "Attempted modular division by 0" << endl;
						}
						programStack.push(0);
					}
					programStack.push(b % a);
					break;
				case '!':
					a = programStack.pop();
					if(a == 0) programStack.push(1);
					else programStack.push(0);
					break;
				case '`':
					a = programStack.pop();
					b = programStack.pop();
					if(b > a) programStack.push(1);
					else programStack.push(0);
					break;
				case '>':
					currDirection = RIGHT;
					break;
				case '<':
					currDirection = LEFT;
					break;
				case '^':
					currDirection = UP;
					break;
				case 'v':
					currDirection = DOWN;
					break;
				case '?':
					a = rand() % 4;
					if(a == 0) currDirection = RIGHT;
					else if(a == 1) currDirection = LEFT;
					else if(a == 2) currDirection = UP;
					else currDirection = DOWN;
					break;
				case '_':
					a = programStack.pop();
					if(a == 0) currDirection = RIGHT;
					else currDirection = LEFT;
					break;
				case '|':
					a = programStack.pop();
					if(a == 0) currDirection = DOWN;
					else currDirection = UP;
					break;
				case '"':
					isStringMode = true;
					break;
				case ':':
					programStack.duplicate();
					break;
				case 92: // backslash- this can't be put into a char
						// because of escaping
					programStack.swap();
					break;
				case '$':
					programStack.drop();
					break;
				case '.':
					a = programStack.pop();
					cout << a;
					// we don't need to worry about space/newlines mode
					break;
				case ',':
					a = programStack.pop();
					cout << a;
					if(newlinesOn) cout << endl;
					else if(spacesOn) cout < " ";
					break;
			}
		}
	}
}
