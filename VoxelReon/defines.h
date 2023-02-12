#pragma once


#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>


char* basepath;


SDL_Window* win;
SDL_GLContext glcontext;
char *
getbasepath()
{
	return basepath;
}

static void
fail(char* msg)
{
	printf("ERROR: %s", msg);
	//state_changeto(CLOSING);
}

int windoww;
int windowh;

void
updatewindowbounds(int w, int h)
{
	glViewport(0, 0, w, h);
	windoww = w;
	windowh = h;
	printf("resize\n");
}

void
getwindowsize(int *w, int*h)
{
	*w = windoww;
	*h = windowh;
}


void
centermouse()
{
	printf("c\n");
	SDL_WarpMouseInWindow(win, windoww/2, windowh/2);
}

/*
mat4_t projection = getprojectionmatrix(90, (float)windoww / (float)windowh, .1, 100);
	
	static float i = 4;
	static int wobbledir = 0;
	if(wobbledir)
	{
		i+= .3;
		if(i>6)
			wobbledir = 0;
	} else {
		i -= .1;
		if(i<-6)
			wobbledir = 1;
	}
	
	vec3_t pos;
	pos.x = i;
	pos.y = 4;
	pos.z = 3;
	vec3_t target;
	target.x = 0;
	target.y = 0;
	target.z = 0;
	vec3_t up;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	mat4_t view = getviewmatrix(pos, target, up);
	
	mat4_t mvp;
	dotmat4mat4(&mvp, &projection, &view);
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.mat);
	*/