#pragma once
#include "StrStream.h"
#include "SEnv.h"
#include "SPool.h"
#define FORM_START_CHAR '('
#define FORM_END_CHAR ')'
#define COMMENT_START_CHAR ';'
#define COMMENT_END_CHAR '\n'
#define STRING_WRAPPER '"'
#define isFormChar(c) ((c) == FORM_START_CHAR || (c) == FORM_END_CHAR)
#define isWhiteSpace(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == ',')
#define isDigit(c) ((c) >= '0' && (c) <= '9')
#define isSpecialChar(c) (isFormChar(c) || (c) == COMMENT_START_CHAR)
#define isLetter(c) ((c) >= 'A' && (c) <= 'z')

class SFormList;
class SFormItem;
class SForm;
class SEnv;
class SPool;

class SchemeRuntime
{
private:
	StrStream *stream;
	SFormList *list;
	SEnv *env;
	void resetStream();
	void resetFormList();
public:
	SPool *itemsPool;
	SEnv *getEnv() const;
public:
	void initEnv();
	SFormItem *findInEnv(string name);
	void pushToCurEnv(string name, SFormItem *value);
	SFormItem *evalInNewEnv(SFormItem *item);
	SFormItem *evalInCurEnv(SFormItem *item);
	bool downEnv(); // 在环境树的最底层下创建一个新的环境作为当前环境
	bool upEnv(); // 取消环境树的当前环境，改成当前环境的上一层环境（根环境除外）
public:
	SchemeRuntime(void);
	virtual ~SchemeRuntime(void);
public:
	void loadStr(string str);
	void loadProgram(string filepath);
	SFormList* lexAnalise();
	bool run();
};

