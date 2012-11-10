#include "SFormItem.h"

SFormItem* s_func_define(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("define(native func) accepts at least 2 params");
	}
	SFormItem *firstItem = params[0];
	if(firstItem->type == ID && params_count == 2) // define variable
	{
		SFormItem *valueItem = params[1]->eval(runtime);
		runtime->pushToCurEnv(string(*firstItem->value.strVal), valueItem);
		firstItem->drop(runtime);
	}
	else if(firstItem->type == FORM) // define func
	{
		SForm *firstForm = static_cast<SForm*>(firstItem);
		if(firstForm->size() < 1)
		{
			throw new Exception("define func need at least the func's name");
		}
		SFormItem *funcNameItem = (*firstForm)[0];
		if(funcNameItem->type != ID)
		{
			throw new Exception("define func need accepts a identifier as funcName");
		}
		string funcName = *(funcNameItem->value.strVal);
		funcNameItem->drop(runtime);
		list<string> *funcParams = new list<string>();
		for(size_t i=1;i<firstForm->size();i++)
		{
			SFormItem* paramItem = (*firstForm)[i];
			if(paramItem->type != ID)
			{
				throw new Exception("when define func, params should be identifier");
			}
			// TODO: 检测参数是否有重复
			string paramName = *paramItem->value.strVal;
			funcParams->push_back(paramName);
			paramItem->drop(runtime);
		}
		SFormList *body = new SFormList(runtime);
		for(size_t i=1;i<params_count;i++)
		{
			body->push(static_cast<SForm*>(params[i]));
		}
		SFormItem *functor = new SFormItem(runtime, funcParams, body);
		runtime->pushToCurEnv(funcName, functor);
	}
	else
	{
		throw new Exception("wrong first param for define(native func)");
	}
	return new SFormItem(runtime, NIL);
}

SFormItem* s_func_lambda(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("lambda(native func) accepts at least 2 params");
	}
	SFormItem *firstItem = params[0];
	if(firstItem->type != FORM)
	{
		throw new Exception("first param of lambda func must be form");
	}
	SForm *firstForm = static_cast<SForm*>(firstItem);
	list<string> *funcParams = new list<string>();
	for(size_t i=0;i<firstForm->size();i++)
	{
		SFormItem* paramItem = (*firstForm)[i];
		if(paramItem->type != ID)
		{
			throw new Exception("when define func, params should be identifier");
		}
		// TODO: 检测参数是否有重复
		string paramName = *paramItem->value.strVal;
		funcParams->push_back(paramName);
		paramItem->drop(runtime);
	}
	SFormList *body = new SFormList(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		body->push(static_cast<SForm*>(params[i]));
	}
	SFormItem *functor = new SFormItem(runtime, funcParams, body);
	return functor;
}

SFormItem* s_func_display(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	string str;
	for(size_t i=0;i<params_count;i++)
	{
		SFormItem* param = params[i]->eval(runtime);
		str += param->toString();
	}
	cout << str;
	return new SFormItem(runtime, NIL);
}

SFormItem* s_func_number_add(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	int sum = 0;
	for(size_t i=0;i<params_count;i++)
	{
		SFormItem* param = params[i]->eval(runtime);
		if(param->type != NUM)
		{
			throw new Exception("number-add only accepts number params");
		}
		sum += param->value.numVal;
	}
	stringstream ss;
	ss << sum;
	return new SFormItem(runtime, ss.str().c_str(), NUM);
}

SFormItem* s_func_number_sub(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	int sum = 0;
	for(size_t i=0;i<params_count;i++)
	{
		SFormItem* param = params[i]->eval(runtime);
		if(param->type != NUM)
		{
			throw new Exception("number-add only accepts number params");
		}
		if(i == 0)
		{
			sum = param->value.numVal;
		}
		else
		{
			sum -= param->value.numVal;
		}
	}
	stringstream ss;
	ss << sum;
	return new SFormItem(runtime, ss.str().c_str(), NUM);
}

