#include "main.hpp"

using namespace DBL;

const char *Graphics::tileAssetFilename = "TileAssets.png";
const char *Graphics::actorAssetFilename = "ActorAssets.png";

Graphics::Graphics():
display(NULL),
isFullScreen(false),
printAlignment(PRINT_LEFT),
nextGid(0),
drawDepth(0)
{}

Graphics::~Graphics()
{}

int DBL::SCREEN_WIDTH(){ return game->graphics.DisplayWidth(); }

int DBL::SCREEN_HEIGHT(){ return game->graphics.DisplayHeight(); }

int Graphics::DisplayWidth(){ return al_get_display_width(display); }
int Graphics::DisplayHeight(){ return al_get_display_height(display); }

bool Graphics::CreateDisplay(ALLEGRO_EVENT_QUEUE *event_queue, int w, int h, bool window)
{
	display = al_create_display(w, h);
	displaySize.x = w; displaySize.y = h;
	if(display)
	{
		al_register_event_source(event_queue, al_get_display_event_source(display));

		if(!window){ SetFullscreen(true); }
		FlipScreen();
		return true;
	}
	else return false;
}



void Graphics::DestroyDisplay(){ al_destroy_display(display); }

void Graphics::LoadResources()
{
	//actor, tiles, graphics
	LoadTileAssets();
	LoadActorAssets();
	LoadTextTypes();
}

void Graphics::FreeResources()
{
	//actor, tiles, graphics
	FreeAllGraphics();
	FreeTileAssets();
	FreeActorAssets();
	FreeTextTypes();
}

bool Graphics::IsFullscreen(){ return isFullScreen; }


void Graphics::SetFullscreen(bool isFullScreen)
{
	this->isFullScreen = isFullScreen;
	if(!isFullScreen) al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 0);
	if(isFullScreen) al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 1);
}

void Graphics::SetWindowTitle(std::string title)
{
	al_set_window_title(display, title.c_str());
}

void Graphics::ClearToColor(Color color)
{
	al_draw_filled_rectangle(0, 0, DisplayWidth(), DisplayHeight(), color);
}

void Graphics::FlipScreen(){ al_flip_display(); }

void Graphics::SetDepthToDraw(int depth){ drawDepth = depth; }
int Graphics::GetDepthToDraw(){ return drawDepth; }

void Graphics::CenterDrawRectangleOnPosition(int x, int y)
{
	//center draw rectangle using its known size
	drawRectangleStartPosition.x = x - drawRectangleSize.x/2;
	drawRectangleStartPosition.y = y - drawRectangleSize.y/2;

	//if it's trying to leave a void to the top and left, push it down or right
	//respectively to prevent this
	if(drawRectangleStartPosition.x < 0) drawRectangleStartPosition.x = 0;
	if(drawRectangleStartPosition.y < 0) drawRectangleStartPosition.y = 0;

	//if draw rectangle is larger than window and the 
	//drawRectangle threatens to leave a black space to the bottom or right
	//prevent this by pushing it up or left respectively to the window's edge
	if(drawRectangleStartPosition.x+drawRectangleSize.x > 
		int(game->world.levelCollection.levels[drawDepth].tiles.size())*TILE_SIZE)
	{
		int depthWidth = (game->world.levelCollection.levels[drawDepth].tiles.size())*TILE_SIZE;
		drawRectangleStartPosition.x = depthWidth - drawRectangleSize.x;
	}

	if(drawRectangleStartPosition.y+drawRectangleSize.y > 
		int(game->world.levelCollection.levels[drawDepth].tiles.size())*TILE_SIZE)
	{
		int depthWidth = (game->world.levelCollection.levels[drawDepth].tiles.size())*TILE_SIZE;
		drawRectangleStartPosition.y = depthWidth - drawRectangleSize.y;
	}
}

void Graphics::SetDrawRectangleSize(int width, int height)
{
	drawRectangleSize.x = width;
	drawRectangleSize.y = height;
}

XYPair<> Graphics::GetTileCoordinateUnderScreenPosition(int x, int y)
{
	//Find absolute position under (x,y)
	XYPair<> ret(drawRectangleStartPosition.x+x, drawRectangleStartPosition.y+y);

	//Divide by size of tile
	ret.x = ret.x / TILE_SIZE;
	ret.y = ret.y / TILE_SIZE;

	return ret;
}

