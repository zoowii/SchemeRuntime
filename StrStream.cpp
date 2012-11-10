#include "StrStream.h"


StrStream::StrStream(string sourceStr)
{
	this->sourceStr = sourceStr;
	this->curPos = 0;
	this->restSize = sourceStr.size();
}


StrStream::~StrStream(void)
{
}

char StrStream::shift()
{
	if(this->curPos >= this->sourceStr.size())
	{
		return NULL;
	}
	this->restSize--;
	return this->sourceStr.at(this->curPos++);
}

bool StrStream::unshift()
{
	if(this->curPos <= 0)
	{
		return false;
	}
	this->restSize++;
	this->curPos--;
	return true;
}

bool StrStream::hasNext()
{
	return this->curPos < this->sourceStr.size();
}

char StrStream::nextCharWithoutAdvance()
{
	if(this->curPos >= this->sourceStr.size())
	{
		return NULL;
	}
	else
	{
		return this->sourceStr.at(this->curPos);
	}
}

size_t StrStream::remainingSize()
{
	return this->restSize;
}

