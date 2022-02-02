#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

void pause_submit_func(MenuItem* m);
void pause_submit_func(MenuItem* m)
{
	if (m->isActive())
	{
		if (m->getVal() == "RESUME")
		{
			m->getScene()->togglePause();
		}
		else if (m->getVal() == "QUIT")
		{
			EventHandler::toggleQuit();
		}
	}

}

//vector<GameObject*> Scene::subscriberPointers;
//vector<GameObject> EventHandler::subscriberObjects;
bool Scene::useJoystick;
vector<string> Scene::buttons;
SDL_GameController* Scene::gGameController;
//Scene* EventHandler::level = nullptr;
const int JOYSTICK_DEAD_ZONE = 8000;
bool paused;
//SceneState Scene::state;
vector<GameObject*> Scene::getSubscribers()
{
	return subscriberPointers;
}

vector<GameObject*> Scene::getCollidableSubscribers()
{
	return collidableObjects;
}

Scene::Scene()
{
	this->subscribe(this);
	 //Scene::subscriberPointers = new vector<GameObject*>();
	current.x = 0;
	current.y = 0;
	previous.x = 0;
	previous.y = 0;
	next.x = 0;
	next.y = 0;
}					

Scene::~Scene()
{
	for (int i = 0; i < this->children.size(); i++)
	{
		delete this->children.at(i);
	}
	for (int i = 0; i < this->subscriberPointers.size(); i++)
	{
		delete this->subscriberPointers.at(i);
	}
}

void Scene::buildScene(int tileW, int tileH, int mapW, int mapH)
{
	SDL_Texture* t = Util::loadTexture("Assets/Overworld.png");
	int x = 0, y = 0;
	state = SCENE_LOADING;
	for (size_t i = 0; i < mapW; i++)
	{

			for (size_t j = 0; j <= mapH; j++)
			{
				if (j % 1 == 0)
				{
					this->addChild(new Tile(this, Util::getRenderTarget(), x, y, tileW, tileH, t));
				}
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
				//SDL_Delay(1);
			}
		
	}
	mapWidth = mapW * tileW;
	mapHeight = mapH * tileH;

	//MENU IN MAIN LOOP TEST START

	this->pauseMenu = new Menu(this);
	pauseMenu->addItem(new MenuItem(this, "RESUME", "RESUME", ((Util::getScreenW() / 2) - 300), ((Util::getScreenH() / 2) - 150), 100, 100, TextState::WHITE_BLACK_BG));
	pauseMenu->addItem(new MenuItem(this, "QUIT", "QUIT", (Util::getScreenW() / 2) - 200, ((Util::getScreenH() / 2) - 50), 100, 100, TextState::WHITE_BLACK_BG));
	pauseMenu->setSubmit(pause_submit_func);
	pauseMenu->setActive(false);

	//MENU IN MAIN LOOP TEST END

	state = SCENE_READY;
}

