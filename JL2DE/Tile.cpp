#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

bool change = false;

Tile::Tile(Scene* scene, SDL_Renderer* _renderTarget, int x, int y, int w, int h, SDL_Texture* _texture)
{
	texture = _texture;
	Util::getRenderTarget();
	previous.w = w;
	previous.h = h;
	previous.x = x;
	previous.y = y;
	current.w = w;
	current.h = h;
	current.x = x;
	current.y = y;
	next.w = w;
	next.h = h;
	next.x = x;
	next.y = y;
	Animation* d_idle = new Animation(this, 6, texture, ANIM_IDLE, true);
	d_idle->addFrame(new Frame(0, 240, 16, 16));
	d_idle->addFrame(new Frame(0, 240, 16, 16));
	multimap<AnimationAction, Animation*>* d_anim_actions = new multimap<AnimationAction, Animation*>();
	d_anim_actions->insert(make_pair(ANIM_IDLE, d_idle));
	animationDirections.insert(make_pair(ANIM_UP, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_DOWN, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_LEFT, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_RIGHT, d_anim_actions));
	setCurrentAnimationDirectionsIndex(ANIM_UP);
	//unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	//minstd_rand0 g(seed);
	//this->r = 0;
	//this->g = g() % 255;
	//this->b = g() % 255;
	setActive(true);
	scene->subscribe(this);
	setCurrentAnimationIndex(ANIM_IDLE);
}

Tile::Tile(Scene* scene)
{
	scene->subscribe(this);
}

Tile::~Tile()
{

}

void Tile::render()
{
	//if (current.x < (Util::getScreenW() + current.w) && current.x > (current.w * -1)
	//	&& current.y < (Util::getScreenH() + current.h) && current.y >(current.h * -1))
	//{
		//if (currentAnimation != nullptr)
		//{
			SDL_RenderCopyEx(Util::getRenderTarget(), texture, this->currentFrame->getRect(), &current, 0, 0, flip);
		//}
		//else
		//{
		//	//SDL_Rect r;
		//	//r.x 
		//	SDL_RenderCopyEx(Util::getRenderTarget(), texture, NULL, &current, 0, 0, flip);
		//}
	//}
}

void Tile::handleInput(vector<ButtonEvent>* buttonEvents)
{

	//for (int i = 0; i < buttonEvents->size(); i++)
	//{

	//	
	//	switch (buttonEvents->at(i))
	//	{
	//	case ButtonEvent::START:
	//		change = true;
	//		break;
	//	case ButtonEvent::START_RELEASE:
	//		change = false;
	//		break;
	//	default:
	//		break;
	//	}
	//}


	//if (change)
	//{
	//	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	//	minstd_rand0 g(seed);
	//	this->r = g() % 255;
	//	this->g = g() % 255;
	//	this->b = g() % 255;
	//}
}
//
//void Tile::move()
//{
//}
