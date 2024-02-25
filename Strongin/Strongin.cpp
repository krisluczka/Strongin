#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

struct board {
    /*
        Board representation

        -1 - null
         0 - yellow
         1 - red
    */
    int_fast8_t state[7][6] = { {-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1} };
    bool side = false;

    /*
        Heights of every column
    */
    uint_fast8_t height[7] = { 0,0,0,0,0,0,0 };

    /*
        Setting a specified bit
    */
    constexpr inline void set( bool side, uint_fast8_t column, uint_fast8_t row ) {
        state[column][row] = side;
    }

    /*
        Checks the possibility of making a move
    */
    inline bool check_move( int_fast8_t column ) {
        if ( height[column] < 6 ) {
            return true;
        } else return false;
    }

    /*
        Making a move
        Returns false if it was impossible
    */
    inline bool make_move( int_fast8_t column ) {
        if ( height[column] < 6 ) {
            state[column][height[column]] = side;
            height[column]++;
            side = !side;
        } else return false;

        return true;
    }

    /*
        Forces a move
        Returns false if the board is full
    */
    inline bool force_move( int_fast8_t column ) {
        int_fast8_t og_column = column;
        while ( height[column] == 6 ) {
            column++;
            if ( column > 6 ) column = 0;
            if ( column == og_column ) return false;
        }

        if ( height[column] < 6 ) {
            state[column][height[column]] = side;
            height[column]++;
            side = !side;
        } else return false;

        return true;
    }

    /*
        Displays current board state
    */
    void display() {
        for ( int_fast8_t row = 5; row >= 0; row-- ) {
            for ( int_fast8_t column = 0; column < 7; column++ ) {
                if ( state[column][row] == 1 )
                    cout << "R ";
                else if ( state[column][row] == 0 )
                    cout << "Y ";
                else
                    cout << "- ";
            }
            cout << endl;
        }
        cout << "1 2 3 4 5 6 7" << endl;
    }
};

/*
    Checking whether board has a winning side
*/
int_fast8_t check_for_win( board* state ) {
    // vertical
    for ( int_fast8_t j = 0; j < 7; j++ ) {
        for ( int_fast8_t i = 0; i < 3; i++ ) {
            if ( state->state[j][i] != -1
                && state->state[j][i] == state->state[j][i + 1]
                && state->state[j][i] == state->state[j][i + 2]
                && state->state[j][i] == state->state[j][i + 3] ) {

                return state->state[j][i];
            }
        }
    }

    // horizontal
    for ( int_fast8_t i = 0; i < 6; i++ ) {
        for ( int_fast8_t j = 0; j < 4; j++ ) {
            if ( state->state[j][i] != -1
                && state->state[j][i] == state->state[j + 1][i]
                && state->state[j][i] == state->state[j + 2][i]
                && state->state[j][i] == state->state[j + 3][i] ) {

                return state->state[j][i];
            }
        }
    }

    // diagonal up
    for ( int_fast8_t i = 0; i < 3; i++ ) {
        for ( int_fast8_t j = 0; j < 4; j++ ) {
            if ( state->state[j][i] != -1
                && state->state[j][i] == state->state[j + 1][i + 1]
                && state->state[j][i] == state->state[j + 2][i + 2]
                && state->state[j][i] == state->state[j + 3][i + 3] ) {

                return state->state[j][i];
            }
        }
    }

    // diagonal down
    for ( int_fast8_t i = 3; i < 6; i++ ) {
        for ( int_fast8_t j = 0; j < 4; j++ ) {
            if ( state->state[j][i] != -1
                && state->state[j][i] == state->state[j + 1][i - 1]
                && state->state[j][i] == state->state[j + 2][i - 2]
                && state->state[j][i] == state->state[j + 3][i - 3] ) {

                return state->state[j][i];
            }
        }
    }

    return -1;
}

