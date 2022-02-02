#pragma once
#include <stdio.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_mixer.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h> 
#include <chrono>
#include <random>
#include <math.h>

#define DllExport   __declspec( dllexport )


#if defined(_WIN32)
#include <Windows.h>
#include "Windef.h"
#elif defined(__APPLE__)
// this is a Mac OSX environment!
#elif defined(__linux__)
// this is a Linux environment! (any GNU/Linux distribution)
#endif

using namespace std;

enum ButtonEvent
{
	UP,
	UP_RELEASE,
	LEFT,
	LEFT_RELEASE,
	DOWN,
	DOWN_RELEASE,
	RIGHT,
	RIGHT_RELEASE,
	A,
	A_RELEASE,
	B,
	B_RELEASE,
	X,
	X_RELEASE,
	Y,
	Y_RELEASE,
	PAUSE,
	PAUSE_RELEASE,
	START,
	START_RELEASE,
	SELECT,
	SELECT_RELEASE,
	CANCEL_MOVEMENT
};

enum TextState
{
	BLACK,
	WHITE,
	WHITE_BLACK_BG,
	BLACK_WHITE_BG
};

enum AnimationDirection
{
	ANIM_UP,
	ANIM_DOWN,
	ANIM_LEFT,
	ANIM_RIGHT
};

enum AnimationAction
{
	ANIM_IDLE,
	ANIM_MOVING,
	ANIM_ATTACKING
};

enum SceneState
{
	NO_SCENE_LOADED,
	SCENE_LOADING,
	SCENE_READY,
	SCENE_UNLOADING
};

enum SceneSortLayer
{
	BACKGROUND_PARALAX_0,
	BACKGROUND_PARALAX_1,
	BACKGORUND_PARALAX_2,
	MID_BEHIND,
	MID_FRONT,
	FORE_BEHIND,
	FORE_FRONT
};

enum SoundFX_Key
{
	PLAYER_JUMP_FX,
	PLAYER_DAMAGE_FX,
	PLAYER_AXE_FX,
};

//enum CharacterState
//{
//	
//};

class TagConstansts
{
public:
	static const string ENEMY_TAG() { return "ENEMY"; }
	static const string SWORD_TAG() { return"SWORD"; }
};

class GameObject;

class SoundFX
{
private:
	Mix_Chunk* soundFX;
public:
	SoundFX(string path);
	void play();
};

class Music
{
private:
	Mix_Music* music;
public:
	Music(string path);
	void play();
	void togglePause();
};

class Frame
{
public:
	Frame(SDL_Rect* r);
	Frame(int frameXpos, int frameYpos, int frameW, int frameH);
	Frame(int frameXpos, int frameYpos, int frameW, int frameH, void(*frame_action)());
	void setFrameAction(void(*frame_action)());
	void executeFrameAction();
	void setRect(SDL_Rect* r);
	SDL_Rect* getRect();
private:
	Frame();
	SDL_Rect* frame;
	void(*frame_action)();
	bool hasAction;
};

class Animation
{
public:
	Animation(GameObject* parentObject, vector<Frame> frames, unsigned int frameRate, SDL_Texture* texture, AnimationAction next, bool loop = true);
	Animation(GameObject* parentObject, unsigned int frameRate, SDL_Texture* texture, AnimationAction next, bool loop = true);
	Animation(GameObject* parentObject, unsigned int frameRate, SDL_Texture* texture, int cellW, int cellH, int cellColCount, int cellRowCount, AnimationAction next, bool loop = true);
	//Animation(unsigned int frameRate, SDL_Texture* texture, int cellW, int cellH, int cellColCount, int cellRowCount, bool loop = true);
	~Animation();
	Animation* addFrame(Frame* frame);
	Frame* getCurrentFrame(); //returns current frame without advancing animation
	Frame* play(); //advances animation based on frameRate then returns that frame
	//void loadSpriteSheet(int cellW, int cellH, int cellColCount, int cellRowCount);
	void setLoop(bool l);
	bool isLoop();
	int frameCount;
	//static loadAnimations();
private:
	Animation();
	unsigned int frameIndex;
	bool loop;
	AnimationAction nextAnimationIndex;
	GameObject* parentObject;
	SDL_Texture* texture;
	Frame* currentFrame;
	vector<Frame*> frames;
	unsigned int frameRate;
	unsigned int currentFrameRateCount;
};

