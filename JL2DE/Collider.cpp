#pragma once
#include "JL2DE.h"

Collider::Collider(int x, int y, int w, int h, bool isTrigger) 
{
	this->collider = new SDL_Rect();
	this->collider->x = x;
	this->collider->y = y;
	this->collider->w = w;
	this->collider->h = h;
}

Collider::Collider(int x, int y, int w, int h, bool isTrigger, GameObject* _parent)
{
	this->collider = new SDL_Rect();
	this->collider->x = x;
	this->collider->y = y;
	this->collider->w = w;
	this->collider->h = h;
	this->parent = _parent;
}

Collider::Collider(SDL_Rect* collider_rect, bool isTrigger)
{
	this->collider = collider_rect;
}

Collider::~Collider()
{

}

void Collider::setTriggerAction(void(*triggerAction)(GameObject*))
{
	this->triggerAction = triggerAction;
}

void Collider::setColliderAction(void(*colliderAction)(GameObject*))
{
	this->colliderAction = colliderAction;
}

SDL_Rect* Collider::getRect()
{
	return this->collider;
}

void Collider::trigger(GameObject* g)
{
	triggerAction(g);
}

GameObject * Collider::getParent()
{
	return parent;
}

void Collider::setParent(GameObject* parent)
{
	this->parent =  parent;
}

void Collider::drawColliderBounds()
{
	SDL_SetRenderDrawColor(Util::getRenderTarget(), 0x0, 0xFF, 0x0, 0x0);
	SDL_RenderDrawRect(Util::getRenderTarget(), collider);
}

void Collider::moveBy(int xDelta, int yDelta)
{
	this->collider->x += xDelta;
	this->collider->y += yDelta;
}

void Collider::moveTo(int x, int y)
{
	this->collider->x = x;
	this->collider->y = y;
}