/*
    Evaluates based on the principles found in the internet

    Returns a score for the yellow (negative means red is winning)
*/
int_fast64_t evaluate( board* state ) {
    int_fast64_t yellow = 0;
    int_fast64_t red = 0;

    // three in a row
    bool check = true;
    for ( int_fast8_t i = 2; i < 4; i++ ) {
        for ( int_fast8_t j = 2; j < 5; j++ ) {
            if ( state->state[j][i] == state->state[j - 1][i] && state->state[j][i] == state->state[j + 1][i] ) break;
            if ( state->state[j][i] == state->state[j][i - 1] && state->state[j][i] == state->state[j][i + 1] ) break;
            if ( state->state[j][i] == state->state[j][i - 1] && state->state[j][i] == state->state[j + 1][i] ) break;
            if ( state->state[j][i] == state->state[j][i + 1] && state->state[j][i] == state->state[j - 1][i] ) break;
            
            check = false;
        }
        if ( check ) {
            if ( state->side ) red += 33;
            else               yellow += 33;
            break;
        }
    }

    // for four in a row
    int_fast8_t winning = check_for_win( state );
    if ( winning == 0 ) yellow += 100;
    else if ( winning == 1 ) red += 100;

    // odd even strategy
    for ( int_fast8_t i = 0; i < 3; i++ ) {
        for ( int_fast8_t j = 0; j < 7; j++ ) {
            // yellow need to fill the odd ones
            if ( state->state[j][i * 2] == 0 ) {
                yellow += 10;
            }

            // red need to fill the even ones
            if ( state->state[j][i * 2 + 1] == 1 ) {
                red += 10;
            }
        }
    }

    // center and side columns are important
    for ( int_fast8_t i = 0; i < 6; i++ ) {
        if ( state->state[3][i] == 0 )      yellow += 5;
        else if ( state->state[3][i] == 1 ) red += 5;

        if ( state->state[2][i] == 0 )      yellow += 2;
        else if ( state->state[2][i] == 1 ) red += 2;

        if ( state->state[4][i] == 0 )      yellow += 2;
        else if ( state->state[4][i] == 1 ) red += 2;
    }

    return yellow - red;
}

/*
    Pseudo random number generator
*/
uint_fast64_t random8 = 6789124;
uint_fast8_t random( uint_fast64_t seed ) {
    random8 ^= 6789912367124;
    random8 += 932094;
    random8 ^= 531323;
    random8 -= 8123903293;
    return (seed ^ random8) % 256;
}

/*
    Analyzing the position with algorithm derived
    from the random tree forest algorithms

    Evaluates given amount of randomized games
    and returns an average score
*/
int_fast64_t analyze( board* state, uint_fast8_t sample ) {
    int_fast64_t result = 0;
    uint_fast8_t column = 0;
    uint_fast8_t moves = 0;
    board* random_state = new board;
    int_fast8_t win;

    for ( uint_fast64_t i = 0; i < pow( 2, sample ); i++ ) {
        win = -1;
        // creating new board by copying the og
        *random_state = *state;

        // resetting the moves
        moves = 0;

        // randomizing the tree
        while ( win == -1 && moves < 42 ) {
            // makin legal moves
            column = random( rand() ) % 7;
            random_state->force_move( column );
            moves++;
        }

        // evaluate and add to the overall score
        result += evaluate( random_state );
    }

    // remember to free up the memory, kids!
    delete random_state;

    // take average
    result >>= sample;

    // return
    return result;
}

/*
    Searching through given position
*/
int_fast8_t search( board* state ) {
    int_fast64_t scores[7];
    int_fast8_t move = -1;
    int_fast64_t best_score = -9223372036854775807;
    board* new_state;
    // generate all the moves
    for ( int_fast8_t i = 0; i < 7; i++ ) {
        if ( state->check_move( i ) ) {
            // copying board
            new_state = new board;
            *new_state = *state;

            // making move
            new_state->make_move( i );

            // analyzing the move
            if ( state->side )
                scores[i] = -analyze( new_state, 16 );
            else 
                scores[i] = analyze( new_state, 16 );

            //cout << " Column " << int( i + 1 ) << ": " << scores[i] << endl;

            // searching for the best move
            if ( scores[i] > best_score ) {
                move = i;
                best_score = scores[i];
            }

            // always remember to free up the memory, kids!
            delete new_state;
        } else {
            // if it's impossible, don't do it
            scores[i] = -9223372036854775807;
        }
    }

    // return the found move
    return move;
}

int main() {
    srand( time( NULL ) );
    board state;

    string input;
    cout << " Connect 4 engine \n";
    cout << " #     Strongin     #\n";
    cout << " by Krzysztof Luczka,\n\n";
    cout << " license and source code on\n";
    cout << " https://github.com/krisluczka/Strongin\n\n";
    cout << " Y - for yellow (first), R - red (second) >> "; cin >> input;

    state.display();

    int column = 0;
    int move = 0;
    if ( input != "Y" && input != "y" ) {
        move = search( &state );
        cout << " Strongin made move " << move + 1 << "\n\n";
        state.make_move( move );
        state.display();
    }
    while ( check_for_win( &state ) == -1 ) {
        do {
            cout << endl;
            cout << " Type a column index >> "; cin >> column;
            if ( column < 1 ) column = 1;
            if ( column > 7 ) column = 7;
        } while ( !state.make_move( column - 1 ) );
        state.display();
        move = search( &state );
        cout << " Strongin made move " << move + 1 << "\n\n";
        state.make_move( move );
        state.display();
    }
}