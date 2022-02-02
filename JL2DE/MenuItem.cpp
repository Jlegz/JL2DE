#pragma once
#include "JL2DE.h"
using namespace Events;

//Text* MenuItem::txt = nullptr;
//string MenuItem::val;
//SDL_Rect* MenuItem::size;
//bool MenuItem::active;
//bool MenuItem::selected;


MenuItem::MenuItem(Scene* scene)
{
	isUI = true;
	scene->subscribe(this);
}
MenuItem::MenuItem(Scene* scene, string txt, string val, SDL_Rect* size, TextState state)
{
	isUI = true;
	MenuItem::txtObj = new Text(scene, size->x, size->y, size->w, size->h, false, state, FORE_FRONT);
	MenuItem::text = txt;
	MenuItem::val = val;
	this->alwaysRender = true;
	this->selected = false;
	scene->subscribe(this);
}

MenuItem::MenuItem(Scene* scene, string txt, string val, int x, int y, int w, int h, TextState state)
{
	isUI = true;
	MenuItem::txtObj = new Text(scene, x, y, w, h, false, state, FORE_FRONT);
	MenuItem::text = txt;
	MenuItem::val = val;
	this->alwaysRender = true;
	this->selected = false;
	scene->subscribe(this);
}

MenuItem::~MenuItem()
{
	delete MenuItem::txtObj;
}
void MenuItem::setText(string txt)
{
	MenuItem::text = txt;
}
Text * MenuItem::getTextObj()
{
	return MenuItem::txtObj;
}

void MenuItem::setTextObj(Text * textObj)
{
	MenuItem::txtObj = textObj;
}

void MenuItem::setVal(string val)
{

}

string MenuItem::getText()
{
	return MenuItem::text;
}

string MenuItem::getVal()
{
	return MenuItem::val;
}

void MenuItem::render()
{
	if (selected)
	{
		//if (MenuItem::txtObj->getState() == TextState::BLACK)
		//{
		//	MenuItem::txtObj->print(MenuItem::text, TextState::WHITE_BLACK_BG);
		//}
		//else if (MenuItem::txtObj->getState() == TextState::WHITE)
		//{
			MenuItem::txtObj->print(MenuItem::text, TextState::BLACK_WHITE_BG);
		//}
		//else if (MenuItem::txtObj->getState() == TextState::WHITE_BLACK_BG)
		//{
		//	MenuItem::txtObj->print(MenuItem::text, TextState::BLACK);
		//}
		//else if (MenuItem::txtObj->getState() == TextState::BLACK_WHITE_BG)
		//{
		//	MenuItem::txtObj->print(MenuItem::text, TextState::WHITE);
		//}
	}
	else
	{
		MenuItem::txtObj->print(MenuItem::text, TextState::WHITE);
	}
}

MenuItem* MenuItem::select()
{
	selected = true;
	return this;
}

MenuItem* MenuItem::deSelect()
{
	selected = false;
	return this;
}