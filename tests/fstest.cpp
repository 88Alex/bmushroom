#include "../src/fungespace.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int main()
{
	FungeSpace fs = FungeSpace();
	srand(time(NULL));
	for(int i = 1; i <= 100; i++)
	{
		int x = rand() % 1000000;
		int y = rand() % 1000000;
		int val = rand() % 256;
		cout << "Writing " << val << " to (" << x << ", " << y << ")" << endl;
		fs.set(x, y, val);
		int res = fs.get(x, y);
		if(res == val)
		{
			cout << "Value read back OK" << endl;
		}
		else
		{
			cerr << "Incorrect value: " << res << " (expected: " << val << ")" << endl;
		}
	}
}