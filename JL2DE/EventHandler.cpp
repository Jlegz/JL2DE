#pragma once
#include "JL2DE.h"
using namespace Config;
using namespace Events;

bool EventHandler::quit = false;

bool EventHandler::hasQuit()
{
	return quit;
}

bool EventHandler::toggleQuit()
{
	quit = !quit;
	return quit;
}