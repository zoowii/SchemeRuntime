#ifndef nativefunctype_h
#define nativefunctype_h

enum NativeFuncType
{
	NATIVE_FUNC_DEFINE,			// define
	NATIVE_FUNC_LAMBDA,			// lambda
	NATIVE_FUNC_DISPLAY,		// display
	NATIVE_FUNC_IF,				// if
	NATIVE_FUNC_COND,			// cond
	NATIVE_FUNC_CALL_CC,		// call/cc or call-with-current-continuation
	NATIVE_FUNC_LIST,			// list
	NATIVE_FUNC_N_TH,			// n-th
	NATIVE_FUNC_STR_LEN,		// str-len
	NATIVE_FUNC_LIST_SIZE,		// list-size
	NATIVE_FUNC_PAIR,			// pair
	NATIVE_FUNC_CAR,			// car
	NATIVE_FUNC_CDR,			// cdr
	NATIVE_FUNC_AND,			// and
	NATIVE_FUNC_OR,				// or
	NATIVE_FUNC_ADD,			// +
	NATIVE_FUNC_SUB,			// -
	NATIVE_FUNC_MUL,			// *
	NATIVE_FUNC_EQUAL,			// =
	NATIVE_FUNC_LESS,			// <
	NATIVE_FUNC_LESS_OR_EQUAL,	// <=
	NATIVE_FUNC_BIGGER,			// >
	NATIVE_FUNC_BIGGER_OR_EQUAL // >=
};

#endif
