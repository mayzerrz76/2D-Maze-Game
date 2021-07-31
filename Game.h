#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

class Game
{
public:

    //Game Constructor/Destructor
    Game();
    virtual ~Game();

    //Generic Function prototypes
    bool InitSDL();
    void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
    SDL_Surface* loadImage(const char* path);
    SDL_Texture* loadTexture(SDL_Surface* surface);
    SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
    void CloseShop();
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
    bool ProgramIsRunning();
    bool InitGame();
    void CreateGameImages();
    void RunGame();
    void DrawGame();

    //Dino function prototypes
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

private:

    //Declare old time integer and call function to get ticks
    int frameTimer = 0;
    int gameTime = 0;

    //Tile function prototypes
    void getMapData();
    void generateSolidLayer();
    void drawLayer(int layer[]);
    bool isTileSolid(int tile);

    //Update player function
    void UpdateDino();

    //Skel function prototypes
    void updateEastWestSkel();
    void updateNorthSouthSkel();
    void updateEastWestZombie();

    //Collision function prototypes
    void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos );
    void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction);
    void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos);
    bool EnemyTileCollision(SDL_Rect movingRect, int &direction);
    void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect);
    bool CheckCollision(SDL_Rect r1,SDL_Rect r2);
    void updateExplosion();

    //Destruction function prototypes
    void destroySkel();
    void destroySkel2();
    void destroyDino();
    void destroyZombie();
    void destroyExplosion();

    //Create a window
    SDL_Window *gameWindow;

    //Textures
    //Create a texture for the tile sheet
    SDL_Texture* textureMap = NULL;

    //Dino Textures
    SDL_Texture* dinoEastSheet = NULL;              //sheet of east dinos texture
    SDL_Texture* dinoWestSheet = NULL;              //sheet of west dinos texture
    SDL_Texture* dinoNorthSheet = NULL;             //sheet of north dinos texture
    SDL_Texture* dinoSouthSheet = NULL;             //sheet of south dinos texture
    SDL_Texture* dinoSheetGeneric = NULL;           //generic sheet of dinos texture

    //Skeleton 1 Textures
    SDL_Texture* skelEastSheet = NULL;              //sheet of east skel texture
    SDL_Texture* skelWestSheet = NULL;              //sheet of west skel texture
    SDL_Texture* skelSheetEW = NULL;                //generic sheet of skel 1 texture

    //Skeleton 2 Textures
    SDL_Texture* skelNorthSheet = NULL;             //sheet of north skel texture
    SDL_Texture* skelSouthSheet = NULL;             //sheet of south skel texture
    SDL_Texture* skelSheetNS = NULL;                //generic sheet of skel 2 texture

    //Zombie Textures
    SDL_Texture* textureZombieEastSheet = NULL;     //sheet of east zombie texture
    SDL_Texture* textureZombieWestSheet = NULL;     //sheet of west zombie texture
    SDL_Texture* textureZombieSheetEW = NULL;       //generic sheet of zombie texture

    //Explosion texture
    SDL_Texture* explosionSheet = NULL;             //sheet of explosion texture
    SDL_Texture* explosionSheet2 = NULL;            //sheet of explosion texture
    SDL_Texture* explosionSheet3 = NULL;            //sheet of explosion texture

    //End of game textures
    SDL_Texture* textureWin = NULL;
    SDL_Texture* textureLose = NULL;

    //Game instructions texture
    SDL_Texture* textureInstruct = NULL;

    //Create a temporary surface because we always need one at hand
    SDL_Surface* tempSurface = NULL;

    //Create a Renderer 'cause we always need one of these as well
    SDL_Renderer* renderer;

    //Rectangles
    //Source and destination rectangles for tile sheet and placement of one tile
    SDL_Rect dstRect;
    SDL_Rect srcRect;

    //SDL_Rect rectBackground;
    SDL_Rect solidTileRect;

    //All the dino rectangles
    SDL_Rect rectDinoEastSheet;         //Rectangle for sheet of east walking dinos
    SDL_Rect rectDinoEast;              //One east walking dino rectangle
    SDL_Rect rectDinoWestSheet;         //Rectangle for sheet of west walking dinos
    SDL_Rect rectDinoWest;              //One west walking dino rectangle
    SDL_Rect rectDinoNorthSheet;        //Rectangle for sheet of north walking dinos
    SDL_Rect rectDinoNorth;             //One north walking dino rectangle
    SDL_Rect rectDinoSouthSheet;        //Rectangle for sheet of south walking dinos
    SDL_Rect rectDinoSouth;             //One south walking dino rectangle

    SDL_Rect rectDinoPlacement;         //Where to place the dino on the screen
    SDL_Rect rectDino;                  //Rectangle containing dino placed on screen
    SDL_Rect rectDinoSheet;             //Sheet containing dinos for currect direction

    //All the skel rectangles
    SDL_Rect rectSkelEastSheet;         //Rectangle for sheet of east walking skel
    SDL_Rect rectSkelEast;              //One east walking skel rectangle
    SDL_Rect rectSkelWestSheet;         //Rectangle for sheet of west walking skel
    SDL_Rect rectSkelWest;              //One west walking skel rectangle
    SDL_Rect rectSkelNorthSheet;        //Rectangle for sheet of north walking skel
    SDL_Rect rectSkelNorth;             //One north walking skel rectangle
    SDL_Rect rectSkelSouthSheet;        //Rectangle for sheet of south walking skel
    SDL_Rect rectSkelSouth;             //One south walking skel rectangle

    SDL_Rect rectSkelPlacementEW;       //Where to place the e/w skel on the screen
    SDL_Rect rectSkelSheetEW;           //Sheet containing e/w skel for currect direction
    SDL_Rect rectSkelEW;                //Rectangle containing e/w skel placed on screen

    SDL_Rect rectSkelPlacementNS;       //Where to place the n/s skel on the screen
    SDL_Rect rectSkelSheetNS;           //Sheet containing n/s skel for currect direction
    SDL_Rect rectSkelNS;                //Rectangle containing n/s skel placed on screen

    //All the zombie rectangles
    SDL_Rect rectZombieEastSheet;       //Rectangle for sheet of east walking skel
    SDL_Rect rectZombieEast;            //One east walking skel rectangle
    SDL_Rect rectZombieWestSheet;       //Rectangle for sheet of west walking skel
    SDL_Rect rectZombieWest;            //One west walking skel rectangle

    SDL_Rect rectZombiePlacementEW;     //Where to place the e/w zombie on the screen
    SDL_Rect rectZombieSheetEW;         //Sheet containing e/w zombie for currect direction
    SDL_Rect rectZombieEW;              //Rectangle containing e/w zombie placed on screen

    //Rectangles for explosions

    //Explosion 1 with zombie
    SDL_Rect rectExplosionPlacement;    //Where to place the zombie explosion on the screen
    SDL_Rect rectExplosion;             //Rectangle containing explosion 1 placed on screen
    SDL_Rect rectExplosionSheet;        //Sheet containing explosion 1

    //Explosion 2 with skeleton 1
    SDL_Rect rectExplosion2Placement;   //Where to place the skel 1 explosion on the screen
    SDL_Rect rectExplosion2;            //Rectangle containing explosion 2 placed on screen
    SDL_Rect rectExplosion2Sheet;       //Sheet containing explosion 2

    //Explosion 3 with skeleton 2
    SDL_Rect rectExplosion3Placement;   //Where to place the skel 2 explosion on the screen
    SDL_Rect rectExplosion3;            //Rectangle containing explosion 3 placed on screen
    SDL_Rect rectExplosion3Sheet;       //Sheet containing explosion 3

    //End of game rectangles
    SDL_Rect rectPlayerLose;            //Rectangle containing player wins image
    SDL_Rect rectPlayerWin;             //Rectangle containing player loses image

    //Instruction rectangle             //Rectangle containing the game instructions
    SDL_Rect rectInstruct;

    //Game constants
    const int SCREEN_WIDTH = 640;           //Width of game screen
    const int SCREEN_HEIGHT = 640;          //Height of game screen
    const int DINO_WIDTH = 32;              //Width of dino player sprite
    const int DINO_HEIGHT = 32;             //Height of dino player sprite
    const int SKEL_WIDTH = 32;              //Width of skeleton enemy sprite
    const int SKEL_HEIGHT = 32;             //Height of skeleton enemy sprite
    const int ZOMBIE_WIDTH = 32;            //Width of zombie enemy sprite
    const int ZOMBIE_HEIGHT = 30;           //Height of zombie enemy sprite
    const int FPS = 30;                     //Set our frames per second to 30
    const int FRAME_DELAY = 100/FPS;        //Frame delay based on FPS
    const int MAX_DINO_FRAME = 8;           //number of dinos per sheet
    const int SKEL_SPEED = 1;               //the skel moves faster than the dino
    const int MAX_SKEL_FRAME = 9;           //number of skels per sheet
    const int ZOMBIE_SPEED = 1;             //the skel moves faster than the dino
    const int MAX_ZOMBIE_FRAME = 8;         //number of zombies per sheet
    const int MAX_EXPLOSION_FRAME = 16;     //number of explosions per sheet

    //Background variables
    int displayRow = 0;             //number of rows you want displayed
    int displayCol = 0;             //number of columns you want displayed
    int mapRow = 0;                 //number of rows in map
    int mapCol = 0;                 //number of columns in map
    int tileHeight = 0;             //height of tiles in tile sheet
    int tileWidth = 0;              //width of tiles in tile sheet

    //Dino variables
    const int DINO_SPEED = 1;       //Speed of the player dino
    int xDinoPos = 0;               //x position of dino on screen
    int yDinoPos = 64;              //y position of dino on screen
    int dinoWidth;                  //width of one dino frame
    int dinoHeight;                 //height of one dino frame
    int SpriteFrame = 0;            //Current sheet frame for dino
    int FrameCounter = 0;           //Frame counter for dino
    bool dinoDestroyed = false;     //Boolean indicating if player dino is destroyed
    bool atMazeExit = false;        //Boolean indicating player is at end of maze

    //Skeleton 1 variables
    int skelWidth;                  //width of one skel frame
    int skelHeight;                 //height of one skel frame
    int SpriteFrameSkelEW = 0;      //Current sheet frame for skel 1
    int FrameCounterSkelEW = 0;     //Frame counter for skel 1
    int xSkelEWpos = 96;            //x position of e/w skel on screen
    int ySkelEWpos = 160;           //y position of e/w skel on screen
    bool skelDestroyed = false;     //Boolean indicating if enemy skel 1 is destroyed

    //Skelton 2 variables
    int SpriteFrameSkelNS = 0;      //Current sheet frame for skel 2
    int FrameCounterSkelNS = 0;     //Frame counter for skel 2
    int xSkelNSpos = 320;           //x position of n/s skel on screen
    int ySkelNSpos = 70;            //y position of n/s skel on screen
    bool skel2Destroyed = false;    //Boolean indicating if enemy skel 2 is destroyed

    //Zombie variables
    int zombieWidth;                //width of one zombie frame
    int zombieHeight;               //height of one zombie frame
    int SpriteFrameZombieEW = 0;    //Current sheet frame for zombie
    int FrameCounterZombieEW = 0;   //Frame counter for zombie
    int xZombieEWpos = 352;         //x position of e/w zombie on screen
    int yZombieEWpos = 576;         //y position of e/w zombie on screen
    bool zombieDestroyed = false;   //Boolean indicating if enemy zombie is destroyed

    //Explosion variables
    int explosionHeight;            //Height of an explosion
    int explosionWidth;             //Width of an explosion

    //Explosion 1 variables
    int SpriteFrameExplosion = 0;       //Current sheet frame for explosion 1
    int FrameCounterExplosion = 0;      //Frame counter for explosion 1
    bool explosionDestroyed = false;    //Boolean indicating if explosion 1 is destroyed
    bool explosion = false;             //Boolean indicating if explosion 1 has occureed

    //Explosion 2 variables
    int SpriteFrameExplosion2 = 0;      //Current sheet frame for explosion 2
    int FrameCounterExplosion2 = 0;     //Frame counter for explosion 2
    bool explosion2Destroyed = false;   //Boolean indicating if explosion 2 is destroyed
    bool explosion2 = false;            //Boolean indicating if explosion 2 has occureed

    //Explosion 3 variables
    int SpriteFrameExplosion3 = 0;      //Current sheet frame for explosion 3
    int FrameCounterExplosion3 = 0;     //Frame counter for explosion 3
    bool explosion3Destroyed = false;   //Boolean indicating if explosion 3 is destroyed
    bool explosion3 = false;            //Boolean indicating if explosion 3 has occureed

    //Direction variables for sprite objects
    //Note:  west = 0, east = 1, south = 2, north = 3
    int dinoDirection = 1;
    int skelDirectionEW = 1;
    int skelDirectionNS = 2;
    int zombieDirectionEW = 1;

    //place holders for array of frame ID numbers
    int* layer1, *layer2;

    int sheetWidth = 0;             //width of destination game screen
    int sheetHeight = 0;            //height of destination game screen

    vector<int> solidTiles;         //array of solid tile frameIDs
    struct SolidTileStruct          //structure to hold position of solid tile data
    {
        int x;          //x position of solid tile
        int y;          //y position of solid tile
        bool solid;     //we know the tile is solid but put a 1 here anyway
        int frameID;    //frame ID of solid tile
    };
    SolidTileStruct tiles[300];
    int numSolidTiles = 0;
};

#endif // GAME_H
