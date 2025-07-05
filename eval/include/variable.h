#pragma once

/*
    Every value types.
*/
typedef enum
{
    VALUE_INTEGER,
    VALUE_FLOAT
} ShdValueType;

/*
    Every execution signals.
*/
typedef enum
{
    SIGNAL_NORMAL,
    SIGNAL_BREAK,
    SIGNAL_CONTINUE
} ExecSignal;

typedef union
{
    int int_value;
    float float_value;
} ShdValueData;

typedef struct
{
    ShdValueType value_type;
    ShdValueData value_data;
    ExecSignal signal;
} ShdValue;

/*
    Represents a variable.
    name: the variable's name
    value: the variable's value
*/
typedef struct
{
    char *name;
    ShdValue value;
} ShdVariable;