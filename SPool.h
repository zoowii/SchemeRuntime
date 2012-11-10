#pragma once
#include "SFormItem.h"
#include <set>

class SFormItem;

/*
 * SFormItem对象（包括派生对象）池，所有存活的SFormItem对象，无论是游离的还是被其他变量绑定的，
 * 对象的地址都在这里有存储。所有在运行结束后要释放这里面的SFormItem对象
 * 但考虑到env可能被重复使用，所以env中有的SFormItem对象不应该立刻被删除，为了达到这个目的，使用releaseExcept方法可以做到。
 **/

class SPool
{
private:
	set<SFormItem*> items;
public:
	SPool(void);
	virtual ~SPool(void);
public:
	void add(SFormItem* item);
	void remove(SFormItem* item);
	void releaseExcept(set<SFormItem*> *items);
	void display() const;
};

