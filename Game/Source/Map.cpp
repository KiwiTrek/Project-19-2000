#include "App.h"

#include "Audio.h"
#include "EntityManager.h"
#include "NPC.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "Render.h"
#include "AssetsManager.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Map::Map() : Module(), mapLoaded(false)
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name, TEXT_LEN, "map");
}

Map::~Map()
{}

void Map::Init()
{
	active = false;
}

bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");

	strcpy_s(folder,TEXT_LEN, config.child("folder").child_value());

	return true;
}


bool Map::Start() 
{
	mapFile.reset();
	data.backgroundColor = { 0, 0, 0, 0 };
	data.height = 0;
	data.mapLayer.Clear();
	memset(data.name, 0, TEXT_LEN);
	data.tileHeight = 0;
	data.tileSets.Clear();
	data.tileWidth = 0;
	data.type = MapTypes::MAPTYPE_UNKNOWN;
	data.width = 0;
	mapLoaded = false;
	
	return true;
}

void Map::Draw()
{
	if (mapLoaded == false)
	{
		return;
	}
	ListItem<MapLayer*>* L;
	L = data.mapLayer.start;
	TileSet* T;
	// Iterate for all layers
	while (L != NULL)
	{
		if (L->data->properties.GetProperty("NoDraw") == 0 || app->render->debug)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = L->data->Get(x, y);
					if (tileId > 0)
					{
						T = GetTileSetFromTileId(tileId);
						SDL_Rect n = T->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(T->texture, pos.x, pos.y, false, &n);
					}
				}
			}
		}
		L = L->next; // Next layer
	}
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret(0, 0);

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	ret.x = x / data.tileWidth;
	ret.y = y / data.tileHeight;
	
	return ret;
}


SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgId;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

bool Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tileSets.start;

	while (item != NULL)
	{
		if (item->data->texture != nullptr) app->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tileSets.Clear();

	// Remove all layers
	ListItem<MapLayer*>* layerList;
	layerList = data.mapLayer.start;

	while (layerList != NULL)
	{
		RELEASE(layerList->data);
		layerList = layerList->next;
	}
	data.mapLayer.Clear();

	memset(data.name, 0, TEXT_LEN);
	data.width = 0;
	data.height = 0;
	data.tileWidth = 0;
	data.tileHeight = 0;
	data.backgroundColor = {0,0,0,0};
	data.type = MAPTYPE_UNKNOWN;
	mapFile.reset();

	return true;
}

bool Map::LoadNewMap(const char* filename)
{
	bool ret = true;

	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", folder, filename);

	char* buffer = nullptr;
	size_t size = app->assetsManager->LoadXML(tmp, &buffer);

	pugi::xml_parse_result result = mapFile.load_buffer(buffer, size);
	RELEASE_ARRAY(buffer);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		// Load map
		ret = LoadMap(filename);

		// Load tileset
		for (pugi::xml_node tileSet = mapFile.child("map").child("tileset"); tileSet && ret; tileSet = tileSet.next_sibling("tileset"))
		{
			TileSet* set = new TileSet();
			if (ret == true)
			{
				ret = LoadTileSetDetails(tileSet, set);
			}
			if (ret == true)
			{
				ret = LoadTileSetImage(tileSet, set);
			}
			if (ret == true)
			{
				ret = LoadTileSetProperties(tileSet, set);
			}
			LOG("Stop1");
			data.tileSets.Add(set);
			LOG("Stop2");
		}

		// Load layers
		for (pugi::xml_node layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* set = new MapLayer();
			LOG("Stop3");
			if (ret == true)
			{
				ret = LoadLayer(layer, set);
			}
			LOG("Stop4");
			data.mapLayer.Add(set);
		}
		LogInfo();
	}
	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap(const char* filename)
{
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		return false;
	}
	else
	{
		LOG("Filling map info");
		char strType[TEXT_LEN] = { 0 };
		strcpy_s(strType, TEXT_LEN, map.attribute("orientation").as_string());
		strcpy_s(data.name, filename);
		data.type = StrToMapType(strType);
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		char color[TEXT_LEN] = { 0 };
		strcpy_s(color, TEXT_LEN, map.attribute("backgroundcolor").as_string());
		//color.Trim();
		//sscanf_s(color.GetString(), "%02x%02x%02x", (uint)&data.backgroundColor.r, (uint)&data.backgroundColor.g, (uint)&data.backgroundColor.b);
		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 255;
	}
	return true;
}

