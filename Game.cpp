#include "Game.h"


Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}

//Functions

void Game::getMapData()
{
    //Initialize the layers for tile data
    layer1 = layer2 = nullptr;

    //Read map data into the file stream object
    ifstream in("graphics/map.map");

    //IF(Map was not loaded successfully into input stream object)
    if(!in.good())

        //Print error message to terminal
        printf("The map did not load.");

    //Declare a string to use as a buffer
    string buffer;

    //1. Get the line that says 'destWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    sheetWidth = atoi(buffer.c_str());

    //2. Get the line that says 'destHeight'
    getline(in, buffer, ' ');
    //Get the height value
    getline(in, buffer, '\n');
    sheetHeight = atoi(buffer.c_str());

    //3. Get the line that says 'mapWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapCol = atoi(buffer.c_str());

    //4. Get the line that says 'mapHeight'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapRow = atoi(buffer.c_str());

    //5. Get the line that says 'tile_width'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileWidth = atoi(buffer.c_str());

    //6. Get the line that says 'tile_height'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileHeight = atoi(buffer.c_str());

    //7. Get the line that says 'solid_tiles'
    getline(in, buffer, '\n');

    //8. Get the row containing solid tiles
    getline(in, buffer, '\n');

    //create a character pointer and copy buffer string into it
    char* str = new char[buffer.size() + 1];
    strcpy(str, buffer.c_str());

    //Create a character pointer for each token in solid tile string
    char* token = strtok(str, " ,");

    //WHILE(Not at the end of solid tile string)
    while(token != NULL)
    {
        //Push token as an integer onto vector
        solidTiles.push_back(atoi(token));
        //Get next token from solid tile string
        token = strtok(NULL, " ,");
    }

    //Delete string character pointer
    delete[] str;
    delete token;

    //Set up an array for each layer before getting data for each layer
    layer1 = new int[mapCol * mapRow];
    layer2 = new int[mapCol * mapRow];

    //9. Get the row containing 'layer1' of background (not solid tiles)
    getline(in, buffer, '\n');

    int i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer1[i] = data;                   //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    //10. Get the row containing 'layer2' of solid tile placement
    getline(in, buffer, '\n');

    i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer2[i] = data;                   //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    //Close input stream object
    in.close();

}//END loadMap

SDL_Surface* Game::loadImage(const char* path)
{
    //Load image at specified path
    tempSurface = SDL_LoadBMP(path);

    //IF(Surface could not be loaded)
    if( tempSurface == NULL )
    {
        //Print error message to terminal
        printf("Unable to load image at path: %s\n", path);
    }

    //Return surface created from bitmap
    return tempSurface;

}//END loadImage

SDL_Texture* Game::loadTexture(SDL_Surface* tempSurface)
{
    //Declare a texture pointer to load and return
    SDL_Texture* newTexture = NULL;

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    //IF(Texture was not created)
    if( newTexture == NULL )
    {
        //Display error message to terminal
        printf("Unable to create texture\n");
    }

    //Free memory allocated for surface passed in
    SDL_FreeSurface(tempSurface);

    //Return texture pointer
    return newTexture;

}//END loadTexture

void Game::FillRect(SDL_Rect &rect, int x, int y, int width, int height)
{
    //Initialize the rectangle
    rect.x = x;         //initial x position of upper left corner
    rect.y = y;         //initial y position of upper left corner
    rect.w = width;     //width of rectangle
    rect.h = height;    //height of rectangle

}//END FillRect

void Game::CloseShop()
{
    //Destroy all objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    delete tempSurface;

    //SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureMap);

    //IF(Dino textures are not destroyed)
    if (!dinoDestroyed)

        //Destroy dino textures
        destroyDino();

    //IF(Skeleton 1 textures are not destroyed)
    if (!skelDestroyed)

        //Destroy skel 1 textures
        destroySkel();

    //IF(Skeleton 2 textures are not destroyed)
    if (!skel2Destroyed)

        //Destroy skel 2 texture
        destroySkel2();


    //IF(Zombie textures are not destroyed)
    if (!zombieDestroyed)
    {
        //Destroy zombie textures
        destroyZombie();
    }

    //IF(There explosion textures are not destroyed)
    if(!explosionDestroyed || !explosion2Destroyed || !explosion3Destroyed)
    {
        //Destroy explosion textures
        destroyExplosion();
    }

    //Destroy pointers to layer arrays
    if (layer1 != NULL)
    {
        delete[] layer1;
        layer1 = NULL;
    }
    if (layer2 != NULL)
    {
        delete[] layer2;
        layer2 = NULL;
    }

    //Release vector memory
    solidTiles.clear();

    //Quit the program
    SDL_Quit();

}//End Close Shop

