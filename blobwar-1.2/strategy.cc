#include "strategy.h"

void Strategy::applyMove(const movement& mv) {
	this->_blobs.set(mv.nx, mv.ny, this->_current_player);

	this->_blobs.set(mv.ox, mv.oy,
			(2 - std::max(std::abs(mv.ox - mv.nx), std::abs(mv.oy - mv.ny)))
					* (this->_current_player + 1) - 1);

	//parallelisable
	for(int i=std::max(mv.nx-1,0); i<std::min(mv.nx+2,8);i++){
		for(int j=std::max(mv.ny-1,0); j<std::min(mv.ny+2,8);j++){
				this->_blobs.set(i,j,(this->_blobs.get(i,j)==-1)?-1:_current_player);
			}
	}

}

bidiarray<Sint16> Strategy::applyFakeMove(const movement& mv, bidiarray<Sint16> Fakeblobs, Uint16 cp) {
	Fakeblobs.set(mv.nx, mv.ny, cp);

	Fakeblobs.set(mv.ox, mv.oy,
			(2 - std::max(std::abs(mv.ox - mv.nx), std::abs(mv.oy - mv.ny)))
					* (cp + 1) - 1);

	//parallelisable
	for(int i=std::max(mv.nx-1,0); i<std::min(mv.nx+2,8);i++){
		for(int j=std::max(mv.ny-1,0); j<std::min(mv.ny+2,8);j++){
				Fakeblobs.set(i,j,(Fakeblobs.get(i,j)==-1)?-1:cp);
			}
	}
	return Fakeblobs;
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

Sint32 Strategy::estimateFakeScore(bidiarray<Sint16> Fakeblobs){
	Sint32 retour = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {

				switch (Fakeblobs.get(i, j)) {

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


vector<movement>& Strategy::computeValidMoves(
		vector<movement>& valid_moves) const {

	//std::mutex mut; //WARNING c++11

	valid_moves.clear();

	//avec le mutex toutes les bocles sont parallelisables.
	movement mv(0, 0, 0, 0);
	//iterate on starting position
	for (mv.ox = 0; mv.ox < 8; mv.ox++) {
		for (mv.oy = 0; mv.oy < 8; mv.oy++) {
			if (_blobs.get(mv.ox, mv.oy) == (int) _current_player) {
				//iterate on possible destinations
				for (mv.nx = std::max(0, mv.ox - 2);
						mv.nx <= std::min(7, mv.ox + 2); mv.nx++) {
					for (mv.ny = std::max(0, mv.oy - 2);
							mv.ny <= std::min(7, mv.oy + 2); mv.ny++) {
						if (_holes.get(mv.nx, mv.ny))
							continue;
						if (_blobs.get(mv.nx, mv.ny) == -1) {
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

vector<movement>& Strategy::computeFakeMoves (vector<movement>& valid_moves,bidiarray<Sint16> Fakeblobs, Uint16 cp) const{
	//std::mutex mut; //WARNING c++11

		valid_moves.clear();

		//avec le mutex toutes les bocles sont parallelisables.
		    movement mv(0,0,0,0);
		    //iterate on starting position
		    for(mv.ox = 0 ; mv.ox < 8 ; mv.ox++) {
		        for(mv.oy = 0 ; mv.oy < 8 ; mv.oy++) {
		            if (Fakeblobs.get(mv.ox, mv.oy) == (int) cp) {
		                //iterate on possible destinations
		                for(mv.nx = std::max(0,mv.ox-2) ; mv.nx <= std::min(7,mv.ox+2) ; mv.nx++) {
		                    for(mv.ny = std::max(0,mv.oy-2) ; mv.ny <= std::min(7,mv.oy+2) ; mv.ny++) {
		                        if (_holes.get(mv.nx, mv.ny)) continue;
		                        if (Fakeblobs.get(mv.nx, mv.ny) == -1){
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
    //alphabeta
	int i=1;
	movement mv;
	while(true){
		mv=MinMax(i);
		_saveBestMove(mv);
		cout<< "level "<< i << "saved________________\n";//pour faire joli, a virer pour les perfs
		i++;
	}
}

movement Strategy::MinMax(int maxlevel){
	vector<movement> movelist;
	movelist=computeValidMoves(movelist);
	Sint32 bestvalue=((_current_player==0)?50000:-50000);
	Sint32 value=0;
	movement bestmove;
	for(movement m : movelist){

		value=MinMaxScore(1,maxlevel,_current_player,m,this->_blobs);
		if(isBetter(value,bestvalue,_current_player)){
			bestvalue=value;
			bestmove=m;
		}
		cout<< value << "\t /" << bestvalue <<"\n"; // DEBUG

	}


	return bestmove;
}

Sint32 Strategy::MinMaxScore(int level, int maxlevel, Uint16 cp,movement mv, bidiarray<Sint16> Fakeblobs){

	bidiarray<Sint16> FB;
	FB=applyFakeMove(mv,Fakeblobs,cp);

	if(level==maxlevel){
		return estimateFakeScore(FB);
	}

	vector<movement> movelist;
	movelist=computeFakeMoves(movelist,Fakeblobs,cp);
	Sint32 bestvalue=((cp==0)?50000:-50000);
	Sint32 value=0;
	for(movement m: movelist){
		value=MinMaxScore(level+1,maxlevel,1-cp,m,Fakeblobs);
		if(isBetter(value,bestvalue,cp)){
			bestvalue=value;
		}
	}
	return bestvalue;


}


movement Strategy::AlphaBeta(int maxlevel){
	vector<movement> movelist;
	movelist=computeValidMoves(movelist);
	Sint32 bestvalue=((_current_player==0)?50000:-50000);
	Sint32 value=0;
	movement bestmove;
	for(movement m : movelist){

		value=AlphaBetaScore(1,maxlevel,_current_player,m,this->_blobs,-5000,5000);
		if(isBetter(value,bestvalue,_current_player)){
			bestvalue=value;
			bestmove=m;
		}


		cout<< value << "\t /" << bestvalue <<"\n"; // DEBUG

	}

	return bestmove;
}

Sint32 Strategy::AlphaBetaScore(int level, int maxlevel, Uint16 cp, movement mv,
		bidiarray<Sint16> Fakeblobs, Sint32 A, Sint32 B) {


	bidiarray<Sint16> FB;
	FB = applyFakeMove(mv, Fakeblobs, cp);

	if (level == maxlevel) {
		return estimateFakeScore(FB);
	}

	vector<movement> movelist;
	movelist = computeFakeMoves(movelist, Fakeblobs, cp);
	Sint32 value = 0;
	Sint32 alpha = A;
	Sint32 beta = B;

	//cond noeud min
	if (cp == 0) {
		for (movement m : movelist) {
			value = AlphaBetaScore(level + 1, maxlevel, 1 - cp, m, Fakeblobs,
					alpha, beta);
			if (value < beta) {
				beta = value;
			}
			if (alpha >= beta) {

				return alpha;
			}
		}
		return beta;
	} else {
		//cond noeud max
		for (movement m : movelist) {
			value = AlphaBetaScore(level + 1, maxlevel, 1 - cp, m, Fakeblobs,
					alpha, beta);
			if (value > alpha) {
				alpha = value;
			}
			if (alpha >= beta) {

				return beta;

			}
		}
		return alpha;
	}

}


bool Strategy::isBetter(Sint32 value, Sint32 bestvalue, Uint16 cp ){
	return (cp==0)?(value<bestvalue):(value>bestvalue);
}