void Graphics::DrawGraphic(int gid, int x, int y, int flags)
{
	if(graphics.count(gid) == 1)
	{
		ALLEGRO_BITMAP *bitmap = graphics.at(gid);

		if(flags & 1) //center on x
			x -= al_get_bitmap_width(bitmap);
		if(flags & 2) //center on y
			y -= al_get_bitmap_height(bitmap);

		al_draw_bitmap(bitmap, x, y, 0);
	}
}

void Graphics::DrawColoredBox(float x1, float y1, float x2, float y2, Color color)
{
	
	
	al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(color.r, color.g, color.b, color.a));
}
void Graphics::DrawBorder(float x1, float y1, float x2, float y2, Color color, float thickness)
{
	al_draw_rectangle(x1, y1, x2, y2, al_map_rgba(color.r, color.g, color.b, color.a), thickness);
}

void Graphics::ShowErrorMessage(const char *errorMessage)
{
	al_show_native_message_box(display, "Fatal Error",
	"Please report error: ", errorMessage, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void Graphics::DrawActors(std::list<Actor*> &actors)
{
	drawDepth = game->GetGameLevel();
	al_hold_bitmap_drawing(true);
	for(auto actor = actors.begin(); actor != actors.end(); actor++)
	{
		
		if( (*actor)->images && (*actor)->images->isVisible && (*actor)->position && 
			(*actor)->position->depth == drawDepth)
		{
			for(auto img = (*actor)->images->images.begin(); img != (*actor)->images->images.end(); img++)
			{
				ALLEGRO_BITMAP *bitmap = actorTileset.at(img->index);
				al_draw_tinted_bitmap(bitmap, img->tint, 
					int( (*actor)->position->absolutePos.x - drawRectangleStartPosition.x ),
					int( (*actor)->position->absolutePos.y - drawRectangleStartPosition.y ),
					0);
			}
		}
	}
	al_hold_bitmap_drawing(false);
}

void Graphics::DrawActorImage(IndexedImage img, int x, int y, int flags)
{
	//Consider checking here if actor is off screen to decide not to draw them

	if(actorTileset.count(img.index) == 1)
	{
		ALLEGRO_BITMAP *actorTile = actorTileset.at(img.index);
		if(actorTile)
		{
			//al_draw_tinted_bitmap(actorTile, img.tint, 

		}
	}
}

void Graphics::DrawTileset(Tileset &tileset)
{
	int firstX = drawRectangleStartPosition.x/TILE_SIZE;
	int firstY = drawRectangleStartPosition.y/TILE_SIZE;
	int lastX = (firstX + drawRectangleSize.x/TILE_SIZE)+1;
	int lastY = (firstY + drawRectangleSize.y/TILE_SIZE)+1;

	if(lastX > (int)tileset.tiles.size()-1) lastX = tileset.tiles.size()-1;
	if(lastY > (int)tileset.tiles[0].size()-1) lastY = tileset.tiles[0].size()-1;

	for(int y = firstY; y <= lastY; y++) for(int x = firstX; x <= lastX; x++)
	{

		int tileId = tileset.tiles[x][y].tileMaterialIndex;
		const int VARIANCE = 10;
		int tileVariation = 255 - tileset.tiles[x][y].tintVariation*VARIANCE;
		Color tileColor(tileVariation, tileVariation, tileVariation);

		if(tileTileset.count(tileId) == 1)
		{
			ALLEGRO_BITMAP *tileImage = tileTileset.at(tileId);
			if(tileImage)
			{
				al_draw_tinted_bitmap(tileImage, tileColor,
					int( (x*TILE_SIZE) /*+ drawRectangleStartPosition.x%TILE_SIZE*/ - drawRectangleStartPosition.x ),
					int( (y*TILE_SIZE) /*+ drawRectangleStartPosition.y%TILE_SIZE*/ - drawRectangleStartPosition.y ),
					0);
			}
		}
	}
}

void Graphics::DrawTile(int id, int x, int y, int width, int height)
{

	if(tileTileset.count(id) == 1)
	{


		ALLEGRO_BITMAP *tileTile = tileTileset.at(id);
		if(tileTile)
		{
			al_draw_scaled_bitmap(tileTile, 0, 0, TILE_SIZE, TILE_SIZE, x, y, width, height, 0);
				//x - drawRectangleStartPosition.x, 
				//y - drawRectangleStartPosition.y, 
				//0);
		}
	}
}

/* Draw with respect to the x and y */
int Graphics::PrintText(int type, Color color, int x, int y, const std::string &text)
{
	if((int)textTypes.size() > type)
	{
		if(printAlignment == PRINT_RIGHT)
		{
			x -= al_get_text_width(textTypes[type]->font, text.c_str());
		}
		else if(printAlignment == PRINT_CENTER)
		{
			x -= al_get_text_width(textTypes[type]->font, text.c_str())/2;
		}

		al_draw_text(textTypes[type]->font, color, x, y, 0, text.c_str());
		return y+al_get_font_line_height(textTypes[type]->font);
	}

	return 0;
}

int Graphics::GetTextWidth(int type, const std::string &text)
{
	int ret = 0;
	if((int)textTypes.size() > type)
	{
		ret = al_get_text_width(textTypes[type]->font, text.c_str());
	}
	
	return ret;
}

int Graphics::GetTextLineHeight(int type)
{
	int ret = 0;
	if((int)textTypes.size() > type)
	{
		ret = al_get_font_line_height(textTypes[type]->font);
	}
	
	return ret;
}

#pragma warning(push)
#pragma warning(disable: 4996) //vsprintf is unsafe and depricated but vsprintf_s is nonstandard
int Graphics::PrintTextRect(int type, Color color, int x, int y, 
	int width, int beginHeight, int maxHeight, bool draw, const std::string &text)
{
     char stext[2048]; // Copy of the passed text.
	 ALLEGRO_FONT *font = textTypes[type]->font;

     char * pch; // A pointer to each word.
     char word[255]; // A string containing the word (for convienence)
     char breakchar[12]; // Contains the break line character "\n "
     char Lines[40][1024]; // A lovely array of strings to hold all the lines (40 max atm)
     char TempLine[1024]; // Holds the string data of the current line only.
     int CurrentLine = 0; // Counts which line we are currently using.
     int q; // Used for loops

     // Setup our strings
     strncpy(stext, text.c_str(), 2048);
     strncpy(breakchar,"\n ", 12);
     strncpy(TempLine,"", 1024);
     for(q = 0;q < 40;q+=1)
     {
          sprintf(Lines[q],"");
     }
     //-------------------- Code Begins

     pch = strtok (stext," ");                               // Get the first word.
     do
     {
          strcpy(word,"");                                  // Truncate the string, to ensure there's no crazy stuff in there from memory.
          sprintf(word,"%s ",pch);
          sprintf(TempLine,"%s%s",TempLine,word);             // Append the word to the end of TempLine
          // This code checks for the new line character.
          if(strcmp(word,breakchar) == 0)
          {
                CurrentLine+=1;                                 // ActorMovementManager down a Line
                strcpy(TempLine,"");                            // Clear the tempstring
          }
          else
          {
                if(al_get_text_width(font, TempLine) >= (width))   // Check if text is larger than the area.
                {
                      strcpy(TempLine,word);                      // clear the templine and add the word to it.
                      CurrentLine+=1;                             // ActorMovementManager to the next line.
                }
                if(CurrentLine < 40)
                {
                      strcat(Lines[CurrentLine],word);                // Append the word to whatever line we are currently on.
                }
          }
          pch = strtok (NULL, " ");                           // Get the next word.
     }while (pch != NULL);
     // ---------------------------------- Time to draw.
     if(draw == true)                                       //Check whether we are actually drawing the text.
     {
          for(q = 0;q <=CurrentLine;q+=1)                     // ActorMovementManager through each line and draw according to the passed flags.
          {
				if(	q*al_get_font_line_height(font) >= beginHeight)
				{
					if(printAlignment == ALLEGRO_ALIGN_LEFT)
						 al_draw_text(font, color, x, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_LEFT,Lines[q]);
					if(printAlignment == ALLEGRO_ALIGN_CENTRE)
						 al_draw_text(font, color, x + (width/2), y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_CENTRE,Lines[q]);
					if(printAlignment == ALLEGRO_ALIGN_RIGHT)
						 al_draw_text(font, color, x + width, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_RIGHT,Lines[q]);
				}
				if( (maxHeight != 0 && CurrentLine+1)*al_get_font_line_height(font) > maxHeight) break;
          }

     }
     return((CurrentLine+1) * al_get_font_line_height(font));  // Return the actual height of the text in pixels.	
}
#pragma warning(pop)

void Graphics::SetPrintAlignment(int alignment)
{
	printAlignment = alignment;
}

/* Returns the graphic index (gid) the graphic is loaded into*/
int Graphics::LoadGraphic(const char *filename)
{
	ALLEGRO_BITMAP *graphic = al_load_bitmap(filename);
	if(graphic)
	{
		nextGid++;
		graphics.insert(std::pair<int, ALLEGRO_BITMAP*>(nextGid, graphic));
		return nextGid;
	}
	else return 0;
}

void Graphics::FreeGraphic(int graphicIndex)
{
	if(graphics.count(graphicIndex) == 1)
	{
		ALLEGRO_BITMAP *graphic = graphics.at(graphicIndex);
		if(graphic) al_destroy_bitmap(graphic);
	}
}

void Graphics::LoadTileAssets()
{
	ALLEGRO_COLOR mask = al_map_rgb(0, 255, 0);

	int index = 0;
	//Create a valid, empty actor asset to return for invalid actor assets
	ALLEGRO_BITMAP *empty = al_create_bitmap(TILE_SIZE, TILE_SIZE);
	actorTileset.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, empty));

	actorAssetSheet = al_load_bitmap(actorAssetFilename);
	if(actorAssetSheet)
	{
		al_convert_mask_to_alpha(actorAssetSheet, mask);
		int bitmapWidth = al_get_bitmap_width(actorAssetSheet);
		int bitmapHeight = al_get_bitmap_height(actorAssetSheet);
		
		for(int y = 0; y < bitmapHeight; y+= TILE_SIZE)
		for(int x = 0; x < bitmapWidth; x+= TILE_SIZE)
		{
			ALLEGRO_BITMAP *actorAsset = al_create_sub_bitmap(actorAssetSheet, x, y, TILE_SIZE, TILE_SIZE);
			if(actorAsset)
			{
				actorTileset.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, actorAsset));
			}			
		}
	}
}