void Game::destroyDino()
{
    //Delete all memory allocated for dino textures
    SDL_DestroyTexture(dinoSouthSheet);
    SDL_DestroyTexture(dinoNorthSheet);
    SDL_DestroyTexture(dinoEastSheet);
    SDL_DestroyTexture(dinoWestSheet);
    SDL_DestroyTexture(dinoSheetGeneric);
}
void Game::destroySkel()
{
    //Delete all memory allocated for skel 1 textures
    SDL_DestroyTexture(skelEastSheet);
    SDL_DestroyTexture(skelWestSheet);
    SDL_DestroyTexture(skelSheetEW);
}

void Game::destroySkel2()
{
    //Delete all memory allocated for skel 2 textures
    SDL_DestroyTexture(skelNorthSheet);
    SDL_DestroyTexture(skelSouthSheet);
    SDL_DestroyTexture(skelSheetNS);
}

void Game::destroyZombie()
{
    //Delete all memory allocated for zombie textures
    SDL_DestroyTexture(textureZombieEastSheet);
    SDL_DestroyTexture(textureZombieWestSheet);
    SDL_DestroyTexture(textureZombieSheetEW);
}

void Game::destroyExplosion()
{
    //Delete all memory allocated for explosion textures
    SDL_DestroyTexture(explosionSheet);
    SDL_DestroyTexture(explosionSheet2);
    SDL_DestroyTexture(explosionSheet3);
}

SDL_Surface* Game::setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b)
{
    //Declare an unsigned into to hold color key
    Uint32 colorKey;

    //Get color key value using rgb values passed in
    colorKey = SDL_MapRGB(surface->format, r, g, b);

    //Set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);

    //Return the surface w transparent color set
    return surface;

}//END setTransparentColor

bool Game::ProgramIsRunning()
{
    //Declare sdl event object
    SDL_Event event;

    //Declare boolean to indicate game is running
    bool running = true;

    //WHILE(Polling for an event)
    while(SDL_PollEvent(&event))
    {
        //IF(Event is user quitting with x button)
        if(event.type == SDL_QUIT)

            //Set game running boolean to false
            running = false;
    }

    //Return boolean indicating game if is running or not
    return running;
}

bool Game::InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    return true;
}

bool Game::InitSDL()
{
    //Declare a boolean to indicate if game was initialized
    bool init = true;

    //IF(Initializing SDL fails)
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        //Print debug
        printf("SDL failed to initialize!\n");
        //QUIT SDL
        SDL_Quit();
        //Set initialer boolean to false
        init = false;
    }

    //Return initializer boolean
    return init;
}