bool Map::LoadTileSetDetails(pugi::xml_node& node, TileSet* set)
{
	LOG("Filling TileSetDetails");

	set->firstgId = node.attribute("firstgid").as_int();
	strcpy_s(set->name,TEXT_LEN, node.attribute("name").as_string());
	set->tileWidth = node.attribute("tilewidth").as_int();
	set->tileHeight = node.attribute("tileheight").as_int();
	set->spacing = node.attribute("spacing").as_int();
	set->margin = node.attribute("margin").as_int();

	return true;
}

bool Map::LoadTileSetImage(pugi::xml_node& node, TileSet* set)
{
	pugi::xml_node image = node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		return false;
	}
	else
	{
		LOG("Filling TileSetDetails");
		char tmp[TEXT_LEN] = { 0 };
		sprintf_s(tmp, TEXT_LEN, "%s%s", folder, image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp);
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return true;
}

bool Map::LoadTileSetProperties(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;
	for (pugi::xml_node tileNode = node.child("tile"); tileNode && ret; tileNode = tileNode.next_sibling("tile"))
	{
		Tile* tileProperties = new Tile;
		tileProperties->id = tileNode.attribute("id").as_int();
		ret = LoadProperties(tileNode.child("properties"), tileProperties->properties);
		set->tileSetPropList.Add(tileProperties);
	}
	return ret;
}

bool Map::StoreId(pugi::xml_node& node, MapLayer* layer, int index)
{
	layer->data[index] = node.attribute("gid").as_uint(0);

	return true;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	strcpy_s(layer->name,TEXT_LEN, node.attribute("name").as_string());
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->data = new uint[(data.width * data.height * sizeof(uint))];
	memset(layer->data, 0, size_t(data.width * data.height * sizeof(uint)));
	pugi::xml_node gidNode;

	int i = 0;
	for (gidNode = node.child("data").child("tile"); gidNode && ret; gidNode = gidNode.next_sibling("tile"))
	{
		if (ret == true)
		{
			ret = StoreId(gidNode, layer, i);
		}
		++i;
	}

	LOG("Layer [%s] has loaded %d tiles", layer->name, i);

	ret = LoadProperties(node.child("properties"), layer->properties);

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	pugi::xml_node property = node.child("property");
	if (property != NULL)
	{
		for (property; property != NULL; property = property.next_sibling("property"))
		{
			LOG("Stop5");
			Properties::Property* prop = new Properties::Property();
			LOG("Stop5.1");
			strcpy_s(prop->name,TEXT_LEN, property.attribute("name").as_string());
			LOG("Stop5.2");
			prop->value = property.attribute("value").as_int();
			LOG("Stop5.3");

			properties.list.Add(prop);
			LOG("Stop6");
		}

	}
	LOG("Stop7");
	return true;
}

//bool Map::CreateWalkabilityMap(int* width, int* height, uchar** buffer) const
//{
//	ListItem<MapLayer*>* item;
//	item = data.mapLayer.start;
//
//	for (item = data.mapLayer.start; item != NULL; item = item->next)
//	{
//		MapLayer* layer = item->data;
//
//		if (layer->properties.GetProperty("Navigation", 0) == 0)
//		{
//			continue;
//		}
//		uchar* map = new uchar[layer->width * layer->height];
//		memset(map, 1, layer->width * layer->height);
//
//		for (int y = 0; y < data.height; ++y)
//		{
//			for (int x = 0; x < data.width; ++x)
//			{
//				int i = (y * layer->width) + x;
//
//				int tileId = layer->Get(x, y);
//				TileSet* tileset = (tileId > 0) ? GetTileSetFromTileId(tileId) : NULL;
//
//				if (tileset != NULL)
//				{
//					switch (tileId - tileset->firstgId)
//					{
//					case 0:
//						map[i] = (uchar)254;
//						break;
//					case 1:
//						map[i] = (uchar)0;
//						break;
//					default:
//						map[i] = (uchar)0;
//						break;
//					}
//				}
//			}
//		}
//		*buffer = map;
//		*width = data.width;
//		*height = data.height;
//		return true;
//	}
//	return false;
//}

MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}

MapTypes Map::StrToMapType(const char* s)
{
	char strType1[TEXT_LEN] = { 0 };
	char strType2[TEXT_LEN] = { 0 };
	char strType3[TEXT_LEN] = { 0 };
	char strType4[TEXT_LEN] = { 0 };
	strcpy_s(strType1, TEXT_LEN, "unknown");
	strcpy_s(strType2, TEXT_LEN, "orthogonal");
	strcpy_s(strType3, TEXT_LEN, "isometric");
	strcpy_s(strType4, TEXT_LEN, "staggered");

	if (strcmp(s, strType1) == 0) return MapTypes::MAPTYPE_UNKNOWN;
	if (strcmp(s, strType2) == 0) return MapTypes::MAPTYPE_ORTHOGONAL;
	if (strcmp(s, strType3) == 0) return MapTypes::MAPTYPE_ISOMETRIC;
	if (strcmp(s, strType4) == 0) return MapTypes::MAPTYPE_STAGGERED;
	
	return MapTypes::MAPTYPE_UNKNOWN;
}

TileSet* Map::GetTileSetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tileSets.end;
	TileSet* set = item->data;

	while (item != NULL)
	{
		if (set->firstgId <= id)
		{
			return set;
		}
		item = item->prev;
		set = item->data;
	}

	return set;
}

Tile* TileSet::GetPropList(int id) const
{
	ListItem<Tile*>* tile = tileSetPropList.start;
	Tile* t = tile->data;

	while (tile != NULL)
	{
		t = tile->data;
		if (t->id == id)
		{
			return t;
		}
		tile = tile->next;
	}

	return t;
}

void Map::LogInfo()
{
	LOG("--------------------------------------------------------------------------");
	LOG("#Map Data: ");
	LOG("Name=%s", data.name);
	LOG("Width=%d", data.width);
	LOG("Height=%d", data.height);
	LOG("TileWidth=%d", data.tileWidth);
	LOG("TileHeight=%d", data.tileHeight);
	LOG("\n");

	ListItem<TileSet*>* infoList;
	infoList = data.tileSets.start;
	while (infoList != NULL)
	{
		LOG("#TileSet");
		LOG("Name=%s", infoList->data->name);
		LOG("Firstgid=%d", infoList->data->firstgId);
		LOG("Margin=%d", infoList->data->margin);
		LOG("Spacing=%d", infoList->data->spacing);
		LOG("Tile_width=%d", infoList->data->tileWidth);
		LOG("Tile_height=%d", infoList->data->tileHeight);

		LOG("texWidth=%d", infoList->data->texWidth);
		LOG("texHeight=%d", infoList->data->texHeight);
		LOG("numTilesWidth=%d", infoList->data->numTilesWidth);
		LOG("numTilesHeight=%d", infoList->data->numTilesHeight);
		LOG("\n");

		infoList = infoList->next;
	}

	// LOG the info for each loaded layer
	ListItem<MapLayer*>* layerList;
	layerList = data.mapLayer.start;

	ListItem<Properties::Property*>* propertyList;
	propertyList = data.mapLayer.start->data->properties.list.start;
	while (layerList != NULL)
	{
		LOG("#Layer");
		LOG("Name=%s", layerList->data->name);
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);

		while (propertyList != NULL)
		{
			LOG("#Property");
			LOG("Name=%s", propertyList->data->name);
			LOG("Value=%d", propertyList->data->value);
			propertyList = propertyList->next;
		}

		LOG("\n");
		layerList = layerList->next;
	}
	LOG("--------------------------------------------------------------------------");
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* property;
	property = list.start;

	char prop[TEXT_LEN] = { 0 };
	strcpy_s(prop, TEXT_LEN, value);

	while (property != NULL)
	{
		if (strcmp(property->data->name, prop) == 0)
		{
			return property->data->value;
		}
		property = property->next;
	}

	return defaultValue;
}

