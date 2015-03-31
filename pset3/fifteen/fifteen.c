/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            // I changed this line a bit trying to make it humorous,
            // hope it wasn't a total fail.
            printf("\nYOU WON!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
* Clears screen using ANSI escape sequences.
*/
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
* Greets player.
*/
void greet(void)
{
    // I thought I would add some humor sense games are fun.
    clear();
    printf("GAME OF FIFTEEN\n Git'er Done!\n");
    usleep(2000000);
}

/**
* Initializes the game's board with tiles numbered 1 through d*d - 1,
* (i.e., fills board with values but does not actually print them),
* whereby board[i][j] represents row i and column j.
*/
void init(void)
{
    // TODO: sets value of board positions.
    int num = 1;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(i == d - 1 && j == d - 1)
            {
                /**
* Setting missing block should be the last number.
* Set mising block to 95 which is the ASCII value for the underscore.
*/
                board[i][j] = 95;
                break;
            }

            board[i][j] = d * d - num;
            num++;
        }
        /**
* If there are and even amount of tiles, the last two printed tiles
* on the bottom row will be 1 and 2 (reverseing the order). If there are
* an odd amount of tiles, the last two printed tiles will be 2 and 1
* sticking with the order of the set.
*/
        if(d * d % 2 == 0)
        {
            board[d-1][d-2] = 2;
            board[d-1][d-3] = 1;
        }
    }
}

/**
* Prints the board in its current state.
*/
void draw(void)
{
    // TODO:
          /**
* Creates board with init() specs and places
* spaces between printed values. Also sets
* value of 95 to ASCII char and other numbers to a 2 digit int.
*/
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == 95)
            {
                printf("%2c ", 95) ;
            }
            else
                printf("%2i ", board[i][j]);
        }
        printf("\n");
    }
}

/**
* If tile borders empty space, moves tile and returns true, else
* returns false.
*/
bool move(int tile)
{
    // TODO:
            /**
* If a move can be made
* (the "box" with a numberical value
* that is to be moved is touching the box with _ )
* then the positions the two boxes get switched,
* otherwise the move cannot be made.
*/
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                if(board[i][j - 1] == 95)
                {
                    board[i][j - 1] = board[i][j];
                    board[i][j] = 95;
                    return true;
                }
                else if(board[i][j + 1] == 95)
                {
                    board[i][j + 1] = board[i][j];
                    board[i][j] = 95;
                    return true;
                }
                else if(board[i - 1][j] == 95)
                {
                    board[i - 1][j] = board[i][j];
                    board[i][j] = 95;
                    return true;
                }
                else if(board[i + 1][j] == 95)
                {
                    board[i + 1][j] = board[i][j];
                    board[i][j] = 95;
                    return true;
                }
                else
                {
                    return false;
                }
            } 
        } 
    } 
    return false;
} 

/**
* Returns true if game is won (i.e., board is in winning configuration),
* else false.
*/
bool won(void)
{
    // TODO:
            /**
* Checks to see if positions are correct to win.
*/
    int num = 1;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(i == d - 1 && j == d - 1)
            {
                if(board[i][j] == 95)
                {
                    return true;
                }
            }
            if(board[i][j] == (d * d) - ( d * d - num))
            {
                num++;
                continue;
            }
            else
            {
                return false;
            }
        } 
    } 
    return true;
} 

/**
* Saves the current state of the board to disk (for testing).
*/
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
