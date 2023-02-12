#pragma once

uint32_t ticks = 0;

int fpscap = 1;
const int fpsmax = 60;

GLuint matrix;

vec3_t pos;
float rotx, roty;

void
state_game_run()
{
	int newticks = SDL_GetTicks();
	double deltatime = newticks - ticks;
	ticks = newticks;
	
	static uint32_t lastcheck = 0;
	static int frame = 0;
	frame++;
	if(newticks - lastcheck >= 1000)
	{
		lastcheck = newticks;
		printf("FPS: %i\n", frame);
		frame=0;
	}
	
	int windoww, windowh;
	getwindowsize(&windoww, &windowh);
	
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
			state_changeto(CLOSING);
		//handle input
		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				state_changeto(CLOSING);
				break;
			}
		}
		else if(e.type == SDL_WINDOWEVENT)
		{
			if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				updatewindowbounds(e.window.data1, e.window.data2);
		}
	}
	
	const uint8_t *keyboard = SDL_GetKeyboardState(0);
	int mousex, mousey;
	SDL_GetMouseState(&mousex, &mousey);
	centermouse();
	double deltamousex = mousex - windoww/2;
	double deltamousey = mousey - windowh/2;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	
	mat4_t projection = getprojectionmatrix(90, (float)windoww / (float)windowh, .1, 1000);
	
	vec3_t up;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	
	rotx += deltamousex/800;
	roty -= deltamousey/800;
	
	vec3_t forwardcamera;
	forwardcamera.x = sin(rotx) * cos(roty);
	forwardcamera.y = sin(roty);
	forwardcamera.z = -cos(rotx) * cos(roty);
	
	vec2_t forwardmovement;
	forwardmovement.x = forwardcamera.x;
	forwardmovement.y = -cos(rotx);
	
	vec2_t right;
	right.x = -forwardcamera.y;
	right.y = -forwardcamera.x;
	
	if(keyboard[SDL_SCANCODE_W])
	{
		pos.x += forwardmovement.x * (deltatime / 1000);
		pos.z += forwardmovement.y * (deltatime / 1000);
	}
	if(keyboard[SDL_SCANCODE_A])
	{
		pos.x += forwardmovement.y * (deltatime / 1000);
		pos.z -= forwardmovement.x * (deltatime / 1000);
	}
	if(keyboard[SDL_SCANCODE_S])
	{
		pos.x -= forwardmovement.x * (deltatime / 1000);
		pos.z -= forwardmovement.y * (deltatime / 1000);
	}
	if(keyboard[SDL_SCANCODE_D])
	{
		pos.x -= forwardmovement.y * (deltatime / 1000);
		pos.z += forwardmovement.x * (deltatime / 1000);
	}
	if(keyboard[SDL_SCANCODE_LSHIFT])
	{
		pos.y -= 1 * (deltatime / 1000);
	}
	if(keyboard[SDL_SCANCODE_SPACE])
	{
		pos.y += 1 * (deltatime / 1000);
	}
	
	forwardcamera.x += pos.x;
	forwardcamera.y += pos.y;
	forwardcamera.z += pos.z;
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	mat4_t view = getviewmatrix(pos, forwardcamera, up);
	
	mat4_t mvp;
	dotmat4mat4(&mvp, &projection, &view);
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.mat);
	
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
						  0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
						  3,                  // size
						  GL_FLOAT,           // type
						  GL_FALSE,           // normalized?
						  0,                  // stride
						  (void*)0            // array buffer offset
						  );
	glDrawArrays(GL_TRIANGLES, 0, chunk.points); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	
	SDL_GL_SwapWindow(win);
	
	if(fpscap)
	{
		uint32_t ticksdiff = SDL_GetTicks() - ticks;
		if(ticksdiff < 1000 / fpsmax)
			SDL_Delay(1000 / fpsmax - ticksdiff);
	}
}