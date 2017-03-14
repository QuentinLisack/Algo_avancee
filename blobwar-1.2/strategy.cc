#include "strategy.h"

void Strategy::applyMove(const movement& mv) {
	this->_blobs.set(mv.nx, mv.ny, this->_current_player);

	this->_blobs.set(mv.ox, mv.oy,
			(2 - std::max(std::abs(mv.ox - mv.nx), std::abs(mv.oy - mv.ny)))
					* (this->_current_player + 1) - 1);

	//parallelisable
	for(int i=std::max(mv.nx-1,0); i<std::min(mv.nx+2,8);i++){
		for(int j=std::max(mv.ny-1,0); j<std::min(mv.ny+2,8);j++){
				this->_blobs.set(i,j,(this->_blobs.get[i,j]==-1)?-1:_current_player);
			}
	}

}

Sint32 Strategy::estimateCurrentScore() const {
	Sint32 retour = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			switch (this->_blobs.get(i, j)) {

			case 0:
				retour--;
				break;
			case 1:
				retour++;
				break;
			default:
				break;
			}

		}
	}

	return retour;
}

vector<movement>& Strategy::computeValidMoves (vector<movement>& valid_moves) const {

	//std::mutex mut; //WARNING c++11

	valid_moves.clear();

	//avec le mutex toutes les bocles sont parallelisables.
	    movement mv(0,0,0,0);
	    //iterate on starting position
	    for(mv.ox = 0 ; mv.ox < 8 ; mv.ox++) {
	        for(mv.oy = 0 ; mv.oy < 8 ; mv.oy++) {
	            if (_blobs.get(mv.ox, mv.oy) == (int) _current_player) {
	                //iterate on possible destinations
	                for(mv.nx = std::max(0,mv.ox-2) ; mv.nx <= std::min(7,mv.ox+2) ; mv.nx++) {
	                    for(mv.ny = std::max(0,mv.oy-2) ; mv.ny <= std::min(7,mv.oy+2) ; mv.ny++) {
	                        if (_holes.get(mv.nx, mv.ny)) continue;
	                        if (_blobs.get(mv.nx, mv.ny) == -1){
	                        	//mut.lock();//WARNING c++11
	                        	valid_moves.push_back(mv);
	                        	//mut.unlock();//WARNING c++11
	                        }
	                    }
	                }
	            }
	        }
	    }



    return valid_moves;
}

void Strategy::computeBestMove () {
    // To be improved...

    //The following code find a valid move.
    movement mv(0,0,0,0);
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

