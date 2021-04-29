#include "GuiCheckBox.h"

#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneManager.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->offsetText = this->text.Length() * 24 + (24 * 3);

	normal = { 0,144,60, 60 };
	focused = { 61,144,60, 60 };
	pressed = { 123,144,60, 60 };
	disabled = { 185,144,60, 60 };
	check = { 245,144,60, 60 };

	checked = false;
}

GuiCheckBox::~GuiCheckBox()
{}

bool GuiCheckBox::Update(float dt)
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
				checked = !checked;
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

bool GuiCheckBox::Update(float dt, int minId, int maxId)
{
	if ((app->scene->currentButton->data->id >= minId) && (app->scene->currentButton->data->id <= maxId))
	{
		if (app->scene->currentButton->next != nullptr && app->input->CheckButton("down", KEY_DOWN))
		{
			if (app->scene->currentButton->next->data->id <= maxId)
			{
				app->scene->currentButton->data->state = GuiControlState::NORMAL;
				app->scene->currentButton = app->scene->currentButton->next;
				app->scene->currentButton->data->state = GuiControlState::FOCUSED;
				app->audio->PlayFx(hover);
			}
		}
		else if (app->scene->currentButton->prev != nullptr && app->input->CheckButton("up", KEY_DOWN))
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
			checked = !checked;
			NotifyObserver();
		}
	}
	return true;
}

bool GuiCheckBox::Draw(int cPosX, int cPosY)
{
	// Draw the right button depending on state
	app->render->DrawText(guiFont, text.GetString(),/* cPosX +*/ bounds.x, /*cPosY + */bounds.y + (bounds.h / 8) - bounds.h, textSize, 2, white);

	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
		if (checked)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &check);
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		}
		break;
	}
	case GuiControlState::NORMAL:
	{
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
		if(checked)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &check);
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		}
		break;
	}
	case GuiControlState::FOCUSED:
	{
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
		if(checked)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &check);
		}
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		}
		break;
	}
	case GuiControlState::PRESSED:
	{
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 255, 0, 0, 100);
		app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
		if(checked)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &check);
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