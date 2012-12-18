#ifndef exception_h
#define exception_h

#include <string>
using namespace std;

class Exception
{
private:
	string msg;
public:
	Exception(void) {};
	Exception(string msg) : msg(msg) {}
	virtual ~Exception(void);
public:
	string what() const
	{
		return msg;
	}
};

#endif