void Scene::handleInput(vector<ButtonEvent> *buttonEvents)
{
	//const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	//int speed = 20;
	//for (int i = 0; i < buttonEvents->size(); i++)
	//{
	//	if (buttonEvents->at(i) == A)
	//	{
	//		setCurrentAnimationIndex(ANIM_ATTACKING);
	//		movingUp = false;
	//		movingDown = false;
	//		movingLeft = false;
	//		movingRight = false;
	//		canMove = false;
	//	}
	//	if (buttonEvents->at(i) == UP && canMove)
	//	{
	//		setCurrentAnimationIndex(ANIM_MOVING);

	//		bool facingUp = true;
	//		bool facingDown = false;
	//		bool facingLeft = false;
	//		bool facingRight = false;
	//		movingUp = true;
	//	}
	//	if (buttonEvents->at(i) == DOWN && canMove)
	//	{
	//		setCurrentAnimationIndex(ANIM_MOVING);

	//		bool facingUp = false;
	//		bool facingDown = true;
	//		bool facingLeft = false;
	//		bool facingRight = false;
	//		movingDown = true;
	//	}
	//	if (buttonEvents->at(i) == LEFT && canMove)
	//	{
	//		setCurrentAnimationIndex(ANIM_MOVING);

	//		bool facingUp = false;
	//		bool facingDown = false;
	//		bool facingLeft = true;
	//		bool facingRight = false;
	//		movingLeft = true;
	//	}
	//	if (buttonEvents->at(i) == RIGHT && canMove)
	//	{
	//		setCurrentAnimationIndex(ANIM_MOVING);

	//		bool facingUp = false;
	//		bool facingDown = false;
	//		bool facingLeft = false;
	//		bool facingRight = true;
	//		movingRight = true;
	//	}
	//	if (buttonEvents->at(i) == UP_RELEASE && canMove)
	//	{
	//		//setCurrentAnimationIndex(ANIM_IDLE);
	//		movingUp = false;
	//	}
	//	if (buttonEvents->at(i) == DOWN_RELEASE && canMove)
	//	{
	//		//setCurrentAnimationIndex(ANIM_IDLE);
	//		movingDown = false;
	//	}
	//	if (buttonEvents->at(i) == LEFT_RELEASE && canMove)
	//	{
	//		//setCurrentAnimationIndex(ANIM_IDLE);
	//		movingLeft = false;
	//	}
	//	if (buttonEvents->at(i) == RIGHT_RELEASE && canMove)
	//	{
	//		//setCurrentAnimationIndex(ANIM_IDLE);
	//		movingRight = false;
	//	}
	//	if (buttonEvents->at(i) == CANCEL_MOVEMENT)
	//	{
	//		setCurrentAnimationIndex(ANIM_IDLE);

	//		movingUp = false;
	//		movingDown = false;
	//		movingLeft = false;
	//		movingRight = false;

	//	}
	//	if (buttonEvents->at(i) == A_RELEASE)
	//	{

	//	}
	//}

	//if (movingDown && canMove)
	//{
	//	//flip = SDL_FLIP_NONE;
	//	//setCurrentAnimationDirectionsIndex(ANIM_DOWN);
	//	//int nexty = current.y + speed;
	//	//if (nexty >= (Util::getScreenH() - (getCollider()->getRect()->h)))
	//	//{
	//	//	this->getScene()->current.y -= speed;
	//	//	this->getScene()->next.y -= speed;
	//	//}
	//	//else
	//	//{
	//		current.y += speed;
	//		//getCollider()->getRect()->y += speed;
	//	//}
	//}
	//if (movingUp && canMove)
	//{
	//	//flip = SDL_FLIP_NONE;
	//	//setCurrentAnimationDirectionsIndex(ANIM_UP);
	//	//int nexty = current.y - speed;
	//	//if (nexty <= 0)
	//	//{
	//	//	this->getScene()->current.y += speed;
	//	//}
	//	//else
	//	//{
	//		current.y -= speed;
	//		//getCollider()->getRect()->y -= speed;
	//	//}
	//}
	//if (movingRight && canMove)
	//{
	//	//flip = SDL_FLIP_NONE;
	//	//setCurrentAnimationDirectionsIndex(ANIM_RIGHT);
	//	//int nextX = current.x + speed;
	//	//if (nextX >= Util::getScreenW() - (getCollider()->getRect()->w))
	//	//{
	//	//	this->getScene()->current.x -= speed;
	//	//}
	//	//else
	//	//{
	//		this->current.x += speed;
	//		//getCollider()->getRect()->x += speed;
	//	//}

	//}
	//if (movingLeft && canMove)
	//{
	//	//flip = SDL_FLIP_NONE;
	//	////flip = SDL_FLIP_HORIZONTAL;
	//	//setCurrentAnimationDirectionsIndex(ANIM_LEFT);
	//	//int nextX = current.x - speed;
	//	//if (nextX <= 0)
	//	//{
	//	//	this->getScene()->current.x += speed;
	//	//}
	//	//else
	//	//{
	//		this->current.x -= speed;
	//		//getCollider()->getRect()->x -= speed;
	//	//}
	//}
	////if (movingLeft == false && movingRight == false && movingUp == false && movingDown == false && attacking == false)
	////{
	////	setCurrentAnimationIndex(ANIM_IDLE);
	////}
}

bool Scene::subscribe(GameObject* subscriber)
{
	if (subscriber->isUI)
	{
		subscriberUIPointers.push_back(subscriber);
	}
	else
	{
		subscriberPointers.push_back(subscriber);
	}

	subscriber->setScene(this);
	if (subscriber->getCollider() != nullptr)
	{
		collidableObjects.push_back(subscriber);
		//return true;
	}
	return false;
}