// 比较大小（小于），可以比较字符串，数字，布尔值等，暂时只支持整数，并且只有所有参数都满足 左边 < 右边 时才会返回true
SFormItem* s_func_number_less(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("< at least accepts 2 params");
	}
	bool result = true;
	SFormItem *lastItem = params[0]->eval(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		SFormItem *paramItem = params[i]->eval(runtime);
		if(lastItem->type == NUM && paramItem->type == NUM)
		{
			if(lastItem->value.numVal >= paramItem->value.numVal)
			{
				result = false;
				break;
			}
			else
			{
				lastItem = paramItem;
			}
		}
		else
		{
			throw new Exception("now < only accepts numbers");
		}
	}
	return new SFormItem(runtime, result);
}

// 比较大小（小于等于），可以比较字符串，数字，布尔值等，暂时只支持整数，并且只有所有参数都满足 左边 <= 右边 时才会返回true
SFormItem* s_func_number_less_or_equal(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("< at least accepts 2 params");
	}
	bool result = true;
	SFormItem *lastItem = params[0]->eval(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		SFormItem *paramItem = params[i]->eval(runtime);
		if(lastItem->type == NUM && paramItem->type == NUM)
		{
			if(lastItem->value.numVal > paramItem->value.numVal)
			{
				result = false;
				break;
			}
			else
			{
				lastItem = paramItem;
			}
		}
		else
		{
			throw new Exception("now < only accepts numbers");
		}
	}
	return new SFormItem(runtime, result);
}

// 比较大小（大于），可以比较字符串，数字，布尔值等，暂时只支持整数，并且只有所有参数都满足 左边 > 右边 时才会返回true
SFormItem* s_func_number_bigger(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("> at least accepts 2 params");
	}
	bool result = true;
	SFormItem *lastItem = params[0]->eval(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		SFormItem *paramItem = params[i]->eval(runtime);
		if(lastItem->type == NUM && paramItem->type == NUM)
		{
			if(lastItem->value.numVal <= paramItem->value.numVal)
			{
				result = false;
				break;
			}
			else
			{
				lastItem = paramItem;
			}
		}
		else
		{
			throw new Exception("now > only accepts numbers");
		}
	}
	return new SFormItem(runtime, result);
}

// 比较大小（大于等于），可以比较字符串，数字，布尔值等，暂时只支持整数，并且只有所有参数都满足 左边 >= 右边 时才会返回true
SFormItem* s_func_number_bigger_or_equal(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("> at least accepts 2 params");
	}
	bool result = true;
	SFormItem *lastItem = params[0]->eval(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		SFormItem *paramItem = params[i]->eval(runtime);
		if(lastItem->type == NUM && paramItem->type == NUM)
		{
			if(lastItem->value.numVal < paramItem->value.numVal)
			{
				result = false;
				break;
			}
			else
			{
				lastItem = paramItem;
			}
		}
		else
		{
			throw new Exception("now > only accepts numbers");
		}
	}
	return new SFormItem(runtime, result);
}

SFormItem* s_func_cond(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 1 || params[0]->type != FORM)
	{
		throw new Exception("cond need at least one form as param");
	}
	for(size_t i=0;i<params_count;i++)
	{
		if(params[i]->type != FORM)
		{
			throw new Exception("cond only accepts forms");
		}
		SForm *param = (SForm*)params[i];
		if(param->size() < 2)
		{
			throw new Exception("cond only accepts params which are forms and at least have 2 items"); 
		}
		SFormItem *condItem = (*param)[0]->eval(runtime);
		if(condItem->type != BOOLEAN)
		{
			throw new Exception("cond's params must be forms whose first item is of type boolean");
		}
		if(condItem->value.boolVal)
		{
			SFormList *l = new SFormList(runtime);
			list<SFormItem*>::const_iterator it = param->begin();
			it++;
			while(it != param->end())
			{
				l->push((*it));
				it++;
			}
			return runtime->evalInNewEnv(l);
		}
		else
		{
			continue;
		}
	}
	return new SFormItem(runtime, false);
}

// scheme中的if函数，if (true/false) true-do-this false-do-this false-do-this ...，
// 即在true-do-this之后可以有若干个false-do-this，如果判定为false，则作为一个SFormList一起在新环境中运行，运行结束后退出环境，
// 并返回最后一个form的值；如果一个false-do-this都没有，就返回false
SFormItem* s_func_if(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("if at least accepts 2 params");
	}
	SFormItem *condItem = params[0]->eval(runtime);
	if(condItem->type != BOOLEAN)
	{
		throw new Exception("if needs boolean value as first param");
	}
	if(condItem->value.boolVal == true)
	{
		return runtime->evalInNewEnv(params[1]);
	}
	else if(params_count > 2)
	{
		SFormList *l = new SFormList(runtime);
		for(size_t i=2;i<params_count;i++)
		{
			l->push(static_cast<SForm*>(params[i]));
		}
		return runtime->evalInNewEnv(l);
	}
	else
	{
		return new SFormItem(runtime, false);
	}
}

