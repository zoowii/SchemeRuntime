#include "SFormItem.h"


SForm::SForm(SchemeRuntime* runtime):SFormItem(runtime, FORM)
{
	this->items = new list<SFormItem *>();
	runtime->itemsPool->add(this);
}


SForm::~SForm(void)
{
	/*
	list<SFormItem *>::iterator it = this->items->begin();
	while(it != this->items->end())
	{
		delete (*it);
		it++;
	}
	*/
	delete this->items;
}

void SForm::push(SFormItem* item)
{
	this->items->push_back(item);
}

void SForm::unshift(SFormItem* item)
{
	this->items->push_front(item);
}

SFormItem* SForm::shift()
{
	SFormItem *item = this->items->front();
	this->items->pop_front();
	return item;
}

SFormItem* SForm::pop()
{
	SFormItem *item = this->items->back();
	this->items->pop_back();
	return item;
}

// TODO
string SForm::toString() const
{
	stringstream ss;
	ss << "form(";
	ss << this->size();
	ss << ")===========\n";
	list<SFormItem *>::iterator it = this->items->begin();
	size_t i = 0;
	while(it != this->items->end())
	{
		ss << ++i << ": " << (*it)->toString() << endl;
		it++;
	}
	ss << "/form";
	return ss.str();
}

size_t SForm::size() const
{
	return this->items->size();
}

// todo: don't use this. remove the method if possible
SFormItem* SForm::operator[](const size_t index) const
{
	if(index < 0 || index >= this->items->size()) 
	{
		throw new Exception("index out of range");
	}
	list<SFormItem *>::const_iterator it = this->items->begin();
	for(size_t i=0;i<index;i++)
	{
		it++;
	}
	return *it;
}

list<SFormItem *>::const_iterator SForm::begin() const
{
	return this->items->begin();
}

list<SFormItem *>::const_iterator SForm::end() const
{
	return this->items->end();
}

void SForm::removeComments()
{
	list<SFormItem *> *newItems = new list<SFormItem *>();
	list<SFormItem *>::const_iterator it = this->items->begin();
	while(it != this->items->end())
	{
		if((*it)->type != COMMENT)
		{
			newItems->push_back(*it);
		}
		it++;
	}
	delete this->items;
	this->items = newItems;
}

SFormItem* SForm::eval(SchemeRuntime *runtime) const
{
	if(this->size() <= 0)
	{
		return new SFormItem(runtime, NIL);
	}
	SFormItem *func = (*this)[0];
	if(func->type == ID || func->type == FORM)
	{
		func = func->eval(runtime);
	}
	size_t params_count = this->size() - 1;
	SFormItem **params  = new SFormItem*[params_count];
	size_t i = 0;
	list<SFormItem *>::const_iterator it = items->begin();
	it++;
	while(it != items->end())
	{
		params[i] = *it; // 先不要eval，在调用函数内部根据需要eval
		i++;
		it++;
	}
	SFormItem *result = NULL;
	if(func->type == ORIGINAL_FUNC || func->type == SS_FUNCTOR)
	{
		result = func->apply(runtime, params, params_count);
	}
	else
	{
		throw new Exception("form can be evaled only if the first item is not func");
	}

	delete[] params;
	return result;
}

void SForm::drop(SchemeRuntime* runtime)
{
	runtime->itemsPool->remove(this);
	delete this;
}

set<SFormItem*>* SForm::getItems() const
{
	set<SFormItem*> *s = new set<SFormItem*>();
	s->insert(const_cast<SForm*>(this));
	list<SFormItem*>::const_iterator it = this->items->begin();
	while(it != this->items->end())
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

