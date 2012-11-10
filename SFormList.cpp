#include "SFormItem.h"


SFormList::SFormList(SchemeRuntime* runtime) : SFormItem(runtime, FORMLIST)
{
	this->forms = new list<SFormItem *>();
	runtime->itemsPool->add(this);
}


SFormList::~SFormList(void)
{
	/*
	list<SForm *>::iterator it = this->forms->begin();
	while(it != this->forms->end())
	{
		delete (*it);
		it++;
	}
	*/
	delete this->forms;
}

void SFormList::push(SFormItem* form)
{
	this->forms->push_back(form);
}

void SFormList::unshift(SFormItem* form)
{
	this->forms->push_front(form);
}

SFormItem* SFormList::pop()
{
	SFormItem* item = this->forms->back();
	this->forms->pop_back();
	return item;
}

// TODO
string SFormList::toString() const
{
	string str("formlist\n");
	list<SFormItem *>::iterator it = this->forms->begin();
	while(it != this->forms->end())
	{
		str += (*it)->toString();
		str += "\n";
		it++;
	}
	str += "/formlist";
	return str;
}

size_t SFormList::size() const
{
	return this->forms->size();
}

// todo: don't use this. remove the method if possible
SFormItem* SFormList::operator[](const size_t index) const
{
	if(index < 0 || index >= this->forms->size()) 
	{
		throw new Exception("index out of range");
	}
	list<SFormItem *>::const_iterator it = this->forms->begin();
	for(size_t i=0;i<index;i++)
	{
		it++;
	}
	return *it;
}

list<SFormItem *>::const_iterator SFormList::begin() const
{
	return this->forms->begin();
}

list<SFormItem *>::const_iterator SFormList::end() const
{
	return this->forms->end();
}

void SFormList::removeComments()
{
	list<SFormItem *> *newItems = new list<SFormItem *>();
	list<SFormItem *>::const_iterator it = this->forms->begin();
	while(it != this->forms->end())
	{
		if((*it)->type != COMMENT)
		{
			(*it)->removeComments();
			newItems->push_back(*it);
		}
		it++;
	}
	delete this->forms;
	this->forms = newItems;
}

SFormItem* SFormList::eval(SchemeRuntime *runtime) const
{
	SFormItem *result = NULL;
	list<SFormItem *>::const_iterator it = this->forms->begin();
	while(it != this->forms->end())
	{
		result = (*it)->eval(runtime);
		it++;
	}
	return result;
}

void SFormList::drop(SchemeRuntime* runtime)
{
	runtime->itemsPool->remove(this);
	delete this;
}

set<SFormItem*>* SFormList::getItems() const
{
	set<SFormItem*> *s = new set<SFormItem*>();
	s->insert(const_cast<SFormList*>(this));
	list<SFormItem *>::const_iterator it = this->forms->begin();
	while(it != this->forms->end())
	{
		set<SFormItem*> *s1 = (*it)->getItems();
		set<SFormItem*>::const_iterator it1 = s1->begin();
		while(it1 != s1->end())
		{
			s->insert(*it1);
			it1++;
		}
		delete s1;
		it++;
	}
	return s;
}