// 比较相等，可以比较字符串，数字，布尔值，函数等，暂时只支持整数，并且只有所有参数都相等时才会相等
SFormItem* s_func_equal(SchemeRuntime* runtime, SFormItem** params, size_t params_count)
{
	if(params_count < 2)
	{
		throw new Exception("= at least accepts 2 params");
	}
	bool result = true;
	SFormItem *lastItem = params[0]->eval(runtime);
	for(size_t i=1;i<params_count;i++)
	{
		SFormItem *paramItem = params[i]->eval(runtime);
		if(lastItem->type == NUM && paramItem->type == NUM)
		{
			if(lastItem->value.numVal != paramItem->value.numVal)
			{
				result = false;
				break;
			}
			else
			{
				lastItem = paramItem;
			}
		}
		else
		{
			throw new Exception("now = only accepts numbers");
		}
	}
	return new SFormItem(runtime, result);
}

SFormItem::SFormItem(SchemeRuntime* runtime, string funcName, NativeFuncType funcType, bool isNative) : type(ORIGINAL_FUNC)
{
	switch(funcType)
	{
	case NATIVE_FUNC_DEFINE:
		{
			this->value.nativeFunc.func = s_func_define;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_LAMBDA:
		{
			this->value.nativeFunc.func = s_func_lambda;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_DISPLAY:
		{
			this->value.nativeFunc.func = s_func_display;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_ADD:
		{
			this->value.nativeFunc.func = s_func_number_add;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_IF:
		{
			this->value.nativeFunc.func = s_func_if;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_COND:
		{
			this->value.nativeFunc.func = s_func_cond;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_EQUAL:
		{
			this->value.nativeFunc.func = s_func_equal;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_SUB:
		{
			this->value.nativeFunc.func = s_func_number_sub;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_LESS:
		{
			this->value.nativeFunc.func = s_func_number_less;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_LESS_OR_EQUAL:
		{
			this->value.nativeFunc.func = s_func_number_less_or_equal;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_BIGGER:
		{
			this->value.nativeFunc.func = s_func_number_bigger;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	case NATIVE_FUNC_BIGGER_OR_EQUAL:
		{
			this->value.nativeFunc.func = s_func_number_bigger_or_equal;
			this->value.nativeFunc.funcName = new string(funcName);
		}
		break;
	default:
		{
			throw new Exception("un supported native func");
		}
	}
	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, TokenType type) : type(type)
{
	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, char* strVal, TokenType type) : type(type)
{
	if(type == NUM)
	{
		this->value.numVal = atoi(strVal);
	}
	else
	{
		this->value.strVal = new string(strVal);
	}

	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, const char* strVal, TokenType type) : type(type)
{
	if(type == NUM)
	{
		this->value.numVal = atoi(strVal);
	}
	else
	{
		this->value.strVal = new string(strVal);
	}
	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, bool boolVal): type(BOOLEAN)
{
	this->value.boolVal = boolVal;
	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, list<string> *funcParamNames, SFormList *funcBody, list<FunctorNamePairValuePair> *funcBoundParams): type(SS_FUNCTOR)
{
	this->value.functor.funcParamNames = funcParamNames;
	this->value.functor.funcBody = funcBody;
	if(funcBoundParams != NULL)
	{
		this->value.functor.funcBoundParams = funcBoundParams;
	}
	else
	{
		this->value.functor.funcBoundParams = new list<FunctorNamePairValuePair>();
	}

	runtime->itemsPool->add(this);
}

SFormItem::SFormItem(SchemeRuntime* runtime, list<SFormItem> *list): type(LIST)
{
	this->value.list = list;
	runtime->itemsPool->add(this);
}

SFormItem::~SFormItem(void)
{
	if(this->type == STRING)
	{
		delete this->value.strVal;
	}
	else if(this->type == SS_FUNCTOR)
	{
		delete this->value.functor.funcParamNames;
		list<FunctorNamePairValuePair>::const_iterator it = this->value.functor.funcBoundParams->begin();
		while(it != this->value.functor.funcBoundParams->end())
		{
			delete it->name;
			it++;
		}
		delete this->value.functor.funcBoundParams;
	}
	else if(this->type == LIST)
	{
		delete this->value.list;
	}
	else if(this->type == ORIGINAL_FUNC)
	{
		delete this->value.nativeFunc.funcName;
	}
}

// TODO: 完善
string SFormItem::toString() const
{
	stringstream ss;
	if(this->type == LIST)
	{
	}
	else if(this->type == SS_FUNCTOR)
	{
		if(this->value.functor.funcBoundParams->size() == 0)
		{
			ss << "<Function(" << this->value.functor.funcParamNames->size() << ")>";
		}
		else
		{
			ss << "<Function(" << this->value.functor.funcBoundParams->size() << ")(" << this->value.functor.funcParamNames->size() << ")>";
		}
	}
	else if(this->type == STRING)
	{
		ss << *this->value.strVal;
	}
	else if(this->type == NUM)
	{
		ss << this->value.numVal;
	}
	else if(this->type == BOOLEAN)
	{
		ss << (this->value.boolVal ? "true" : "false"); // TODO
	}
	else if(this->type == ID)
	{
		ss << "ID: ";
		ss << *this->value.strVal;
	}
	else if(this->type == ORIGINAL_FUNC)
	{
		ss << "<native func>" << this->value.nativeFunc.funcName;
	}
	else
	{
		
	}
	return ss.str();
}

SFormItem* SFormItem::eval(SchemeRuntime *runtime) const
{
	if(this->type == ID)
	{
		SFormItem *value = runtime->findInEnv(*(this->value.strVal));
		if(value == NULL)
		{
			throw new Exception(string("unknown identifier: ") + *this->value.strVal);
		}
		else
		{
			return value;
		}
	}
	else
	{
		return const_cast<SFormItem*>(this);
	}
}

SFormItem* SFormItem::apply(SchemeRuntime* runtime, SFormItem** params, size_t params_count) const
{
	if(this->type == ORIGINAL_FUNC)
	{
		return this->value.nativeFunc.func(runtime, params, params_count);
	}
	else if(this->type == SS_FUNCTOR)
	{
		SFormItem *result = NULL;
		runtime->downEnv();
		size_t needed_params_count = this->value.functor.funcParamNames->size();
		if(params_count > needed_params_count)
		{
			throw new Exception("too many params for the functor");
		}
		else if(params_count == needed_params_count)
		{
			list<FunctorNamePairValuePair>::const_iterator it = this->value.functor.funcBoundParams->begin();
			while(it != this->value.functor.funcBoundParams->end())
			{
				runtime->pushToCurEnv(string(*(it->name)), it->value->eval(runtime));
				it++;
			}
			size_t i=0;
			list<string>::const_iterator it1 = this->value.functor.funcParamNames->begin();
			while(it1 != this->value.functor.funcParamNames->end())
			{
				runtime->pushToCurEnv(string(*it1), params[i++]->eval(runtime));
				it1++;
			}
			result = this->value.functor.funcBody->eval(runtime);
		}
		else
		{
			// currying
			list<FunctorNamePairValuePair> *funcBoundParams = new list<FunctorNamePairValuePair>(*this->value.functor.funcBoundParams);
			list<string> *funcParamName = new list<string>();
			size_t i = 0;
			list<string>::const_iterator it = this->value.functor.funcParamNames->begin();
			while(it != this->value.functor.funcParamNames->end())
			{
				if(i < params_count)
				{
					FunctorNamePairValuePair p;
					p.name = new string(*it);
					p.value = params[i++];
					funcBoundParams->push_back(p);
				}
				else
				{
					funcParamName->push_back(*it);
				}
				it++;
			}
			result = new SFormItem(runtime, funcParamName, this->value.functor.funcBody, funcBoundParams);
		}
		runtime->upEnv();
		return result;
	}
	else
	{
		throw new Exception("only func can be evaled");
	}
}

void SFormItem::drop(SchemeRuntime* runtime)
{
	runtime->itemsPool->remove(this);
}

set<SFormItem*>* SFormItem::getItems() const
{
	set<SFormItem*> *s = new set<SFormItem*>();
	s->insert(const_cast<SFormItem*>(this));
	return s;
}