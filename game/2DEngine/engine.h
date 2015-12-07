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
    // In game Textures
    TextureManager backgroundTexture;		// background texture
    TextureManager characterTexture;		// character texture
	TextureManager groundTexture;			// ground texture
    TextureManager enemyTexture;            // enemy texture
    TextureManager assetsTexture;           // assets texture
    TextureManager flagTexture;             // flag texture
    TextureManager spikeTexture;

    // In game objects
    Image   background;						// Background image
    Player  character;						// character image     
    Image   assetDisplay;                   // a box used for testing
	Terrain endFlag;						// a box used for restarting
    Terrain ground[764];                    // ground entity
    Terrain spike[200];
    Enemy   enemies[100];

    // Current Asset counters
    int current_terrain = 0;                // Used to keep track of the terrain 
                                            // we are adding
    int num_of_enemies = 0;                 // Used to keep track of the enemy
                                            // we are adding
    int num_of_spikes = 0;
    
    int mTime = 0;
    int current_asset = 0;                  // Current asset the click will add
    int prevY = 0;                          // Used for collision checking

    char *current_file;

public:
    // Constructor
    Engine();

    // Destructor
    virtual ~Engine();

    // Initialize the game
    void initialize(HWND hwnd);
    // Update the game
    void update();      
    void ai();
    // Collision handling          
    void collisions();  
    void render();      
    void releaseAll();
    void resetAll();
    // Used for writing to a file saving all assets currently on the map
    int saveAll();
	int mousepos_to_gridX(int X);
	int mousepos_to_gridY(int Y);
};

#endif
