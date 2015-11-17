#include "engine.h"

//=============================================================================
// Constructor
//=============================================================================
Engine::Engine()
{}

//=============================================================================
// Destructor
//=============================================================================
Engine::~Engine()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
// Where we load all of our textures
//=============================================================================
void Engine::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // background texture
    if (!backgroundTexture.initialize(graphics,BACKGROUND_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));
    if (!characterTexture.initialize(graphics,CHARACTER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));
	if (!groundTexture.initialize(graphics, GROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground texture"));

    // background
    if (!background.initialize(graphics,0,0,0,&backgroundTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
    // character
    if (!character.initialize(this,playerNS::WIDTH,playerNS::HEIGHT,0,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));
    // terrain
    int dis = 20;
    for (int i = 0; i < 5; ++i)
    {
        if (!ground[i].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 5, &groundTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground"));
        ground[i].setX(ground[i].getX() + dis);
        dis += 20;
    }
    character.setX(0);                           // Character Starting Position
    character.setY(0);
	character.setVelocity(VECTOR2(0, -playerNS::SPEED));

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Engine::update()
{
    if(input->getMouseLButton())
    {
        MessageBox(NULL, "Hello", "Error", MB_OK);
    }

    character.update(frameTime);
	ground[0].update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Engine::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Engine::collisions()
{
	VECTOR2 cv;
	for (int i = 0; i < 5; i++)
	{
		if (character.collides(ground[i], cv))
		{
			if ( (character.getY() + playerNS::HEIGHT ) >= ground[i].getY())
			{
				character.setX(character.getX());
				character.setY(character.getY());
				character.setVelocity(VECTOR2(0, 0));
                character.setJump(true);
			}
			// else {
			// 	character.setX(character.getX() - 1);
			// 	character.setY(character.getY());
			// 	character.setVelocity(VECTOR2(0, 0));
			// }
		}
	}

	// MessageBox(NULL, "HELLo", "Error", MB_OK);
	// character.setY(10);
	// character.setVelocity(VECTOR2(character.getVelocity().x, 0));
}

//=============================================================================
// Render game items
//=============================================================================
void Engine::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    background.draw();                      
    character.draw();
	for (int i = 0; i < 5; ++i)
	{
		ground[i].draw();
	}

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Engine::releaseAll()
{
    characterTexture.onLostDevice();
	groundTexture.onLostDevice();
    backgroundTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Engine::resetAll()
{
    backgroundTexture.onResetDevice();
    characterTexture.onResetDevice();
	groundTexture.onResetDevice();

    Game::resetAll();
    return;
}
