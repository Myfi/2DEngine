#ifndef _2DENGINE_H             
#define _2DENGINE_H             
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "player.h"
#include "terrain.h"
#include "enemy.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class Engine : public Game
{
private:
    // game items
    TextureManager backgroundTexture;		// background texture
    TextureManager characterTexture;		// character texture
	TextureManager groundTexture;			// ground texture
    TextureManager enemyTexture;           // enemy texture
    Image   background;						// Background image
    Player  character;						// character image     
	Terrain ground[764];						// ground entity
    Image   boxTest;                        // a box used for testing
    Enemy   enemies[100];
    Terrain stopBlock[200];
    int current_terrain = 0;
    int num_of_enemies = 0;
    int mTime = 0;
    int current_asset = 1;
    int prevX = 0;

public:
    // Constructor
    Engine();

    // Destructor
    virtual ~Engine();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      
    void ai();          
    void collisions();  
    void render();      
    void releaseAll();
    void resetAll();
	int mousepos_to_gridX(int X);
	int mousepos_to_gridY(int Y);
};

#endif
