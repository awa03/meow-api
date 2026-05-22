# Syntax Structure

### Structs

Member variables will be defined with a prefixed `_{variable_name}`. This syntax is used in an effort to differentiate between structure naming and their member variables.
This assist in identifying the differences between variable typing in cases such as `error.type` and `error._msg`.

### Function Naming

All functions will be named using the snake case syntax seen below.

```
init();
add_log_s("error invalid credentials");
add_log_err(err);
```

"Overloaded functions" (same functionality different argument type) will **always** be denoted by the last word within the function name. This is in an effort to enforce compatibility following function refactors.
