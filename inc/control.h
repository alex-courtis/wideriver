#ifndef CONTROL_H
#define CONTROL_H

#include "slist.h"

typedef void (*control_complete_fn)(void);

// execute a river control command, executing the complete on success and failure
void control_execute(const struct SList *args, control_complete_fn);

#endif // CONTROL_H