void Game::CreateGameImages()
{
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Maze with monsters", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //load the tile sheet
    tempSurface = loadImage("graphics/towntiles.bmp");
    //no need to set transparent color for the tiles
    textureMap = loadTexture(tempSurface);
    //get the map data and create the solid layer
    getMapData();
    generateSolidLayer();

    //load all dino stuff
    //Set up East walking dinos
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/dinoEast.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 97, 68, 43);
    //create the texture for the whole sheet
    dinoEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dino walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dinos
    SDL_QueryTexture(dinoEastSheet, NULL, NULL, &rectDinoEastSheet.w, &rectDinoEastSheet.h);
    //get the width of one frame (one Dino) by dividing the width of the sheet by the number of frames
    dinoWidth = rectDinoEastSheet.w/MAX_DINO_FRAME;
    //get the height of one frame
    dinoHeight = rectDinoEastSheet.h;  //height on one Dino is same as height of sheet
    //Initialize the rectangle dino east
    FillRect(rectDinoEast, 0, 0, dinoWidth, dinoHeight);

    //Set up West walking dinos
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/dinoWest.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 97, 68, 43);
    //create the texture for the whole sheet
    dinoWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dino walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dinos
    SDL_QueryTexture(dinoWestSheet, NULL, NULL, &rectDinoWestSheet.w, &rectDinoWestSheet.h);
    //get the width of one frame (one Dino) by dividing the width of the sheet by the number of frames
    dinoWidth = rectDinoWestSheet.w/MAX_DINO_FRAME;
    //get the height of one frame
    dinoHeight = rectDinoWestSheet.h;  //height on one dino is same as height of sheet
    //Initialize the rectangle for dino west
    FillRect(rectDinoWest, 0, 0, dinoWidth, dinoHeight);

    //Set up North walking dinos
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/WalkNorth/dinoNorth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 97, 68, 43);
    //create the texture for the whole sheet
    dinoNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dino walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dinos
    SDL_QueryTexture(dinoNorthSheet, NULL, NULL, &rectDinoNorthSheet.w, &rectDinoNorthSheet.h);
    //get the width of one frame (one Dino) by dividing the width of the sheet by the number of frames
    dinoWidth = rectDinoNorthSheet.w/MAX_DINO_FRAME;
    //get the height of one frame
    dinoHeight = rectDinoNorthSheet.h;  //height on one dino is same as height of sheet
    //Initialize the rectangle for dino north
    FillRect(rectDinoNorth, 0, 0, dinoWidth, dinoHeight);

    //Set up South walking dinos
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/WalkSouth/dinoSouth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 97, 68, 43);
    //create the texture for the whole sheet
    dinoSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dino walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dinos
    SDL_QueryTexture(dinoSouthSheet, NULL, NULL, &rectDinoSouthSheet.w, &rectDinoSouthSheet.h);
    //get the width of one frame (one Dino) by dividing the width of the sheet by the number of frames
    dinoWidth = rectDinoSouthSheet.w/MAX_DINO_FRAME;
    //get the height of one frame
    dinoHeight = rectDinoSouthSheet.h;  //height on one dino is same as height of sheet
    //Initialize the rectangle for dino south
    FillRect(rectDinoSouth, 0, 0, dinoWidth, dinoHeight);

    //load all skel stuff
    //Set up East walking skel
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/walkEast1.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    skelEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one skel walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of skel
    SDL_QueryTexture(skelEastSheet, NULL, NULL, &rectSkelEastSheet.w, &rectSkelEastSheet.h);
    //get the width of one frame (one Skel) by dividing the width of the sheet by the number of frames
    skelWidth = rectSkelEastSheet.w/MAX_SKEL_FRAME;
    //get the height of one frame
    skelHeight = rectSkelEastSheet.h;  //height on one skel is same as height of sheet
    //Initialize the rectangle skel east
    FillRect(rectSkelEast, 0, 0, skelWidth, skelHeight);

    //Set up West walking skel
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/walkWest1.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    skelWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one skel walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of skel
    SDL_QueryTexture(skelWestSheet, NULL, NULL, &rectSkelWestSheet.w, &rectSkelWestSheet.h);
    //get the width of one frame (one Skel) by dividing the width of the sheet by the number of frames
    skelWidth = rectSkelWestSheet.w/MAX_SKEL_FRAME;
    //get the height of one frame
    skelHeight = rectSkelWestSheet.h;  //height on one skel is same as height of sheet
    //Initialize the rectangle skel west
    FillRect(rectSkelWest, 0, 0, skelWidth, skelHeight);

    //Set up North walking skel
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/WalkNorth/walkNorth1.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    skelNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one skel walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of skel
    SDL_QueryTexture(skelNorthSheet, NULL, NULL, &rectSkelNorthSheet.w, &rectSkelNorthSheet.h);
    //get the width of one frame (one Skel) by dividing the width of the sheet by the number of frames
    skelWidth = rectSkelNorthSheet.w/MAX_SKEL_FRAME;
    //get the height of one frame
    skelHeight = rectSkelNorthSheet.h;  //height on one skel is same as height of sheet
    //Initialize the rectangle skel east
    FillRect(rectSkelNorth, 0, 0, skelWidth, skelHeight);

    //Set up South walking skel
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/WalkSouth/walkSouth1.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    skelSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one skel walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of skel
    SDL_QueryTexture(skelSouthSheet, NULL, NULL, &rectSkelSouthSheet.w, &rectSkelSouthSheet.h);
    //get the width of one frame (one Skel) by dividing the width of the sheet by the number of frames
    skelWidth = rectSkelSouthSheet.w/MAX_SKEL_FRAME;
    //get the height of one frame
    skelHeight = rectSkelSouthSheet.h;  //height on one skel is same as height of sheet
    //Initialize the rectangle skel west
    FillRect(rectSkelSouth, 0, 0, skelWidth, skelHeight);

    //load all zombie stuff
    //Set up East walking zombie
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieEastSheet, NULL, NULL, &rectZombieEastSheet.w, &rectZombieEastSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieEastSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieEastSheet.h;  //height on one skel is same as height of sheet
    //Initialize the rectangle skel east
    FillRect(rectZombieEast, 0, 0, zombieWidth, zombieHeight);

    //Set up West walking zombie
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieWestSheet, NULL, NULL, &rectZombieWestSheet.w, &rectZombieWestSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieWestSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieWestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectZombieWest, 0, 0, zombieWidth, zombieHeight);

    //Set up explosion 1
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/explosion.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    explosionSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(explosionSheet, NULL, NULL, &rectExplosionSheet.w, &rectExplosionSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    explosionWidth = rectExplosionSheet.w/MAX_EXPLOSION_FRAME;
    //get the height of one frame
    explosionHeight = rectZombieWestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectExplosion, 0, 0, explosionWidth, explosionHeight);

    //Set up explosion 2
    //create the surface - load the explosion sheet
    tempSurface = loadImage("graphics/explosion.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    explosionSheet2 = loadTexture(tempSurface);
    //Initialize the rectangle zombie west
    FillRect(rectExplosion2, 0, 0, explosionWidth, explosionHeight);

    //Set up explosion 3
    //create the surface - load the explosion sheet
    tempSurface = loadImage("graphics/explosion.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    explosionSheet3 = loadTexture(tempSurface);
    //Initialize the rectangle zombie west
    FillRect(rectExplosion3, 0, 0, explosionWidth, explosionHeight);

    //Load player wins background
    tempSurface = loadImage("graphics/playerWins.bmp");
    //Create texture
    textureWin = loadTexture(tempSurface);
    //Create a rectangle at position 0, 0 for player wins background
    FillRect(rectPlayerWin, 0, 0, 640, 640);

    //Load player loses background
    tempSurface = loadImage("graphics/playerLoses.bmp");
    //Create texture
    textureLose = loadTexture(tempSurface);
    //Create a rectangle at position 0, 0 for player loses background
    FillRect(rectPlayerLose, 0, 0, 640, 640);

    //Load game instructions bitmap
    tempSurface = loadImage("graphics/instructGame.bmp");
    //Create texture
    textureInstruct = loadTexture(tempSurface);
    //Create a rectangle at position 120, 0 for game instructions
    FillRect(rectInstruct, 120, 0, 400, 62);


}

void Game::generateSolidLayer()   //create a structure to hold solid tile data
{
    int begIndex = 0;   //position of solid tile in tiles structure
    for (int row = 0; row < mapRow; ++row)
    {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer2[row*mapCol+col];//get frame ID of solid tile from layer 2
                if (frame > 0)  //if the frame is not zero
                {
                    if (isTileSolid(layer2[row*mapCol+col])) //if tile is solid
                    {
                        tiles[begIndex].solid = 1;              //then solid field is 1
                        tiles[begIndex].frameID = frame;        //save the frame ID
                        tiles[begIndex].x = col*tileWidth;      //save THIS x position of the solid tile
                        tiles[begIndex].y = row*tileHeight;     //save THIS y position of the solid tile
                        begIndex = begIndex + 1;                //move to the next tile
                    }//end if tile is solid
                }
            }
    }
    numSolidTiles = begIndex - 1;
}

