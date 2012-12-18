#ifndef senv_h
#define senv_h

#include <map>
#include "SFormItem.h"
#include <set>

class SFormItem;

class SEnv
{
private:
	SEnv *parent;
	map<string, SFormItem *> *items;
public:
	SEnv(void);
	SEnv(SEnv *parent);
	virtual ~SEnv(void);
public:
	void push(string name, SFormItem *item);
	SFormItem *find(string name);
	SEnv* getParent();
	void setParent(SEnv *parent);
	set<SFormItem*>* getItems();
	void display();
	void displayCurrent();
};

#endif
