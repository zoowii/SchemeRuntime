#include "stdafx.h"

class StrStream
{
private:
	string sourceStr;
	size_t curPos;
	int restSize; // count of rest chars
public:
	StrStream(string sourceStr);
	virtual ~StrStream(void);
public:
	char shift();
	bool unshift();
	bool hasNext();
	char nextCharWithoutAdvance();
	size_t remainingSize();
};

