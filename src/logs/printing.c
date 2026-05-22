#include <stdio.h>
#include <logs/printing.h>

void print_log_s_(const char* str){
  printf("%s", str);
}

void print_log_err(const err* err){
  print_log_s_(err->_msg);
}


