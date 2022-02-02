#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

//bool up = false, down = false, left = false, right = false;
SDL_Rect* freeWalkZone;
void setFreeWalkZone();

//SDL_Rect sprites[66];

void Character::handleInput(vector<ButtonEvent> *buttonEvents)
{
	Scene* scene = this->getScene();
	if (hasControl() && !this->getScene()->isPaused())
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < buttonEvents->size(); i++)
		{
			//if (buttonEvents->at(i) == B)
			//{
			//	if (getSortOrder() == SceneSortLayer::BACKGROUND_PARALAX_1)
			//	{
			//		setSortOrder(SceneSortLayer::MID_BEHIND);
			//	}
			//	if (getSortOrder() == SceneSortLayer::MID_BEHIND)
			//	{
			//		setSortOrder(SceneSortLayer::BACKGROUND_PARALAX_1);
			//	}
			//}
			if (buttonEvents->at(i) == A)
			{
				setCurrentAnimationIndex(ANIM_ATTACKING);
				attacking = true;
				movingUp = false;
				movingDown = false;
				movingLeft = false;
				movingRight = false;
				canMove = true;
			}
			if (buttonEvents->at(i) == UP && canMove)
			{
				setCurrentAnimationIndex(ANIM_MOVING);
				attacking = false;
				bool facingUp = true;
				bool facingDown = false;
				bool facingLeft = false;
				bool facingRight = false;
				movingUp = true;
			}
			if (buttonEvents->at(i) == DOWN && canMove)
			{
				setCurrentAnimationIndex(ANIM_MOVING);
				attacking = false;
				bool facingUp = false;
				bool facingDown = true;
				bool facingLeft = false;
				bool facingRight = false;
				movingDown = true;
			}
			if (buttonEvents->at(i) == LEFT && canMove)
			{
				setCurrentAnimationIndex(ANIM_MOVING);
				attacking = false;
				bool facingUp = false;
				bool facingDown = false;
				bool facingLeft = true;
				bool facingRight = false;
				movingLeft = true;
			}
			if (buttonEvents->at(i) == RIGHT && canMove)
			{
				setCurrentAnimationIndex(ANIM_MOVING);
				attacking = false;
				bool facingUp = false;
				bool facingDown = false;
				bool facingLeft = false;
				bool facingRight = true;
				movingRight = true;
			}
			if (buttonEvents->at(i) == UP_RELEASE && canMove)
			{
				//setCurrentAnimationIndex(ANIM_IDLE);
				movingUp = false;
			}
			if (buttonEvents->at(i) == DOWN_RELEASE && canMove)
			{
				//setCurrentAnimationIndex(ANIM_IDLE);
				movingDown = false;
			}
			if (buttonEvents->at(i) == LEFT_RELEASE && canMove)
			{
				//setCurrentAnimationIndex(ANIM_IDLE);
				movingLeft = false;
			}
			if (buttonEvents->at(i) == RIGHT_RELEASE && canMove)
			{
				//setCurrentAnimationIndex(ANIM_IDLE);
				movingRight = false;
			}
			if (buttonEvents->at(i) == CANCEL_MOVEMENT)
			{
				setCurrentAnimationIndex(ANIM_IDLE);
				attacking = false;
				movingUp = false;
				movingDown = false;
				movingLeft = false;
				movingRight = false;

			}
			if (buttonEvents->at(i) == A_RELEASE)
			{

			}
		}

		if (movingDown && canMove)
		{
			flip = SDL_FLIP_NONE;
			setCurrentAnimationDirectionsIndex(ANIM_DOWN);
			int nexty = current.y + speed;
			if (nexty >= (Util::getScreenH() - (getCollider()->getRect()->h)))
			{
				//scene->syncMove(0, -speed);
				scene->next.y -= speed;
			}
			else
			{
				//syncMove(0, speed);
				next.y += speed;
				getCollider()->getRect()->y += speed;
			}
		}
		if (movingUp && canMove)
		{
			flip = SDL_FLIP_NONE;
			setCurrentAnimationDirectionsIndex(ANIM_UP);
			int nexty = next.y - speed;
			if (nexty <= 0)
			{
				scene->next.y += speed;
			}
			else
			{
				next.y -= speed;
				getCollider()->getRect()->y -= speed;
			}
		}
		if (movingRight && canMove)
		{
			flip = SDL_FLIP_NONE;
			setCurrentAnimationDirectionsIndex(ANIM_RIGHT);
			int nextX = current.x + speed;
			if (nextX >= Util::getScreenW() - (getCollider()->getRect()->w))
			{
				scene->next.x -= speed;
			}
			else
			{
				
				next.x += speed;
				getCollider()->getRect()->x += speed;
			}

		}
		if (movingLeft && canMove)
		{
			flip = SDL_FLIP_NONE;
			//flip = SDL_FLIP_HORIZONTAL;
			setCurrentAnimationDirectionsIndex(ANIM_LEFT);
			int nextX = current.x - speed;
			if (nextX <= 0)
			{				
				scene->next.x += speed;
			}
			else
			{
				next.x -= speed;
				getCollider()->getRect()->x -= speed;
			}
		}
		if (movingLeft == false && movingRight == false && movingUp == false && movingDown == false && attacking == false)
		{
			setCurrentAnimationIndex(ANIM_IDLE);
		}
	}
}

