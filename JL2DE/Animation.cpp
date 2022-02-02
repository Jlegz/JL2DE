#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

Animation::Animation(GameObject* parentObject, vector<Frame> frames, unsigned int frameRate, SDL_Texture* texture, AnimationAction next, bool loop)
	: parentObject{ parentObject }, frameRate { frameRate }, texture{ texture }, nextAnimationIndex{ next }, loop{ loop }
{
	this->frameIndex = 0;
}

Animation::Animation(GameObject* parentObject, unsigned int frameRate, SDL_Texture* texture, AnimationAction next, bool loop)
	: parentObject{ parentObject }, frameRate{ frameRate }, texture{ texture }, nextAnimationIndex{ next }, loop{ loop }
{
	this->frameIndex = 0;
}

Animation::Animation(GameObject* parentObject, unsigned int frameRate, SDL_Texture* texture, int cellW, int cellH, int cellColCount, int cellRowCount, AnimationAction next, bool loop)
	: parentObject{ parentObject }, frameRate{ frameRate }, texture{ texture }, nextAnimationIndex{ next }, loop{ loop }
{
	this->frameIndex = 0;
	vector<SDL_Rect*> lframes = vector<SDL_Rect*>();
	Util::loadSpriteSheet(cellW, cellH, cellColCount, cellRowCount, &lframes);

	for (int i = 0; i < lframes.size(); i++)
	{
		new Frame(lframes.at(i));
	}
}

Animation::~Animation()
{
	for (int i = 0; i < frames.size(); i++)
	{
		delete frames.at(i);
	}
	delete parentObject;
}

Animation* Animation::addFrame(Frame * frame)
{
	this->frames.push_back(frame);
	return this;
}

Frame* Animation::getCurrentFrame()
{
	return this->currentFrame;
}

Frame* Animation::play()
{
	if (this->currentFrame == nullptr)
	{
		this->currentFrame = this->frames.at(this->frameIndex);
	}

	if (this->frameCount >= this->frameRate)
	{
		this->frameCount = 0;
		if ((this->frames.size() - 1) > this->frameIndex)
		{
			this->frameIndex++;
			this->frames.at(this->frameIndex)->executeFrameAction();
			this->currentFrame = this->frames.at(this->frameIndex);
		}
		else if (this->loop && (this->frames.size() - 1) == this->frameIndex)
		{
			this->frameIndex = 0;
			this->frames.at(this->frameIndex)->executeFrameAction();
			this->currentFrame = this->frames.at(this->frameIndex);
		}
		else if(!this->loop && (this->frames.size() - 1) == this->frameIndex)
		{
			this->frameIndex = 0;
			parentObject->canMove = true;
			parentObject->setCurrentAnimationIndex(nextAnimationIndex);
			this->frames.at(this->frameIndex)->executeFrameAction();
			//////////////go to next animation
		}
	}
	this->frameCount++;
	return this->currentFrame;
}

void Animation::setLoop(bool l)
{
	this->loop = l;
}

bool Animation::isLoop()
{
	return this->loop;
}
