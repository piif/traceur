#ifdef SIMULATE

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

typedef unsigned char byte;
typedef unsigned char boolean;

int atoi(string s) {
	istringstream istr(s);
	int i = -1;
	istr >> i;
	return i;
}

class Stream {
public:
	void print(string s) {
		cout << s;
	}
	void print(int v) {
		cout << v;
	}
	void println(string s) {
		cout << s << endl;
	}
	void println(int v) {
		cout << v << endl;
	}
};

Stream Serial;

#endif // SIMULATE
