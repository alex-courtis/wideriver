#ifndef ARGS_H
#define ARGS_H

// parse river command args, NULL on failure
const struct Cmd *args_cmd(int argc, char **argv);

// populate cfg
void args_cli(int argc, char **argv);

#endif // ARGS_H