void Graphics::LoadActorAssets()
{
	ALLEGRO_COLOR mask = al_map_rgb(0, 255, 0);

	int index = 0;
	//Create a valid, empty tile asset to return for invalid tile assets
	ALLEGRO_BITMAP *empty = al_create_bitmap(TILE_SIZE, TILE_SIZE);
	tileTileset.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, empty));

	tileAssetSheet = al_load_bitmap(tileAssetFilename);
	if(tileAssetSheet)
	{
		al_convert_mask_to_alpha(tileAssetSheet, mask);
		int bitmapWidth = al_get_bitmap_width(tileAssetSheet);
		int bitmapHeight = al_get_bitmap_height(tileAssetSheet);
		
		for(int y = 0; y < bitmapHeight; y+= TILE_SIZE)
		for(int x = 0; x < bitmapWidth; x+= TILE_SIZE)
		{
			ALLEGRO_BITMAP *tileAsset = al_create_sub_bitmap(tileAssetSheet, x, y, TILE_SIZE, TILE_SIZE);
			if(tileAsset)
			{
				tileTileset.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, tileAsset));
			}			
		}
	}
}

void Graphics::LoadTextTypes()
{
	TextType *common24 = new TextType("Cambria.ttf", 24);
	TextType *COMMON16 = new TextType("Cambria.ttf", 16);
	TextType *fancy48 = new TextType("Drawn Old English.ttf", 48);
	textTypes.push_back(common24);
	textTypes.push_back(COMMON16);
	textTypes.push_back(fancy48);
}

void Graphics::FreeAllGraphics()
{
	for(auto iter = graphics.begin(); iter != graphics.end(); iter++)
	{
		if( (*iter).second) al_destroy_bitmap((*iter).second);
	}
	graphics.clear();
}

void Graphics::FreeTileAssets()
{
	for(auto iter = tileTileset.begin(); iter != tileTileset.end(); iter++)
	{
		if( (*iter).second) al_destroy_bitmap((*iter).second);
	}
	if(tileAssetSheet) al_destroy_bitmap(tileAssetSheet);
	tileTileset.clear();
}

void Graphics::FreeActorAssets()
{
	for(auto iter = actorTileset.begin(); iter != actorTileset.end(); iter++)
	{
		if( (*iter).second) al_destroy_bitmap((*iter).second);
	}
	if(actorAssetSheet) al_destroy_bitmap(actorAssetSheet);
	actorTileset.clear();
}

void Graphics::FreeTextTypes()
{
	while(!textTypes.empty())
	{
		if(textTypes.back())
		{
			delete textTypes.back();
		}
		textTypes.pop_back();
	}
}