class Object
{
public:
	///<summary>
	//SDL_Rect value of position from previous frame.
	///</summary>
	SDL_Rect previous;
	///<summary>
	//SDL_Rect value of position of the current frame.
	///</summary>
	SDL_Rect current;
	///<summary>
	//SDL_Rect value of position of the next frame.
	///</summary>
	SDL_Rect next;
	///<summary>
	//true if this object is the child of another.
	///</summary>
	bool child = false;
	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;
	bool facingUp = false;
	bool facingDown = false;
	bool facingLeft = false;
	bool facingRight = false;
public:
	///<summary>
	//return SDL_Rect value of position from previous frame.
	///</summary>
	SDL_Rect getPrevious() { return previous; };
	///<summary>
	//return SDL_Rect value of position from current frame.
	///</summary>
	SDL_Rect getCurrent() { return current; };
	///<summary>
	//return SDL_Rect value of position from next frame.
	///</summary>
	SDL_Rect getNext() { return next; };
	///<summary>
	//set SDL_Rect x and y value of position from previous frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setPrevious(SDL_Rect _previous) { previous.x = _previous.x; previous.y = _previous.y; };
	///<summary>
	//set SDL_Rect x and y value of position from previous frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setPrevious(int _newX, int _newY) { previous.x = _newX; previous.y = _newY; };
	///<summary>
	//set SDL_Rect x and y value of position from current frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setCurrent(SDL_Rect _current) { current.x = _current.x; current.y = _current.y; };
	///<summary>
	//set SDL_Rect x and y value of position from current frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setCurrent(int _newX, int _newY) { current.x = _newX; current.y = _newY; };
	///<summary>
	//set SDL_Rect x and y value of position from next frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setNext(SDL_Rect _next) { next.x = _next.x; next.y = _next.y; };
	///<summary>
	//set SDL_Rect x and y value of position from next frame.  Only the SDL_Rect::x and SDL_Rect::y are used.
	///</summary>
	void setNext(int _newX, int _newY) { next.x = _newX; next.y = _newY; };
	///<summary>
	//If object is subscribed to an active Scene, Scene::handleSubscribersInput() will execute this method, passing a std::vector<Button> of
	//button events, abstracted by the scene from an unknown input device.
	///</summary>
	virtual void handleInput(vector<ButtonEvent> *buttonEvents) {};
	///<summary>
	//If object is subscribed to an active Scene, <c>Scene::moveSubscibers()</c> will execute this method, updating the Object::curret to Object::next
	///</summary>
	virtual void move() {};
	///<summary>
	//This will update the object and its childrens' current positions.  Calling only after Scene::moveSubscibers() has executed,
	//otherwise <c>Scene::moveSubscibers()</c> will overwrite the data.
	///</summary>
	virtual void lateMove(int xDelta, int yDelta) {};
	///<summary>
	//This will update the object and its childrens' next positions.  Calling only before Scene::moveSubscibers() has executed,
	//otherwise new position data will be updated a frame ahead of the next.
	///</summary>
	virtual void syncMove(int xDelta, int yDelta) {};

	bool isChild() { return child; };

	void setIsChild(bool _isChild) { child = _isChild; };
};
 


class Collider : public Object
{
public:

private:
	Collider();
	GameObject* parent;
	SDL_Rect* collider;
	bool isTrigger; // decides whether the collider physically stops the other collider, or simply calls the trigger action.
	bool isStatic; // decides whether collider will move with GameObject or remain statically placed.
	void(*triggerAction)(GameObject*);
	void(*colliderAction)(GameObject*);
public:
	Collider(SDL_Rect* collider_rect, bool isTrigger);
	Collider(int x, int y, int w, int h, bool isTrigger);
	Collider(int x, int y, int w, int h, bool isTrigger, GameObject* _parent);
	~Collider();
	//void lateMove(int xDelta, int yDelta) override;
	void moveTo(int x, int y);
	void moveBy(int xDelta, int yDelta);
	SDL_Rect* getRect();
	GameObject* getParent();
	void setParent(GameObject* parent);
	void setTriggerAction(void(*triggerAction)(GameObject*));
	void setColliderAction(void(*triggerAction)(GameObject*));
	void trigger(GameObject* g);
	void drawColliderBounds();
};

