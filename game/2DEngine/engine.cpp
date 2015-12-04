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
    if (!enemyTexture.initialize(graphics, ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy texture"));

    // background
    if (!background.initialize(graphics,0,0,0,&backgroundTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
    // character
    if (!character.initialize(this,playerNS::WIDTH,playerNS::HEIGHT,6,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));
    // boxTest
    if (!boxTest.initialize(graphics, terrainNS::WIDTH, terrainNS::HEIGHT, 0, &groundTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boxTest"));
    boxTest.setX(0);
    boxTest.setY(0);
    if (!enemies[0].initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, 3, &enemyTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy"));
	if (!endFlag.initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 5, &groundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing end flag"));
	endFlag.setX(600);
	endFlag.setY(GAME_HEIGHT - terrainNS::HEIGHT);

    num_of_enemies ++;
    // terrain
    int dis = 20;
    for (int i = 0; i < 5; ++i)
    {
        if (!ground[i].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 5, &groundTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground"));
        ground[i].setX(GAME_WIDTH / 2 + 10 + dis);
        current_terrain ++;
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

	// EDITING CONTROLS
	if (editmode)
	{
	    if(input->getMouseLButton())
	        mTime ++;
	    if (mTime > 0 && !input->getMouseLButton())
	    {
	        if (current_asset == 0) 
	        {
	            if (!ground[current_terrain].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 5, &groundTexture))
	                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground"));
	            // MessageBox(NULL, "HELLo", "Error", MB_OK);
	            ground[current_terrain].setX((input->getMouseX() / 20) * 20);
	    		ground[current_terrain].setY((input->getMouseY() / 20) * 20);
	    		ground[current_terrain].setStartX((input->getMouseX() / 20) * 20);
	    		ground[current_terrain].setStartY((input->getMouseY() / 20) * 20);
                ground[current_terrain].update(frameTime);
	            mTime = 0;
                if (ground[current_terrain].getInitialized())
                {
                    current_terrain++;
                }
	        }
	        else if (current_asset == 1) 
	        {
	            if (!enemies[num_of_enemies].initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, 3, &enemyTexture))
	                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemies"));
	            // MessageBox(NULL, "HELLo", "Error", MB_OK);
	            enemies[num_of_enemies].setX((input->getMouseX() / 20) * 20);
	            enemies[num_of_enemies].setY((input->getMouseY() / 20) * 20);
	            enemies[num_of_enemies].setStartX((input->getMouseX() / 20) * 20);
	            enemies[num_of_enemies].setStartY((input->getMouseY() / 20) * 20);
                enemies[num_of_enemies].update(frameTime);
	            mTime = 0;
                if (enemies[num_of_enemies].getInitialized())
                {
                    num_of_enemies++;
                }
	        }
	        else if (current_asset == 2)
	        {
	        	character.setX(input->getMouseX());
	        	character.setY(input->getMouseY());
	        	character.setStartX(input->getMouseX());
	        	character.setStartY(input->getMouseY());
	        }
	        else if (current_asset == 3)
	        {
	        	endFlag.setX((input->getMouseX() / 20) * 20);
	        	endFlag.setX((input->getMouseY() / 20) * 20);
	        	endFlag.setStartX((input->getMouseX() / 20) * 20);
	        	endFlag.setStartY((input->getMouseY() / 20) * 20);
	        }
	    }
	} else {
	    // UPDATE PLAYING
	    prevX = character.getY();
		endFlag.update(frameTime);
		character.update(frameTime);
		
		
		for (int i = 0; i < 5; i++) 
		{
			ground[i].update(frameTime);
		}
	    
	    for (int i = 0; i < num_of_enemies; ++i)
	    {
	        enemies[i].update(frameTime);
	    }
	}
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
	for (int i = 0; i < current_terrain; i++)
	{	
		if (character.collides(endFlag, cv)) {
			initialize(hwnd);
		}
		if (character.collides(ground[i], cv))
		{
			if ((character.getY() + playerNS::HEIGHT - 2 ) <= ground[i].getY())
			{
				character.setX(character.getX());
				character.setY(character.getY());
				character.setVelocity(VECTOR2(0, 0));
				character.setJump(true);
			}
			else if (character.getY() >= (ground[i].getY() + 20 - 3))
			{
				character.setX(character.getX());
				character.setY(character.getY() + 4);
				character.setVelocity(VECTOR2(0, 0));
				character.setJump(true);
			}
			else if ((character.getX()) <= ground[i].getX())
			{
				character.setX(character.getX() - 3);
				character.setY(character.getY());
				character.setVelocity(VECTOR2(0, 0));
			}
			else if (character.getX() > (ground[i].getX()))
			{
				character.setX(character.getX() + 3);
				character.setY(character.getY());
				character.setVelocity(VECTOR2(0, 0));
			}
		}
	}

    for (int i = 0; i < num_of_enemies; i++)
    {
        if (character.collides(enemies[i], cv) && enemies[i].getActive())
        {
            if (character.getY() > prevX)
            {
                enemies[i].setActive(false);
                character.jump();
            } else {
                // MessageBox(NULL, "I die", "Error", MB_OK);
				initialize(hwnd);
            }
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
	for (int i = 0; i < current_terrain; ++i)
        ground[i].draw();
	

	endFlag.draw();

    for (int i = 0; i < num_of_enemies; ++i)
    {
        if (enemies[i].getActive())
            enemies[i].draw();
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

//=============================================================================
// Used to convert the mouse X coordinate to snap to screen coordinate
//=============================================================================
int Engine::mousepos_to_gridX(int x)
{
	int correctX = x;
	return correctX;
}

//=============================================================================
// Used to convert the mouse Y coordinate to snap to screen coordinate
//=============================================================================
int Engine::mousepos_to_gridY(int y)
{
	int correctY = y;
	return correctY;
}