#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

SDL_Window* Util::gameWindow = nullptr;
SDL_Surface* Util::gScreenSurface = nullptr, *Util::newScreenSurface = nullptr;
SDL_Renderer* Util::gRenderTarget = nullptr;
int Util::screenW = 0, Util::screenH = 0;
multimap<long, long> Util::availableDisplayResolutions;
bool Util::askUserForResolution = true;
int Util::frameRate = 1;
int Util::countedFrames = 0;
int Util::adjustedTileWidth = 0;
int Util::adjustedTileHeight = 0;
// This function will initialize SDL and a window (and the window's base surface).  All other surfaces will be handled by SurfaceManager.
bool Util::init()
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	gameWindow = nullptr;
	gScreenSurface = nullptr;
	// Initialization flag
	bool success = true;
	int initReturn = 0;

	//load configuration file attriubtes
	if (ConfigList::isEmpty())
	{
		ConfigList::loadConfig();
	}
	loadDisplayResolutions();

	//cout << "SDL Successfully Initialized.  Press [ENTER] to begin game!";
	//cin.get();
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	if (ConfigList::getAttributeByKey(Constants::INPUT_TYPE())->getValue() == "JOYSTICK")
	{
		Scene::useJoystick = true;
		Scene::gGameController = NULL;
		initReturn = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);
		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			cout << "Warning: No joysticks connected!\n" << "Keyboard input will be used instead." << endl;;
			Scene::useJoystick = false;
		}
		else
		{
			//Load joystick
			Scene::gGameController = SDL_GameControllerOpen(0);
			if (Scene::gGameController == NULL)
			{
				cout << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << endl << "Keyboard input will be used instead." << endl;
				Scene::useJoystick = false;
				//initReturn = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
			}
		}
	}
	else
	{
		Scene::useJoystick = false;
		Scene::gGameController = NULL;
		initReturn = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_AUDIO);
	}

	if (initReturn < 0)
	{
		// If we fail to initialize, we write a failure message to the console and return false success flag.
		cout << "SDL initialization failed!  SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Create window
		//string useDisplayDefault

		screenW = atoi(ConfigList::getAttributeByKey(Constants::WINDOW_W())->getValue().c_str());
		screenH = atoi(ConfigList::getAttributeByKey(Constants::WINDOW_H())->getValue().c_str());	
		float tempW = screenW, tempH = screenH, tempAdjW = adjustedTileWidth, tempAdjH = adjustedTileHeight;
		tempAdjW = ceil(tempW / 15);
		tempAdjH = ceil(tempH / 10);
		adjustedTileWidth = tempAdjW;
		adjustedTileHeight = tempAdjH;
		if (ConfigList::getAttributeByKey(Constants::CONFIG_CHK())->getValue() == "TRUE")
		{
			askUserForResolution = true;
		}
		else
		{
			askUserForResolution = false;
		}

		if (askUserForResolution)
		{
			//here is where we load a small window
			SDL_Rect currentRez;
			int test = SDL_GetDisplayBounds(0, &currentRez);
			gameWindow = SDL_CreateWindow("Select Game Resolution", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, currentRez.w/2, currentRez.h/2, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
			if (gameWindow == NULL)
			{
				// If the returned window pointer is still null, then the creation failed.
				// We write a failure message to the console and return false success flag.
				//printf("SDL window creation failed! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// If the returned window pointer is NOT null, then we exctract the new window's surface.
				gScreenSurface = SDL_GetWindowSurface(gameWindow);
				//printf("Window created successfully!\n");
				success = true;
			}
		}
		else
		{
			//SDL_WindowFlags sdlWinFlg;
			if (ConfigList::getAttributeByKey(Constants::FULL_SCREEN())->getValue() == "TRUE")
			{
				//SDL_Rect r =  SDL_Rect();
				//SDL_GetDisplayBounds(0, &r);
				gameWindow = SDL_CreateWindow("JL2DE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS);
			}
			else
			{
				gameWindow = SDL_CreateWindow("JL2DE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
			}

			if (gameWindow == NULL)
			{
				// If the returned window pointer is still null, then the creation failed.
				// We write a failure message to the console and return false success flag.
				//printf("SDL window creation failed! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// If the returned window pointer is NOT null, then we exctract the new window's surface.
				gScreenSurface = SDL_GetWindowSurface(gameWindow);
				//printf("Window created successfully!\n");
			}
		}

		//Intiatialize SDL_Renderer
		gRenderTarget = SDL_CreateRenderer(Util::getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
	}
	return success;
}

bool Util::cleanup()
{
	return true;
}

SDL_Window* Util::getWindow()
{
	if (gameWindow == NULL)
	{
		init();
	}
	return gameWindow;
}

SDL_Renderer* Util::getRenderTarget()
{
	if (gRenderTarget == NULL)
	{
		init();
	}
	return gRenderTarget;
}

SDL_Texture * Util::loadTexture(string imagePath)
{
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image" << imagePath.c_str() << "! SDL_image Error: " << IMG_GetError();
	}
	else
	{
		//Create white from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Util::getRenderTarget(), loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", imagePath.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int Util::getScreenW()
{
	return screenW;
}

void Util::setScreenW(int w)
{
	Util::screenW = w;
}

void Util::setScreenH(int h)
{
	Util::screenH = h;
}

void Util::resizeWindow(int h, int w, bool fullscreen)
{
	Util::setScreenW(w);
	Util::setScreenH(h);
	ConfigList::getAttributeByKey(Constants::WINDOW_W())->setValue(to_string(w));
	ConfigList::getAttributeByKey(Constants::WINDOW_H())->setValue(to_string(h));
	SDL_SetWindowSize(Util::getWindow(), Util::getScreenW(), Util::getScreenH());
	SDL_SetWindowPosition(Util::getWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	if (fullscreen)
	{
		SDL_SetWindowFullscreen(Util::getWindow(), SDL_WINDOW_FULLSCREEN);
	}
}
int Util::getScreenH()
{
	return screenH;
}

multimap<long, long>* Util::getAvailableDisplayResolutions()
{
	return &Util::availableDisplayResolutions;
}

bool Util::getAskUserForResolution()
{
	return askUserForResolution;
}

void Util::setAskUserForResolution(bool _askUserForResolution)
{
	askUserForResolution = _askUserForResolution;
}

void Util::addRectsPosition(SDL_Rect* r1, SDL_Rect* r2, SDL_Rect* result)
{
	result->x = (r1->x + r2->x);
	result->y = (r1->y + r2->y);
}

void Util::addRectsDimensions(SDL_Rect * r1, SDL_Rect * r2, SDL_Rect * result)
{
	result->h = (r1->h + r2->h);
	result->w = (r1->w + r2->w);
}

void Util::addRectsAll(SDL_Rect * r1, SDL_Rect * r2, SDL_Rect * result)
{
	result->x = (r1->x + r2->x);
	result->y = (r1->y + r2->y);
	result->h = (r1->h + r2->h);
	result->w = (r1->w + r2->w);
}

void Util::loadSpriteSheet(int cellW,  int cellH, int cellColCount, int cellRowCount, vector<SDL_Rect*>* frames)
{
	int x = 0, y = 0, h = cellH, w = cellW, count = 0, yMax = cellH * cellRowCount, xMax = cellW * cellColCount;
	for (size_t i = 0; i <= cellRowCount; i++)
	{
		for (size_t j = 1; j <= cellColCount; j++)
		{
			//if (count == 40)
			//{
			//	count = count;
			//}
			if (y < yMax)
			{
				if (x < xMax)
				{
					frames->push_back(new SDL_Rect());
					frames->at(count)->x = x;
					frames->at(count)->y = y;
					frames->at(count)->h = h;
					frames->at(count)->w = w;
					x += w;
					count++;
				}
				else
				{
					frames->push_back(new SDL_Rect());
					frames->at(count)->x = x;
					frames->at(count)->y = y;
					frames->at(count)->h = h;
					frames->at(count)->w = w;
					x = 0;
					y += h;
				}
			}
			else
			{
				if (x <= xMax)
				{
					frames->push_back(new SDL_Rect());
					frames->at(count)->x = x;
					frames->at(count)->y = y;
					frames->at(count)->h = h;
					frames->at(count)->w = w;
					x += w;
					count++;
				}
				else
				{
					//return frames;
				}
			}
		}
	}
}


bool Util::containsSize(long w, long h, multimap<long, long>* s)
{
	for (multimap<long, long>::iterator it = s->begin(); it != s->end(); ++it)
	{
		if ((*it).first == w && (*it).second == h)
		{
			return true;
		}
	}
	return false;
}

bool Util::containsValue(int i, vector<int>* v)
{
	for (int j = 0; j < v->size(); j++)
	{
		if (v->at(j) == i)
		{
			return true;
		}
	}
	return false;
}

void Util::setAdjustedTileWidth(int w)
{
	adjustedTileWidth = w;
}

int Util::getAdjustedTileWidth()
{
	return adjustedTileWidth;
}

void Util::setAdjustedTileHeight(int h)
{
	adjustedTileHeight = h;
}

int Util::getAdjustedTileHeight()
{
	return adjustedTileHeight;
}

#if defined(_WIN32)
void Util::loadDisplayResolutions()
{
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	//int size = dm.dmSize;
	//vector<long, long> sizes;
	//multimap<long, long> sizes;
	//bool test = EnumDisplaySettings(NULL, 1, &dm);
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
	{
		if (containsSize(dm.dmPelsWidth, dm.dmPelsHeight, &Util::availableDisplayResolutions) == false)
		{
			Util::availableDisplayResolutions.insert(make_pair(dm.dmPelsWidth, dm.dmPelsHeight));
			//cout << dm.dmPelsWidth << " x " << dm.dmPelsHeight << endl;
		}
		//cout << "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << endl;
	}
	cout << "------------------------------------------------------------" << endl;
	for (multimap<long, long>::iterator it = Util::availableDisplayResolutions.begin(); it != Util::availableDisplayResolutions.end(); ++it)
	{	
		string rezString = ""; 
		rezString= to_string((*it).first) + " x " + to_string((*it).second);
		cout << "Resolution '" << rezString << "' detected." << endl;
	}
}
#elif defined(__APPLE__)
void Util::loadDisplayResolutions()
{
	//nothing yet
}
#elif defined(__linux__)
void Util::loadDisplayResolutions()
{
	//nothing yet
}
#endif

