/*
* BMushroom Befunge Interpreter
* Main File
* Developed by Alexander Kitaev
* Correspondence: alex.kitaev58@gmail.com
*/

// comment out this next line for releases
#define DEBUG_ON

#ifdef DEBUG_ON
#define DEBUG(code) code
#else
#define DEBUG(code)
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib> // for exit(), etc.
#include <boost/regex.hpp>
#include "stack.cpp"

using namespace std;

void displayHelp()
{
	cout << "Usage: bmushroom [options] program.bf" << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << "  -a, --abort-on-error:" << endl;
	cout << "    immediately abort if an error occurs" << endl;
	cout << "    (i.e. stack underflow or division by 0)" << endl;
	cout << "  -w, --warn-on-error:" << endl;
	cout << "    warn, but do not abort, if an error occurs" << endl;
	cout << "  " << endl;
	cout << "  (note: default action on errors is to do nothing)" << endl;
	cout << "  (note: it is strongly advised to use either -a or -w)" << endl;
	cout << "  (note: -a overrides -w if both are set)" << endl;
	cout << endl;
	cout << "  -l: print a newline after outputting a number" << endl;
	cout << "    (useful when outputting a long list of numbers)" << endl;
	cout << "  -s: print a space after outputting a number" << endl;
	cout << "    (useful when outputting a long list of numbers)" << endl;
	cout << "  (note: -l overrides -s if both are set)" << endl;
	cout << endl;
	cout << "  -i, --abort-on-invalid:" << endl;
	cout << "    abort immediately if an invalid command is executed" << endl;
	cout << "    (useful to make sure comments aren't being executed)" << endl;
	cout << "  -I, --warn-on-invalid:" << endl;
	cout << "    warn if an invalid command is executed" << endl;
	cout << "    (useful when debugging)" << endl;
	cout << endl;
	cout << "  -b, --befunge93:" << endl;
	cout << "    run program in Befunge93 compatibility mode" << endl;
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
	bool abortOnInvalid = false;
	bool warnOnInvalid = false;
	bool befunge93 = false;
	boost::basic_regex<char> tempRegex;
	boost::smatch m; // dummy variable required by Boost

	string arg;
	for(int i = 1; i < argc; i++) // search all except for first and last
	{
		arg = argv[i];
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*h");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--help") != string::npos)
		{
			displayHelp();
			exit(0);
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*a");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--abort-on-error") != string::npos)
		{
			abortOn = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*w");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--warn-on-error") != string::npos)
		{
			warningsOn = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*n");
		if(boost::regex_search(arg, m, tempRegex))
		{
			newlinesOn = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*s");
		if(boost::regex_search(arg, m, tempRegex))
		{
			spacesOn = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*i");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--abort-on-invalid") != string::npos)
		{
			abortOnInvalid = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*I");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--warn-on-invalid") != string::npos)
		{
			warnOnInvalid = true;
		}
		tempRegex = boost::basic_regex<char>("^-[A-Za-z]*b");
		if(boost::regex_search(arg, m, tempRegex)
			|| arg.find("--befunge93") != string::npos)
		{
			befunge93 = true;
		}
	}

	string filename = argv[argc - 1]; // the last of the arguments
	ifstream programFile(filename.c_str());
	vector<string> lines = vector<string>();
	string temp;

	// load all the lines into the vector
	while(true)
	{
		getline(programFile, temp);
		if(programFile.eof()) break;
		lines.push_back(temp);
	}

	// find the length of the longest line
	int longestLength = 1;
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); ++i)
	{
		if((*i).size() > longestLength) longestLength = (*i).size();
	}

	// create the char matrix
	int sizeX = longestLength;
	int sizeY = lines.size();
	char** program = new char*[sizeX];
	for(int i = 0; i < sizeX; i++)
	{
		program[i] = new char[sizeY];
	} // amazingly this is the only way to do this

	for(int i = 0; i < sizeX; i++)
	{
		for(int j = 0; j < sizeY; j++)
		{
			string line = lines.at(j);
			if(i < line.length()) program[i][j] = line.at(i);
			else program[i][j] = ' ';
		}
	}

	// execute the program
	int xPos = 0;
	int yPos = 0;
	int deltaX = 1;
	int deltaY = 0;
	int a = 0; // temporary variable 1
	int b = 0; // temporary variable 2
	bool isStringMode = false;
	bool skipNextCell = false;
	Stack<Stack<short> > stackStack = Stack<Stack<short> >();
	Stack<short> programStack = Stack<short>();
	Stack<short> tempStack = Stack<short>();
	stackStack.push(programStack);
	char currChar;

	while(true)
	{
		currChar = program[xPos][yPos];
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
				case 'a':
					programStack.push(10);
					break;
				case 'b':
					programStack.push(11);
					break;
				case 'c':
					programStack.push(12);
					break;
				case 'd':
					programStack.push(13);
					break;
				case 'e':
					programStack.push(14);
					break;
				case 'f':
					programStack.push(15);
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
					if(a != 0) programStack.push(b / a);
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
					if(a != 0) programStack.push(b % a);
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
					deltaX = 1;
					deltaY = 0;
					break;
				case '<':
					deltaX = -1;
					deltaY = 0;
					break;
				case '^':
					deltaX = 0;
					deltaY = -1;
					break;
				case 'v':
					deltaX = 0;
					deltaY = 1;
					break;
				case '?':
					a = rand() % 4;
					if(a == 0)
					{
						deltaX = 1; deltaY = 0;
					}
					else if(a == 1)
					{
						deltaX = 0; deltaY = 1;
					}
					else if(a == 2)
					{
						deltaX = -1; deltaY = 0;
					}
					else if(a == 3)
					{
						deltaX = 0; deltaY = -1;
					}
					break;
				case ']': // turn right
					a = deltaY;
					deltaY = deltaX;
					deltaX = -a;
					break;
				case '[': // turn left
					a = deltaX;
					deltaX = deltaY;
					deltaY = -a;
					break;
				case 'r':
					deltaX *= -1;
					deltaY *= -1;
					break;
				case 'x':
					deltaX = programStack.pop();
					deltaY = programStack.pop();
					break;
				case '_':
					a = programStack.pop();
					deltaY = 0;
					if(a == 0) deltaX = 1;
					else deltaY = -1;
					break;
				case '|':
					a = programStack.pop();
					deltaX = 0;
					if(a == 0) deltaY = 1;
					else deltaY = -1;
					break;
				case '"':
					isStringMode = true;
					break;
				case '\'':
					a = (xPos + deltaX) % sizeX;
					b = (yPos + deltaY) % sizeY;
					programStack.push(program[a][b]);
					break;
				case 's':
					a = (xPos + deltaX) % sizeX;
					b = (yPos + deltaY) % sizeY;
					program[a][b] = programStack.pop();
					break;
				case ':':
					programStack.duplicate();
					break;
				case '\\':
					programStack.swap();
					break;
				case '$':
					programStack.drop();
					break;
				case 'n':
					programStack.clear();
					break;
				case '{':
					// TODO
					break;
				case '}':
					// TODO
					break;
				case 'u':
					// TODO
					break;
				case '.': // output integer
					a = programStack.pop();
					cout << a;
					if(newlinesOn) cout << endl;
					else if(spacesOn) cout << " ";
					break;
				case ',': // output character
					a = programStack.pop();
					cout << static_cast<char>(a);
					break;
				case '#':
					skipNextCell = true;
					break;
				case '&':
					cin >> a;
					programStack.push(a);
					break;
				case '~':
					char input;
					cin >> input;
					programStack.push(static_cast<short>(input));
					break;
				case '@':
					exit(0);
				case 'q':
					a = programStack.pop();
					exit(a);
				case ' ':
					break;
				default: // something that should not have been executed
					if(abortOnInvalid)
					{
						cerr << "Error: " << filename << " @ " << xPos
							<< ", " << yPos << ":" << endl;
						cerr << "Invalid instruction " << currChar << endl;
						exit(1);
					}
					else if(warnOnInvalid)
					{
						cerr << "Warning: " << filename << " @ " << xPos
							<< ", " << yPos << ":" << endl;
						cerr << "Invalid instruction " << currChar << endl;
					}
					break;
			}
		}
		xPos += deltaX;
		yPos += deltaY;
		xPos %= sizeX;
		yPos %= sizeY;
	}
}