bool Game::isTileSolid(int tile)
{
    for(unsigned int i = 0; i < solidTiles.size(); i++)
    {
        if(solidTiles[i] == tile)
        {
            return true;
        }
    }
    return false;
}

void Game::drawLayer(int layer[])
{
    //Draw a Layer
        for (int row = 0; row < mapRow; ++row)
        {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer[row*mapCol+col];//get frame ID number to draw
                //if frameID is not a zero then grab it from sheet
                if(frame > 0)
                {
                    //Compute the x and y destination tile position
                    int xRawBytes = ((frame - 1) * (tileWidth + 2));
                    int yRawBytes = ((frame - 1) * (tileHeight + 2));
                    int xSource = (xRawBytes) % (sheetWidth) - ((frame-1)/8)*2;
                    int ySource = (yRawBytes / (sheetWidth)) * (tileHeight + 2);

                    //Initialize the source rectangle with position of tile you want to draw
                    FillRect(srcRect, xSource, ySource, tileWidth, tileHeight);

                    // Compute the x and y location on game screen
                    int xDest = col*tileWidth;
                    int yDest = row*tileHeight;

                    //Initialize the destination rectangle with x and y position on game screen
                    FillRect(dstRect, xDest, yDest, tileWidth, tileHeight);

                    //Grab tile from sheet and place on game screen
                    SDL_RenderCopy(renderer, textureMap, &srcRect, &dstRect);

                }//end if frame > 0
            }//end for col
        }//end for row
}
void Game::UpdateDino()
{
    //Increment time to delay each frame
    FrameCounter++;
    //If delay time is greater than max amount
    if(FrameCounter > FRAME_DELAY)
    {
        FrameCounter = 0;   //start new delay time
        SpriteFrame++;      //advance to next frame
    }
    if(SpriteFrame > MAX_DINO_FRAME - 1) //if last frame is reached
        SpriteFrame = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (dinoDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectDinoEast.x = SpriteFrame * rectDinoEast.w;
        if ( xDinoPos > SCREEN_WIDTH - DINO_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            xDinoPos = SCREEN_WIDTH - DINO_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for dino placement
        FillRect(rectDino, rectDinoEast.x, rectDinoEast.y, rectDinoEast.w, rectDinoEast.h);
        dinoSheetGeneric = dinoEastSheet;
    }// end walking east

    if (dinoDirection == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectDinoWest.x = SpriteFrame * rectDinoWest.w;
       // xDinoPos = xDinoPos - DINO_SPEED;              //decrement x
        if ( xDinoPos < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            xDinoPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dino placement
        FillRect(rectDino, rectDinoWest.x, rectDinoWest.y, rectDinoWest.w, rectDinoWest.h);
        dinoSheetGeneric = dinoWestSheet;
    }// end walking West

    if (dinoDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectDinoSouth.x = SpriteFrame * rectDinoSouth.w;
        //yDinoPos = yDinoPos + DINO_SPEED;                       //increment y
        if ( yDinoPos > SCREEN_HEIGHT - DINO_HEIGHT )    //check bottom
        {
            yDinoPos = SCREEN_HEIGHT - DINO_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for dino placement
        FillRect(rectDino, rectDinoSouth.x, rectDinoSouth.y, rectDinoSouth.w, rectDinoSouth.h);
        //copy the south facing dinos to the generic sheet
        dinoSheetGeneric = dinoSouthSheet;
    }// end walking south

    if (dinoDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectDinoNorth.x = SpriteFrame * rectDinoNorth.w;
        //yDinoPos = yDinoPos - DINO_SPEED;                       //decrement y
        if ( yDinoPos < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            yDinoPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dino placement
        FillRect(rectDino, rectDinoNorth.x, rectDinoNorth.y, rectDinoNorth.w, rectDinoNorth.h);
        //copy the north facing dino to the generic dino sheet
        dinoSheetGeneric = dinoNorthSheet;
    }// end walking north

    //IF(Dino is at end of maze)
    if(xDinoPos > 576 && yDinoPos > 512)

        //Set boolean indicating player sprite is at exit to true
        atMazeExit = true;

    //Initialize the placement rectangle to the position on the screen the dino will be drawn.
    FillRect(rectDinoPlacement, xDinoPos, yDinoPos, dinoWidth, dinoHeight);

    //Now check to see if the dino has collided with a solid tile
    PlayerTileCollision(rectDinoPlacement, xDinoPos, yDinoPos);

    //Now check to see if the dino has collided with skelton 1
    playerEnemyCollide(rectDinoPlacement, rectSkelPlacementEW);
    //Now check to see if the dino has collided with skelton 1
    playerEnemyCollide(rectDinoPlacement, rectSkelPlacementNS);
    //Now check to see if the dino has collided with zombie
    playerEnemyCollide(rectDinoPlacement, rectZombiePlacementEW);
}

void Game::updateEastWestSkel()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterSkelEW++;
    //If delay time is greater than max amount
    if(FrameCounterSkelEW > FRAME_DELAY)
    {
        FrameCounterSkelEW = 0;   //start new delay time
        SpriteFrameSkelEW++;      //advance to next frame
    }
    if(SpriteFrameSkelEW > MAX_SKEL_FRAME - 1) //if last frame is reached
        SpriteFrameSkelEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (skelDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectSkelEast.x = SpriteFrameSkelEW * rectSkelEast.w;
        xSkelEWpos = xSkelEWpos + SKEL_SPEED;              //increment x
        //create the rectangle on screen for skel placement
        FillRect(rectSkelEW, rectSkelEast.x, rectSkelEast.y, rectSkelEast.w, rectSkelEast.h);
        skelSheetEW = skelEastSheet;
    }// end walking east

    if (skelDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectSkelWest.x = SpriteFrameSkelEW * rectSkelWest.w;
        xSkelEWpos = xSkelEWpos - SKEL_SPEED;              //decrement x
        //create the rectangle on screen for skel placement
        FillRect(rectSkelEW, rectSkelWest.x, rectSkelWest.y, rectSkelWest.w, rectSkelWest.h);
        skelSheetEW = skelWestSheet;
    }// end walking West

    //Now check to see if the skel has collided with a solid tile
    collision = EnemyTileCollision(rectSkelPlacementEW, skelDirectionEW);

    if (collision == true)
    {
        FrameCounterSkelEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the skel will be drawn.
    FillRect(rectSkelPlacementEW, xSkelEWpos, ySkelEWpos, skelWidth, skelHeight);
}

void Game::updateNorthSouthSkel()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterSkelNS++;
    //If delay time is greater than max amount
    if(FrameCounterSkelNS > FRAME_DELAY)
    {
        FrameCounterSkelNS = 0;   //start new delay time
        SpriteFrameSkelNS++;      //advance to next frame
    }
    if(SpriteFrameSkelNS > MAX_SKEL_FRAME - 1) //if last frame is reached
        SpriteFrameSkelNS = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (skelDirectionNS == 2)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectSkelSouth.x = SpriteFrameSkelNS * rectSkelSouth.w;
        ySkelNSpos = ySkelNSpos + SKEL_SPEED;              //increment x
        //create the rectangle on screen for skel placement
        FillRect(rectSkelNS, rectSkelSouth.x, rectSkelSouth.y, rectSkelSouth.w, rectSkelSouth.h);
        skelSheetNS = skelSouthSheet;
    }// end walking south

    if (skelDirectionNS == 3)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectSkelNorth.x = SpriteFrameSkelNS * rectSkelNorth.w;
        ySkelNSpos = ySkelNSpos - SKEL_SPEED;              //decrement x
        //create the rectangle on screen for skel placement
        FillRect(rectSkelNS, rectSkelNorth.x, rectSkelNorth.y, rectSkelNorth.w, rectSkelNorth.h);
        skelSheetNS = skelNorthSheet;
    }// end walking North

    //Now check to see if the skel has collided with a solid tile
    collision = EnemyTileCollision(rectSkelPlacementNS, skelDirectionNS);

    if (collision == true)
    {
        FrameCounterSkelNS = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the skel will be drawn.
    FillRect(rectSkelPlacementNS, xSkelNSpos, ySkelNSpos, skelWidth, skelHeight);
}

void Game::updateExplosion()
{
    //IF(Explosion with zombie happens)
    if(explosion && !explosionDestroyed)
    {
        //Increment time to delay each frame
        FrameCounterExplosion++;
        //If delay time is greater than max amount
        if(FrameCounterExplosion > FRAME_DELAY)
        {
            FrameCounterExplosion = 0;   //start new delay time
            SpriteFrameExplosion++;      //advance to next frame
        }

        //Advance explosion rectangle to next frame in sheet
        rectExplosion.x = SpriteFrameExplosion* rectExplosion.w;

        //Initialize the placement rectangle to the position on the screen the skel will be drawn.
        FillRect(rectExplosionPlacement, xZombieEWpos, yZombieEWpos, explosionWidth, explosionHeight);

        //IF(last frame is reached)
        if(SpriteFrameExplosion > MAX_EXPLOSION_FRAME - 1)
        {
            //Destroy explosion texture
            SDL_DestroyTexture(explosionSheet);

            explosionDestroyed = true;
        }


    }//END IF(Explosion with zombie)

    //IF(Explosion with skeleton 1 happens)
    if(explosion2 && !explosion2Destroyed)
    {
        //Increment time to delay each frame
        FrameCounterExplosion2++;

        //If delay time is greater than max amount
        if(FrameCounterExplosion2 > FRAME_DELAY)
        {
            FrameCounterExplosion2 = 0;   //start new delay time
            SpriteFrameExplosion2++;      //advance to next frame
        }

        //Advance explosion rectangle to next frame in sheet
        rectExplosion2.x = SpriteFrameExplosion2 * rectExplosion2.w;

        //Initialize the placement rectangle to the position on the screen the skel will be drawn.
        FillRect(rectExplosion2Placement, xSkelEWpos, ySkelEWpos, explosionWidth, explosionHeight);

        //IF(last frame is reached)
        if(SpriteFrameExplosion2 > MAX_EXPLOSION_FRAME - 1)
        {
            //Destroy explosion texture
            SDL_DestroyTexture(explosionSheet2);

            explosion2Destroyed = true;
        }


    }//END IF(Explosion with skeleton 1)

    //IF(Explosion with skeleton 2 happens)
    if(explosion3 && !explosion3Destroyed)
    {
        //Increment time to delay each frame
        FrameCounterExplosion3++;

        //If delay time is greater than max amount
        if(FrameCounterExplosion3 > FRAME_DELAY)
        {
            FrameCounterExplosion3 = 0;   //start new delay time
            SpriteFrameExplosion3++;      //advance to next frame
        }

        //Advance explosion rectangle to next frame in sheet
        rectExplosion3.x = SpriteFrameExplosion3 * rectExplosion3.w;

        //Initialize the placement rectangle to the position on the screen the skel will be drawn.
        FillRect(rectExplosion3Placement, xSkelNSpos, ySkelNSpos, explosionWidth, explosionHeight);

        //IF(last frame is reached)
        if(SpriteFrameExplosion3 > MAX_EXPLOSION_FRAME - 1)
        {
            //Destroy explosion texture
            SDL_DestroyTexture(explosionSheet3);

            explosion3Destroyed = true;
        }


    }//END IF(Explosion with skeleton 1)

}//END updateExplosion

void Game::updateEastWestZombie()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterZombieEW++;
    //If delay time is greater than max amount
    if(FrameCounterZombieEW > FRAME_DELAY)
    {
        FrameCounterZombieEW = 0;   //start new delay time
        SpriteFrameZombieEW++;      //advance to next frame
    }
    if(SpriteFrameZombieEW > MAX_SKEL_FRAME - 1) //if last frame is reached
        SpriteFrameZombieEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (zombieDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectZombieEast.x = SpriteFrameZombieEW * rectZombieEast.w;
        xZombieEWpos = xZombieEWpos + ZOMBIE_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieEast.x, rectZombieEast.y, rectZombieEast.w, rectZombieEast.h);
        textureZombieSheetEW = textureZombieEastSheet;
    }// end walking east

    if (zombieDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectZombieWest.x = SpriteFrameZombieEW * rectZombieWest.w;
        xZombieEWpos = xZombieEWpos - SKEL_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieWest.x, rectZombieWest.y, rectZombieWest.w, rectZombieWest.h);
        textureZombieSheetEW = textureZombieWestSheet;
    }// end walking West

    //Now check to see if the zombie has collided with a solid tile
    collision = EnemyTileCollision(rectZombiePlacementEW, zombieDirectionEW);

    if (collision == true)
    {
        FrameCounterZombieEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the zombie will be drawn.
    FillRect(rectZombiePlacementEW, xZombieEWpos, yZombieEWpos, zombieWidth, zombieHeight);
}

