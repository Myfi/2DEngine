#include "engine.h"
#include "audio.h"

Audio b = Audio();

using namespace std;

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
	b.release();
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
// Where we load all of our textures
// Called by: WinMain initially
//=============================================================================
void Engine::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    current_terrain = 0;
    num_of_enemies = 0;
	num_of_spikes = 0;

    // Textures
    if (!backgroundTexture.initialize(graphics,BACKGROUND_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));
    if (!characterTexture.initialize(graphics,CHARACTER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));
	if (!groundTexture.initialize(graphics, GROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground texture"));
    if (!enemyTexture.initialize(graphics, ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy texture")); 
    if (!assetsTexture.initialize(graphics, ASSET_DISPLAY))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing asset display texture"));
    if (!flagTexture.initialize(graphics, END_FLAG))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing end flag texture"));
    if (!spikeTexture.initialize(graphics, SPIKE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing end spike texture"));

	ifstream myfile(current_file);
	//ofstream myOutfile("Saves\\TestOut.txt");
	int x, y, type;
	int i = 0;
	int dis = 20;
	//If the file was opened sucessfully, read from it
	if (!myfile.fail()) {
		while (myfile >> type) {
			myfile >> x;
			myfile >> y;
			//myOutfile << type << " " << x << " " << y << "\n";

			//terrain
			if (type == 0) {
				if (!ground[i].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 5, &groundTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground"));
				ground[i].setX(x);
				ground[i].setY(y);
				ground[i].setStartX(x);
				ground[i].setStartY(y);
				current_terrain++;
				dis += 20;
				i++;
			}
			//Background
			if (type == 6) {
				if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
			}
			//Character
			if (type == 2) {
				if (!character.initialize(this, playerNS::WIDTH, playerNS::HEIGHT, 6, &characterTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));
				character.setX(x);                           // Character Starting Position
				character.setY(y);
				character.setStartX(x);                           // Character Starting Position
				character.setStartY(y);
				character.setVelocity(VECTOR2(0, -playerNS::SPEED));
			}
			//endflag
			if (type == 3) {
				if (!endFlag.initialize(this, 16, 32, 0, &flagTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing end flag"));
				endFlag.setX(x);
				endFlag.setY(y);
				endFlag.setStartX(x);
				endFlag.setStartY(y);
			}
			//Bird
			if (type == 1) {
				if (!enemies[num_of_enemies].initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, 3, &enemyTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemies"));
				// MessageBox(NULL, "HELLo", "Error", MB_OK);
				enemies[num_of_enemies].setX(x);
				enemies[num_of_enemies].setY(y);
				enemies[num_of_enemies].setStartX(x);
				enemies[num_of_enemies].setStartY(y);
				enemies[num_of_enemies].update(frameTime);
				mTime = 0;
				if (enemies[num_of_enemies].getInitialized())
				{
					num_of_enemies++;
				}
			}
			//Spike
			if (type == 4) {
				if (!spike[num_of_spikes].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 0, &spikeTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing spike"));
				spike[num_of_spikes].setX(x);
				spike[num_of_spikes].setY(y);
				spike[num_of_spikes].setStartX(x);
				spike[num_of_spikes].setStartY(y);
				mTime = 0;
				if (spike[num_of_spikes].getInitialized())
				{
					num_of_spikes++;
				}
			}
		}
	}
	myfile.close();
	//myOutfile.close();

    if (!assetDisplay.initialize(graphics, 32, 32, 1, &assetsTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boxTest"));
	assetDisplay.setX(100);
    assetDisplay.setY(5);
    assetDisplay.setFrameDelay(0.001);

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
		// If the Spacebar is pressed change the asset we are currently adding
		if(input->wasKeyPressed(SPACE_KEY))
		{
			if (current_asset > 3)
				current_asset = 0;
			else 
				current_asset++;

            if (current_asset == 0)
                assetDisplay.setCurrentFrame(0);
            else if (current_asset == 1)
                assetDisplay.setCurrentFrame(9);
            else if (current_asset == 2)
                assetDisplay.setCurrentFrame(3);
            else if (current_asset == 3)
                assetDisplay.setCurrentFrame(11);
            else if (current_asset == 4)
                assetDisplay.setCurrentFrame(4);
            assetDisplay.update(frameTime);
		}
	    if(input->getMouseLButton())
	        mTime ++;
	    if (mTime > 0 && !input->getMouseLButton())
	    {
	        if (current_asset == 0) 
	        {
	            if (!ground[current_terrain].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 0, &groundTexture))
	                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ground"));
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
                mTime = 0;
	        }
	        else if (current_asset == 3)
	        {
	        	endFlag.setX(input->getMouseX());
	        	endFlag.setY(input->getMouseY());
	        	endFlag.setStartX(input->getMouseX());
	        	endFlag.setStartY(input->getMouseY());
                mTime = 0;
	        }
            else if (current_asset == 4)
            {
                if (!spike[num_of_spikes].initialize(this, terrainNS::WIDTH, terrainNS::HEIGHT, 0, &spikeTexture))
                    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing spike"));
                spike[num_of_spikes].setX((input->getMouseX() / 20) * 20);
                spike[num_of_spikes].setY((input->getMouseY() / 20) * 20);
                spike[num_of_spikes].setStartX((input->getMouseX() / 20) * 20);
                spike[num_of_spikes].setStartY((input->getMouseY() / 20) * 20);
                mTime = 0;
                if (spike[num_of_spikes].getInitialized())
                {
                    num_of_spikes++;
                }
            }
	    }
        if (input->isKeyDown(CTRL_KEY) && input->wasKeyPressed(X_KEY))
        {
            deleteAll();
        }
        if (input->isKeyDown(CTRL_KEY) && input->wasKeyPressed(CHARACTER_DOWN_KEY))
        {
            saveAll();
        }
	} else {
	    // UPDATE PLAYING
	    prevY = character.getY();
		endFlag.update(frameTime);
		character.update(frameTime);
        assetDisplay.update(frameTime);
		
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
    if (!editmode)
    {
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
                if (character.getY() > prevY)
                {
                    enemies[i].setActive(false);
                    character.jump();
                } else {
    				initialize(hwnd);
                }
            }
        }

        for (int i = 0; i < num_of_spikes; i++)
        {
            if (character.collides(spike[i], cv) && spike[i].getActive())
            {
                initialize(hwnd);
            }
        }
    }
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
    for (int i = 0; i < num_of_spikes; ++i)
    {
        spike[i].draw();
    }
    if (editmode)
        assetDisplay.draw();
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
    enemyTexture.onLostDevice();
    assetsTexture.onLostDevice();
    flagTexture.onLostDevice();
    spikeTexture.onLostDevice();

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
    enemyTexture.onResetDevice();
    assetsTexture.onResetDevice();
    flagTexture.onResetDevice();
    spikeTexture.onResetDevice();

    Game::resetAll();
    return;
}
//=============================================================================
// Change the file path
//=============================================================================
void Engine::changeFile(const char* input)
{
	//throw(GameError(gameErrorNS::FATAL_ERROR, current_file + " || " + input));
	current_file = input;
	initialize(hwnd);
}

//=============================================================================
// Saves all of the currently initialized entities
//=============================================================================
int Engine::saveAll()
{
	ofstream writeFile(current_file);

	writeFile << 6 << " " << 0 << " " << 0 << "\n";

	writeFile << 2 << " " << character.getStartX() << " " << character.getStartY() << "\n";

	for (int i = 0; i < current_terrain; ++i) 
    {
		writeFile << 0 << " " << ground[i].getStartX() << " " << ground[i].getStartY() << "\n";
	}

	writeFile << 3 << " " << endFlag.getStartX() << " " << endFlag.getStartY() << "\n";

	for (int i = 0; i < num_of_enemies; ++i)
	{
		writeFile << 1 << " " << enemies[i].getStartX() << " " << enemies[i].getStartY() << "\n";
	}
	for (int i = 0; i < num_of_spikes; ++i)
	{

		writeFile << 4 << " " << spike[i].getStartX() << " " << spike[i].getStartY() << "\n";
	}
	writeFile.close();

	return 0;
}

//=============================================================================
// Cleans the current file
//=============================================================================
void Engine::deleteAll()
{
    ofstream writeFile(current_file);
    writeFile << 6 << " " << 0 << " " << 0 << "\n";
    writeFile << 2 << " " << character.getStartX() << " " << character.getStartY() << "\n";
    writeFile << 3 << " " << endFlag.getStartX() << " " << endFlag.getStartY() << "\n";
    initialize(hwnd);
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