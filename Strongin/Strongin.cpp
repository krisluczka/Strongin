#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define HASH_SIZE 67108864

using namespace std;

constexpr const uint_fast64_t zobrist_keys[3][42] = { {17135028571398784774, 1212379124054155005, 4155093011491237681, 9597518229305601818, 14485787164365033088, 17660651885345889533, 1794097545570620324, 7094626097515394086, 11620406063779687986, 16869108026770778866, 2873854289025978291, 7401978112353951603, 12429928951725256128, 14933685025941478874, 17221197691073226700, 1422322680640361440, 4286296089494509487, 6359956208661200199, 11752703007634303294, 16924855063334628082, 2916059818507325808, 6000828364730286576, 8094618403453093362, 11179422402156451100, 16335767604833485146, 2342814055809559439, 7663646206583439738, 12187136963795499264, 15328193883878415176, 17762047150023120828, 2107278532443995379, 6989051492470364925, 12230930179176202795, 15383215388961167291, 17742754861535291860, 2018237692455182634, 6762400182376800176, 11574330015992917065, 16877167876012315967, 2884106633978598210, 5914807838644858841, 8062719910786264404 },
{10336902423689113287, 12989393575679631008, 15499841071513535160, 17789684967592014332, 2051511528809591865, 6863406400578713092, 11456680977890104938, 16410370784538650957, 3133588548761098183, 7510712051864289566, 10521162675786970878, 12671291355773599654, 14889075520252697820, 17597683822857784235, 1587253765213193272, 4329814754946366119, 9071778498098400217, 13939970350894991183, 739537919360813935, 5267743399671422814, 8422238559182700791, 10788612870981721754, 13366750631955433312, 402765975794143803, 5288963644882598522, 8427619247602183701, 10791798926348318202, 13317965039933765605, 119958138826715257, 5562220679452097246, 8144791688019099087, 11317274917614029796, 16217053805130193544, 3246560367363656010, 5840322247892325960, 9010621804646110296, 13910191793475795859, 941877269927908403, 3524311983613339832, 6516787842970034538, 8739040371232920443, 14003476423472203842 },
{82664039504793829, 5527173923036034993, 7963366932413937493, 10690005026964913935, 13195968637600320292, 15472299520194717616, 18194367769000676001, 4557279954213343046, 9150558655556868710, 14104365295240635711, 827607301066400757, 5204849567805744244, 8215322240666503778, 10365429204768922774, 12583232823840291131, 15291838918303170027, 17727998949573216825, 2023829273956215977, 6765797415490229944, 11633965001461480643, 16880301090841763727, 2961740240663764632, 6116116928063954882, 8536527556675652378, 11333079188447386568, 16020984119839786886, 2676579445254653202, 7544663812410874164, 10557280525494604751, 12707512609578666541, 15789866629078673106, 2501810051815674086, 6953223173387401334, 12339256414491425840, 15203377432771195951, 17274655735094944279, 1117770950608364310, 3412048078100668946, 6062126884332931834, 8482696125080315824, 11060682152692864172, 16302668599317678709 } };

int_fast64_t hash_table[HASH_SIZE];

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
uint_fast8_t random_8( uint_fast64_t seed ) {
    random8 ^= 6789912367124;
    random8 += 932094;
    random8 ^= 531323;
    random8 -= 8123903293;
    return (seed ^ random8) % 256;
}

uint_fast64_t random64 = 14757395258913895300;
uint_fast64_t random_64( uint_fast64_t seed ) {
    random64 ^= 9391069710217933373;
    random64 += 490245236671343077;
    random64 ^= 1540150833984080914;
    random64 -= 5937773640761221614;
    return seed ^ random64;
}

/*
    Zobrist hashing for the hash table
*/
static inline uint_fast64_t zobrist_hashing( board* state ) {
    uint_fast64_t hash = 0;

    for ( int_fast8_t i = 0; i < 6; i++ )
        for ( int_fast8_t j = 0; j < 7; j++ )
            hash ^= zobrist_keys[state->state[j][i] + 1][j * 7 + i];
        
    return hash;
}

/*
    Analyzing the position with algorithm derived
    from the random tree forest algorithms

    Evaluates given amount of randomized games
    and returns an average score
*/
int_fast64_t analyze( board* state, uint_fast8_t sample ) {
    int_fast64_t result = 0;
    int_fast64_t hash_score = 0;
    uint_fast64_t hash = 0;
    uint_fast8_t column = 0;
    uint_fast8_t moves = 0;
    board* random_state = new board;

    for ( uint_fast64_t i = 0; i < pow( 2, sample ); i++ ) {
        // creating new board by copying the og
        *random_state = *state;

        // resetting the moves
        moves = 0;

        // randomizing the tree
        while ( moves < 42 ) {
            // makin legal moves
            column = random_8( rand() ) % 7;
            random_state->force_move( column );
            moves++;
        }

        // verifying with hash table
        hash = zobrist_hashing( random_state );
        hash_score = hash_table[hash % HASH_SIZE];

        // evaluating by hand and updating the hash table
        if ( hash_score == 0 ) {
            hash_score = evaluate( random_state );
            hash_table[hash % HASH_SIZE] = hash_score;
            result += hash_score;
        }
        // we found the score in the hash table
        else {
            result += hash_score;
        }
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
    memset( hash_table, 0, sizeof( hash_table ) );
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