void Game::RunGame()
{
    UpdateDino();              //Update position of dino
    updateEastWestSkel();     //Update position of east/west skel
    updateNorthSouthSkel();
    updateEastWestZombie();     //Update position of east/west skel
    updateExplosion();
    DrawGame();
}

void Game::DrawGame()
{
    //Increment game timer
    frameTimer++;

    //IF(Frame time is less than frame delay constant)
    if(frameTimer > FRAME_DELAY)
    {
        frameTimer = 0;
        gameTime ++;

    }



    //set up a buffer to store window caption in
    char scores[64];

    //store the updated scores in the buffer
    sprintf(scores, "Timer: %d", gameTime/10);

    //print the scores in the game window caption
    SDL_SetWindowTitle(gameWindow, scores);

    SDL_RenderPresent(renderer);

    //Clear the window
    SDL_RenderClear(renderer);

    //Copy the background)
    //SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

    //draw each layer
    drawLayer (layer1);
    drawLayer (layer2);

    //IF(Player wins)
    if(skelDestroyed && skel2Destroyed && zombieDestroyed && atMazeExit && gameTime <= 600)
    {
        //Update game window title to display game results
        sprintf(scores, "PLAYER WINS");
        //Print the message in game window title
        SDL_SetWindowTitle(gameWindow, scores);
        //Copy the player wins background
        SDL_RenderCopy(renderer, textureWin, NULL, &rectPlayerWin);
        //Present player wins background
        SDL_RenderPresent(renderer);
        SDL_Delay(100);  //delay a bit

    }
    //ELSE IF(Player has lost)
    else if(gameTime > 600)
    {
        //Update game window title to display game results
        sprintf(scores, "PLAYER LOSES");
        //Print the message in game window title
        SDL_SetWindowTitle(gameWindow, scores);
        //Copy the player loses background
        SDL_RenderCopy(renderer, textureLose, NULL, &rectPlayerLose);
        //Present player loses background
        SDL_RenderPresent(renderer);
        SDL_Delay(100);  //delay a bit
    }
    //ELSE(Game is not over)
    else
    {
        //Copy the player loses background
        SDL_RenderCopy(renderer, textureInstruct, NULL, &rectInstruct);
        //Render the dino
        SDL_RenderCopy(renderer, dinoSheetGeneric, &rectDino, &rectDinoPlacement);
        //Render skeleton one
        SDL_RenderCopy(renderer, skelSheetEW, &rectSkelEW, &rectSkelPlacementEW);
        //Render skeleton two
        SDL_RenderCopy(renderer, skelSheetNS, &rectSkelNS, &rectSkelPlacementNS);
        //Render the zombie
        SDL_RenderCopy(renderer, textureZombieSheetEW, &rectZombieEW, &rectZombiePlacementEW);
        //Render explosion 1
        SDL_RenderCopy(renderer, explosionSheet, &rectExplosion, &rectExplosionPlacement);
        //Render explosion 2
        SDL_RenderCopy(renderer, explosionSheet2, &rectExplosion2, &rectExplosion2Placement);
        //Render explosion 3
        SDL_RenderCopy(renderer, explosionSheet3, &rectExplosion3, &rectExplosion3Placement);
        //display the game screen
        SDL_RenderPresent(renderer);
        SDL_Delay(5);  //delay a bit


    }
}

