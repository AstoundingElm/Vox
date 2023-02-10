#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define ArraySize(arr)(sizeof(arr) / sizeof(arr[0]))
#define PINLINE static inline

#define PERROR(msg) fprintf(stderr, msg);

int WIDTH = 800;
int HEIGHT =  600;