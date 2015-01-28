#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//Constants
const int BOARD_COL_SIZE = 7;
const int BOARD_ROW_SIZE = 6;

class ConnectFourBoard {
public:
    
    ConnectFourBoard(); // Default constructor
    
	void print() {
		cout << "Floating Connect Four Board" << endl;
		cout << "  1 2 3 4 5 6 7" << endl;
		for(int i = 0; i < 6; i++) {
			cout << i+1;
			for(int j = 0; j < 7; j++) {
				cout << "|" << grid[i][j];
			}
			cout << "|" << endl;
			cout << "  - - - - - - -" << endl;
		}
	}
    
    // Auxillary member declarations
    
    // Examines the grid for existing pieces and returns a pointer to the
    // appropriate empty slot.
    char* addPiece( char side, int location );  // Defined 
    
    // Examines the grid for four adjacent identical pieces, diagonally,
    // horizontally, and vertically
    bool checkWinner( char currentPlayer ); //Defined
    
    // Checks the logical validity of the inputs side and location by examining
    // the piece's entry location.
    bool checkGrid( char side, int location ); //Defined
    
    // Examines the grid to see if a tie has occurred (tie = no open edges so
    // no new pieces can enter the board).
    bool checkTie(); //Defined
    
    int countPieces( int dx, int dy, char player );

    
    
private:
	
	char grid[BOARD_ROW_SIZE][BOARD_COL_SIZE];
    int row, col;
    
    // Auxillary member declarations.
    
};

// Begin auxillary function definitions for class ConnectFourBoard

// Default constructor
ConnectFourBoard::ConnectFourBoard() 
{
    for ( int i = 0; i < BOARD_ROW_SIZE; i++ ) 
    {
        for ( int j = 0; j < BOARD_COL_SIZE; j++ ) 
        {
            grid[i][j] = ' ';
        }
        
    }
    
}// End constructor.


bool ConnectFourBoard::checkGrid( char side, int location)
{
    bool valid = false;
    switch ( side ) {
        case 't':
            if( grid[0][location-1] == ' ' )
            {
                valid = true;
            }
            break;
        case 'b':
            if( grid[BOARD_ROW_SIZE-1][location-1] == ' ' )
            { 
                valid = true;
            }
            break;
        case 'l':
            if( grid[location-1][0] == ' ' )
            {
                valid = true;
            }
            break;
        case 'r':
            if( grid[location-1][BOARD_COL_SIZE-1] == ' ' )
            {
                valid = true;
            }
            break;
            
        default:
            break;
    }
    
    return valid;

}// End checkGrid.

char* ConnectFourBoard::addPiece( char side, int location )
{
    char *p;
    char error = 'E';
    int i = 0;
    
    switch (side) {
        case 't':
            for( i = 0; i < BOARD_ROW_SIZE; i++ )
            {
                if( grid[i][location-1] == ' ' )
                {
                    continue;
                }
                
                else
                {
                    row = i-1;
                    col = location-1;
                    p = &grid[row][col];
                    return p;
                }
            }
            
            row = i-1;
            col = location-1;
            p = &grid[row][col];
            break;
            
        case 'b':
            for( i = BOARD_ROW_SIZE-1; i >= 0; i-- )
            {
                if( grid[i][location-1] == ' ' )
                {
                    continue;
                }
                
                else
                {
                    row = i+1;
                    col = location-1;
                    p = &grid[row][col];
                    return p;
                }
            }
            
            row = i+1;
            col = location-1;
            p = &grid[row][col];
            break;
            
        case 'l':
            for( i = 0; i < BOARD_COL_SIZE ; i++ )
            {
                if( grid[location-1][i] == ' ' )
                {
                    continue;
                }
                
                else
                {
                    row = location-1;
                    col = i-1;
                    p = &grid[row][col];
                    return p;
                }
            }
            
            row = location-1;
            col = i-1;
            p = &grid[row][col];
            break;
            
        case 'r':
            for( i = BOARD_COL_SIZE-1; i >= 0 ; i-- )
            {
                if( grid[location-1][i] == ' ' )
                {
                    continue;
                }
                
                else
                {
                    row = location-1;
                    col = i+1;
                    p = &grid[row][col];
                    return p;
                }
            }
            
            row = location-1;
            col = i+1;
            p = &grid[row][col];
            break;
            
        default:
            row = 0;
            col = 0;
            p = &error;
            break;
    }// End switch.
    
    return p;
    
}// End addPiece.

bool ConnectFourBoard::checkTie()
{
    bool tie = true;
    
    for( int i = 0; i < BOARD_COL_SIZE; i++ )
    {
        if( grid[0][i] == ' ' )
        {
            tie = false;
        }
        
        if( grid[BOARD_ROW_SIZE-1][i] == ' ' )
        {
            tie = false;
        }
        
        if( grid[i][0] == ' ' )
        {
            tie = false;
        }
        
        if( grid[BOARD_COL_SIZE-1][i] == ' ' ) 
        {
            tie = false;
        }
    }
    return tie;
}// End checkTie.

int ConnectFourBoard::countPieces( int dx, int dy, char player )
{
    int x = row, y = col;
    int counter = 0;
 
    for( int i = 0; i < 3; i++ )
    {
        /*cout << "x: " << x << " " << "y: " << y << endl;
        cout << "counter: " << counter << endl;*/
        
        x += dx;
        y += dy;
        
        if( x < 0 || x > BOARD_ROW_SIZE || y < 0 || y > BOARD_COL_SIZE )
        {
            break;
        }
        
        if( grid[x][y] == player )
        {
            counter++;
        }
 
        else
        {
            break;
        }
    }
    
    return counter;
 
}

