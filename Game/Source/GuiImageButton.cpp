#include "GuiImageButton.h"
#include "GuiButton.h"
#include "GuiControl.h"
#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneManager.h"


GuiImageButton::GuiImageButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex) : GuiButton(id, bounds, text), tex(tex)
{
	this->offsetText = this->text.Length() * 20;

	normal = { 0,215,300, 60 };
	focused = { 0,281,300, 60 };
	pressed = { 0,345,300, 60 };
	disabled = { 0,410,300, 60 };


	normalSmall = { 0,477,200,60 };
	focusedSmall = { 0,543,200,60 };
	pressedSmall = { 0,607,200,60 };
	disabledSmall = { 0,672,200,60 };

	hoverPlay = true;
	clickPlay = true;

	itemId = ItemId::NONE;
	count.Clear();
	sec = { 288,416,32,32 };
}

GuiImageButton::~GuiImageButton()
{}

bool GuiImageButton::Update(float dt)
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
bool GuiImageButton::Update(float dt, int minId, int maxId)
{
	this->state = GuiControlState::FOCUSED;

	if ((app->scene->currentButton->data->id >= minId) && (app->scene->currentButton->data->id <= maxId))
	{
		if (app->scene->currentButton->next != nullptr && (app->input->CheckButton("down", KEY_DOWN) || app->input->CheckButton("right", KEY_DOWN)))
		{
			if (app->scene->currentButton->next->data->id <= maxId)
			{
				app->scene->currentButton->data->state = GuiControlState::NORMAL;
				app->scene->currentButton = app->scene->currentButton->next;
				app->scene->currentButton->data->state = GuiControlState::FOCUSED;
				app->audio->PlayFx(hover);
			}
		}
		else if (app->scene->currentButton->prev != nullptr && (app->input->CheckButton("up", KEY_DOWN) || app->input->CheckButton("left", KEY_DOWN)))
		{
			if (app->scene->currentButton->prev->data->id >= minId)
			{
				app->scene->currentButton->data->state = GuiControlState::NORMAL;
				app->scene->currentButton = app->scene->currentButton->prev;
				app->scene->currentButton->data->state = GuiControlState::FOCUSED;
				app->audio->PlayFx(hover);
			}
		}
		if (app->input->CheckButton("select", KEY_REPEAT))
		{
			if (app->scene->currentButton->data->state == GuiControlState::FOCUSED)
				app->audio->PlayFx(click);
			app->scene->currentButton->data->state = GuiControlState::PRESSED;
		}
		if (app->input->CheckButton("select", KEY_UP))
		{
			NotifyObserver();
		}
	}


	return true;
}

bool GuiImageButton::Draw(int cPosX, int cPosY)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
		if (text != nullptr)
		{
			app->render->DrawText(guiFont, text.GetString(),/* cPosX +*/ bounds.x + (textSize / 2) + (bounds.w - (text.Length() - 1) * textSize / 2) / 2,/* cPosY +*/ bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 0,0,0,255 });
		}
		if (count != nullptr)
		{
			app->render->DrawText(guiFont, count.GetString(), bounds.x + (bounds.w - count.Length() * 20), bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 0,0,0,255 });
		}
		app->render->DrawTexture(tex, cPosX + bounds.x + 12, cPosY + bounds.y + (bounds.h / 2) - sec.h / 2, false, &sec);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		}
		break;
	}
	case GuiControlState::NORMAL:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
		if (text != nullptr)
		{
			app->render->DrawText(guiFont, text.GetString(),/* cPosX +*/ bounds.x + (textSize / 2) + (bounds.w - text.Length() * textSize / 2) / 2,/* cPosY +*/ bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		if (count != nullptr)
		{
			app->render->DrawText(guiFont, count.GetString(), bounds.x - 4 + (bounds.w - count.Length() * 20), bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		app->render->DrawTexture(tex, cPosX + bounds.x + 16, cPosY + bounds.y + (bounds.h / 2) - sec.h / 2, false, &sec);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		}
		break;
	}
	case GuiControlState::FOCUSED:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
		if (text != nullptr)
		{
			app->render->DrawText(guiFont, text.GetString(),/* cPosX +*/ bounds.x + (textSize / 2) + (bounds.w - text.Length() * textSize / 2) / 2,/* cPosY +*/ bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		if (count != nullptr)
		{
			app->render->DrawText(guiFont, count.GetString(), bounds.x + (bounds.w - count.Length() * 20), bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		app->render->DrawTexture(tex, cPosX + bounds.x + 12, cPosY + bounds.y + (bounds.h / 2) - sec.h / 2, false, &sec);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		}
		break;
	}
	case GuiControlState::PRESSED:
	{
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
		if (text != nullptr)
		{
		app->render->DrawText(guiFont, text.GetString(),/* cPosX +*/ bounds.x + (textSize / 2) + (bounds.w - text.Length() * textSize / 2) / 2,/* cPosY +*/ bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		if (count != nullptr)
		{
			app->render->DrawText(guiFont, count.GetString(), bounds.x + (bounds.w - count.Length() * 20), bounds.y + (bounds.h / 2) - textSize / 2, textSize, 2, { 255,255,255,255 });
		}
		app->render->DrawTexture(tex, cPosX + bounds.x + 12, cPosY + bounds.y + (bounds.h / 2) - sec.h / 2, false, &sec);
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