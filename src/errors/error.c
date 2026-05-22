#include <string.h>
#include <errors/errors.h>


// TODO: refactor to safe string version.
err make_err_s(char* msg){
  err error;
  size_t msg_sz = sizeof(msg);

  // copy string over
  error._msg = (char*)malloc( + 1); 
  error._msg = strncpy(error._msg, msg, msg_sz);
  error._msg[msg_sz] = '\0';  

  return error;
};
