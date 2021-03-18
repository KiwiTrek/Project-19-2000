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

	normal = { 0,0,216, 109 };
	focused = { 0,109,216, 109 };
	pressed = { 0,218,216, 109 };
	disabled = { 0,327,216, 109 };
}

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, const char* secondText) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->secondaryText = true;
	this->secondText = secondText;
	this->offsetText = this->text.Length() * 20;
	this->offsetTextSecond = this->secondText.Length() * 20;

	normal = { 0,0,216, 109 };
	focused = { 0,109,216, 109 };
	pressed = { 0,218,216, 109 };
	disabled = { 0,327,216, 109 };
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
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
		if (secondaryText)
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 11, disabledFont, text.GetString());
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 11, disabledFont, secondText.GetString());
		}
		else
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), disabledFont, text.GetString());
		}
		if (app->render->drawAllGui)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		}
		break;
	}
	case GuiControlState::NORMAL:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
		if (secondaryText)
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, defaultFont, text.GetString());
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, defaultFont, secondText.GetString());
		}
		else
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), defaultFont, text.GetString());
		}
		if (app->render->drawAllGui)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		}
		break;
	}
	case GuiControlState::FOCUSED:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
		if (secondaryText)
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, hoverFont, text.GetString());
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, hoverFont, secondText.GetString());
		}
		else
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), hoverFont, text.GetString());
		}
		if (app->render->drawAllGui)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		}
		break;
	}
	case GuiControlState::PRESSED:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
		if (secondaryText)
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2) - 12, pressedFont, text.GetString());
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetTextSecond) / 2, cPosY + bounds.y + (bounds.h / 2) + 12, pressedFont, secondText.GetString());
		}
		else
		{
			app->fonts->DrawText(cPosX + bounds.x + (bounds.w - offsetText) / 2, cPosY + bounds.y + (bounds.h / 2), pressedFont, text.GetString());
		}
		if (app->render->drawAllGui)
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