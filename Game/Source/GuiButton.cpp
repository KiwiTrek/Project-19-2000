#include "GuiButton.h"

#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Fonts.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->secondaryText = false;
	this->secondText = nullptr;
	this->offsetText = this->text.Length() * 24;
	this->offsetTextSecond = 0;

	normal = { 0,215,300, 60 };
	focused = { 0,281,300, 60 };
	pressed = { 0,345,300, 60 };
	disabled = { 0,410,300, 60 };


	normalSmall = { 0,477,200,60 };
	focusedSmall = { 0,543,200,60 };
	pressedSmall = { 0,607,200,60 };
	disabledSmall = { 0,672,200,60 };

}

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, const char* secondText) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->secondaryText = true;
	this->secondText = secondText;
	this->offsetText = this->text.Length() * 20;
	this->offsetTextSecond = this->secondText.Length() * 20;

	normal = { 0,190,300, 60 };
	focused = { 0,281,300, 60 };
	pressed = { 0,345,300, 60 };
	disabled = { 0,410,300, 60 };

	normalSmall = { 0,477,200,60 };
	focusedSmall = { 0,543,200,60 };
	pressedSmall = { 0,607,200,60 };
	disabledSmall = { 0,672,200,60 };

}

GuiButton::~GuiButton()
{}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (state == GuiControlState::NORMAL)
			{
				app->audio->PlayFx(hover);
			}

			if (state != GuiControlState::PRESSED)
			{
				state = GuiControlState::FOCUSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				if (state == GuiControlState::FOCUSED)
				{
					app->audio->PlayFx(click);
				}
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiButton::Draw(int cPosX, int cPosY)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (observer->currentScene == SceneType::TITLE_SCREEN)
		{
			if ((id > 4 && id < 11) || id == 18)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
			}
		}
		if (observer->currentScene == SceneType::GAMEPLAY)
		{

			if ((id < 14) || id == 21)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		if (secondaryText)
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 11, disabledFont, text.GetString());
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 11, disabledFont, secondText.GetString());
		}
		else
		{
			app->render->DrawText(guiFont, text.GetString(), cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), 64, 2, { 0,255,255,255 });
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		}
		break;
	}
	case GuiControlState::NORMAL:
	{

		if (observer->currentScene == SceneType::TITLE_SCREEN)
		{
			if ((id > 4 && id < 11) || id == 18)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
			}
		}
		if (observer->currentScene == SceneType::GAMEPLAY)
		{

			if ((id < 14) || id == 21)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		if (secondaryText)
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, defaultFont, text.GetString());
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, defaultFont, secondText.GetString());
		}
		else
		{
			app->render->DrawText(guiFont, text.GetString(), cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), 64, 2, { 0,255,255,255 });
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		}
		break;
	}
	case GuiControlState::FOCUSED:
	{

		if (observer->currentScene == SceneType::TITLE_SCREEN)
		{
			if ((id > 4 && id < 11) || id == 18)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
			}
		}
		if (observer->currentScene == SceneType::GAMEPLAY)
		{

			if ((id < 14) || id == 21)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		if (secondaryText)
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, hoverFont, text.GetString());
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, hoverFont, secondText.GetString());
		}
		else
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), hoverFont, text.GetString());
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		}
		break;
	}
	case GuiControlState::PRESSED:
	{

		if (observer->currentScene == SceneType::TITLE_SCREEN)
		{
			if ((id > 4 && id < 11) || id == 18)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
			}
		}
		if (observer->currentScene == SceneType::GAMEPLAY)
		{

			if ((id < 14) || id == 21)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedSmall);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
			}
		}
		
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 255, 0, 0, 100);
		if (secondaryText)
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, pressedFont, text.GetString());
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, pressedFont, secondText.GetString());
		}
		else
		{
			//app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), pressedFont, text.GetString());
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 255, 0, 0, 100);
		}
		break;
	}
	default:
	{
		break;
	}
	}

	return false;
}