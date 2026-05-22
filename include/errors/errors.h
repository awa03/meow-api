#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>

// TODO: Add more types
typedef enum{
  INVALID_ARG,
  NULL_VAL_EXCEPTION,
} type;

/*
 * Error Type
 * */
typedef struct {
  char* _msg;
  type _type;
} err;

err make_err_s(char* msg);

#endif

