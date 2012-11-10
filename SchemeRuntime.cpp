#include "SchemeRuntime.h"
#include "LexState.h"


char cs[] = { '+', '-', '*', '/', '?', '!', '=', '$', '<', '>', '%', '&', '|', '_', '(', ')', '{', '}', ';', ':', '#'};
size_t cs_size = sizeof(cs) / sizeof(char);

bool isInAlphabet(char c)
{
	for(size_t i = 0;i<cs_size;i++)
	{
		if(cs[i] == c)
		{
			return true;
		}
	}
	return isLetter(c) || isDigit(c);
}

bool isIdChar(char c)
{
	return isInAlphabet(c) && !isSpecialChar(c) && !isWhiteSpace(c) && c != COMMENT_START_CHAR;
}

bool isIdStartChar(char c)
{
	return isInAlphabet(c) && !isSpecialChar(c) && !isWhiteSpace(c) && c != COMMENT_START_CHAR && !isDigit(c);
}


SchemeRuntime::SchemeRuntime(void)
{
	this->stream = NULL;
	this->env = new SEnv();
	this->itemsPool = new SPool();
	this->initEnv();
	this->list = NULL;
}


SchemeRuntime::~SchemeRuntime(void)
{
	delete this->stream;
	this->itemsPool->releaseExcept(this->env->getItems());
	delete this->env;
	delete this->itemsPool;
}

