#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

SDL_Window* win;
SDL_GLContext glcontext;

enum states {MENU, GAME, STARTING, CLOSING, MAX_STATES};

void
state_menu_init()
{
	
}

void
state_menu_run()
{
	
}

void
state_menu_close()
{
	
}

void
state_game_init()
{
	
}

char* basepath;
void
swapWindow()
{
	SDL_GL_SwapWindow(win);
}


uint32_t ticks = 0;

int fpscap = 1;
const int fpsmax = 60;

void state_changeto(enum states newstate);

void
state_game_run()
{
	ticks = SDL_GetTicks();
	
	SDL_Event e;
	
	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				state_changeto(CLOSING);
				break;
			}
		}
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	swapWindow();
	
	if(fpscap)
	{
		uint32_t ticksdiff = SDL_GetTicks() - ticks;
		if(ticksdiff < 1000 / fpsmax)
			SDL_Delay(1000 /fpsmax - ticksdiff);
	}
}

void
state_game_close()
{
	
}

enum events{INITALIZE, RUN, CLOSE, MAX_EVENTS};

static void run();
static void transition();
static void cleanup();
static void start();
static void initalize();
static void exitgame();

void(*const statetable[MAX_STATES][MAX_EVENTS])(void) = 
{
	{state_menu_init, state_menu_run, state_menu_close},//menu
	{state_game_init, state_game_run, state_game_close},//game
	{cleanup, start, initalize},//handle starting/closing
	{exitgame, 0, 0}
};

static void(*const transitionfunc[2]) (void) = 
{
	
	run, transition
};


static const  enum states FIRSTSTATE = CLOSING;

static int isrunning = 1;
static int istransitioning = 0;

static  enum states currentstate = STARTING;
static enum states nextstate;

int std_w;
int std_h;



int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	while(isrunning)
		
	(*transitionfunc[istransitioning]) ();
	
	
	return 0;
}

void state_changeto(enum states newstate)
{
	printf("STATUS: state %i is requesting a change to state %i\n", currentstate, newstate);
	istransitioning = 1;
	nextstate = newstate;
}

static void run()
{
	(*statetable[currentstate][RUN])();
}

static void
fail(const char *msg)
{
	printf("FAILED: %s", msg);
	state_changeto(CLOSING);
}

static void transition()
{
	
	printf("STATUS: transition: \"cleaning up\" last state. cleaning up the STARTING state will initalize the program.\n");
	
	(*statetable[currentstate][CLOSE]) ();
	currentstate = nextstate;
	printf("STATUS: transition: initalizing the new state. if the new state is CLOSING, then this will shutdown the program\n");
	(*statetable[currentstate][INITALIZE]) ();
	istransitioning = 0;
}

static void
cleanup()
{
	printf("STATUS: cleaning up for either an exit or a reboot\n");
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

//state: STARTING
//event: RUN
static void start()
{
	state_changeto(FIRSTSTATE);
}

//state: STARTING
//event: CLOSE
//the close event for the starting state is what gets run before the first main state,
//therefore the programs's init function is this
static void initalize()
{
	printf("STATUS: initalizing the program\n");
	
	if(SDL_Init(SDL_INIT_EVERYTHING))
		//SDL2 init failed
		fail("SDL_Init(SDL_INIT_EVERYTHING)");
	
	win = SDL_CreateWindow("Blocks", 100, 100, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(!win)
		//SDL2 didn't create a window
		fail("SDL_CreateWindow");
	
	glcontext = SDL_GL_CreateContext(win);
	if(!glcontext)
		//SDL2 didint create the required link with opengl
		fail("SDL_GL_CreateContext()");
	
	if(glewInit())
		//glew couldn't do to wrangling
		fail("glewInit()");
	
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	
	
	glClearColor(1,0,0,1);
	basepath = SDL_GetBasePath();
}

//this is the only CLOSING state function****
//state: CLOSING
//event: INITALIZE
//no other functions are in this state because the program exits after this function exits.
static void
exitgame()
{
	//cleanup before closing.
	cleanup();
	printf("STATUS: exiting the program.\n");
	isrunning = 0;
}
