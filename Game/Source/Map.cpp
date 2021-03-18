#include "App.h"

#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
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

	folder.Create(config.child("folder").child_value());

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
		if (L->data->properties.GetProperty("NoDraw") == 0 || app->render->drawAll)
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
						if (T->GetPropList(tileId - T->firstgId)->properties.GetProperty("NoDraw") == 0)
						{
							app->render->DrawTexture(T->texture, pos.x, pos.y, false, &n);
						}
					}
				}
			}
		}
		L = L->next; // Next layer
	}
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;
	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

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

	mapFile.reset();

	return true;
}

bool Map::Load(const char* filename)
{
	bool ret = true;

	SString tmp("%s%s", folder.GetString(), filename);
	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		// Load map
		ret = LoadMap();

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
			data.tileSets.Add(set);
		}

		// Load layers
		for (pugi::xml_node layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* set = new MapLayer();
			if (ret == true)
			{
				ret = LoadLayer(layer, set);
			}
			data.mapLayer.Add(set);
		}
		LogInfo();
	}
	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
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
		SString strType(map.attribute("orientation").as_string());
		data.type = StrToMapType(strType);
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString color(map.attribute("backgroundcolor").as_string());
		color.Trim();
		sscanf_s(color.GetString(), "%02x%02x%02x", (uint)&data.backgroundColor.r, (uint)&data.backgroundColor.g, (uint)&data.backgroundColor.b);
		data.backgroundColor.a = 255;
	}
	return true;
}

bool Map::LoadTileSetDetails(pugi::xml_node& node, TileSet* set)
{
	LOG("Filling TileSetDetails");

	set->firstgId = node.attribute("firstgid").as_int();
	set->name = node.attribute("name").as_string();
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
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
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

	layer->name = node.attribute("name").as_string();
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

	LOG("Layer [%s] has loaded %d tiles", layer->name.GetString(), i);

	ret = LoadProperties(node.child("properties"), layer->properties);

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	pugi::xml_node property;
	for (property = node.child("property"); property; property = property.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property();

		prop->name = property.attribute("name").as_string();
		prop->value = property.attribute("value").as_int();

		properties.list.Add(prop);
	}

	return true;
}

bool Map::CreateWalkabilityMap(int* width, int* height, uchar** buffer) const
{
	ListItem<MapLayer*>* item;
	item = data.mapLayer.start;

	for (item = data.mapLayer.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Navigation", 0) == 0)
		{
			continue;
		}
		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTileSetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					switch (tileId - tileset->firstgId)
					{
					case 0:
						map[i] = (uchar)254;
						break;
					case 1:
						map[i] = (uchar)0;
						break;
					default:
						map[i] = (uchar)0;
						break;
					}
				}
			}
		}
		*buffer = map;
		*width = data.width;
		*height = data.height;
		return true;
	}
	return false;
}

MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}

MapTypes Map::StrToMapType(SString s)
{
	SString StrType[4];
	StrType[0] = "unknown", StrType[1] = "orthogonal", StrType[2] = "isometric", StrType[3] = "staggered";
	MapTypes type = MapTypes::MAPTYPE_UNKNOWN;
	for (int i = 0; i < 4; ++i)
	{
		if (s == StrType[i])
		{
			return type;
		}
		++type;
	}
	return MAPTYPE_UNKNOWN;
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
		LOG("Name=%s", infoList->data->name.GetString());
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
		LOG("Name=%s", layerList->data->name.GetString());
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);

		while (propertyList != NULL)
		{
			LOG("#Property");
			LOG("Name=%s", propertyList->data->name.GetString());
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

	SString prop = value;

	while (property != NULL)
	{
		if (property->data->name == prop)
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

	SString prop = name;

	while (property != NULL)
	{
		if (property->data->name == prop)
		{
			property->data->value = value;
			return;
		}
		property = property->next;
	}
}

void Map::SetTileProperty(int x, int y, const char* property, int value, bool notMovCollision, bool isObject)
{
	// MapLayer
	ListItem <MapLayer*>* mapLayer = data.mapLayer.start;
	SString layerName;
	if (isObject)
	{
		layerName = "Objects";
	}
	else
	{
		layerName = "Collisions";
	}
	while (mapLayer != NULL)
	{
		if (mapLayer->data->name == layerName)
		{
			break;
		}
		mapLayer = mapLayer->next;
	}

	// TileSet
	ListItem <TileSet*>* tileSet = data.tileSets.start;
	SString tileSetName;
	if (notMovCollision)
	{
		tileSetName = "level_1_tileset";
	}
	else
	{
		tileSetName = "meta_data";
	}
	while (tileSet != NULL)
	{
		if (tileSet->data->name == tileSetName)
		{
			break;
		}
		tileSet = tileSet->next;
	}

	// Gets CollisionId
	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgId);
	if (id < 0)
	{
		return;
	}
	Tile* currentTile = tileSet->data->GetPropList(id);
	currentTile->properties.SetProperty(property, value);
}

int Map::GetTileProperty(int x, int y, const char* property, bool notMovCollision, bool isObject) const
{
	int ret;
	// MapLayer
	ListItem<MapLayer*>* mapLayer = data.mapLayer.start;
	SString layerName;
	if (isObject)
	{
		layerName = "Objects";
	}
	else
	{
		layerName = "Collisions";
	}
	while (mapLayer != NULL)
	{
		if (mapLayer->data->name == layerName)
		{
			break;
		}
		mapLayer = mapLayer->next;
	}

	// TileSet
	ListItem<TileSet*>* tileSet = data.tileSets.start;
	SString tileSetName;
	if (notMovCollision)
	{
		tileSetName = "level_1_tileset";
	}
	else
	{
		tileSetName = "meta_data";
	}
	while (tileSet != NULL)
	{
		if (tileSet->data->name == tileSetName)
		{
			break;
		}
		tileSet = tileSet->next;
	}

	// Gets CollisionId
	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgId);
	if (id < 0)
	{
		ret = 0;
		return ret;
	}
	Tile* currentTile = tileSet->data->GetPropList(id);
	ret = currentTile->properties.GetProperty(property, 0);
	return ret;
}