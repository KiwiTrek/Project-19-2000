#include "Render.h"

#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Map.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

Render::~Render()
{}

bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		app->vsync = true;
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}

	return true;
}

bool Render::Start()
{
	LOG("Render start");

	SDL_RenderGetViewport(renderer, &viewport);
	debug = false;

	camera.x = 0;
	camera.y = 0;

	return true;
}

bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	bool ret = true;

	std::list<SplineInfo*>::iterator item = splines.begin();

	for (; item != splines.end(); ++item) {

		if (*item != nullptr) {

			if (!(*item)->Update(dt)) {

				delete(*item);
				(*item) = nullptr;

			}

		}

	}

	splines.remove(nullptr);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	return ret;
}

bool Render::PostUpdate()
{
	if (app->scene->current->currentScene == SceneType::GAMEPLAY)
	{
		app->scene->current->DrawPauseMenu();
	}

	// Draw full screen rectangle in front of everything
	if (app->scene->onTransition)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, (uchar)(255.0f * app->scene->transitionAlpha));
	}

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

bool Render::CleanUp()
{
	LOG("Destroying SDL render");

	std::list<SplineInfo*>::iterator item = splines.begin();

	for (; item != splines.end(); ++item) {

		if (*item != nullptr) {

			delete(*item);
			(*item) = nullptr;

		}

	}

	splines.clear();

	SDL_DestroyRenderer(renderer);

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

bool Render::DrawTexture(SDL_Texture* texture, int x, int y, bool fullscreen, const SDL_Rect* section, bool invert, float speed, double angle, int pivotX, int pivotY) const
{
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (fullscreen == true)
	{
		uint w, h;
		app->win->GetWindowSize(w, h);
		rect.w = (int)w;
		rect.h = (int)h;
	}
	else
	{
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (invert)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool useCamera) const
{
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (useCamera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera) const
{
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	if (useCamera)
	{
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	}
	else
	{
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);
	}

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera) const
{
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];
	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Render::ToggleVsync(bool vsync, Module* restart)
{
	app->vsync = vsync;

	//SDL_DestroyRenderer(renderer);
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (vsync)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	//renderer = SDL_CreateRenderer(app->win->window, -1, flags);
	//if (renderer == NULL)
	//{
	//	LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
	//}
	//else
	//{
	//	camera.w = app->win->screenSurface->w;
	//	camera.h = app->win->screenSurface->h;
	//	camera.x = 0;
	//	camera.y = 0;
	//}
}

bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint)
{
	bool ret = true;

	int length = strlen(text);
	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	for (int i = 0; i < length; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		posX += ((float)recGlyph.w * scale + spacing);
	}

	return ret;
}

bool Render::DrawTextAnimated(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint, int timeStep)
{
	bool ret = true;

	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	for (int i = 0; i < timeStep; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		posX += ((float)recGlyph.w * scale + spacing);
	}

	return ret;
}

void Render::CreateSpline(int* position, const int& finaPos, const float& time, const SplineType& type)
{

	SplineInfo* info = new SplineInfo(position, finaPos, time, type);

	if (info != nullptr)
		splines.push_back(info);
	else
		LOG("Error when creating the spline");
}