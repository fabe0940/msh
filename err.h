#ifndef ERR_H
#define ERR_H

#define ERR_EXEC "msh: command not found"
#define ERR_EXPORT "msh: export: invalid syntax"
#define ERR_FILE "msh: unable to access file"
#define ERR_FORK "msh: unable to fork"
#define ERR_MALLOC "msh: unable to allocate memory"
#define ERR_NULL "msh: invalid null pointer"
#define ERR_UNKNOWN "msh: unknown error"

void error(const char* msg);

#endif