void Scene::remove(GameObject* subscriber)
{
	for (int i = 0; i < subscriberPointers.size(); i++)
	{

	}
}

void Scene::clear()
{
	subscriberPointers.clear();
}

//void tempTriggerI(GameObject* go)
//{
//	int xDelta = go->getCurrent().x - go->getCurrent().x;
//	int yDelta = go->getCurrent().y - go->getCurrent().y;
//	go->lateMove(xDelta, yDelta);
//}
//
//void tempTriggerJ(GameObject* go)
//{
//	int xDelta = go->getCurrent().x - go->getCurrent().x;
//	int yDelta = go->getCurrent().y - go->getCurrent().y;
//	go->lateMove(xDelta, yDelta);
//}

SceneState Scene::getState()
{
	return state;
}

void Scene::setState(SceneState _state)
{
	state = _state;
}

void Scene::moveSubscribers()
{
	for (int i = 0; i < subscriberPointers.size(); i++)
	{
		if (!subscriberPointers.at(i)->isChild())
		{
			if (!Scene::isPaused())
			{
				if (subscriberPointers.at(i)->isActive())
				{
					subscriberPointers.at(i)->move();
				}
			}
			else if (Scene::isPaused() && !subscriberPointers.at(i)->isPausable)
			{
				if (subscriberPointers.at(i)->isActive())
				{
					subscriberPointers.at(i)->move();
				}
			}
		}
	}
	for (int i = 0; i < subscriberUIPointers.size(); i++)
	{
		subscriberUIPointers.at(i)->move();
	}
	this->move();
}

void Scene::renderUI()
{
	for (size_t i = 0; i < subscriberUIPointers.size(); i++)
	{
		if (subscriberUIPointers.at(i)->isActive())
		{
			subscriberUIPointers.at(i)->render();
		}
	}
}

void Scene::renderSubscribers()
{
	for (size_t i = 0; i < subscriberPointers.size(); i++)
	{
			if (this->isPaused() == false)
			{
				subscriberPointers.at(i)->animate();
			}
			if ((subscriberPointers.at(i)->getCurrent().x < (Util::getScreenW() + subscriberPointers.at(i)->getCurrent().w)
				&& subscriberPointers.at(i)->getCurrent().x >(subscriberPointers.at(i)->getCurrent().w * -1)
				&& subscriberPointers.at(i)->getCurrent().y < (Util::getScreenH() + subscriberPointers.at(i)->getCurrent().h)
				&& subscriberPointers.at(i)->getCurrent().y >(subscriberPointers.at(i)->getCurrent().h * -1))
				|| subscriberPointers.at(i)->alwaysRender)
			{
				subscriberPointers.at(i)->render();
				if (subscriberPointers.at(i)->getCollider() != nullptr)
				{
					subscriberPointers.at(i)->getCollider()->drawColliderBounds();
				}
			}
	}
}

void Scene::closeAllSubscribers()
{

}

void Scene::closeSubscriber(GameObject* subscriber)
{
}

bool Scene::isPaused()
{
	return paused;
}

bool Scene::togglePause()
{
	for (int i = 0; i < subscriberPointers.size(); i++)
	{
		//if(subscriberPointers.at(i)
	}
	paused = !paused;
	this->pauseMenu->setActive(paused);
	for (size_t i = 0; i < this->pauseMenu->items.size(); i++)
	{
		this->pauseMenu->items[i]->current.x = 100;//(Util::getScreenW() / 2) - (this->pauseMenu->items[i]->current.w / 2);
		this->pauseMenu->items[i]->current.y = 100;//(Util::getScreenH() / 2) + (this->pauseMenu->items[i]->current.h / 2);
	}
	this->pauseMenu->current.x = 100;
	this->pauseMenu->current.y = 100;
	return paused;
}

