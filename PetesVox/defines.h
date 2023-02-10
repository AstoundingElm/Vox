#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>



#define PINLINE static inline

#define PERROR(msg) fprintf(stderr, msg);

int WIDTH = 800;
int HEIGHT =  600;