//Note:  west = 0, east = 1, south = 2, north = 3
void Game::moveUp()
{
    //y's decrease to move north
    //if we are not facing north
    if (dinoDirection != 3)
    {
        dinoDirection = 3;    //then face north
        FrameCounter = 0;       //start new position
    }

    //Move dino sprite north
    yDinoPos -= DINO_SPEED;
}

void Game::moveDown()
{
    //y's increase to move south
    //if we are not facing south
    if (dinoDirection != 2)
    {
        dinoDirection = 2;     //then face south
        FrameCounter = 0;        //start new position
    }

    //Move dino sprite south
    yDinoPos += DINO_SPEED;
}

void Game::moveLeft()
{
    //x's increase to move west
    //if we are not facing west
    if (dinoDirection != 0)
    {
        dinoDirection = 0;    //then face west
        FrameCounter = 0;       //start new position
    }

    //Move dino west
    xDinoPos -= DINO_SPEED;
}

void Game::moveRight()
{
    //x's decrease to move east
    //if we are not facing east
    if (dinoDirection != 1)
    {
        dinoDirection = 1;    //then face east
        FrameCounter = 0;       //start new position
    }

    //Move dino east
    xDinoPos += DINO_SPEED;
}

void Game::Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos )
{

    bool intersectsTop = false;     //Collision with top of obstacle
    bool intersectsRight = false;   //Collision with right side of obstacle
    bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)

    if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
        intersectsRight = true;         //then the collision is on the right side of the obstacle
    // y up is neg
    if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
        intersectsTop = true;           //then the collision is on the top of the obstacle

    //compute the height & width of the intersection rectangle (where the two rects collide)
    //to determine which side of the obstacle the movingRect hit
    int cHeight, cWidth;

    if(intersectsTop)  //if the collision is on the top of the obstacle
    {
       cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
    }
    else    //else collision was on bottom, left, or right
    {
        cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
    }
    if(intersectsRight) //if the collision is on the right of the obstacle
    {
         cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
    }
    else    //else collision was on left, top, or bottom
    {
        cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
    }

    if (cHeight > cWidth) //was the collision from the left or the right?
        moveInXDirection = true;    //if so then moving in the x direction is true
    else
        moveInXDirection = false;   //if not then moving in the x direction is false

    //now determine which direction the sprite needs to back off from
    if(moveInXDirection)            //if moving left or right
    {
        if(intersectsRight)         //if colliding on the right
        {
            xPos = xPos + 1;    //stop by increasing x (move back right)
        }
        else                        //else colliding on the left
        {
            xPos = xPos - 1;    //stop by decreasing x (move back left)
        }
    }
    else                            //else moving up or down
    {
        if(intersectsTop)           //if colliding with top
        {
            yPos = yPos - 1;    //stop by decreasing y (move back up)
        }
        else                        //else colliding with bottom
        {
            yPos = yPos + 1;    //stop by increasing y (move back down)
        }
    }//end: back off, bub.
}//end Collide