SFormList* SchemeRuntime::lexAnalise()
{
	if(stream == NULL)
	{
		return NULL;
	}
	this->resetFormList();
	LexState savedState = START, state = START;
	SFormList *formList = new SFormList(this);
	SForm *curForm = new SForm(this);
	string curItemStr("");
	size_t nestedFormCount = 0; // 处在第几层form(包含嵌套)
	char c; // current char

// stream_loop:
	while(this->stream->hasNext())
	{
		c = this->stream->shift();
		switch(state)
		{
		case START:
			{
				if(isWhiteSpace(c))
				{
					// do nothing
				}
				else if(c == FORM_START_CHAR)
				{
					state = IN_FORM;
					nestedFormCount++;
					/*
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curForm->push(new SFormItem(this, str, FORM_START));
					*/
				}
				else if(c == COMMENT_START_CHAR)
				{
					savedState = state;
					state = IN_COMMENT;
				}
				else
				{
					savedState = state;
					this->stream->unshift();
					throw new Exception("un expected error in state start");
				}
			}
			break;
		case IN_FORM:
			{
				if(c == COMMENT_START_CHAR)
				{
					savedState = state;
					state = IN_COMMENT;
				}
				else if(isIdStartChar(c))
				{
					state = IN_ID;
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
				else if(isDigit(c))
				{
					state = IN_NUM;
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
				else if(isWhiteSpace(c))
				{
					// do nothing
				}
				else if(c == FORM_END_CHAR)
				{
					state = DONE_FORM;
				}
				else if(c == FORM_START_CHAR)
				{
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curForm->push(new SFormItem(this, str, FORM_START));
					nestedFormCount++;
				}
				else if(c == STRING_WRAPPER)
				{
					state = IN_STRING;
				}
				else
				{
					savedState = state;
					this->stream->unshift();
					throw new Exception("wrong char in form");
				}
			}
			break;
		case IN_ID:
			{
				if(isIdChar(c))
				{
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
				else if(isWhiteSpace(c))
				{
					state = DONE_ID;
				}
				else if(isFormChar(c) || c == COMMENT_START_CHAR)
				{
					state = DONE_ID;
					this->stream->unshift();
				}
				else
				{
					savedState = state;
					this->stream->unshift();
					throw new Exception("wrong char in form");
				}
			}
			break;
		case IN_NUM:
			{
				if(isWhiteSpace(c))
				{
					state = DONE_NUM;
				}
				else if(isDigit(c))
				{
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
				else if(isFormChar(c) || c == COMMENT_START_CHAR)
				{
					state = DONE_NUM;
					this->stream->unshift();
				}
				else
				{
					savedState = state;
					this->stream->unshift();
					throw new Exception("wrong char in form, may be you use identifier starts with digit");
				}
			}
			break;
		case IN_COMMENT:
			{
				if(c == COMMENT_END_CHAR)
				{
					state = DONE_COMMENT;
				}
				else
				{
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
			}
			break;
		case IN_STRING:
			{
				// escape char
				if(c == STRING_WRAPPER)
				{
					state = DONE_STRING;
				}
				else
				{
					char *str = new char[2];
					str[0] = c;
					str[1] = '\0';
					curItemStr.append(string(str));
				}
			}
			break;
		case DONE_FORM:
			{
				this->stream->unshift();
				if(nestedFormCount < 1)
				{
					savedState = state;
					throw new Exception("done form error: nested form count < 1");
				}
				else if(nestedFormCount == 1)
				{
					formList->push(curForm);
					curForm = new SForm(this);
					state = START;
					savedState = START; // may not need
					nestedFormCount = 0;
				}
				else
				{
					SForm *tmpForm = new SForm(this);
					while(curForm->size() > 0)
					{
						SFormItem* tmpItem = curForm->pop();
						if(tmpItem->type == FORM_START)
						{
							tmpItem->drop(this);
							break;
						}
						tmpForm->unshift(tmpItem);
					}
					curForm->push(tmpForm);
					nestedFormCount--;
					state = IN_FORM;
				}
			}
			break;
		case DONE_ID:
			{
				this->stream->unshift();
				curForm->push(new SFormItem(this, curItemStr.c_str(), ID));
				curItemStr = string();
				state = IN_FORM;
			}
			break;
		case DONE_NUM:
			{
				this->stream->unshift();
				curForm->push(new SFormItem(this, curItemStr.c_str(), NUM));
				curItemStr = string();
				state = IN_FORM;
			}
			break;
		case DONE_COMMENT:
			{
				this->stream->unshift();
				curForm->push(new SFormItem(this, curItemStr.c_str(), COMMENT));
				curItemStr = string();
				state = savedState;
				savedState = START; // may not need
			}
			break;
		case DONE_STRING:
			{
				this->stream->unshift();
				curForm->push(new SFormItem(this, curItemStr.c_str(), STRING));
				curItemStr = string();
				state = IN_FORM;
			}
			break;
		default:
			{
				throw new Exception("unknown state");
				// break stream_loop
			}
		}
	}
	this->resetStream();
	this->list = formList;
	return formList;
}

void SchemeRuntime::initEnv()
{
	if(env != NULL)
	{
		delete env;
		env = new SEnv();
	}
	else
	{
		env = new SEnv();
	}
	// TODO: put init env items to env
	env->push("newline", new SFormItem(this, "\n", STRING));
	env->push("define", new SFormItem(this, "define", NATIVE_FUNC_DEFINE, true));
	env->push("lambda", new SFormItem(this, "lambda", NATIVE_FUNC_LAMBDA, true));
	env->push("display", new SFormItem(this, "display", NATIVE_FUNC_DISPLAY, true));
	env->push("if", new SFormItem(this, "if", NATIVE_FUNC_IF, true));
	env->push("cond", new SFormItem(this, "cond", NATIVE_FUNC_COND, true));
	env->push("+", new SFormItem(this, "+", NATIVE_FUNC_ADD, true));
	env->push("-", new SFormItem(this, "-", NATIVE_FUNC_SUB, true));
	env->push("=", new SFormItem(this, "=", NATIVE_FUNC_EQUAL, true));
	env->push("<", new SFormItem(this, "<", NATIVE_FUNC_LESS, true));
	env->push("<=", new SFormItem(this, "<=", NATIVE_FUNC_LESS_OR_EQUAL, true));
	env->push(">", new SFormItem(this, ">", NATIVE_FUNC_BIGGER, true));
	env->push(">=", new SFormItem(this, ">=", NATIVE_FUNC_BIGGER_OR_EQUAL, true));
	SFormItem *trueItem = new SFormItem(this, true);
	env->push("true", trueItem);
	env->push("false", new SFormItem(this, false));
	env->push("else", trueItem);
}

SEnv* SchemeRuntime::getEnv() const
{
	return this->env;
}

bool SchemeRuntime::downEnv()
{
	SEnv* newEnv = new SEnv(this->env);
	this->env = newEnv;
	return true;
}

bool SchemeRuntime::upEnv()
{
	if(env->getParent() == NULL)
	{
		cout << "now is root env, can't up any more" << endl;
		return false;
	}
	SEnv* curEnv = env;
	env = env->getParent();
	curEnv->setParent(NULL);
	delete curEnv;
	return true;
}

SFormItem* SchemeRuntime::findInEnv(string name)
{
	return this->env->find(name);
}

void SchemeRuntime::pushToCurEnv(string name, SFormItem *value)
{
	this->env->push(name, value);
}

void SchemeRuntime::resetStream()
{
	if(stream != NULL)
	{
		delete stream;
		stream = NULL;
	}
}

void SchemeRuntime::resetFormList()
{
	if(this->list != NULL)
	{
		list->drop(this);
		list = NULL;
	}
}

void SchemeRuntime::loadStr(string str)
{
	if(stream != NULL)
	{
		delete stream;
		stream = new StrStream(str);
	}
	else
	{
		stream = new StrStream(str);
	}
}

bool SchemeRuntime::run()
{
	if(list == NULL)
	{
		return false;
	}
	list->removeComments();
	list->eval(this);
	resetFormList();
	this->itemsPool->releaseExcept(this->env->getItems());
	return true;
}

SFormItem* SchemeRuntime::evalInNewEnv(SFormItem *item)
{
	this->downEnv();
	SFormItem* result = this->evalInCurEnv(item);
	this->upEnv();
	return result;
}

SFormItem* SchemeRuntime::evalInCurEnv(SFormItem *item)
{
	return item->eval(this);
}

