#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

struct board {
    /*

    */
    uint_fast64_t yellow = 0;
    uint_fast64_t red = 0;

    /*
        0 - yellow
        1 - red
    */
    bool side = false;

    /*
        Heights of every column
    */
    uint_fast8_t height[7] = { 0,0,0,0,0,0,0 };

    /*
        Setting a specified bit
    */
    constexpr inline void set( bool side, uint_fast8_t column, uint_fast8_t row ) {

    }

    /*
        Making a move
        Returns false if it was impossible
    */
    inline bool make_move( uint_fast8_t column ) {


        return true;
    }

    void display() {

    }
};

bool check_for_win( board* state, bool side ) {
    return false;
}

/*
    Evaluates based on the principles found in the internet

    Returns a score for the yellow (negative means red is winning)
*/
int_fast64_t evaluate( board* state ) {
    int_fast64_t result = 0;

    return result;
}

/*
    Pseudo random generator
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

    // take average
    result >>= sample;

    // return
    return result;
}

int main() {
    srand( time( NULL ) );

    board state;

    state.display();
}