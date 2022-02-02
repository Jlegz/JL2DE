#pragma once
#include "JL2DE.h"
#include <thread>     
using namespace Config;
using namespace Events;

void rez_submit_func(MenuItem* m);
void rez_submit_func(MenuItem* m)
{
	int width = atoi(ConfigList::split(m->getVal(), 'X').at(0).c_str());
	int height = atoi(ConfigList::split(m->getVal(), 'X').at(1).c_str());
	ConfigList::getAttributeByKey(Constants::CONFIG_CHK())->setValue("FALSE");
	Util::setAskUserForResolution(false);
	Util::resizeWindow(height, width, false);
}




int main(int argc, char* args[])
{
	if (!Util::init())		// Check for successful SDL and Config initialization
	{
		return 1;			// If we fail, we exit main() with a non zero return value.
	}
	else
	{						// Otherwise, we enter our main loop!
		GameObject::addSoundFX(SoundFX_Key::PLAYER_AXE_FX, "Assets/Sword Attack 2.wav");
		int tileCoutH = (Util::getScreenH() / 100);
		int tileCoutW = (Util::getScreenW() / 100);

		Scene* rezSelectScene = new Scene();
		Scene* activeScene = new Scene();
		Scene* loadingScene = new Scene();
		loadingScene->buildScene(0, 0, 0, 0);
		Text* loadingText = new Text(loadingScene, 32, 32, 32, 32, true, TextState::WHITE, FORE_FRONT);
		//Text* t = nullptr;
		Menu* m = nullptr;
		MenuItem* i1 = nullptr;
		MenuItem* i2 = nullptr;
		MenuItem* i3 = nullptr;
		MenuItem* i4 = nullptr;

		if (Util::getAskUserForResolution())
		{			
			//t = new Text(rezSelectScene, 0, 32, 32, 32, true, TextState::WHITE, FORE_FRONT);
			m = new Menu(rezSelectScene);
			rezSelectScene->buildScene(0, 0, 0, 0);
			int itemStartY = 64;
			for (multimap<long, long>::iterator it = Util::getAvailableDisplayResolutions()->begin(); it != Util::getAvailableDisplayResolutions()->end(); ++it)
			{
				string rezString = "";
				rezString = to_string((*it).first) + "X" + to_string((*it).second);

				m->addItem(new MenuItem(rezSelectScene, rezString, rezString, 250, (itemStartY += 32), 25, 25, TextState::BLACK_WHITE_BG));
			}
			m->setSubmit(rez_submit_func);
		}


			std::thread loadSceneThread(&Scene::buildScene, activeScene, Util::getAdjustedTileWidth(), Util::getAdjustedTileHeight(), 20, 20);
			SDL_Rect playerStart = SDL_Rect();
			playerStart.x = (Util::getScreenW() / 2) - (Util::getAdjustedTileWidth());
			playerStart.y = (Util::getScreenH() / 2) - (Util::getAdjustedTileHeight());
			playerStart.w = (Util::getAdjustedTileWidth() * 2);
			playerStart.h = (Util::getAdjustedTileHeight() * 2);
			SDL_Rect xStart = SDL_Rect();
			xStart.x = (Util::getScreenW() / 4);
			xStart.y = (Util::getScreenH() / 4);
			SDL_Rect yStart = SDL_Rect();
			yStart.x = (Util::getScreenW() * 4);
			yStart.y = (Util::getScreenH() * 4);
			Character* p = nullptr;
			Character* x = nullptr;
			Character* y = nullptr;




		Text* fpsCounter = nullptr;
		int lcountedFrames = 0;


		while (!EventHandler::hasQuit())
		{
			SDL_RenderClear(Util::getRenderTarget());

			if (Util::getAskUserForResolution())
			{
				rezSelectScene->play();
			}
			else
			{
				if (activeScene->getState() == SCENE_READY)// && lcountedFrames / (SDL_GetTicks() / 1000.0f) > 10)
				{
					if (p == nullptr)
					{
						p = new Character(activeScene, 1, 1, 20, &playerStart, "Assets/Crusader.png");
						//y = new Character(activeScene, 1, 1, 10, &yStart, "Assets/Crusader.png");
						p->setHasControl(true);
						p->setTag(TagConstansts::SWORD_TAG());
						activeScene->addChild(p);
						//y->setHasControl(false);
					}

					if (fpsCounter == nullptr)
					{
						fpsCounter = new Text(activeScene, 32, (Util::getScreenH() - 32), 32, 32, false, TextState::WHITE_BLACK_BG, FORE_FRONT);
					}
					//join thread here!
					if (loadSceneThread.joinable()) loadSceneThread.join();
					{
						activeScene->play();
					}
				}
				else if (activeScene->getState() == SCENE_LOADING)
				{
					loadingScene->play();
				}
			}
			SDL_SetRenderDrawColor(Util::getRenderTarget(), 0x0, 0x0, 0x0, 0x0);
			SDL_RenderPresent(Util::getRenderTarget());

			++lcountedFrames;
			++Util::countedFrames;
			Util::frameRate = lcountedFrames / (SDL_GetTicks() / 1000.0f);
			if(Util::frameRate == 0)
			{
				Util::frameRate = 1;
			}
			string frameRate = to_string(lcountedFrames / (SDL_GetTicks() / 1000.0f));
			if (fpsCounter != nullptr)
			{
				fpsCounter->print("FPS " + frameRate);
			}
			loadingText->print("Loading...");
		}



		if (Scene::gGameController != NULL)
		{
			SDL_GameControllerClose(Scene::gGameController);
		}
		SDL_Quit();
	}
	return 0;
}
