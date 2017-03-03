#include "strategy.h"



void Strategy::applyMove (const mvt& mv) {
        // To be completed...
}

Sint32 Strategy::estimateCurrentScore () const {
        // To be completed...
    return 0;
}

vector<mvt>& Strategy::computeValidMoves (vector<mvt>& valid_moves) const {
        // To be completed...
    return valid_moves;
}

void Strategy::computeBestMove () {
    // To be improved...

    //The following code find a valid mvt.
    mvt mv(0,0,0,0);
    //iterate on starting position
    for(mv.ox = 0 ; mv.ox < 8 ; mv.ox++) {
        for(mv.oy = 0 ; mv.oy < 8 ; mv.oy++) {
            if (_blobs.get(mv.ox, mv.oy) == (int) _current_player) {
                //iterate on possible destinations
                for(mv.nx = std::max(0,mv.ox-2) ; mv.nx <= std::min(7,mv.ox+2) ; mv.nx++) {
                    for(mv.ny = std::max(0,mv.oy-2) ; mv.ny <= std::min(7,mv.oy+2) ; mv.ny++) {
                        if (_holes.get(mv.nx, mv.ny)) continue;
                        if (_blobs.get(mv.nx, mv.ny) == -1) goto end_choice;
                    }
                }
            }
        }
    }

end_choice:
     _saveBestMove(mv);
     return;
}

