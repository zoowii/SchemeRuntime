#pragma once

#include "stdafx.h"

enum TokenType
{
	SPECIAL_SYMBOL,
    NUM,
    ID,
    COMMENT,
    FORM_START,
    FORM_END,
	STRING,
    BOOLEAN,

    // 以下不被词法分析阶段使用
	FORM,
	FORMLIST,
    ORIGINAL_FUNC,
    SS_FUNCTOR,
    NIL,
    LIST,
    // PAIR
};
