/*******************************************************************************
* Title : Final Project
* Author: Ryan May
* Date  : 12/14/2020
* Course: CSC 222-301W
* Description: This program is a maze game that allows a player to move about the
*       maze with a player sprite controlled by the arrow keys.  The maze is comprised
*       of tiles that are loaded in from a tiled bitmap.  The program also reads in a file
*       that is used to determine the layout of the tiles, and which tiles to render to the
*       game screen.  The file also lets the program know which tiles are 'solid'.  Solid tiles
*       are checked for interection with player and enemy sprites to make sure thate sprites stay
*       within the maze.  The enemy sprites are located at dead ends within the maze.  They are checked
*       for collision with solid tiles as well.  Since the enemy sprites move continuously on their own,
*       when they collide with a solid tile, they just reverse their direction until their next collision.
*       The goal of the game is to use the player sprite to collide with all 3 enemy sprites located
*       throughout the maze, and then proceed to the maze exit, all before the 1 minute timer runs out.
*       The timer will be located in the game window title bar, and the game instructions will be placed
*       on the games screen as you are playing the game.  When the player collides with any of the enemy
*       an explosion animation will happen, and the enemy will disappear.  If the time runs out without winning
*       game, a screen will be displayed indicating that the player lost.  If the player destroys all 3 enemy object
*       and makes it to the maze exit in time, a screen will be displayed indicating that the player won.
*       The program deletes any allocated memory, and gracefully releases all resources when game is won, lost, or quit.
*       The program is modularized to separate the game functions and data members from the main function. A
*       game pointer object is created within main, and handles all game data.
*
*
* Data Requirements:
*       Input:      Keyboard input of up and down directional keys.
*                   Keyboard input of escape key to quit
*                   Mouse input to click x out of game window
*
*       Graphics:
*                   towntiles.bmp
*                   playerWins.bmp
*                   playerLoses.bmp
*                   explosion.bmp
*                   instructGame.bmp
*                   dinoEast.bmp
*                   dinoWest.bmp
*                   dinoSouth.bmp
*                   dinoNorth.bmp
*                   walkEast1.bmp
*                   walkWest1.bmp
*                   walkSouth1.bmp
*                   walkNorth1.bmp
*                   eastZombie.bmp
*                   westZombie.bmp
*
* BEGIN main
*    Allocate memory for game
*    Declare a  pointer to get keyboard state
*    IF(Initializing game fails)
*       Print debug message
*       Terminate game w function that destroys all allocated memory
*    END IF(Initializing game fails)
*    Create all the game images
*    WHILE(The game is running)
*       IF(Up arrow is pressed)
*           Move player up
*       END IF(Up arrow is pressed)
*       IF(Down arrow is pressed)
*           Move player down
*       END IF(Down arrow is pressed)
*       IF(Left arrow is pressed)
*           Move player left
*       END IF(Left arrow is pressed)
*       IF(Right arrow is pressed)
*           Move player right
*       END IF(Right arrow is pressed)
*       Update game variables and render to game screen
*    END WHILE(Game is running)
*    Print message to terminal for debug purposes
*    Terminate game w function that destroys all allocated memory
*    Return zero to main
* END main
********************************************************************************/
#include "Game.h"

int main(int argc, char *argv[])
{
    //Allocate memory for game
    Game* game = new Game();

    //Declare a  pointer to get keyboard state
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //IF(Initializing game fails)
    if(!game->InitGame())
    {
        //Print debug message
        printf("Unable to initialize from main.\n");

        //Terminate game w function that destroys all allocated memory
        game->CloseShop();

    }//END IF(Initializing game fails)

    //Create all the game images
    game->CreateGameImages();

    //WHILE(The game is running)
    while(game->ProgramIsRunning())
    {
        //IF(Up arrow is pressed)
        if(keys[SDL_SCANCODE_UP])
        {
            //Move player up
            game->moveUp();

        }//END IF(Up arrow is pressed)

        //IF(Down arrow is pressed)
        if(keys[SDL_SCANCODE_DOWN])
        {
            //Move player down
            game->moveDown();

        }//END IF(Down arrow is pressed)

        //IF(Left arrow is pressed)
        if(keys[SDL_SCANCODE_LEFT])
        {
            //Move player left
            game->moveLeft();

        }//END IF(Left arrow is pressed)

        //IF(Right arrow is pressed)
        if(keys[SDL_SCANCODE_RIGHT])
        {
            //Move player right
            game->moveRight();

        }//END IF(Right arrow is pressed)

        //Update game variables and render to game screen
        game->RunGame();

    }//END WHILE(Game is running)

    //Print message to terminal for debug purposes
    printf("Wow, what a semester!!");

    //Terminate game w function that destroys all allocated memory
    game->CloseShop();

    //Return zero to main
    return 0;
}
