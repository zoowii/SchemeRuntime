#pragma once

#include "stdafx.h"
#include "TokenType.h"
#include "SchemeRuntime.h"
#include "NativeFuncType.h"
#include <list>
#include <set>

class SFormItem;
class SFormList;
class SchemeRuntime;

typedef struct _FunctorParamNameValuePair
{
	string *name;
	SFormItem *value;
} FunctorNamePairValuePair; 

typedef struct _S_FUNCTOR
{
	list<string> *funcParamNames;
	list<FunctorNamePairValuePair> *funcBoundParams;
	SFormList *funcBody;
} S_FUNCTOR;

typedef struct _S_NATIVE_FUNC
{
	string *funcName;
	SFormItem* (*func)(SchemeRuntime* runtime, SFormItem** params, size_t params_count);
} S_NATIVE_FUNC;

typedef union _SFormItemValue
{
	string *strVal;
	int numVal;
	bool boolVal;


	// for form item typed of func
	S_FUNCTOR functor;

	// for form item typed of list
	list<SFormItem> *list;

	// for native func
	S_NATIVE_FUNC nativeFunc;
} SFormItemValue;

class SFormItem
{
public: // TODO: change to private
	TokenType type;
	SFormItemValue value;
public:
	SFormItem(SchemeRuntime* runtime, string funcName, NativeFuncType funcType, bool isNative); // constructor for native func. the third param is used to seperate from other constructors
	SFormItem(SchemeRuntime* runtime, char* strVal, TokenType type); // constructor for string and number.
	SFormItem(SchemeRuntime* runtime, const char*strVal, TokenType type);
	SFormItem(SchemeRuntime* runtime, bool boolVal); // constructor for boolean
	SFormItem(SchemeRuntime* runtime, list<string> *funcParamNames, SFormList *funcBody, list<FunctorNamePairValuePair> *funcBoundParams = NULL); // constructor for functor	
	SFormItem(SchemeRuntime* runtime, list<SFormItem> *list); // constructor for list
	SFormItem(SchemeRuntime* runtime, TokenType type);
	virtual ~SFormItem(void);
public:
	virtual void removeComments() {};
	virtual string toString() const;
	virtual SFormItem* eval(SchemeRuntime *runtime) const;
	virtual SFormItem* apply(SchemeRuntime* runtime, SFormItem** params, size_t params_count) const;
	virtual void drop(SchemeRuntime* runtime);
	virtual set<SFormItem*>* getItems() const; // 获取本身或者子孙中所有的SFormItem对象的指针
};

class SForm : public SFormItem
{
private:
	list<SFormItem *>* items;
public:
	SForm(SchemeRuntime* runtime);
	virtual ~SForm(void);
public:
	void push(SFormItem* item);
	void unshift(SFormItem* item);
	SFormItem* shift();
	SFormItem* pop();
	size_t size() const;
	SFormItem* operator[](const size_t index) const;
	virtual list<SFormItem *>::const_iterator begin() const;
	virtual list<SFormItem *>::const_iterator end() const;
	virtual void removeComments();
	virtual string toString() const;
	virtual SFormItem* eval(SchemeRuntime *runtime) const;
	virtual void drop(SchemeRuntime* runtime);
	virtual set<SFormItem*>* getItems() const;
};

class SFormList : public SFormItem
{
private:
	list<SFormItem *> *forms;
public:
	SFormList(SchemeRuntime* runtime);
	virtual ~SFormList(void);
public:
	void push(SFormItem* form);
	void unshift(SFormItem* form);
	SFormItem* pop();
	size_t size() const;
	SFormItem* operator[](const size_t index) const;
	list<SFormItem *>::const_iterator begin() const;
	list<SFormItem *>::const_iterator end() const;
	virtual void removeComments();
public:
	virtual string toString() const;
	virtual SFormItem* eval(SchemeRuntime *runtime) const;
	virtual void drop(SchemeRuntime* runtime);
	virtual set<SFormItem*>* getItems() const;
};

