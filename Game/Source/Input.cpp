#include "App.h"
#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define MAX_KEYS 300

Input::Input() : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	gamePad = new KeyState[NUM_PAD_BUTTONS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	memset(gamePad, 0, sizeof(KeyState) * NUM_PAD_BUTTONS);
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
	delete[] mouseButtons;
	delete[] gamePad;
}

// Called before render is available
bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0)
	{
		LOG("SDL_INIT_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//CONTROLS CONFIG
	for (pugi::xml_node a = config.child("select"); a; a = a.next_sibling())
	{
		InputButton* c = new InputButton;
		c->keyId = a.attribute("keyId").as_int(-1);
		c->gamePadId = a.attribute("padId").as_int(-1);
		c->name.Create(a.child_value());
		controlConfig.Add(c);
	}

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	// KEYBOARD
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	// MOUSE
	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if(mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	// WINDOW
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			windowEvents[WE_QUIT] = true;
			break;
		}
		case SDL_WINDOWEVENT:
		{
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			mouseButtons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			int scale = app->win->GetScale();
			mouseMotionX = event.motion.xrel / scale;
			mouseMotionY = event.motion.yrel / scale;
			mouseX = event.motion.x / scale;
			mouseY = event.motion.y / scale;
			LOG("Mouse motion x %d y %d", mouseMotionX, mouseMotionY);
			break;
		}
		case(SDL_CONTROLLERDEVICEADDED):
		{
			HandleDeviceConnection(event.cdevice.which);
			break;
		}
		case(SDL_CONTROLLERDEVICEREMOVED):
		{
			HandleDeviceRemoval(event.cdevice.which);
			break;
		}
		}
	}

	// GAMEPAD
	const bool* padKeys = UpdateGamepadsInput();

	for (int i = 0; i < NUM_PAD_BUTTONS; ++i)
	{
		if (padKeys[i] == 1)
		{
			if (gamePad[i] == KEY_IDLE)
				gamePad[i] = KEY_DOWN;
			else
				gamePad[i] = KEY_REPEAT;
		}
		else
		{
			if (gamePad[i] == KEY_REPEAT || gamePad[i] == KEY_DOWN)
				gamePad[i] = KEY_UP;
			else
				gamePad[i] = KEY_IDLE;
		}
	}

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	if (pad.haptic != nullptr)
	{
		SDL_HapticStopAll(pad.haptic);
		SDL_HapticClose(pad.haptic);
	}
	if (pad.controller != nullptr) SDL_GameControllerClose(pad.controller);

	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

void Input::HandleDeviceConnection(int index)
{
	if (SDL_IsGameController(index))
	{
		if (pad.enabled == false)
		{
			if (pad.controller = SDL_GameControllerOpen(index))
			{
				LOG("Found a gamepad named %s", SDL_GameControllerName(pad.controller));
				pad.enabled = true;
				pad.l_dz = pad.r_dz = pad.l2_dz = pad.r2_dz = 0.1f;
				pad.haptic = SDL_HapticOpen(index);
				if (pad.haptic != nullptr) LOG("... gamepad has force feedback capabilities");
				pad.index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad.controller));
			}
		}
	}
}

void Input::HandleDeviceRemoval(int index)
{
	// If an existing gamepad has the given index, deactivate all SDL device functionallity
	if (pad.enabled && pad.index == index)
	{
		SDL_HapticClose(pad.haptic);
		SDL_GameControllerClose(pad.controller);
		memset(&pad, 0, sizeof(GamePad));
	}
}

bool* Input::UpdateGamepadsInput()
{
	bool gamePadTemp[NUM_PAD_BUTTONS] = {};
	float l2, r2, l_x, l_y, r_x, r_y;
	if (pad.enabled == true)
	{
		gamePadTemp[0] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_A) == 1;
		gamePadTemp[1] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_B) == 1;
		gamePadTemp[2] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_X) == 1;
		gamePadTemp[3] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_Y) == 1;
		gamePadTemp[4] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_BACK) == 1;
		gamePadTemp[5] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_GUIDE) == 1;
		gamePadTemp[6] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_START) == 1;
		gamePadTemp[7] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1;
		gamePadTemp[8] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1;
		gamePadTemp[9] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1;
		gamePadTemp[10] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1;
		gamePadTemp[11] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1;
		gamePadTemp[12] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1;
		gamePadTemp[13] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1;
		gamePadTemp[14] = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1;

		l2 = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
		r2 = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;
		l_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
		l_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
		r_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
		r_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

		// Apply deadzone. All values below the deadzone will be discarded
		gamePadTemp[15] = (fabsf(l2) > pad.l2_dz) ? true : false;
		gamePadTemp[16] = (fabsf(r2) > pad.r2_dz) ? true : false;
		gamePadTemp[17] = (l_x > pad.l_dz) ? true : false;
		gamePadTemp[18] = (l_x < -pad.l_dz) ? true : false;
		gamePadTemp[19] = (l_y > pad.l_dz) ? true : false;
		gamePadTemp[20] = (l_y < -pad.l_dz) ? true : false;
		gamePadTemp[21] = (r_x > pad.r_dz) ? true : false;
		gamePadTemp[22] = (r_x < -pad.r_dz) ? true : false;
		gamePadTemp[23] = (r_y > pad.r_dz) ? true : false;
		gamePadTemp[24] = (r_y < -pad.r_dz) ? true : false;

		if (pad.rumble_countdown > 0) pad.rumble_countdown--;
	}

	return gamePadTemp;
}

bool Input::ShakeController(int id, float dt, int duration, float strength)
{
	// Check if the gamepad is active and allows rumble
	if (!pad.enabled || pad.haptic == nullptr || SDL_HapticRumbleSupported(pad.haptic) != SDL_TRUE) return false;

	// If the pad is already in rumble state and the new strength is below the current value, ignore this call
	if (duration < pad.rumble_countdown && strength < pad.rumble_strength) return false;

	if (SDL_HapticRumbleInit(pad.haptic) == -1)
	{
		LOG("Cannot init rumble for controller number %d", id);
	}
	else
	{
		SDL_HapticRumbleStop(pad.haptic);
		SDL_HapticRumblePlay(pad.haptic, strength, duration / dt); //Conversion from frames to ms

		pad.rumble_countdown = duration;
		pad.rumble_strength = strength;

		return true;
	}
}

const char* Input::GetControllerName() const
{
	// Check if the given id has a valid controller
	if (pad.enabled == true && pad.controller != nullptr) return SDL_GameControllerName(pad.controller);
	return "unplugged";
}