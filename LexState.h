#pragma once

enum LexState
{
	START,
    IN_FORM,
    IN_ID,
    IN_NUM,
    IN_COMMENT,
    IN_STRING,
    DONE_FORM,
    DONE_ID,
    DONE_NUM,
    DONE_COMMENT,
    DONE_STRING,
    ERROR
};
