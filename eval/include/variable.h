#pragma once

typedef enum
{
    VALUE_INTEGER,
    VALUE_FLOAT
} ShdValueType;

typedef union
{
    int int_value;
    float float_value;
} ShdValueData;

typedef struct
{
    ShdValueType value_type;
    ShdValueData value_data;
} ShdValue;

typedef struct
{
    char *name;
    ShdValue value;
} ShdVariable;