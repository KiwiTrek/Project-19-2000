#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "List.h"
#include "SString.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define NUM_PAD_BUTTONS 25

struct SDL_Rect;
struct _SDL_GameController;
struct _SDL_Haptic;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum ControllerButton
{
	CONTROLLER_BUTTON_A,
	CONTROLLER_BUTTON_B,
	CONTROLLER_BUTTON_X,
	CONTROLLER_BUTTON_Y,
	CONTROLLER_BUTTON_BACK,
	CONTROLLER_BUTTON_GUIDE,
	CONTROLLER_BUTTON_START,
	CONTROLLER_BUTTON_LEFTSTICK,
	CONTROLLER_BUTTON_RIGHTSTICK,
	CONTROLLER_BUTTON_LEFTSHOULDER,
	CONTROLLER_BUTTON_RIGHTSHOULDER,
	CONTROLLER_BUTTON_DPAD_UP,
	CONTROLLER_BUTTON_DPAD_DOWN,
	CONTROLLER_BUTTON_DPAD_LEFT,
	CONTROLLER_BUTTON_DPAD_RIGHT,
	CONTROLLER_AXIS_TRIGGERLEFT,
	CONTROLLER_AXIS_TRIGGERRIGHT,
	CONTROLLER_AXIS_LEFTX_P,
	CONTROLLER_AXIS_LEFTX_N,
	CONTROLLER_AXIS_LEFTY_P,
	CONTROLLER_AXIS_LEFTY_N,
	CONTROLLER_AXIS_RIGHTX_P,
	CONTROLLER_AXIS_RIGHTX_N,
	CONTROLLER_AXIS_RIGHTY_P,
	CONTROLLER_AXIS_RIGHTY_N
};

struct GamePad
{
	//Input data
	//bool start, back, guide;
	//bool x, y, a, b, l1, r1, l3, r3;
	//bool up, down, left, right;
	//bool l2, r2;
	//bool l_x, l_y, r_x, r_y;
	float l2_dz, r2_dz, l_dz, r_dz;

	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller;
	_SDL_Haptic* haptic;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;
};

struct InputButton
{
	SString name;
	int keyId;
	int gamePadId;
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetPadKey(int id) const
	{
		return gamePad[id];
	}

	bool CheckButton(const char* c, KeyState state) const
	{
		int padId, keyId;
		ListItem<InputButton*>* b = controlConfig.start;
		while (b != nullptr)
		{
			if (b->data->name == c)
			{
				break;
			}
			b = b->next;
		}

		if (b == nullptr)
		{
			return false;
		}

		if (GetKey(b->data->keyId) == state || GetPadKey(b->data->gamePadId) == state)
		{
			return true;
		}
		return false;
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	// Activates SDL device funcionallity when a gamepad has been connected
	void HandleDeviceConnection(int index = 0);

	// Deactivates SDL device funcionallity when a gamepad has been disconnected
	void HandleDeviceRemoval(int index = 0);

	// Called at PreUpdate
	// Iterates through all active gamepads and update all input data
	bool* UpdateGamepadsInput();

	bool ShakeController(int id, float dt, int duration, float strength = 0.5f);
	const char* GetControllerName() const;

public:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	GamePad pad;
	KeyState* gamePad;
	List<InputButton*> controlConfig;
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};

#endif // __INPUT_H__