#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>



#define PINLINE static inline

#define PERROR(msg) fprintf(stderr, msg);

#define WIDTH 800
#define HEIGHT 600