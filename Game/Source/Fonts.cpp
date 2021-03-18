#include "Fonts.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#define _CRT_SECURE_NO_WARNINGS

Fonts::Fonts()
{
	name.Create("fonts");
}

Fonts::~Fonts()
{}

void Fonts::Init()
{
	active = true;
}

int Fonts::Load(const char* texturePath, const char* characters, uint rows)
{
	int id = -1;

	if (texturePath == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font\n");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texturePath);
	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texturePath, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
	{
		if (fonts[id].texture == nullptr)
		{
			break;
		}
	}

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texturePath, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	font.totalLength = strlen(characters);
	strcpy_s(font.table, characters);
	font.columns = (font.totalLength / rows);
	app->tex->GetSize(font.texture, font.charW, font.charH);
	font.charW = font.charW / font.columns;
	font.charH = font.charH / font.rows;

	LOG("Successfully loaded BMP font from %s", texturePath);

	return id;
}

void Fonts::Unload(int fontId)
{
	if (fontId >= 0 && fontId < MAX_FONTS && fonts[fontId].texture != nullptr)
	{
		app->tex->UnLoad(fonts[fontId].texture);
		fonts[fontId].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", fontId);
	}
}

void Fonts::DrawText(int x, int y, int fontId, const char* text) const
{
	if (text == nullptr || fontId < 0 || fontId >= MAX_FONTS || fonts[fontId].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", fontId);
		return;
	}

	const Font* font = &fonts[fontId];
	SDL_Rect spriteRect;
	uint len = strlen(text);
	spriteRect.w = font->charW;
	spriteRect.h = font->charH;

	for (uint i = 0; i < len; ++i)
	{
		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				// Retrieve the position of the current character in the sprite
				spriteRect.x = (j % font->columns) * spriteRect.w;
				spriteRect.y = (j / font->columns) * spriteRect.h;
				// Blit the character at its proper position
				app->render->DrawTexture(font->texture, x, y, false, &spriteRect);
				break;
			}
		}
		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}