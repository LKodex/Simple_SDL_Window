#include <SDL/SDL.h>
#include <SDL/SDL_error.h>
#include <stdio.h>

// Functions
void gameInitialize();
void gameLoop();
void gameRender();
void gameInputHandler();
void gameExit();

// Variables
SDL_Window* window;
SDL_Renderer* renderer;

const int HEIGHT = 1080;
const int WIDTH = 1920;

int drawColorRed = 255;
int drawColorGreen = 0;
int drawColorBlue = 255;
int drawColorAlpha = 255;

char windowName[] = "SDL Game";
bool gameShouldQuit = false;
bool colorChanges = false;


int main(int argc, char* argv[])
{
	gameInitialize();

	gameLoop();

	gameExit();

	return 0;
}

void gameInitialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
}

void gameLoop()
{
	while (!gameShouldQuit)
	{
		gameInputHandler();
		gameRender();
	}
}

void gameRender()
{
	SDL_SetRenderDrawColor(renderer, drawColorRed, drawColorGreen, drawColorBlue, drawColorAlpha);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void gameInputHandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				gameShouldQuit = !gameShouldQuit;
				break;
			case SDL_MOUSEMOTION:
				if (colorChanges)
				{
					int mouseX = event.motion.x;
					int mouseY = event.motion.y;
					printf("[Mouse]\tx:%d\ty:%d\n", mouseX, mouseY);
					drawColorRed = (Uint8)(mouseY / ((HEIGHT / 2.0) / 255.0));
					drawColorGreen = (Uint8)((255 - (mouseY / ((HEIGHT / 2.0) / 255.0)) + 255 - (mouseY / ((WIDTH / 2.0) / 255.0))) / 2);
					drawColorBlue = (Uint8)(mouseY / ((WIDTH / 2.0) / 255.0));
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						colorChanges = !colorChanges;
						break;
				}
		}
	}
}

void gameExit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
