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
    }
};

/*
    Checking whether board has a winning side
*/
bool check_for_win( board* state, bool side ) {
    return false;
}

/*
    Evaluates based on the principles found in the internet

    Returns a score for the yellow (negative means red is winning)
*/
int_fast64_t evaluate( board* state ) {
    int_fast64_t result = 0;
    int_fast64_t yellow = 0;
    int_fast64_t red = 0;

    // for three in a row +200
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
            if ( state->side ) yellow += 1000;
            else               red += 1000;
            break;
        }
    }

    // for four in a row +10000

    // dominating the center
    for ( int_fast8_t i = 0; i < 6; i++ ) {
        if ( *state->state[3] == 1 )      red += 50;
        else if ( *state->state[3] == 0 ) yellow += 50;

        if ( *state->state[4] == 1 )      red += 25;
        else if ( *state->state[4] == 0 ) yellow += 25;

        if ( *state->state[2] == 1 )      red += 25;
        else if ( *state->state[2] == 0 ) yellow += 25;

        if ( *state->state[5] == 1 )      red += 10;
        else if ( *state->state[5] == 0 ) yellow += 10;

        if ( *state->state[1] == 1 )      red += 10;
        else if ( *state->state[1] == 0 ) yellow += 10;
    }

    // odd even strategy
    for ( int_fast8_t i = 0; i < 3; i++ ) {
        for ( int_fast8_t j = 0; j < 7; j++ ) {
            // yellow need to fill the odd ones
            if ( state->state[j][i * 2] == 0 ) {
                yellow += 200;
            }

            // red need to fill the odd ones
            if ( state->state[j][i * 2 + 1] == 1 ) {
                red += 200;
            }
        }
    }

    result = yellow - red;

    return result;
}

/*
    Pseudo random number generator
*/
uint_fast64_t random8 = 6789124;
uint_fast8_t random( uint_fast64_t seed ) {
    random8 ^= 6789912367124;
    random8 += 932094;
    random8 ^= 531323;
    random8 -= 8123903295;
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
    board* random_state = new board;

    for ( uint_fast64_t i = 0; i < pow( 2, sample ); i++ ) {
        // creating new board by copying the og
        *random_state = *state;

        // making random legal moves
        column = random( rand() ) % 7;
        while ( random_state->make_move( column ) ) {
            column = random( rand() ) % 7;

            // check if it's winning
        }

        // evaluate and add to the overall score
        result += evaluate( random_state );
    }

    // remember to free up the memory, kids!
    delete random_state;
    cout << ":" << result << "   ";
    // take average
    result >>= sample;

    cout << result << endl;

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
                scores[i] = -analyze( new_state, 12 );
            else 
                scores[i] = analyze( new_state, 12 );

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

    while ( 1 ) {
        state.make_move( search( &state ) );
        state.display();
        cout << endl;
        system( "pause > nul" );
    }

    state.display();
}