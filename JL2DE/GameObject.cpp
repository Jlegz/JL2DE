#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;


vector<int>* GameObject::sortOrderValues =  new vector<int>();
map<SoundFX_Key, SoundFX*> GameObject::soundFX_col = map<SoundFX_Key, SoundFX*>();
//std::map<SoundFX_Key, SoundFX>::iterator GameObject::it;

GameObject::GameObject()
{

}

void GameObject::setActive(bool _active)
{
	this->active = _active;
	for (size_t i = 0; i < this->children.size(); i++)
	{
		this->children[i]->setActive(_active);
	}
}

bool GameObject::isActive()
{
	return this->active;
}

void GameObject::setSortOrder(SceneSortLayer _sortOrder)
{
	this->sortOrder = _sortOrder;
	if (!Util::containsValue(_sortOrder, sortOrderValues))
	{
		sortOrderValues->push_back(_sortOrder);
		//sort(sortOrderValues->begin(), sortOrderValues->end());
	}
}

void GameObject::setTag(string _tag)
{
	tag = _tag;
}

string GameObject::getTag()
{
	return tag;
}

SceneSortLayer GameObject::getSortOrder()
{
	return this->sortOrder;
}

vector<int>* GameObject::getSortOrderValues()
{
	return sortOrderValues;
}

void GameObject::move()
{

	setPrevious(current);
	setCurrent(next);
		int xDelta = current.x - previous.x;
		int yDelta = current.y - previous.y;
		if (collider != nullptr)
		{
			//collider->moveBy(xDelta, yDelta);
		}
		checkForCollision();
		if (!SDL_RectEquals(&current, &previous))
		{
			//cout << "X: " << current.x << "\t\t\tY: " << current.y << endl;
			if (children.size() > 0)
			{
				for (size_t i = 0; i < children.size(); i++)
				{
					children.at(i)->move();
					//children.at(i)->syncMove(xDelta, yDelta);
				}
			}
		}

}

//void GameObject::lateMove(int xDelta, int yDelta)
//{
//	//next.x += xDelta;
//	//next.y += yDelta;
//	current.x += xDelta;
//	current.y += yDelta;
//	checkForCollision();
//	if (children.size() > 0)
//	{
//		//if (!SDL_RectEquals(&current, &previous))
//		//{
//		for (size_t i = 0; i < children.size(); i++)
//		{
//			children.at(i)->lateMove(xDelta, yDelta);
//		}
//		//}
//	}
//	if (collider != nullptr)
//	{
//		collider->moveBy(xDelta, yDelta);
//	}
//}

void GameObject::syncMove(int xDelta, int yDelta)
{
	next.x += xDelta;
	next.y += yDelta;
	//checkForCollision();
	if (children.size() > 0)
	{
		if (!SDL_RectEquals(&current, &previous))
		{
			for (size_t i = 0; i < children.size(); i++)
			{
				children.at(i)->syncMove(xDelta, yDelta);
			}
		}
	}
	//if (collider != nullptr)
	//{
	//	collider->moveBy(xDelta, yDelta);
	//}
}

vector<GameObject*> GameObject::getCollidedObjects()
{
	return collidedObjects;
}

void GameObject::addSoundFX(SoundFX_Key key, string path)
{
	GameObject::soundFX_col[key] = new SoundFX(path);
}

SoundFX* GameObject::getSoundFX(SoundFX_Key key)
{
	//GameObject::it = 
	return GameObject::soundFX_col.find(key)->second;
}

void GameObject::playSoundFX(SoundFX_Key key)
{
	GameObject::soundFX_col.find(key)->second->play();
}

bool GameObject::checkForCollision()
{
	if (this->collider != nullptr)
	{
		for (size_t j = 0; j < scene->getCollidableSubscribers().size(); j++)
		{
			SDL_Rect diff;
			if (this != scene->getCollidableSubscribers().at(j)
			 && scene->getCollidableSubscribers().at(j)->getCollider() != nullptr
			 && isAlreadyCollided(scene->getCollidableSubscribers().at(j)) == false
		     && SDL_IntersectRect(scene->getCollidableSubscribers().at(j)->getCollider()->getRect(), collider->getRect(), &diff))
			{
				//if(SDL_IntersectRect(scene->getCollidableSubscribers().at(j)->getCollider())
				//this->canMove = false;
				if (diff.w < diff.h)
				{
					if (this->facingLeft || this->movingLeft)
					{
						current.x += diff.w;
						next.x += diff.w;
						collider->moveBy(diff.w, 0);
					}
					if (this->facingRight || this->movingRight)
					{
						current.x -= diff.w;
						next.x -= diff.w;
						collider->moveBy(diff.w*-1, 0);
					}
				}
				else if (diff.w > diff.h)
				{
					if (this->facingUp || this->movingUp)
					{
						current.y += diff.h;
						next.y += diff.h;
						collider->moveBy(0, diff.h);
					}
					if (this->facingDown || this->movingDown)
					{
						current.y -= diff.h;
						next.y -= diff.h;
						collider->moveBy(0, diff.h*-1);
					}
				}
				else
				{
					//if (this->facingLeft || this->movingLeft)
					//{
						current.x += diff.w;
						next.x += diff.w;
						collider->moveBy(diff.w, 0);
					//}
					//if (this->facingRight || this->movingRight)
					//{
						current.x -= diff.w;
						next.x -= diff.w;
						collider->moveBy(diff.w*-1, 0);
					//}
					//if (this->facingUp || this->movingUp)
					//{
						current.y += diff.h;
						next.y += diff.h;
						collider->moveBy(0, diff.h);
					//}
					//if (this->facingDown || this->movingDown)
					//{
						current.y -= diff.h;
						next.y -= diff.h;
						collider->moveBy(0, diff.h*-1);
					//}
				}
			}
		}
		if (!collidedObjects.empty())
		{
			return true;
			collidedObjects.clear();
		}
		else
		{
			return false;
		}
	}
}

void GameObject::clearCollidedObjects()
{
	collidedObjects.clear();
}

void GameObject::addCollidedObject(GameObject* go)
{
	collidedObjects.push_back(go);
}

bool GameObject::isAlreadyCollided(GameObject* go)
{
	for (size_t i = 0; i < collidedObjects.size(); i++)
	{
		if (collidedObjects.at(i) == go)
		{
			return true;
		}
	}
	return false;
}

void GameObject::addChild(GameObject* o)
{
	this->children.push_back(o);
	o->setIsChild(true);
}

void GameObject::animate()
{
	if (!animationDirections.empty())
	{		
		animationActions = animationDirections.find(currentAnimationDirectionsIndex)->second;
		this->currentFrame = animationActions->find(currentAnimationIndex)->second->play();
	}
}

void GameObject::setCurrentAnimationIndex(AnimationAction index)
{
	this->currentAnimationIndex = index;
}

void GameObject::setCurrentAnimationDirectionsIndex(AnimationDirection index)
{
	this->currentAnimationDirectionsIndex = index;
}

void GameObject::addAnimation(Animation * a)
{
	//this->animationDirections.push_back(a);
}

void GameObject::setCollider(Collider * c)
{
	collider = c;
}

Collider* GameObject::getCollider()
{
	return collider;
}

void GameObject::setScene(Scene* _scene)
{
	scene = _scene;
}

Scene* GameObject::getScene()
{
	return scene;
}

GameObject::~GameObject()
{
	//delete previousRect;
	//delete currentRect;
}
