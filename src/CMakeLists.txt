set(MAIN_SRC main.cpp)

# Form the full path to the source files...
PREPEND(MAIN_SRC)

# ... and pass the variable to the parent scope.
set(MAIN_SRC ${MAIN_SRC}  PARENT_SCOPE)