class Scene;
class Tile;
class Menu;

class GameObject : public Object
{
	//member properties
public:
	int count = 0;
	bool isPausable;
	bool isUI;
	bool canMove = true;
	GameObject();
	//GameObject();
	virtual ~GameObject();
protected:
	///<summary>
	/// Key:   Direction of an animation- determines which map of animations will be used.
	/// Value: Another map of Animations value. 
	///</summary>
	multimap<AnimationDirection, multimap<AnimationAction, Animation*>*> animationDirections;
	///<summary>
	/// Key:   Animation action of an animation- determines which animation will be used.
	/// Value: Animations value. 
	///</summary>
	multimap<AnimationAction, Animation*>* animationActions;
	vector<SDL_Rect*> frames;
	vector<GameObject*> children;
	Frame* currentFrame;
	Animation* currentAnimation;
	SDL_RendererFlip flip;
private:
	Scene* scene;
	ButtonEvent bEvent;
	Collider* collider;
	vector<GameObject*> collidedObjects;
	void clearCollidedObjects();
	void addCollidedObject(GameObject* go);
	bool isAlreadyCollided(GameObject* go);
	bool active = true;
	SceneSortLayer sortOrder;
	AnimationDirection currentAnimationDirectionsIndex;
	AnimationAction currentAnimationIndex;
	static vector<int>* sortOrderValues;
	static map<SoundFX_Key, SoundFX*> soundFX_col;
	static std::map<SoundFX_Key, SoundFX*>::iterator it;
	string tag;
	//member functions
public:
	bool alwaysRender;
	virtual void render() {};
	virtual void move() override;
	///<summary>
	//This will update the object and its childrens' current positions.  Calling only after Scene::moveSubscibers() has executed,
	//otherwise <c>Scene::moveSubscibers()</c> will overwrite the data.
	///</summary>
	//void lateMove(int xDelta, int yDelta) override;
	///<summary>
	//This will update the object and its childrens' next positions.  Calling only before Scene::moveSubscibers() has executed,
	//otherwise new position data will be updated a frame ahead of the next.
	///</summary>
	void syncMove(int xDelta, int yDelta) override;
	void setActive(bool _active);
	bool isActive();
	void setSortOrder(SceneSortLayer _sortOrder);
	SceneSortLayer getSortOrder();
	static vector<int>* getSortOrderValues();
	void setScene(Scene* _scene);
	Scene* getScene();
	bool checkForCollision();
	virtual void addChild(GameObject* o);
	virtual void animate();
	void addAnimation(Animation* a);
	void setCollider(Collider* c);
	void setCurrentAnimationIndex(AnimationAction index);
	void setCurrentAnimationDirectionsIndex(AnimationDirection index);
	Collider* getCollider();
	vector<GameObject*> getCollidedObjects();
	static void addSoundFX(SoundFX_Key key, string path);
	static SoundFX* getSoundFX(SoundFX_Key key);
	static void playSoundFX(SoundFX_Key key);
	//bool operator > (GameObject* go) { return this->sortOrder > go->getSortOrder(); }
	//bool operator < (GameObject* go) { return this->sortOrder < go->getSortOrder(); }
	void setTag(string _tag);
	string getTag();
private:
	virtual void updateChildren() {};
};

class MapLayer : public GameObject
{
public:
	vector<Tile*> tiles;
	int paralaxSpeed; // 0 does not move 100 moves at same speed as parent
	MapLayer(int tileW, int tileH, int mapW, int mapH, SDL_Texture* tileSetTexture);
	void buildMapLayer(int tileW, int tileH, int mapW, int mapH);
	void render() override;
private:
	MapLayer() {}
};

class Map : public GameObject
{
	vector<MapLayer*> mapLayers;
	void addMapLayer(MapLayer* mapLayer);
};

