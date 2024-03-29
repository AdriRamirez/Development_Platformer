
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false)
        return;

    /*
    // L04: DONE 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;

    while (tileset != NULL) {
        app->render->DrawTexture(tileset->data->texture,0,0);
        tileset = tileset->next;
    }
    */

    // L05: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        //L06: DONE 7: use GetProperty method to ask each layer if your �Draw� property is true.
        if (mapLayerItem->data->properties.GetProperty("draw") != NULL && mapLayerItem->data->properties.GetProperty("draw")->value) {

            if (mapLayerItem->data->properties.GetProperty("parallax") != NULL && mapLayerItem->data->properties.GetProperty("parallax")->value) {

                for (int x = 0; x < mapLayerItem->data->width; x++)
                {
                    for (int y = 0; y < mapLayerItem->data->height; y++)
                    {
                        // L05: DONE 9: Complete the draw function
                        int gid = mapLayerItem->data->Get(x, y);

                        //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                        TileSet* tileset = GetTilesetFromTileId(gid);

                        SDL_Rect r = tileset->GetTileRect(gid);
                        iPoint pos = MapToWorld(x, y);


                            app->render->DrawTexture(tileset->texture,
                                pos.x = pos.x - app->render->camera.x / mapLayerItem->data->properties.GetProperty("parallax_value")->numValue,
                                pos.y,
                                &r);
                    }
                }
            }
            else
            {
                for (int x = 0; x < mapLayerItem->data->width; x++)
                {
                    for (int y = 0; y < mapLayerItem->data->height; y++)
                    {
                        // L05: DONE 9: Complete the draw function
                        int gid = mapLayerItem->data->Get(x, y);

                        //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                        TileSet* tileset = GetTilesetFromTileId(gid);

                        SDL_Rect r = tileset->GetTileRect(gid);
                        iPoint pos = MapToWorld(x, y);

                        app->render->DrawTexture(tileset->texture,
                            pos.x,
                            pos.y,
                            &r);
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }
}

// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// L06: DONE 2: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // L05: DONE 2: clean up all layer data
    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;
    
    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    CollidersLevel1();

    if(ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// L05: DONE 4: Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();

        if (p->name == SString("parallax") || p->name == SString("draw")) {
            p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!
        }
        if (p->name == SString("parallax_value")) {
            p->numValue = propertieNode.attribute("value").as_float();
        }

        properties.list.Add(p);
    }

    return ret;
}

void Map::CollidersLevel1() {
    // Create colliders

    //Death Collider
    PhysBody* cD = app->physics->CreateRectangle(3200, 672, 6400, 32, STATIC);
    cD->ctype = ColliderType::DEATH;

    //First part of the map
    PhysBody* c0 = app->physics->CreateRectangle(0-16, 576-288, 32, 576, STATIC);
    c0->ctype = ColliderType::PLATFORM;
    PhysBody* c1 = app->physics->CreateRectangle(415, 544, 830, 64, STATIC);
    c1->ctype = ColliderType::PLATFORM;
    PhysBody* c2 = app->physics->CreateRectangle(832 + 48, 544 - 32, 96, 128, STATIC);
    c2->ctype = ColliderType::PLATFORM;
    PhysBody* c3 = app->physics->CreateRectangle(928 + 48, 576 - 96, 96, 192, STATIC);
    c3->ctype = ColliderType::PLATFORM;
    PhysBody* c4 = app->physics->CreateRectangle(1024 + 176, 576 - 32, 352, 64, STATIC);
    c4->ctype = ColliderType::PLATFORM;
    PhysBody* c5 = app->physics->CreateRectangle(1120 + 80, 384 - 16, 160, 32, STATIC);
    c5->ctype = ColliderType::PLATFORM;
    PhysBody* c6 = app->physics->CreateRectangle(1376 + 48, 576 - 144, 96, 288, STATIC);
    c6->ctype = ColliderType::PLATFORM;
    PhysBody* c7 = app->physics->CreateRectangle(1472 + 48, 576 - 112, 96, 224, STATIC);
    c7->ctype = ColliderType::PLATFORM;

    //Second part 
    PhysBody* c8 = app->physics->CreateRectangle(1664 + 48, 576 - 96, 96, 192, STATIC);
    c8->ctype = ColliderType::PLATFORM;
    PhysBody* c9 = app->physics->CreateRectangle(1856 + 32, 576 - 80, 64, 160, STATIC);
    c9->ctype = ColliderType::PLATFORM;
    PhysBody* c10 = app->physics->CreateRectangle(2016 + 80, 576 - 64, 160, 128, STATIC);
    c10->ctype = ColliderType::PLATFORM;
    PhysBody* c11 = app->physics->CreateRectangle(2176 + 208, 576 - 32, 416, 64, STATIC);
    c11->ctype = ColliderType::PLATFORM;
    PhysBody* c12 = app->physics->CreateRectangle(2304 + 80, 416 - 16, 160, 32, STATIC);
    c12->ctype = ColliderType::PLATFORM;
    PhysBody* c13 = app->physics->CreateRectangle(2592 + 80, 576 - 144, 160, 288, STATIC);
    c13->ctype = ColliderType::PLATFORM;
    PhysBody* c14 = app->physics->CreateRectangle(2752 + 256, 576 - 32, 512, 64, STATIC);
    c14->ctype = ColliderType::PLATFORM;
    PhysBody* c15 = app->physics->CreateRectangle(2752 + 64, 416 - 80, 128, 160, STATIC);
    c15->ctype = ColliderType::PLATFORM;
    PhysBody* c16 = app->physics->CreateRectangle(2752 + 144, 256 - 32, 288, 64, STATIC);
    c16->ctype = ColliderType::PLATFORM;
    PhysBody* c17 = app->physics->CreateRectangle(2976 + 80, 416 - 32, 160, 64, STATIC);
    c17->ctype = ColliderType::PLATFORM;
    PhysBody* c18 = app->physics->CreateRectangle(3136 + 48, 416 - 208, 96, 416, STATIC);
    c18->ctype = ColliderType::PLATFORM;

    //Third part
    PhysBody* c19 = app->physics->CreateRectangle(3360 + 176, 576 - 32, 352, 64, STATIC);
    c19->ctype = ColliderType::PLATFORM;
    PhysBody* c20 = app->physics->CreateRectangle(3392 + 48, 352 - 176, 96, 352, STATIC);
    c20->ctype = ColliderType::PLATFORM;
    PhysBody* c21 = app->physics->CreateRectangle(3488 + 64, 352 - 32, 128, 64, STATIC);
    c21->ctype = ColliderType::PLATFORM;
    PhysBody* c22 = app->physics->CreateRectangle(3712 + 64, 576 - 80, 128, 160, STATIC);
    c22->ctype = ColliderType::PLATFORM;
    PhysBody* c23 = app->physics->CreateRectangle(3712 + 112, 224 - 32, 224, 64, STATIC);
    c23->ctype = ColliderType::PLATFORM;
    PhysBody* c24 = app->physics->CreateRectangle(3840 + 48, 576 - 176, 96, 352, STATIC);
    c24->ctype = ColliderType::PLATFORM;
    PhysBody* c25 = app->physics->CreateRectangle(3935 + 96, 576 - 160, 192, 320, STATIC);
    c25->ctype = ColliderType::PLATFORM;

    //Fourth part
    PhysBody* c26 = app->physics->CreateRectangle(4256 + 32, 576 - 96, 64, 192, STATIC);
    c26->ctype = ColliderType::PLATFORM;
    PhysBody* c27 = app->physics->CreateRectangle(4448 + 32, 576 - 128, 64, 256, STATIC);
    c27->ctype = ColliderType::PLATFORM;
    PhysBody* c28 = app->physics->CreateRectangle(4640 + 16, 576 - 96, 32, 192, STATIC);
    c28->ctype = ColliderType::PLATFORM;
    PhysBody* c29 = app->physics->CreateRectangle(4800 + 80, 576 - 64, 160, 128, STATIC);
    c29->ctype = ColliderType::PLATFORM;
    PhysBody* c30 = app->physics->CreateRectangle(4960 + 144, 576 - 32, 288, 64, STATIC);
    c30->ctype = ColliderType::PLATFORM;
    PhysBody* c31 = app->physics->CreateRectangle(5248 + 80, 576 - 80, 160, 160, STATIC);
    c31->ctype = ColliderType::PLATFORM;
    PhysBody* c32 = app->physics->CreateRectangle(5504 + 48, 576 - 144, 96, 288, STATIC);
    c32->ctype = ColliderType::PLATFORM;
    PhysBody* c33 = app->physics->CreateRectangle(5728 + 16, 576 - 112, 32, 224, STATIC);
    c33->ctype = ColliderType::PLATFORM;
    PhysBody* c34 = app->physics->CreateRectangle(5984 + 208, 576 - 32, 416, 64, STATIC);
    c34->ctype = ColliderType::PLATFORM;
    PhysBody* c35 = app->physics->CreateRectangle(6400 + 16, 576 - 288, 32, 576, STATIC);
    c35->ctype = ColliderType::PLATFORM;

}

// L06: DONE 7: Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


