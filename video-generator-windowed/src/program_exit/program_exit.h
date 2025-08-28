#ifndef PROGRAM_EXIT_H
#define PROGRAM_EXIT_H

extern bool running;

void handle_sigint(int);

void program_exit();

#endif