class Scene : public GameObject
{
private:
	int mapWidth = 0;
	int mapHeight = 0;
	SDL_Rect* worldOrigin;
	void handleJoystick();
	void handleGameController();
	void handleKeyboard();
	void remove(GameObject *subscriber);
	void clear();
	void handleSubscribersInput();
	void moveSubscribers();
	void renderUI();
	void renderSubscribers();
	void closeAllSubscribers();
	void closeSubscriber(GameObject *subscriber);
	void handleInput(vector<ButtonEvent> *buttonEvents) override;
	vector<GameObject*> collidableObjects;
	vector<GameObject*> subscriberPointers;
	vector<GameObject*> subscriberUIPointers;
	//map<SceneSortLayer, vector<GameObject*>*> subsciberPointerLayers;
	Music* sceneMusic;
	SceneState state;

public:
	SceneState getState();
	void setState(SceneState _state);
	vector<GameObject*> getSubscribers();
	vector<GameObject*> getCollidableSubscribers();
	void move();
	static bool useJoystick;
	static SDL_GameController* gGameController;
	static vector<string> buttons;
	Menu* pauseMenu;
	Scene();
	//Scene(int tileW, int tileH, int mapW, int mapH);
	~Scene();
	//void buildScene(int tileW, int tileH);
	void buildScene(int tileW, int tileH, int mapW, int mapH);
	bool subscribe(GameObject* subscriber);
	//bool subscribe(Collider* subscriber);
	bool isPaused();
	bool togglePause();
	void play();
	Map* map;
	//void setMusic(Music* _music);
	//Music* getMusic();
};

class Text : public GameObject
{
public:
	Text(Scene* scene);
	Text(Scene* scene, int x, int y, int w, int h, bool isPausable, TextState state, SceneSortLayer sortLayer);
	void printL(string str, TextState state);
	void printL(string str);
	void print(string str, TextState state);
	void print(string str);
	TextState getState();
	void setState(TextState state);
	void clear();
	bool init(int x, int y, int w, int h, TextState state);
	void renderTextBuffer();
	void render() override;
private:
	//Text();
	SDL_Rect cursorPosition;
	static SDL_Texture* white;
	static SDL_Texture* black;
	static SDL_Texture* white_on_black;
	static SDL_Texture* black_on_white;
	TextState state;
	vector<int> buffer;
	map<char, int> alphaNumerics;
	void fillAlphaMap(map<char, int>* map);
};

class MenuItem : public GameObject
{
public:
	MenuItem(Scene* scene);
	MenuItem(Scene* scene, string txt, string val, SDL_Rect* size, TextState state);
	MenuItem(Scene* scene, string txt, string val, int x, int y, int w, int h, TextState state);
	~MenuItem();
	void setText(string txt);
	Text* getTextObj();
	void MenuItem::setTextObj(Text* textObj);
	void setVal(string val);
	void render() override;
	MenuItem* select();
	MenuItem* deSelect();
	string getText();
	string getVal();
private:
	Text* txtObj;
	string text;
	string val;
	SDL_Rect* size; // The MenuItem instance's size->x and size->y will be ingored if added to a Menu object.  Instead its position will be relative to the Menu and its x and y coords.
	bool active;
	bool selected;
};

class Menu : public GameObject
{
public:
	Menu(Scene* scene);
	Menu(Scene* scene, bool defaultPlacement);
	Menu(Scene* scene, SDL_Rect* pos, vector< MenuItem*> items);
	Menu(Scene* scene, vector< MenuItem*> items, bool defaultPlacement);
	void setItems(vector<MenuItem*> items);
	void setSubmit(void(*submit_func)(MenuItem*));
	Menu* addItem(MenuItem* item);
	void handleInput(vector<ButtonEvent> *buttonEvents) override;
	MenuItem* last();
	vector<MenuItem*> items;
	MenuItem* next();
	void submit();
	~Menu();
private:
	void(*submit_func)(MenuItem*);
	int selectedItemIndex;
	SDL_Rect * padding;  // Used to 
	SDL_Rect* pos;
};

class Character : public GameObject
{
public:
	Character(Scene* scene, int health, int power, int speed, SDL_Rect* startingPos, string spritesheetPath);
	~Character();
	void handleInput(vector<ButtonEvent> *buttonEvents) override;
	void move() override;
	void render() override;
	void setHasControl(bool _hasControl);
	bool hasControl();
	bool attacking;
private:
	Scene* level;
	SDL_Texture* texture;
	int health;
	int power;
	int speed;
	bool control;
};

class Tile : public GameObject
{
private:
	SDL_Texture* texture;

public:
	Tile(Scene* scene, SDL_Renderer* _renderTarget, int x, int y, int w, int h, SDL_Texture* _texture);
	~Tile();
	Tile(Scene* scene);
	void render() override;
	void handleInput(vector<ButtonEvent> *buttonEvents) override;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	//void move() override;
};

