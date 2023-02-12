#include "defines.h"
#include "pmath.h"
#include "chunk.h"

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

GLuint vertexbuffer;
GLuint program;

chunk_t chunk;

const static  enum states FIRSTSTATE = GAME;

static int isrunning = 1;
static int istransitioning = 0;

static  enum states currentstate = STARTING;
static enum states nextstate;

void state_changeto(enum states newstate)
{
	printf("STATUS: state %i is requesting a change to state %i\n", currentstate, newstate);
	istransitioning = 1;
	nextstate = newstate;
}

#include "mainloop.h"
void
state_game_init()
{
	glGenBuffers(1, &vertexbuffer);
	
	//load shaders 'n stuff
	
	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	
	char *file;
	char *file_contents;
	long input_file_size;
	FILE *input_file;
	const char *intermediary;
	
	file = (char *)calloc(1024, sizeof(char));
	strncat(file, getbasepath(), 1024);
	strncat(file, "vertexshader", 1024 - strlen(file));
	
	input_file = fopen(file, "rb");
	free(file);
	if(!input_file)
		fail("getting a handle on the vertexshader file");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	file_contents = malloc((input_file_size + 1) * (sizeof(char)));
	fread(file_contents, sizeof(char), input_file_size, input_file);
	fclose(input_file);
	file_contents[input_file_size] = 0;
	
	intermediary = file_contents;
	glShaderSource(vertexshader, 1, &intermediary, NULL);
	glCompileShader(vertexshader);
	free(file_contents);
	
	file = (char *)calloc(1024, sizeof(char));
	strncat(file, getbasepath(), 1024);
	strncat(file, "fragmentshader", 1024 - strlen(file));
	
	input_file = fopen(file, "rb");
	free(file);
	if(!input_file)
		fail("getting a handle on the fragmentshader file");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	file_contents = malloc((input_file_size + 1) * (sizeof(char)));
	fread(file_contents, sizeof(char), input_file_size, input_file);
	fclose(input_file);
	file_contents[input_file_size] = 0;
	
	intermediary = file_contents;
	glShaderSource(fragmentshader, 1, &intermediary, NULL);
	glCompileShader(fragmentshader);
	free(file_contents);
	
	int errorlen;
	char *message;
	
	glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &errorlen);
	message = (char *)malloc(errorlen * sizeof(char));
	glGetShaderInfoLog(vertexshader, errorlen, 0, message);
	printf("INFO: vertexshader info:\n%s\n", message);
	free(message);
	
	glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &errorlen);
	message = (char *)malloc(errorlen * sizeof(char));
	glGetShaderInfoLog(fragmentshader, errorlen, 0, message);
	printf("INFO: fragmentshader info:\n%s\n", message);
	free(message);
	
	program = glCreateProgram();
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorlen);
	message = (char *)malloc(errorlen * sizeof(char));
	glGetProgramInfoLog(program, errorlen, 0, message);
	printf("INFO: opengl program info:\n%s\n", message);
	free(message);
	
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	
	
	matrix = glGetUniformLocation(program, "MVP");
	
	//load the world
	chunk = callocchunk();
	long i;
	for(i=0; i<CHUNKSIZE; i++)
	{
		chunk.data[i + CHUNKSIZE*i + CHUNKSIZE*CHUNKSIZE*i].id=1;
	}
	
	for(i=0; i<100000; i++)
		chunk.data[rand()%(CHUNKSIZE*CHUNKSIZE*CHUNKSIZE)].id=1;
	
	mesh_t mesh = getmesh(chunk, 0, 0, 0, 0, 0, 0);
	free(chunk.data);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.size * sizeof(GLfloat), mesh.data, GL_DYNAMIC_DRAW);
	
	free(mesh.data);
	
	chunk.points = mesh.size;
	
	pos.x = 0;
	pos.y = 0;
	pos.z = 3;
	rotx = 0;
	roty = 0;
	
	centermouse();
	SDL_ShowCursor(0);
	
	ticks = SDL_GetTicks();
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

static void run()
{
	(*statetable[currentstate][RUN])();
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