void Scene::handleKeyboard()
{
	SDL_Event event;
	vector<ButtonEvent> buttonEvents;
	while (SDL_PollEvent(&event))
	{
		//if (Scene::isPaused())
		//{
		//	if (event.type == SDL_KEYUP)
		//	{
		//		switch (event.key.keysym.sym)
		//		{
		//		case SDLK_ESCAPE:
		//			Scene::togglePause();
		//			break;
		//		case SDLK_q:
		//			//EventHandler::toggleQuit();
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}
		/*else
		{*/
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					//positive vertical input
				case SDLK_UP:
					buttonEvents.push_back(UP);
					break;
				case SDLK_w:
					buttonEvents.push_back(A);
					break;
					//negative vertical input
				case SDLK_s:
					buttonEvents.push_back(B);
					break;
					//negative vertical input
				case SDLK_DOWN:
					buttonEvents.push_back(DOWN);
					break;
				case SDLK_LEFT:
					buttonEvents.push_back(LEFT);
					break;
				case SDLK_a:
					buttonEvents.push_back(LEFT);
					break;
					//positive horizontal input
				case SDLK_RIGHT:
					buttonEvents.push_back(RIGHT);
					break;
				case SDLK_d:
					buttonEvents.push_back(RIGHT);
					break;
				case SDLK_RETURN:
					buttonEvents.push_back(START);
					break;
				default:
					break;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					//positive vertical input
				case SDLK_UP:
					buttonEvents.push_back(UP_RELEASE);
					break;
				case SDLK_w:
					buttonEvents.push_back(A);
					break;
					//negative vertical input
				case SDLK_DOWN:
					buttonEvents.push_back(DOWN_RELEASE);
					break;
				case SDLK_s:
					buttonEvents.push_back(B);
					break;
					//negative horizontal input
				case SDLK_LEFT:
					buttonEvents.push_back(LEFT_RELEASE);
					break;
				case SDLK_a:
					buttonEvents.push_back(LEFT_RELEASE);
					break;
					//positive horizontal input
				case SDLK_RIGHT:
					buttonEvents.push_back(RIGHT_RELEASE);
					break;
				case SDLK_d:
					buttonEvents.push_back(RIGHT_RELEASE);
					break;
				case SDLK_ESCAPE:
					Scene::togglePause();
					buttonEvents.push_back(CANCEL_MOVEMENT);
					break;
				case SDLK_RETURN:
					buttonEvents.push_back(START_RELEASE);
					break;
				default:
					break;
				}
			}
		//}
	}
	for (int i = 0; i < subscriberPointers.size(); i++)
	{
		subscriberPointers.at(i)->handleInput(&buttonEvents);
	}
	for (int i = 0; i < subscriberUIPointers.size(); i++)
	{
		subscriberUIPointers.at(i)->handleInput(&buttonEvents);
	}
}

void Scene::handleJoystick()
{
	SDL_Event event;
	vector<ButtonEvent> buttonEvents;
	//Text buttonOutput = Text(16, (Util::getScreenH() - 64), 16, 16, false);
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_JOYAXISMOTION && !Scene::isPaused())
		{
			//Motion on controller 0
			if (event.jaxis.which == 0)
			{
				//X axis motion
				if (event.jaxis.axis == 6 || event.jaxis.axis == 0)// || event.jaxis.axis == 3)
				{
					//Left of dead zone
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						//xDir = -1;
						buttonEvents.push_back(LEFT);
					}
					//Right of dead zone
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						//xDir = 1;
						buttonEvents.push_back(RIGHT);
					}
					else
					{
						buttonEvents.push_back(RIGHT_RELEASE);
						buttonEvents.push_back(LEFT_RELEASE);
					}
				}
				else if (event.jaxis.axis == 5 || event.jaxis.axis == 1)// || event.jaxis.axis == 4)
				{
					//Below of dead zone
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						//yDir = -1;
						buttonEvents.push_back(UP);
					}
					//Above of dead zone
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						//yDir = 1;
						buttonEvents.push_back(DOWN);
					}
					else
					{
						//yDir = 0;
						buttonEvents.push_back(DOWN_RELEASE);
						buttonEvents.push_back(UP_RELEASE);
					}
				}
			}
		}


		if (event.type == SDL_JOYHATMOTION && !Scene::isPaused())
		{
			if (event.jhat.which == 0)
			{
				buttons.push_back(to_string(event.jhat.value));
			}
			if (event.jhat.value == SDL_HAT_LEFTUP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_LEFTDOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
				buttonEvents.push_back(DOWN);
			}
			if (event.jhat.value == SDL_HAT_RIGHTUP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_RIGHTDOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
				buttonEvents.push_back(DOWN);
			}

			if (event.jhat.value == SDL_HAT_UP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_DOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(DOWN);
			}
			if (event.jhat.value == SDL_HAT_LEFT)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
			}
			if (event.jhat.value == SDL_HAT_RIGHT)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
			}
			if (event.jhat.value == SDL_HAT_CENTERED)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
			}
		}

		if (event.type == SDL_JOYBUTTONUP && !Scene::isPaused())
		{
			if (event.jbutton.which == 0)
			{
				buttons.push_back(to_string(event.jbutton.button));
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_START) //START
				{
					buttonEvents.push_back(CANCEL_MOVEMENT);
					Scene::togglePause();
				}
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					buttonEvents.push_back(A);
				}
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP && Scene::isPaused())
		{
			if (event.jbutton.which == 0)
			{
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					Scene::togglePause();
				}
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					EventHandler::toggleQuit();
				}
			}
		}
	}
	for (int i = 0; i < subscriberPointers.size(); i++)
	{
		subscriberPointers.at(i)->handleInput(&buttonEvents);
	}
	//buttonOutput.printL("TEST");
}

