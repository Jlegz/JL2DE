#pragma once
#include "JL2DE.h"
using namespace Events;

//vector<MenuItem*> Menu::items;
//MenuItem* Menu::selectedItem;
//SDL_Rect* Menu::pos;


Menu::Menu(Scene* scene)
{
	isUI = true;
	scene->subscribe(this);
}

Menu::Menu(Scene* scene, bool defaultPlacement)
{
	isUI = true;
}

Menu::Menu(Scene* scene, SDL_Rect* pos, vector< MenuItem*> items)
{
	isUI = true;
}

Menu::Menu(Scene* scene, vector<MenuItem*> items, bool defaultPlacement)
{
	isUI = true;
}


void Menu::setItems(vector<MenuItem*> items)
{

}
void Menu::setSubmit(void(*submit_func)(MenuItem*))
{
	Menu::submit_func = submit_func;
}

Menu* Menu::addItem(MenuItem* item)
{
	if (Menu::items.empty())
	{
		//item->getTextObj()->next.y = 50;
		item->select();
		Menu::selectedItemIndex = 0;
	}
	else
	{
		//item->getTextObj()->next.y = (Menu::items.at(Menu::items.size() - 1)->getTextObj()->current.y) + (Menu::items.at(Menu::items.size() - 1)->getTextObj()->current.h);
		item->deSelect();
	}
	this->addChild(item);
	Menu::items.push_back(item);
	return this;
}

void Menu::handleInput(vector<ButtonEvent>* buttonEvents)
{
	if (this->isActive())
	{
		for (int i = 0; i < buttonEvents->size(); i++)
		{
			if (buttonEvents->at(i) == UP)
			{
				last()->select();
			}
			else if (buttonEvents->at(i) == DOWN)
			{
				next()->select();
			}
			else if (buttonEvents->at(i) == START || buttonEvents->at(i) == A)
			{
				submit();
			}
		}
	}

}

MenuItem* Menu::last()
{
	if (!Menu::selectedItemIndex == 0)
	{
		Menu::selectedItemIndex--;
		for (int i = 0; i < Menu::items.size(); i++)
		{
			Menu::items.at(i)->deSelect();
		}
	}
	else
	{
		Menu::selectedItemIndex = (Menu::items.size() - 1);
		for (int i = 0; i < Menu::items.size(); i++)
		{
			if (i != selectedItemIndex)
			{
				Menu::items.at(i)->deSelect();
			}
		}
	}
	return Menu::items.at(selectedItemIndex)->select();
}

MenuItem* Menu::next()
{
	if (Menu::selectedItemIndex < (Menu::items.size() - 1))
	{
		Menu::selectedItemIndex++;
		for (int i = 0; i < Menu::items.size(); i++)
		{
			Menu::items.at(i)->deSelect();
		}
	}
	else
	{
		Menu::selectedItemIndex = 0;
		for (int i = 0; i < Menu::items.size(); i++)
		{
			if (i != selectedItemIndex)
			{
				Menu::items.at(i)->deSelect();
			}
		}
	}
	return Menu::items.at(selectedItemIndex);
}
void Menu::submit()
{
	if (Menu::submit_func != nullptr)
	{
		Menu::submit_func(Menu::items.at(Menu::selectedItemIndex));
	}
	else
	{
		cout << "No submit function loaded.  Please use 'Menu::setSubmit(void(*submit_func)())' to load a function for this menu." << endl;
	}
}

Menu::~Menu()
{
	for (int i = 0; i < Menu::items.size(); i++)
	{
		delete Menu::items.at(i);
	}
}

