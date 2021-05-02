#include "Font.h"
#include "App.h"

#include "AssetsManager.h"
#include "Textures.h"

#include "Log.h"

#include "PugiXml/src/pugixml.hpp"

Font::Font(const char* rtpFontFile)
{
	fontLoaded = false;

	char* buffer = nullptr;
	pugi::xml_document xmlDocFontAtlas;
	size_t size = app->assetsManager->LoadXML(rtpFontFile, &buffer);

	pugi::xml_parse_result result = xmlDocFontAtlas.load_buffer(buffer, size);
	RELEASE_ARRAY(buffer);

	pugi::xml_node xmlNodeAtlas;
	pugi::xml_node xmlNodeGlyph;

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", rtpFontFile, result.description());
	else xmlNodeAtlas = xmlDocFontAtlas.child("AtlasTexture");

	if (xmlNodeAtlas.empty() == false)
	{
		const pugi::char_t* path = xmlNodeAtlas.attribute("imagePath").as_string();
		//int atlasWidth = xmlNodeAtlas.attribute("width").as_int();
		//int atlasHeight = xmlNodeAtlas.attribute("height").as_int();

		texture = app->tex->Load(PATH("Fonts/", path));

		charsCount = xmlNodeAtlas.attribute("spriteCount").as_int();
		baseSize = xmlNodeAtlas.attribute("fontSize").as_int();

		xmlNodeGlyph = xmlNodeAtlas.child("Sprite");

		for (int index = 0; xmlNodeGlyph; xmlNodeGlyph = xmlNodeGlyph.next_sibling("Sprite"))
		{
			index = xmlNodeGlyph.attribute("charValue").as_int();
			charsRecs[index].x = xmlNodeGlyph.attribute("positionX").as_int();
			charsRecs[index].y = xmlNodeGlyph.attribute("positionY").as_int();
			charsRecs[index].w = xmlNodeGlyph.attribute("sourceSizeWidth").as_int();
			charsRecs[index].h = baseSize;
		}

		fontLoaded = true;
		xmlDocFontAtlas.reset();
	}
}

Font::~Font()
{
	app->tex->UnLoad(texture);
	fontLoaded = false;
}

SDL_Texture* Font::GetTextureAtlas()
{
	return texture;
}

SDL_Rect Font::GetCharRec(int value)
{
	SDL_Rect rec = { 0 };

	// Get character rectangle corresponding to introduced value
	// NOTE: In our current implementation rectangles are ordered following
	// the character codepoint value but improved implementation would require
	// a for() loop to look for the corresponding value on a hashmap table
	rec = charsRecs[value];

	return rec;
}