bool ConnectFourBoard::checkWinner( char currentPlayer )
{
    bool winner = false;
    
    const int WINNING = 3, SELF = 1; // Make like Charlie Sheen.
    
    // Checking horizontal direction.
    
    int right = countPieces(0, 1, currentPlayer);
    int left = countPieces(0, -1, currentPlayer);

    if( right+left+SELF > WINNING )
    {
        winner = true;
        return winner;
    }
    
    // Checking vertical direction.
    
    int up = countPieces( 1, 0, currentPlayer);
    int down = countPieces(-1, 0, currentPlayer);
    
    if( up+down+SELF > WINNING )
    {
        winner = true;
        return winner;
    }
    
    // Checking diagonals.
    
    int diagUpLeft = countPieces(-1, -1, currentPlayer);
    int diagDownRight = countPieces(1, 1, currentPlayer);
    
    if( diagUpLeft+diagDownRight+SELF > WINNING )
    {
        winner = true;
        return winner;
    }
    
    int diagDownLeft = countPieces(1, -1, currentPlayer);
    int diagUpRight = countPieces(-1, 1, currentPlayer);
    
    if( diagUpRight+diagDownLeft+SELF > WINNING )
    {
        winner = true;
        return winner;
    }
    
    else
    {
        return winner;
    }
    
    //Calculating horizontal matches
    
   /* for( int k = 0; k < 4; k++ )
    {
        while( i < BOARD_ROW_SIZE )
        {
            if( grid[i+k][col] == currentPlayer )
            {
                counterV++;
                i++;
            }
            
            else
            {
                break;
            }
        }
        break;
    }
    
    i = row;
    
    for( int k = 0; k < 4; k++ )
    {
        while( i >= 0 )
        {
            if( grid[i-k][col] == currentPlayer )
            {
                counterV++;
                i--;
            }
            
            else
            {
                break;
            }
        }
        break;
    }
    
    i = row; 
    
    //Calculating vertical matches
    
    for( int k = 0; k < 4; k++ )
    {
        while( j < BOARD_COL_SIZE )
        {
            if( grid[row][j] == currentPlayer )
            {
                counterH++;
            }
        }
        j++;
    }
    
    j = col;
    
    while( j >= 0 )
    {
        for( int k = 0; k < 4; k++ )
        {
            if( grid[row][j] == currentPlayer )
            {
                counterH++;
            }
        }
        j--;
    }
    
    j = col;
    */
}

// Begin class Game

class Game {
public:
    
    Game();
	void play();
    
    // Auxillary member declarations
    
    // Randomly selects player 1 or 2 to go first.
    void setPlayer(); //Defined
    
private:
	void printHeader() {
		cout << "Let's play Floating Connect Four" << endl;
	}
	
	void printPrompt() {
		cout << "Enter side (t, b, l, r) and location (1 to 7): ";
	}
    
	void printError() {
		cout << "Invalid location or side." << endl;
	}
    
	void printWinner( int player ) {
		cout << "Player " << player << " wins!" << endl;
	}
    
	void printTieGame() {
		cout << "Tie game. No one wins." << endl;
	}
    
    ConnectFourBoard board;

    
    // Auxillary member declarations
    
    int location, player;
    char side, currentPlayer;
    char* lastPlay;
    
    // Retrieves user input.
    void getInput(); //Defined
    
    // Switches player from 'R' to 'B' or vice versa after each turn.
    void switchPlayer(); //Defined
    
    // Checks input for syntatical correctness (input side must be t,b,r,s and
    // input location must be 1-7).
    bool checkInput( char side, int location ); //Defined
    
};

Game::Game()
{/* default constructor */}

void Game::setPlayer()
{
    //srand( static_cast<int>( time(0) ) );
    
    player = 1;
    currentPlayer = 'R';
    
}// End setPlayer()

void Game::switchPlayer()
{
    switch ( currentPlayer ) {
        case 'R':
            currentPlayer = 'B';
            player = 2;
            break;
            
        case 'B':
            currentPlayer = 'R';
            player = 1;
            break;
            
        default:
            break;
    }
}// End switchPlayer()

void Game::play() {
    
	printHeader();
    getInput();
    setPlayer();
    checkInput( side, location );
    bool play = true;
    
    while( ( !checkInput(side, location) ) || ( !board.checkGrid(side, location) ) ) 
    {
        printError();
        getInput();
    }

    
    while( play )
    {
        lastPlay = board.addPiece( side, location );
        *lastPlay = currentPlayer;
        board.print();
        if( board.checkWinner( currentPlayer ) )
        {
            printWinner( player );
            play = false;
            break;
        }
        else if ( board.checkTie() )
        {
            printTieGame();
            play = false;
            break;
        }
        switchPlayer();
        getInput();
        while( ( !checkInput( side, location ) ) || ( !board.checkGrid( side, location ) ) )
        {
            printError();
            getInput();
        }
        
    }// End while
    
}// End play()


void Game::getInput()
{
    printPrompt();
    cin >> side >> location;
}

bool Game::checkInput( char side, int location )
{
    bool sideInput, locInput, input;
    
    switch ( side ) {
        case 't':
        case 'b':
        case 'l':
        case 'r':
            sideInput = true;
           break;
            
        default:
            sideInput = false;
            break;
    }// End switch1
    
    switch ( location ) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            locInput = true;
            break;
        case 7:
            if( side == 't' || side == 'b' )
            {
                locInput = true;
            }
            break;
            
        default:
            locInput = false;
            break;
    }// End switch2
    
    if( sideInput && locInput )
    {
        input = true;
    }
    else
    {
        input = false;
    }
    
    return input;
}// End checkInput()

// Begin main function

int main() {
    
	Game myGame;
	myGame.play();
    
	return 0;
}
