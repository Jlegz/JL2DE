#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

SDL_Texture* Text::white = nullptr;
SDL_Texture* Text::black = nullptr;
SDL_Texture* Text::white_on_black = nullptr;
SDL_Texture* Text::black_on_white = nullptr;

Text::Text(Scene* scene) {
	isUI = true;
	init(16, 16, 16, 16, TextState::BLACK);
	this->alwaysRender = true;
	scene->subscribe(this);
}

Text::Text(Scene* scene, int x, int y, int w, int h, bool isPausable, TextState state, SceneSortLayer sortLayer) {
	isUI = true;
	setSortOrder(sortLayer);
	init(x, y, w, h, state);
	this->alwaysRender = true;
	scene->subscribe(this);
}

void Text::printL(string str, TextState state)
{
	Text::print(str, state);
	Text::buffer.push_back(46);
}

void Text::printL(string str)
{
	Text::print(str);
	Text::buffer.push_back(46);
}

void Text::print(string str, TextState state)
{
	this->state = state;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
		{
			Text::buffer.push_back(alphaNumerics[toupper(str[i])]);
		}
		else
		{
			Text::buffer.push_back(alphaNumerics[str[i]]);
		}
	}
}

void Text::print(string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
		{
			Text::buffer.push_back(alphaNumerics[toupper(str[i])]);
		}
		else
		{
			Text::buffer.push_back(alphaNumerics[str[i]]);
		}
	}
}

TextState Text::getState()
{
	return this->state = state;
}

void Text::setState(TextState state)
{
	this->state = state;
}

void Text::clear()
{
	Text::buffer.clear();
}

bool Text::init(int _x, int _y, int _w, int _h, TextState state)
{
	this->state = state;
	cursorPosition.x = _x;
	cursorPosition.y = _y;
	cursorPosition.h = _h;
	cursorPosition.w = _w;
	current.x = _x;
	current.y = _y;
	current.h = _h;
	current.w = _w;
	next.x = _x;
	next.y = _y;
	next.w = _w;
	next.h = _h;
	(white == nullptr) ? white = Util::loadTexture("alphabet_white.png") : white = white;
	(black == nullptr) ? black = Util::loadTexture("alphabet_black.png") : black = black;
	(white_on_black == nullptr) ? white_on_black = Util::loadTexture("alphabet_white_on_black.png") : white_on_black = white_on_black;
	(black_on_white == nullptr) ? black_on_white = Util::loadTexture("alphabet_black_on_white.png"): black_on_white = black_on_white;
	Text::fillAlphaMap(&alphaNumerics);
	Util::loadSpriteSheet(8, 8, 6, 8, &frames);
	return true;
}

void Text::renderTextBuffer()
{
	int originalcurrentWorldPositionX = current.x;
	for (size_t i = 0; i < buffer.size(); i++)
	{
		if (buffer.at(i) == 46)
		{
			current.x = 16;
			current.y += current.h;
			//buffer.at(i) = 47;
		}
		else
		{
			SDL_Rect frame = *frames.at(buffer.at(i));
			SDL_RenderCopy(Util::getRenderTarget(), white, &frame, &current);
			current.x += current.w;
		}
	}
	current.x = originalcurrentWorldPositionX;
	current.y = originalcurrentWorldPositionX;
}

void Text::fillAlphaMap(map<char, int>* map)
{
	alphaNumerics['A'] = 0;			  
	alphaNumerics['B'] = 1;			  
	alphaNumerics['C'] = 2;			  
	alphaNumerics['D'] = 3;			  
	alphaNumerics['E'] = 4;			  
	alphaNumerics['F'] = 5;			  
	alphaNumerics['G'] = 6;			  
	alphaNumerics['H'] = 7;			  
	alphaNumerics['I'] = 8;			  
	alphaNumerics['J'] = 9;			  
	alphaNumerics['K'] = 10;		  
	alphaNumerics['L'] = 11;		  
	alphaNumerics['M'] = 12;
	alphaNumerics['N'] = 13;
	alphaNumerics['O'] = 14;
	alphaNumerics['P'] = 15;
	alphaNumerics['Q'] = 16;
	alphaNumerics['R'] = 17;
	alphaNumerics['S'] = 18;
	alphaNumerics['T'] = 19;
	alphaNumerics['U'] = 20;
	alphaNumerics['V'] = 21;
	alphaNumerics['W'] = 22;
	alphaNumerics['X'] = 23;
	alphaNumerics['Y'] = 24;
	alphaNumerics['Z'] = 25;
	alphaNumerics['.'] = 26;
	alphaNumerics[','] = 27;
	alphaNumerics['!'] = 28;
	alphaNumerics['?'] = 29;
	alphaNumerics[')'] = 30;
	alphaNumerics['('] = 31;
	alphaNumerics[']'] = 32; // 32 (']') is right single quote (')
	alphaNumerics['['] = 33; // 33 ('[') is left single quote (')
	alphaNumerics['0'] = 34;
	alphaNumerics['1'] = 35;
	alphaNumerics['2'] = 36;
	alphaNumerics['3'] = 37;
	alphaNumerics['4'] = 38;
	alphaNumerics['5'] = 39;
	alphaNumerics['6'] = 40;
	alphaNumerics['7'] = 41;
	alphaNumerics['8'] = 42;
	alphaNumerics['9'] = 43;
	alphaNumerics['{'] = 44;
	alphaNumerics['}'] = 45;
	alphaNumerics['|'] = 46; // 46 ('|') is end line
	alphaNumerics[' '] = 47;
}

void Text::render()
{
	int originalcurrentWorldPositionX = current.x, originalcurrentWorldPositionY = current.y;
	for (size_t i = 0; i < buffer.size(); i++)
	{
		if (buffer.at(i) == 46)
		{
			current.x = originalcurrentWorldPositionX;
			current.y += current.h;
			//buffer.at(i) = 47;
		}
		else
		{
			SDL_Rect frame = *frames.at(buffer.at(i));			
			switch (this->state)
			{
				case BLACK:
					SDL_RenderCopy(Util::getRenderTarget(), black, &frame, &current);
					break;
				case WHITE:
					SDL_RenderCopy(Util::getRenderTarget(), white, &frame, &current);
					break;
				case WHITE_BLACK_BG:
					SDL_RenderCopy(Util::getRenderTarget(), white_on_black, &frame, &current);
					break;
				case BLACK_WHITE_BG:
					SDL_RenderCopy(Util::getRenderTarget(), black_on_white, &frame, &current);
					break;
				default:
					SDL_RenderCopy(Util::getRenderTarget(), black, &frame, &current);
					break;
			}
			current.x += current.w;
		}
	}
	current.x = originalcurrentWorldPositionX;
	current.y = originalcurrentWorldPositionY;
	clear();
}

//void Text::handleInput(vector<ButtonEvent>* buttonEvents)
//{
//
//}
//
//void Text::move()
//{
//	//previousPosition = currentWorldPositionPosition;
//	//currentWorldPositionPosition = nextPosition;
//}

