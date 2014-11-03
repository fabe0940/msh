#ifndef ERR_H
#define ERR_H

#define ERR_EXEC "unable to exec()"
#define ERR_FILE "unable to access file"
#define ERR_FORK "unable to fork()"
#define ERR_MALLOC "unable to allocate memory"
#define ERR_NULL "invalid null pointer"
#define ERR_UNKNOWN "unknown error"

void error(const char* msg);

#endif