void Game::PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos)
{
    //loop indexes to cycle through solid tile vector array
    int begIndex = 0;
    //int endIndex = solidTiles.size();
    int endIndex = numSolidTiles;

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;

    for (int i = begIndex; i <= endIndex; i++)
    {
        //Get the x and y location of solid tile
        int xDest = tiles[i].x;
        int yDest = tiles[i].y;

        //make the solid tile a rectangle
        FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

        //make the solid rectangle a constant
        const SDL_Rect& solid = solidTileRect;

        //now check to see if the moving rectangle (sprite) has intersected a solid tile
        if( SDL_HasIntersection(&moving, &solid))
        {
            //the moving sprite has collided with the solid tile so stop that sprite in its tracks
            Collide(movingRect, solidTileRect, xPos, yPos);
        }//end if intersected
    }//end for
}//end tile collision

bool Game::EnemyTileCollision(SDL_Rect movingRect, int &direction)
{
    //loop indexes to cycle through solid tile vector array
    int i = 0;
    //int endIndex = solidTiles.size();

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;
    bool done = false;

        while (i <= numSolidTiles && done == false)
        {
            //Get the x and y location of solid tile
            int xDest = tiles[i].x;
            int yDest = tiles[i].y;

            //make the solid tile a rectangle
            FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

            //make the solid rectangle a constant
            const SDL_Rect& solid = solidTileRect;

            //now check to see if the moving rectangle (sprite) has intersected a solid tile
            if(SDL_HasIntersection(&moving, &solid))
            {
                //the moving sprite has collided with the solid tile so stop that sprite in its tracks
                enemyCollide(movingRect, solidTileRect, direction);
                done = true;       //done checking solid tiles as soon as 1 collision occurs
            }//end if intersected
            i++;
        }//end while
        if (done == true)
            return true;
        else
            return false;
}//end tile collision

