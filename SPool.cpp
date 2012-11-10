#include "SPool.h"


SPool::SPool(void)
{
}


SPool::~SPool(void)
{
	set<SFormItem*>::const_iterator it = this->items.begin();
	while(it != this->items.end())
	{
		delete *it;
		it++;
	}
	this->items.clear();
}

void SPool::add(SFormItem* item)
{
	this->items.insert(item);
}

void SPool::remove(SFormItem* item)
{
	this->items.erase(item);
}

void SPool::display() const
{
	cout << "pool items=======================" << this->items.size() << endl;
	set<SFormItem*>::const_iterator it = this->items.begin();
	while(it != this->items.end())
	{
		cout << (*it)->toString() << endl;
		it++;
	}
	cout << "done pool items====================" << endl;
}

void SPool::releaseExcept(set<SFormItem*> *items)
{
	set<SFormItem *>::const_iterator it = this->items.begin();
	set<SFormItem*> removedItems;
	while(it != this->items.end())
	{
		if(items->count(*it) <= 0)
		{
			removedItems.insert(*it);
			delete *it;
		}
		it++;
	}
	delete items;
	set<SFormItem*>::const_iterator it1 = removedItems.begin();
	while(it1 != removedItems.end())
	{
		this->items.erase(*it1);
		it1++;
	}
}