void Properties::SetProperty(const char* name, int value)
{
	ListItem<Property*>* property;
	property = list.start;

	char prop[TEXT_LEN] = { 0 };
	strcpy_s(prop, TEXT_LEN, name);

	while (property != NULL)
	{
		if (strcmp(property->data->name, prop) == 0)
		{
			property->data->value = value;
			return;
		}
		property = property->next;
	}
}

//void Map::SetTileProperty(int x, int y, const char* property, int value, bool notMovCollision, bool isObject)
//{
//	// MapLayer
//	ListItem <MapLayer*>* mapLayer = data.mapLayer.start;
//	SString layerName;
//	if (isObject)
//	{
//		layerName = "Objects";
//	}
//	else
//	{
//		layerName = "Collisions";
//	}
//	while (mapLayer != NULL)
//	{
//		if (mapLayer->data->name == layerName)
//		{
//			break;
//		}
//		mapLayer = mapLayer->next;
//	}
//
//	// TileSet
//	ListItem <TileSet*>* tileSet = data.tileSets.start;
//	SString tileSetName;
//	if (notMovCollision)
//	{
//		tileSetName = "dev_room_tiles";
//	}
//	else
//	{
//		tileSetName = "meta_data";
//	}
//	while (tileSet != NULL)
//	{
//		if (tileSet->data->name == tileSetName)
//		{
//			break;
//		}
//		tileSet = tileSet->next;
//	}
//
//	// Gets CollisionId
//	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgId);
//	if (id < 0)
//	{
//		return;
//	}
//	Tile* currentTile = tileSet->data->GetPropList(id);
//	currentTile->properties.SetProperty(property, value);
//}

int Map::GetTileProperty(int x, int y, const char* property) const
{
	int ret = 0;
	if (data.type == MapTypes::MAPTYPE_UNKNOWN) return ret;
	// MapLayer
	ListItem<MapLayer*>* mapLayer = data.mapLayer.start;
	char layerName[TEXT_LEN] = { 0 };
	strcpy_s(layerName, TEXT_LEN, "MetaData");
	while (mapLayer != nullptr)
	{
		if (strcmp(mapLayer->data->name,layerName) == 0)
		{
			break;
		}
		mapLayer = mapLayer->next;
	}

	if (mapLayer->data == nullptr || mapLayer == nullptr) return ret;

	// TileSet
	ListItem<TileSet*>* tileSet = data.tileSets.start;
	char tileSetName[TEXT_LEN] = { 0 };
	strcpy_s(tileSetName, TEXT_LEN, "meta_data");
	while (tileSet != nullptr)
	{
		if (strcmp(tileSet->data->name,tileSetName) == 0)
		{
			break;
		}
		tileSet = tileSet->next;
	}

	if (tileSet->data == nullptr || tileSet == nullptr) return ret;

	// Gets CollisionId
	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgId);
	if (id < 0)
	{
		return ret;
	}
	Tile* currentTile = tileSet->data->GetPropList(id);
	ret = currentTile->properties.GetProperty(property, -1);
	return ret;
}

bool Map::Load(pugi::xml_node& save)
{
	bool ret = true;
	CleanUp();
	ret = LoadNewMap(save.child("data").attribute("name").as_string());
	if (ret && data.type != MapTypes::MAPTYPE_UNKNOWN)
	{
		if (strcmp(data.name, "tutorial.tmx") == 0)
		{
			app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
		}
		else if (strcmp(data.name, "home.tmx") == 0)
		{
			app->audio->PlayMusic("Audio/Music/Home.ogg");
		}
	}
	return ret;
}

bool Map::Save(pugi::xml_node& save)
{
	bool ret = true;
	if (data.type != MapTypes::MAPTYPE_UNKNOWN)
	{
		pugi::xml_node mapName = save.append_child("data");
		ret = mapName.append_attribute("name").set_value(data.name);
	}
	else ret = false;
	return ret;
}