#include "GuiButton.h"

#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneManager.h"


GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	strcpy_s(this->text, TEXT_LEN, text);
	this->offsetText = strlen(this->text) * 20;

	normal = { 0,215,300, 60 };
	focused = { 0,281,300, 60 };
	pressed = { 0,345,300, 60 };
	disabled = { 0,410,300, 60 };

	normalSmall = { 0,477,200,60 };
	focusedSmall = { 0,543,200,60 };
	pressedSmall = { 0,607,200,60 };
	disabledSmall = { 0,672,200,60 };

	disabledLeft = { 847,544,50,50 };
	normalLeft = { 544,544,50,50 };
	focusedLeft = { 645,544,50,50 };
	pressedLeft = { 746,544,50,50 };

	disabledRight = { 897,544,50,50 };
	normalRight = { 594,544,50,50 };
	focusedRight = { 695,544,50,50 };
	pressedRight = { 796,544,50,50 };

	hoverPlay = true;
	clickPlay = true;
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
bool GuiButton::Update(float dt, int minId, int maxId)
{
	this->state = GuiControlState::FOCUSED;
	cd += dt;
	if ((app->scene->currentButton->data->id >= minId) && (app->scene->currentButton->data->id <= maxId))
	{
		if (app->scene->currentButton->next != nullptr && (app->input->CheckButton("down", KEY_DOWN) || app->input->CheckButton("right", KEY_DOWN)))
		{
			if (app->scene->currentButton->next->data->id <= maxId)
			{
				app->scene->currentButton->data->state = GuiControlState::NORMAL;
				app->scene->currentButton = app->scene->currentButton->next;
				while (app->scene->currentButton->data->state == GuiControlState::DISABLED)
				{
					app->scene->currentButton = app->scene->currentButton->next;
				}
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
				while (app->scene->currentButton->data->state == GuiControlState::DISABLED)
				{
					app->scene->currentButton = app->scene->currentButton->prev;
				}
				app->scene->currentButton->data->state = GuiControlState::FOCUSED;
				app->audio->PlayFx(hover);
			}
		}
		if (cd >= 0.7f)
		{
			cd = 0.7f;
			if (app->input->CheckButton("select", KEY_DOWN))
			{
				if (app->scene->currentButton->data->state == GuiControlState::FOCUSED)
					app->audio->PlayFx(click);
				app->scene->currentButton->data->state = GuiControlState::PRESSED;
			}
			if (app->input->CheckButton("select", KEY_UP))
			{
				cd = 0.0f;
				NotifyObserver();
			}
		}
	}


	return true;
}

bool GuiButton::Draw(int cPosX, int cPosY)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (bounds.w == 200)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledSmall);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + 6 + (bounds.w - offsetText) / 2,/* cPosY + */bounds.y + (bounds.h / 8), textSize, 2, black);
		}
		else if (bounds.w == 300)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, black);
		}
		else if (bounds.w == 50)
		{
			if (bounds.x <= app->render->camera.w / 2)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledLeft);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledRight);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 100, 100, 100, 100);
		}
		break;
	}
	case GuiControlState::NORMAL:
	{
		if (bounds.w == 200)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalSmall);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + 6 + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 300)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 50)
		{
			if (bounds.x <= app->render->camera.w / 2)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalLeft);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalRight);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 255, 255, 100);
		}
		break;
	}
	case GuiControlState::FOCUSED:
	{

		if (bounds.w == 200)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedSmall);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + 6 + (bounds.w - offsetText) / 2,/* cPosY + */bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 300)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
			app->render->DrawText(guiFont, text,/* cPosX + */bounds.x + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 50)
		{
			if (bounds.x <= app->render->camera.w / 2)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedLeft);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedRight);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		if (app->render->debug)
		{
			app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 0, 0, 255, 100);
		}
		break;
	}
	case GuiControlState::PRESSED:
	{
		if (bounds.w == 200)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedSmall);
			app->render->DrawText(guiFont, text,/* cPosX +*/ bounds.x + 6 + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 300)
		{
			app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
			app->render->DrawText(guiFont, text,/* cPosX + */bounds.x + (bounds.w - offsetText) / 2,/* cPosY +*/ bounds.y + (bounds.h / 8), textSize, 2, white);
		}
		else if (bounds.w == 50)
		{
			if (bounds.x <= app->render->camera.w / 2)
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedLeft);
			}
			else
			{
				app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedRight);
			}
		}
		//app->render->DrawRectangle({ cPosX + bounds.x,cPosY + bounds.y ,bounds.w,bounds.h }, 255, 0, 0, 100);
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