namespace Config {
	class Constants
	{
	public:
		//configuration file path
		static string CONFIGPATH() { return "Assets/config.txt"; }

		// attribute keys
		static const string WINDOW_H() { return "WINDOW_HEIGHT"; }
		static const string WINDOW_W() { return "WINDOW_WIDTH"; }
		static const string FULL_SCREEN() { return "FULL_SCREEN"; }
		static const string INPUT_TYPE() { return "INPUT_TYPE"; }
		static const string DEBUG_MODE() { return "DEBUG_MODE"; }
		static const string CONFIG_CHK() { return "CONFIG_CHK"; }	//default configuration file - use if the file does not exist

			//gets all attribute keys
		static const vector<string> getAttributeKeys();

		// attribute default values - only used if configuration lacks value or has malformed value
		static const string FULL_SCREEN_DEFAULT_VAL() { return "FALSE"; }
		static const string INPUT_TYPE_DEFAULT_VAL() { return "KEYBOARD"; }
		static const string WINDOW_HEIGHT_DEFAULT_VAL() { return "600"; }
		static const string WINDOW_WIDTH_DEFAULT_VAL() { return "800"; }
		static const string DEBUG_MODE_DEFAULT_VAL() { return "TRUE"; }
		static const string CONFIG_CHK_DEFAULT_VAL() { return "TRUE"; }

		static const string DEFAULT_CONFIG_FILE_CONTENTS()
		{
			return
			{
			"#	This is the configuration attributes file for JL2DE.												 \n"
			"#																										 \n"
			"#	All lines begining with '#' will be ignored by the configuration reader.							 \n"
			"#																										 \n"
			"#	All attributes must appear one per line with the following structure:								 \n"
			"#																										 \n"
			"#		[ATTRIBUTE_KEY]=[ATTRIBUTE_VALUE]																 \n"
			"#																										 \n"
			"#	Any attributes whose key does not appear in the following list will not be							 \n"
			"#	added to the configuration attributes.																 \n"
			"#																										 \n"
			"#	CONFIG_CHK	  [TRUE | FALSE]																		 \n"
			"#		This will determine whether the attributes set in this file are used, or if the user			 \n"
			"#		is prompted to select them. TRUE will result in the user being prompted.  If prompted the		 \n"
			"#		newly selected values will overwrite those in this file.										 \n"
			"#	WINDOW_WIDTH  [numeric from (DISPLAY_MIN)-(DISPLAY_MAX)]											 \n"
			"#		If the value is not supported by your display, the nearest match that IS supported will be used. \n"
			"#	WINDOW_HEIGHT [numeric from (DISPLAY_MIN)-(DISPLAY_MAX)]											 \n"
			"#		If the value is not supported by your display, the nearest match that IS supported will be used. \n"
			"#	FULL_SCREEN   [TRUE | FALSE]			Full screen or windowed										 \n"
			"#	INPUT_TYPE    [KEYBOARD | JOYSTICK ]		Joystick or Keyboard									 \n"
			"#		JOYSTICK will only be used if one is connected.													 \n"
			"#	DISPLAY	      [numeric from 0-5]		Which display to target										 \n"
			"#\n"
			"#\n"
			"#\n"
				+ CONFIG_CHK() + "=" + CONFIG_CHK_DEFAULT_VAL() + "\n"
				+ WINDOW_H() + "=" + WINDOW_HEIGHT_DEFAULT_VAL() + "\n"
				+ WINDOW_W() + "=" + WINDOW_WIDTH_DEFAULT_VAL() + "\n"
				+ FULL_SCREEN() + "=" + FULL_SCREEN_DEFAULT_VAL() + "\n"
				+ INPUT_TYPE() + "=" + INPUT_TYPE_DEFAULT_VAL() + "\n"
			};
		}

