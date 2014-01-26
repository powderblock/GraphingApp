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

const int width = 640, height = 480;
const int FPS = 60;
bool running = true;

Uint32 start;

void events();
void lockFPS();
void init();
void update();
void render();

vector<float> points;

int status;
float floatBeingSearchedFor;
float maxPoint;

int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_HWSURFACE);
	SDL_WM_SetCaption("Graph", NULL);

	FILE * pFile;
	if(argc < 2){
		cout << "Error: Improper usage! Please enter the name of the application, followed by the name of the data you wish to read." << endl;
	}
	for(int i = 1; i < argc; i++){
		cout << argv[i] << endl;
		pFile = fopen(argv[i],"r");
		if(pFile == NULL){
			perror ("Error opening file");
		}

		do {
			status = fscanf(pFile, "%f", &floatBeingSearchedFor);
			if(status == 0){
				cerr << "Error reading file." << endl;
			}
			points.push_back(floatBeingSearchedFor);
			if(floatBeingSearchedFor > maxPoint){
				maxPoint = floatBeingSearchedFor;
			}
		} while(status != EOF);
		cout << points.size() << " points so far." << endl;

		fclose(pFile);
	}

	cout << "Finished reading opened file(s)." << endl;
	cout << "Points length is set to: " << points.size() << endl;
	init();

	while(running){
		update();
	}
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
	glOrtho(0, points.size(), 0, maxPoint * 1.1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update(){
	start = SDL_GetTicks();
	events();
	render();
	lockFPS();
}

void events(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				running = false;
				break;
		}
	}
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_STRIP);
		for(int i = 0; i < points.size(); ++i){
			glVertex2f(i, points[i]);
		}
	glEnd();

	SDL_GL_SwapBuffers();
}
