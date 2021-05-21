#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

// Map Types enum
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		Property()
		{
			name.Clear();
			value = 0;
		}

		SString name;
		int value;
	};

	// Destructor
	~Properties()
	{
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		list.Clear();
	}

	// Get/Set property
	int GetProperty(const char* name, int defaultValue = 0) const;
	void SetProperty(const char* name, int value);

	// List of properties
	List<Property*> list;
};

struct Tile
{
	int id;
	Properties properties;
};

struct TileSet
{
	int firstgId;
	SString name;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	List<Tile*> tileSetPropList;

	// Receives a tile id and returns it's Rect position on the tileset
	SDL_Rect GetTileRect(int id) const;

	// Create a function that gives Id and returns properties
	Tile* GetPropList(int id) const;
};

struct MapLayer
{
	SString	name;
	int width = 0;
	int height = 0;
	uint* data = nullptr;
	Properties properties;

	// Constructor
	MapLayer() : data(NULL)
    {
        this->width = 0;
        this->height = 0;
    }

	// Destructor
	~MapLayer()
	{
		RELEASE(data);
	}

	// Function to get the value of x and y
	inline uint Get(int x, int y) const
	{
		uint result = data[y * width + x];
		return result;
	}
};

struct MapData
{
	SString name;
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	SDL_Color backgroundColor;
	MapTypes type = MAPTYPE_UNKNOWN;
	List<TileSet*> tileSets;
	List<MapLayer*> mapLayer;
};

class Map : public Module
{
public:
	// Constructor
	Map();

	// Destructor
	virtual ~Map();

	// Called when program is executed
	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool LoadNewMap(const char* path);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// Translates map position to world position
	iPoint MapToWorld(int x, int y) const;

	// Translates world position to map position
	iPoint WorldToMap(int x, int y) const;

	//// Creates the data required to check the walkability of the whole map for pathfinding
	//bool CreateWalkabilityMap(int* width, int* height, uchar** buffer) const;

	// Changes property to value assigned
	//void SetTileProperty(int x, int y, const char* property, int value, bool metaData);

	// Gets the value of a property in a given tile
	int GetTileProperty(int x, int y, const char* property) const;

	// Stores the data of the whole map
	MapData data;

private:
	// Load functions
	bool LoadMap(const char* filename);
	bool LoadTileSetDetails(pugi::xml_node& node, TileSet* set);
	bool LoadTileSetImage(pugi::xml_node& node, TileSet* set);
	bool LoadTileSetProperties(pugi::xml_node& node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	MapTypes StrToMapType(SString str);
	TileSet* GetTileSetFromTileId(int id) const;
	bool StoreId(pugi::xml_node& node, MapLayer* layer, int index);
	void LogInfo();

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
};

#endif // __MAP_H__
