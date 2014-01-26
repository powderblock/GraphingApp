#include <iostream>
#include <stdio.h>
#include <vector>

#ifdef	_WIN32
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#undef main
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

using namespace std;

const int width = 1280, height = 720;
const int FPS = 60;
bool running = true;

Uint32 start;

void events();
void lockFPS();
void init();
void render();

struct point {
	int x, y;
	point(int x, int y) : x(x), y(y) {}
};

vector<point> points;

int status;
float floatBeingSearchedFor;
float maxHeight, maxWidth;

int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_HWSURFACE);
	SDL_WM_SetCaption("Graph", NULL);

	FILE * pFile;
	if(argc < 2){
		fprintf(stderr, "Error: no arguments given. \nUsage: %s file [file] [file] ...", argv[0]);
		exit(1);
	}
	for(int i = 1; i < argc; ++i){
		int x = 0;
		pFile = fopen(argv[i],"r");
		if(pFile == NULL){
			fprintf(stderr ,"Error opening file %s", argv[i]);
		}

		do {
			status = fscanf(pFile, "%f", &floatBeingSearchedFor);
			if(status == 0){
				fprintf(stderr, "Error reading file %s.", argv[i]);
			}

			if(floatBeingSearchedFor > maxHeight){
				maxHeight = floatBeingSearchedFor;
			}
			
			points.push_back(point(x, floatBeingSearchedFor));
			x += 1;
		} while(status != EOF);

		fclose(pFile);
	}
	
	for(int i = 0; i < points.size(); ++i){
		if(points[i].x > maxWidth){
			maxWidth = points[i].x;
		}
	}

	cout << "Finished reading opened file(s)." << endl;
	cout << "Points length is set to: " << points.size() << endl;
	cout << "Max height for points is set to: " << maxHeight << endl;
	cout << "Max width for points is set to: " << maxWidth << endl;
	init();

	while(running){
		start = SDL_GetTicks();
		events();
		render();
		lockFPS();
	}
	SDL_Quit();
	return 0;
}

void lockFPS(){
	if(1000 / FPS > SDL_GetTicks() - start){
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
	}
}

void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, maxWidth * 1.01, 0, maxHeight * 1.01, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void events(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
				break;
		}
	}
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);
	glBegin(GL_LINES);
		for(int i = 1; i < points.size(); ++i){
			if (points[i].x < points[i-1].x) continue; // Skip the line if it's moving backwards.
			glVertex2f(points[i-1].x, points[i-1].y);
			glVertex2f(points[i].x, points[i].y);
		}
	glEnd();

	SDL_GL_SwapBuffers();
}