		static string UPDATE_CONFIG_FILE_CONTENTS(string newDetails)
		{
			return
			{
			"#	This is the configuration attributes file for JL2DE.												 \n"
			"#																										 \n"
			"#	All lines begining with '#' will be ignored by the configuration reader.							 \n"
			"#																										 \n"
			"#	All attributes must appear one per line with the following structure:								 \n"
			"#																										 \n"
			"#		[ATTRIBUTE_KEY]=[ATTRIBUTE_VALUE]																 \n"
			"#																										 \n"
			"#	Any attributes whose key does not appear in the following list will not be							 \n"
			"#	added to the configuration attributes.																 \n"
			"#																										 \n"
			"#	CONFIG_CHK	  [TRUE | FALSE]																		 \n"
			"#		This will determine whether the attributes set in this file are used, or if the user			 \n"
			"#		is prompted to select them. TRUE will result in the user being prompted.  If prompted the		 \n"
			"#		newly selected values will overwrite those in this file.										 \n"
			"#	WINDOW_WIDTH  [numeric from (DISPLAY_MIN)-(DISPLAY_MAX)]											 \n"
			"#		If the value is not supported by your display, the nearest match that IS supported will be used. \n"
			"#	WINDOW_HEIGHT [numeric from (DISPLAY_MIN)-(DISPLAY_MAX)]											 \n"
			"#		If the value is not supported by your display, the nearest match that IS supported will be used. \n"
			"#	FULL_SCREEN   [TRUE | FALSE]			Full screen or windowed										 \n"
			"#	INPUT_TYPE    [KEYBOARD | JOYSTICK ]		Joystick or Keyboard									 \n"
			"#		JOYSTICK will only be used if one is connected.													 \n"
			"#	DISPLAY	      [numeric from 0-5]		Which display to target										 \n"
			"#\n"
			"#\n"
			"#\n"
			+ newDetails
			};
		}
	};

	class Detail
	{
	private:
		string _key;
		string _value;
	public:
		string getKey();
		string getValue();
		void setValue(string value);
		Detail();
		~Detail();
		Detail(string key, string value);
	};

	static class ConfigList
	{
	private:
		static vector<Detail*> _configurationAttributes;
	public:
		static string getValue(string key);
		static Detail* getAttributeByKey(string key);
		static void add(Detail* _attribute);
		static void updateValue(Detail* _attribute, string _newValue);
		static bool isEmpty();
		static int size();
		static void add(string key, string value);
		static void loadConfig();
		static void unloadConfig();
		static vector<string> split(string source, char delimiter);

	private:
		static void verify();
		static void loadDefaults();
	};
}

namespace Events {
	static class EventHandler
	{
	public:
		static bool hasQuit();
		static bool toggleQuit();
	private:
		static bool quit;

	};
}

static class Util
{
public:
	static int frameRate;
	static int countedFrames;
	static bool init();
	static bool cleanup();
	static SDL_Window* getWindow();
	static SDL_Renderer* getRenderTarget();
	static SDL_Texture* loadTexture(string imagePath);
	static void resizeWindow(int h, int w, bool fullscreen);
	static void setScreenW(int w);
	static void setScreenH(int h);
	static int getScreenW();
	static int getScreenH();
	static multimap<long, long>* getAvailableDisplayResolutions();
	static bool getAskUserForResolution();
	static void setAskUserForResolution(bool _askUserForResolution);
	static void loadDisplayResolutions();
	//static int isDebug();
	//static int* loadSpriteSheet(string path, int cell_w, int cell_h);
	static void addRectsPosition(SDL_Rect* r1, SDL_Rect* r2, SDL_Rect* result);
	static void addRectsDimensions(SDL_Rect* r1, SDL_Rect* r2, SDL_Rect* result);
	static void addRectsAll(SDL_Rect* r1, SDL_Rect* r2, SDL_Rect* result);
	static void loadSpriteSheet(int cellW, int cellH, int cellColCount, int cellRowCount, vector<SDL_Rect*>* frames);
	static bool Util::containsValue(int i, vector<int>* v);
	static void setAdjustedTileWidth(int w);
	static int getAdjustedTileWidth();
	static void setAdjustedTileHeight(int h);
	static int getAdjustedTileHeight();
private:
	static SDL_Window *gameWindow;
	static SDL_Surface *gScreenSurface, *newScreenSurface;
	static SDL_Renderer* gRenderTarget;
	static multimap<long, long> availableDisplayResolutions;
	static int screenW, screenH;
	static bool askUserForResolution;
	static bool containsSize(long w, long h, multimap<long, long>* s);
	static int adjustedTileWidth;
	static int adjustedTileHeight;
};

static class Debug
{

};