void Character::move()
{
	//if (!hasControl())
	//{
		GameObject::move();
	//}
}

void Character::render()
{
	SDL_RenderCopyEx(Util::getRenderTarget(), texture, this->currentFrame->getRect(), &current, 0, 0, flip);
}

Character::Character(Scene* scene, int health, int power, int _speed, SDL_Rect* startingPos, string spritesheetPath)
{
	//setSortOrder(sortOrder);
	previous.w = startingPos->w;
	previous.h = startingPos->h;
	previous.x = startingPos->x;
	previous.y = startingPos->y;
	current.w = startingPos->w;
	current.h = startingPos->h;
	current.x = startingPos->x;
	current.y = startingPos->y;
	next.w = startingPos->w;
	next.h = startingPos->h;
	next.x = startingPos->x;
	next.y = startingPos->y;
	this->texture = Util::loadTexture(spritesheetPath);

	Animation* idle = new Animation(this, 6, this->texture, AnimationAction::ANIM_IDLE, true);
	idle->addFrame(new Frame(0, 0, 64, 64));
	idle->addFrame(new Frame(0, 0, 64, 64));

	multimap<AnimationAction, Animation*>* d_anim_actions = new multimap<AnimationAction, Animation*>();
	d_anim_actions->insert(make_pair(ANIM_IDLE, idle));
	d_anim_actions->insert(make_pair(ANIM_MOVING, idle));
	d_anim_actions->insert(make_pair(ANIM_ATTACKING, idle));

	animationDirections.insert(make_pair(ANIM_UP, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_DOWN, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_LEFT, d_anim_actions));
	animationDirections.insert(make_pair(ANIM_RIGHT, d_anim_actions));
	setCurrentAnimationDirectionsIndex(ANIM_UP);

	setActive(true);
	this->addAnimation(idle);
	this->setCurrentAnimationIndex(AnimationAction::ANIM_IDLE);
	this->level = scene;
	this->setScene(scene);
	this->speed = _speed;
	setFreeWalkZone();
	this->setCollider(new Collider(current.x+60, current.y+60, current.w/6, current.h/6, false, this));
	scene->subscribe(this);
}

void setFreeWalkZone()
{
	freeWalkZone = &SDL_Rect();
	freeWalkZone->x = Util::getScreenW() / 5;
	freeWalkZone->y = Util::getScreenH() / 5;
	freeWalkZone->w = ((Util::getScreenW() / 5) * 4);
	freeWalkZone->h = ((Util::getScreenH() / 5) * 4);
}


void Character::setHasControl(bool _hasControl)
{
	control = _hasControl;
}

bool Character::hasControl()
{
	return control;
}

Character::~Character()
{
	
}