void Scene::handleGameController()
{
	SDL_Event event;
	vector<ButtonEvent> buttonEvents;
	//Text buttonOutput = Text(16, (Util::getScreenH() - 64), 16, 16, false);
	while (SDL_PollEvent(&event) != 0)
	{
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_A) == 1)
		{
			buttonEvents.push_back(A);
		}
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_B) == 1)
		{
			buttonEvents.push_back(B);
		}
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_X) == 1)
		{
			buttonEvents.push_back(X);
		}
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_Y) == 1)
		{
			buttonEvents.push_back(Y);
		}
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_START) == 1)
		{
			buttonEvents.push_back(START);
		}
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_BACK) == 1)
		{
			buttonEvents.push_back(SELECT);
		}		
		if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
		{
			buttonEvents.push_back(UP);
		}


		if (event.type == SDL_JOYHATMOTION && !Scene::isPaused())
		{
			if (event.jhat.which == 0)
			{
				buttons.push_back(to_string(event.jhat.value));
			}
			if (event.jhat.value == SDL_HAT_LEFTUP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_LEFTDOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
				buttonEvents.push_back(DOWN);
			}
			if (event.jhat.value == SDL_HAT_RIGHTUP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_RIGHTDOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
				buttonEvents.push_back(DOWN);
			}

			if (event.jhat.value == SDL_HAT_UP)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(UP);
			}
			if (event.jhat.value == SDL_HAT_DOWN)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(DOWN);
			}
			if (event.jhat.value == SDL_HAT_LEFT)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(LEFT);
			}
			if (event.jhat.value == SDL_HAT_RIGHT)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
				buttonEvents.push_back(RIGHT);
			}
			if (event.jhat.value == SDL_HAT_CENTERED)//event.jaxis.axis == 0 || event.jaxis.axis == 3)
			{
				buttonEvents.push_back(CANCEL_MOVEMENT);
			}
		}

		if (event.type == SDL_JOYBUTTONUP && !Scene::isPaused())
		{
			if (event.jbutton.which == 0)
			{
				buttons.push_back(to_string(event.jbutton.button));
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_START) //START
				{
					buttonEvents.push_back(CANCEL_MOVEMENT);
					Scene::togglePause();
				}
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					buttonEvents.push_back(A);
				}
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP && Scene::isPaused())
		{
			if (event.jbutton.which == 0)
			{
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					Scene::togglePause();
				}
				if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					EventHandler::toggleQuit();
				}
			}
		}
	}
	for (int i = 0; i < subscriberPointers.size(); i++)
	{
		subscriberPointers.at(i)->handleInput(&buttonEvents);
	}
}

void Scene::handleSubscribersInput()
{
	if (useJoystick)
	{
		handleJoystick();
	}
	else
	{
		handleKeyboard();
	}
}

void Scene::play()
{
	//-----------------------------------------------------------------------------------------------------
	//Events-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------
	Scene::handleSubscribersInput();

	//-----------------------------------------------------------------------------------------------------
	//Logic------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------
	Scene::moveSubscribers();

	//-----------------------------------------------------------------------------------------------------
	//Rendering Objects--------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------
	Scene::renderSubscribers();

	//-----------------------------------------------------------------------------------------------------
	//Rendering UI--------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------
	Scene::renderUI();
}

void Scene::move()
{
	GameObject::move();
}
//
//void Level::render()
//{
//}
