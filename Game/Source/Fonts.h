#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "SDL_pixels.h"
#include <string.h>

#define MAX_FONTS 15
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
public:
	// Lookup table
	// All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Length of the lookup table
	uint totalLength;

	// Number of rows per table
	uint rows;

	// Amount of chars per row of the texture
	uint columns;

	// Width of each character
	uint charW;

	// Height of each character
	uint charH;
};

class Fonts : public Module
{
public:
	// Constructor
	Fonts();

	// Destructor
	~Fonts();

	// Called when program is executed
	void Init();

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void Unload(int fontIndex);

	// Create a surface from text
	void DrawText(int x, int y, int fontIndex, const char* text) const;

private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];
};

#endif // !__FONTS_H__
