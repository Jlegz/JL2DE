#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

void MapLayer::buildMapLayer(int tileW, int tileH, int mapW, int mapH)
{
	int x = 0, y = 0;
	for (int i = 0; i < mapW; i++)
	{
		for (size_t j = 0; j <= mapH; j++)
		{
			this->addChild(new Tile(this->getScene(), Util::getRenderTarget(), x, y, tileW, tileH, Util::loadTexture("Assets/Crusader.png")));
			if (x == (mapW * tileW))
			{
				x = 0;
				if (y != mapH * tileH)
				{
					y += tileH;
				}
			}
			else
			{
				x += tileW;
			}
			SDL_Delay(1);
		}
	}
}

MapLayer::MapLayer(int tileW, int tileH, int mapW, int mapH, SDL_Texture* tileSetTexture)
{
	int x = 0, y = 0;
	for (int i = 0; i < mapW; i++)
	{
		for (size_t j = 0; j <= mapH; j++)
		{
			this->addChild(new Tile(this->getScene(), Util::getRenderTarget(), x, y, tileW, tileH, Util::loadTexture("Assets/Crusader.png")));
			if (x == (mapW * tileW))
			{
				x = 0;
				if (y != mapH * tileH)
				{
					y += tileH;
				}
			}
			else
			{
				x += tileW;
			}
			SDL_Delay(1);
		}
	}
}

void MapLayer::render()
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles.at(i)->render();
	}
}

