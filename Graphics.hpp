#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

struct ALLEGRO_BITMAP;

namespace DBL
{
	int SCREEN_WIDTH();
	int SCREEN_HEIGHT();

	enum PRINT_ALIGNMENT
	{
		PRINT_LEFT,
		PRINT_CENTER,
		PRINT_RIGHT
	};

	enum TEXT_TYPES
	{
		COMMON24,
		COMMON16,
		FANCY48
	};

	class Actor;
	class Tileset;
	class TextType;

	/**
		Graphics are completely independent from the rest of the game. 
		The renderer is in charge of holding the graphics, 
		setting the window title and telling what the tileset draw offset is or how big the draw rectangle is. 
		You can tell the renderer to clear the screen to a color, 
		draw a collection of actors, draw a tilemap, 
		a graphic (starting at XY or center there), 
		or text based given text type and color. 
		
		All graphical data is stored in the Graphics library/renderer. 
		
		Also can get the tile coordinate under a certain pixel of the display. 
		
		Loads all the graphics from file and stores them, frees them on deletion. 
	*/
	class Graphics
	{
	public:

		int DisplayWidth();
		int DisplayHeight();

		Graphics();
		~Graphics();

		/* Allegro must be initialized first */
		bool CreateDisplay(ALLEGRO_EVENT_QUEUE *event_queue, int w, int h, bool window = true);
		void DestroyDisplay();

		void LoadResources();
		void FreeResources();

		void SetFullscreen(bool isFullscreen);
		bool IsFullscreen();
		void SetWindowTitle(std::string title);
		void ClearToColor(Color color);
		void ShowErrorMessage(const char *errorMessage);
		void FlipScreen();

		void CenterDrawRectangleOnPosition(int x, int y);
		void SetDrawRectangleSize(int width, int height);
		void SetDepthToDraw(int depth);
		int GetDepthToDraw();

		XYPair<> GetTileCoordinateUnderScreenPosition(int x, int y);

		/* Draw relative to the top left corner of the screen, flags include 1 = center on x, 2 = center on y */
		void DrawGraphic(int gid, int x, int y, int flags);

		/* Drawn with respect to the draw rectangle */
		void DrawActors(std::list<Actor*> &actors);
		void DrawActorImage(IndexedImage img, int x, int y, int flags);

		/* Drawn with respect to the draw rectangle */
		void DrawTileset(Tileset &tileset);
		void DrawTile(int id, int x, int y, int width, int height);

		void DrawColoredBox(float x1, float y1, float x2, float y2, Color color);
		void DrawBorder(float x1, float y1, float x2, float y2, Color color, float thickness=5);

		/* Draw with respect to the screen's top left corner, returns where next line should start */
		int PrintText(int type, Color color, int x, int y, const std::string &text);
		int PrintTextRect(int type, Color color, int x, int y, 
			int width, int beginHeight, int maxHeight, bool draw, const std::string &text);
		int GetTextWidth(int type, const std::string &text);
		int GetTextLineHeight(int type);
		
		//No get, if you need to ask you should just do it
		void SetPrintAlignment(int alignment);

		/* Returns the graphic index (gid) the graphic is loaded into*/
		int LoadGraphic(const char *filename);
		void FreeGraphic(int graphicIndex);

	private:
		static const char *tileAssetFilename;
		static const char *actorAssetFilename;
		
		bool isFullScreen;
		ALLEGRO_DISPLAY *display;

		//Draw rectangle
		XYPair<> drawRectangleSize, drawRectangleStartPosition;
		XYPair<> displaySize;
		int drawDepth;

		/* Text types */
		std::vector<TextType*> textTypes;
		int printAlignment;

		/* Tilesets */
		std::map<int, ALLEGRO_BITMAP*> actorTileset;
		ALLEGRO_BITMAP *actorAssetSheet;

		std::map<int, ALLEGRO_BITMAP*> tileTileset;
		ALLEGRO_BITMAP *tileAssetSheet;

		//Graphics loaded
		std::map<int, ALLEGRO_BITMAP*> graphics;
		int nextGid;

		void LoadTileAssets();
		void LoadActorAssets();
		void LoadTextTypes();

		void FreeAllGraphics();
		void FreeTileAssets();
		void FreeActorAssets();
		void FreeTextTypes();
	};
};

#endif