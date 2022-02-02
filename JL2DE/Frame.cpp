#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

Frame::Frame(SDL_Rect* r) : frame { r }
{
	this->hasAction = false;
}

Frame::Frame(int frameXpos, int frameYpos, int frameW, int frameH)
{
	frame = new SDL_Rect();
	frame->x = frameXpos;
	frame->y = frameYpos;
	frame->w = frameW;
	frame->h = frameH;
	this->hasAction = false;
}

Frame::Frame(int frameXpos, int frameYpos, int frameW, int frameH, void(*frame_action)())
{
	frame = new SDL_Rect();
	frame->x = frameXpos;
	frame->y = frameYpos;
	frame->w = frameW;
	frame->h = frameH;
	this->frame_action = frame_action;
	this->hasAction = true;
}

void Frame::setFrameAction(void(*frame_action)())
{
	this->frame_action = frame_action;
	this->hasAction = true;
}

void Frame::executeFrameAction()
{
	if (this->hasAction)
	{
		this->frame_action();
	}
}

void Frame::setRect(SDL_Rect* r)
{
	this->frame = r;
}

SDL_Rect* Frame::getRect()
{
	return this->frame;
}
