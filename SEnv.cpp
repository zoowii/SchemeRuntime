#include "SEnv.h"

SEnv::SEnv(void) :
		parent(NULL), curProcess(NULL), isLastStmt(false) {
	this->items = new map<string, SFormItem *>();
}

SEnv::SEnv(SEnv *parent) :
		parent(parent), curProcess(NULL), isLastStmt(false) {
	this->items = new map<string, SFormItem *>();
}

SEnv::~SEnv(void) {
	/*
	 map<string, SFormItem *>::const_iterator it = items.begin();
	 set<SFormItem*> s1;
	 size_t i = 0;
	 while(it != items.end())
	 {
	 if(s1.count(it->second) <= 0) {
	 s1.insert(it->second);
	 }
	 it++;
	 }
	 this->items.clear();
	 set<SFormItem*>::const_iterator it1 = s1.begin();
	 while(it1 != s1.end())
	 {
	 delete (*it1);
	 it1++;
	 }
	 if(parent != NULL)
	 {
	 delete parent;
	 }
	 */
	delete this->items;
	if (parent != NULL) {
		delete parent;
	}
}

void SEnv::push(string name, SFormItem *item) {
	(*items)[name] = item;
}

void SEnv::clear() {
	this->items->clear();
}

SFormItem* SEnv::find(string name) {
	map<string, SFormItem*>::const_iterator it = this->items->begin();
	while (it != this->items->end()) {
		if (it->first == name) {
			return it->second;
		}
		it++;
	}
	if (parent == NULL) {
		return NULL;
	} else {
		return parent->find(name);
	}
}

void SEnv::setParent(SEnv *parent) {
	this->parent = parent;
}

set<SFormItem*>* SEnv::getItems() {
	map<string, SFormItem *>::const_iterator it = this->items->begin();
	set<SFormItem*> *itemSet = new set<SFormItem*>();
	while (it != this->items->end()) {
		if (it->second != NULL) {
			itemSet->insert(it->second);
		}
		it++;
	}
	if (this->parent == NULL) {
		return itemSet;
	}
	set<SFormItem*> *s1 = this->parent->getItems();
	set<SFormItem*>::const_iterator it1 = s1->begin();
	while (it1 != s1->end()) {
		itemSet->insert(*it1);
		it1++;
	}
	delete s1;
	return itemSet;
}

void SEnv::display() {
	cout << "env items================" << endl;
	set<SFormItem*> *s = this->getItems();
	set<SFormItem*>::const_iterator it1 = s->begin();
	while (it1 != s->end()) {
		cout << (*it1)->toString() << endl;
		it1++;
	}
	cout << "done env items====================" << endl;
	delete s;
}

void SEnv::displayCurrent() {
	cout << "env items(" << this->items->size() << ")================" << endl;
	map<string, SFormItem*>::const_iterator it = this->items->begin();
	while (it != this->items->end()) {
		cout << "hi" << endl;
		pair<string, SFormItem*> p = *it;
		cout << it->second->toString() << endl;
		it++;
	}
	cout << "done env items====================" << endl;
}

SEnv* SEnv::getParent() {
	return parent;
}