void Game::enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction)
{
    {
        //now determine what side of the obstacle did the movingRect collide with?
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        // adjust moving object's position accordingly
        if(moveInXDirection == true)            //if moving left or right
        {
            if(intersectsRight == true)         //if colliding on the right
            {
                direction = 1;    //change direction to left
            }
            //else
            if(intersectsRight == false)
                //else colliding on the left
            {
                direction = 0;    //change direction to right
            }
        }
        //else                            //else moving up or down
        if (moveInXDirection == false)
        {
            if(intersectsTop == true)           //if colliding with top
            {
                direction = 3;    //then change direction to up
            }
            //else                        //else colliding with bottom
            if(intersectsTop == false)
            {
                direction = 2;    //then change direction to down
            }
        }//end adjustment of direction
     }//end check if there is a collision
}//end Collide

void Game::playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect)
{
    //make the moving rectangle a constant
    const SDL_Rect& PLAYER_RECT = playerRect;
    //make the solid rectangle a constant
    const SDL_Rect& ENEMY_RECT = enemyRect;
    //create a constant rectangle with current zombie position for comparing
    const SDL_Rect& CONST_ZOMBIE_RECT = rectZombiePlacementEW;
    //create a constant rectangle with current skel position for comparing
    const SDL_Rect& CONST_SKEL_RECT = rectSkelPlacementEW;
    //create a constant rectangle with current skel position for comparing
    const SDL_Rect& CONST_SKEL2_RECT = rectSkelPlacementNS;


   if (CheckCollision(playerRect, enemyRect))
    {
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_ZOMBIE_RECT))
        {
            zombieDestroyed = true;
            destroyZombie();
            explosion = true;
        }
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_SKEL_RECT))
        {
            skelDestroyed = true;
            destroySkel();
            explosion2 = true;
        }
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_SKEL2_RECT))
        {
            skel2Destroyed = true;
            destroySkel2();
            explosion3 = true;
        }
    }
}
bool Game::CheckCollision(SDL_Rect r1,SDL_Rect r2)
{
    //check to see if rectangles overlap
    //if not, return false
    if(r1.x > r2.x+r2.w)
    {
      return false;
    }
    else if(r1.y > r2.y+r2.h)
    {
      return false;
    }
    else if(r1.x+r1.w < r2.x)
    {
      return false;
    }
    else if(r1.y+r1.h < r2.y)
    {
      return false;
    }
    else
    {
      return true;
    }
}




