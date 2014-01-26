#include <iostream>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <vector>
#undef main

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

int main(){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_HWSURFACE);
	SDL_WM_SetCaption("Graph", NULL);
	
	init();
	
	FILE * pFile;
	pFile=fopen ("data.txt","r");
	if(pFile==NULL){ 
		perror ("Error opening file");
	}

	while(status != EOF){
		status = fscanf(pFile, "%f", &floatBeingSearchedFor);
		points.push_back(floatBeingSearchedFor);
		if(floatBeingSearchedFor > maxPoint){
			maxPoint = floatBeingSearchedFor;
		}
		if(status == 0){
			cout << "Error reading file." << endl;
		}
	}
	
	fclose (pFile);
	
	cout << "Finished reading opened file." << endl;
	
	for(int i = 0; i < points.size(); ++i){
		cout << points[i] << endl;
	}
	//cout << "Max point is set to: " << maxPoint << endl;
	//cout << "Points length is set to: " << points.size() << endl;
	
	while(running){
		update();
	}
}

void lockFPS(){
	if(1000 / FPS > SDL_GetTicks() - start){
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
	}
}

void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cout << "Max point is set to: " << maxPoint << endl;
	cout << "Points length is set to: " << points.size() << endl;
	glOrtho(0, width, 0, height	, 0, 1);
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
			glVertex2f(i * 2, points[i]);
		}
	glEnd();
	
	SDL_GL_SwapBuffers();
}