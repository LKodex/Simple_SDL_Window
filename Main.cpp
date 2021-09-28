#include <SDL/SDL.h>
#include <stdio.h>

// Functions
void gameInitialize();
void gameLoop();
void gameRender();
void gameInputHandler();
void gameExit();
void playerMovement(SDL_Rect* player);
void playerPhysics();
bool checkCollision(int ax, int ay, int aw, int ah, int posX, int posY);
bool checkCollision(SDL_Rect* rectA, SDL_Rect* rectB);

// Variables
SDL_Window* window;
SDL_Renderer* renderer;

SDL_Rect player;
SDL_Rect health;
SDL_Rect death;

int playerSize = 100;
int healthSize = 100;
int deathSize = 100;

int HEIGHT = 600;
int WIDTH = 800;

bool gameShouldQuit = false;
bool isRight = false;
bool isLeft = false;
bool isUp = false;
bool isDown = false;

int drawColorRed = 235;
int drawColorGreen = 235;
int drawColorBlue = 255;
int drawColorAlpha = 255;

char windowName[] = "Simple SDL Window";

int main(int argc, char* argv[])
{
	gameInitialize();
	gameLoop();
	gameExit();

	return 0;
}

void gameInitialize()
{
	printf("Initializing game");
	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL components
	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP); // Create the main window in fullscreen
	if (window == nullptr)
	{
		printf("Couldn't create the window...\n");
		gameExit();
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE); // Create a renderer for main window
	if (renderer == nullptr)
	{
		printf("Couldn't create the renderer...\n");
		gameExit();
	}

	SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
	// Player rectangle
	player.x = (int)(WIDTH / 2.0) - (playerSize / 2.0);
	player.y = HEIGHT - playerSize;
	player.w = playerSize;
	player.h = playerSize;
	// Health rectangle
	health.x = 100;
	health.y = 100;
	health.w = healthSize;
	health.h = healthSize;
	// Death rectangle
	death.x = WIDTH - 100 - deathSize;
	death.y = 100;
	death.w = deathSize;
	death.h = deathSize;
}

void gameLoop()
{
	while (!gameShouldQuit)
	{
		gameInputHandler();
		gameRender();
		playerMovement(&player);
		playerPhysics();
		if (checkCollision(&player, &health))
		{
			player.w += health.w / 2;
			player.h += health.h / 2;
			player.x = (int)(WIDTH / 2.0) - (playerSize / 2.0);
			player.y = HEIGHT - playerSize;

			health.w -= health.w / 2;
			health.h -= health.h / 2;
		}
		else if (checkCollision(&player, &death))
		{
			death.w += player.w / 2;
			death.h += player.h / 2;
			death.x = WIDTH - 100 - death.w;

			player.w -= player.w / 2;
			player.h -= player.h / 2;
			player.x = (int)(WIDTH / 2.0) - (playerSize / 2.0);
			player.y = HEIGHT - playerSize;
		}
	}
}

void gameRender()
{
	// Clear the window with Render Draw Color
	SDL_SetRenderDrawColor(renderer, drawColorRed, drawColorGreen, drawColorBlue, drawColorAlpha);
	SDL_RenderClear(renderer);

	// Draw Health Rect
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &health);
	SDL_RenderDrawRect(renderer, &health);

	// Draw Death Rect
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &death);
	SDL_RenderDrawRect(renderer, &death);

	// Draw Player Rect
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderFillRect(renderer, &player);
	SDL_RenderDrawRect(renderer, &player);

	// Show on screen
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
			printf("[Mouse]\tx:%d\ty:%d\n", event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				player.x = event.button.x - player.w / 2;
				player.y = event.button.y - player.h / 2;
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_A:
				isLeft = true;
				break;
			case SDL_SCANCODE_D:
				isRight = true;
				break;
			case SDL_SCANCODE_W:
				isUp = true;
				break;
			case SDL_SCANCODE_S:
				isDown = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_A:
				isLeft = false;
				break;
			case SDL_SCANCODE_D:
				isRight = false;
				break;
			case SDL_SCANCODE_W:
				isUp = false;
				break;
			case SDL_SCANCODE_S:
				isDown = false;
				break;
			}
			break;
		}
	}
}

void gameExit()
{
	printf("Exiting the game...");
	if (!(renderer == nullptr)) { SDL_DestroyRenderer(renderer); }
	if (!(window == nullptr)) { SDL_DestroyWindow(window); }
	SDL_Quit();
}

void playerMovement(SDL_Rect* player)
{
	if (isLeft) { player->x -= 10; }
	if (isRight) { player->x += 10; }
	if (isUp) { player->y -= 15; }
	if (isDown) { player->y += 5; }
}
// All checkColiision check if is not colliding
// Check 6 coordinates, 4 coordinates for 1 rectangle and 2 coords for a point
bool checkCollision(int ax, int ay, int aw, int ah, int posX, int posY)
{
	if ((posX > ax + aw) || (posX < ax) || (posY > ay + ah) || (posY < ay))
	{
		return false;
	}
	return true;
}

// Check coordinates from 2 SDL_Rect
bool checkCollision(SDL_Rect* rectA, SDL_Rect* rectB)
{
	if ((rectA->x > rectB->x + rectB->w) || (rectA->x + rectA->w < rectB->x) || (rectA->y > rectB->y + rectB->h) || (rectA->y + rectA->h < rectB->y))
	{
		return false;
	}
	return true;
}

void playerPhysics() {
	if (player.y + player.h >= HEIGHT)
	{
		player.y = HEIGHT - player.h;
	}
	else
	{
		player.y += 5;
	}
	if (player.x <= 0)
	{
		player.x = 0;
	}
	else if (player.x + player.w >= WIDTH)
	{
		player.x = WIDTH - player.w;
	}
	if (player.y <= 0)
	{
		player.y = 0;
	}
}