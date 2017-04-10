#ifndef __STRATEGY_H
#define __STRATEGY_H

#include "common.h"
#include "bidiarray.h"
#include "movement.h"

//custom includes
#include <mutex> //WARNING c++11
#include <assert.h>//debug
#include <omp.h>


class Strategy {

private:
    //! array containing all blobs on the board
    bidiarray<Sint16> _blobs;
    //! an array of booleans indicating for each cell whether it is a hole or not.
    const bidiarray<bool>& _holes;
    //! Current player
    Uint16 _current_player;
    
    //! Call this function to save your best move.
    //! Multiple call can be done each turn,
    //! Only the last move saved will be used.
    void (*_saveBestMove)(movement&);

public:
        // Constructor from a current situation
    Strategy (bidiarray<Sint16>& blobs, 
              const bidiarray<bool>& holes,
              const Uint16 current_player,
              void (*saveBestMove)(movement&))
            : _blobs(blobs),_holes(holes), _current_player(current_player), _saveBestMove(saveBestMove)
        {
        }
    
              
    
        // Copy constructor
    Strategy (const Strategy& St)
            : _blobs(St._blobs), _holes(St._holes),_current_player(St._current_player) 
        {}
    
        // Destructor
    ~Strategy() {}
    
        /** 
         * Apply a move to the current state of blobs
         * Assumes that the move is valid
         */
    void applyMove (const movement& mv);


    		/**
	 * Apply a move a given map
	 * Assumes that the move is valid
	 */

    bidiarray<Sint16> applyFakeMove(const movement& mv, bidiarray<Sint16> Fakeblobs, Uint16 cp);

        /**
         * Compute the vector containing every possible moves
         */
    vector<movement>& computeValidMoves (vector<movement>& valid_moves) const;

    	/**
    	 * Compute the vector containing every possible moves on a given map
    	 */
    vector<movement>& computeFakeMoves (vector<movement>& valid_moves,bidiarray<Sint16> Fakeblobs,Uint16 cp) const;
        /**
         * Estimate the score of the current state of the game
         */
    Sint32 estimateCurrentScore () const;

    /**
            * Estimate the score of the given state of the game
            */
    Sint32 estimateFakeScore(bidiarray<Sint16> Fakeblobs);

        /**
         * Find the best move.
	 */
	void computeBestMove();

	/*
	 * initialise the minmax search for the best move
	 * maxlevel: number of moves to compute
	 * return best move
	 */
	movement MinMax(int maxlevel);

	/**
	 * implement the minmax search for the best move
	 * cp: current player for the upcoming move
	 * mv: movement to be tested
	 * Fakeblob: state of the map before mv is applied
	 * return best score
	 */
	Sint32 MinMaxScore(int level, int maxlevel, Uint16 cp, movement mv,
			bidiarray<Sint16> Fakeblobs);

	/**
	 * initialise the alphabeta search for the best move
	 * maxlevel: number of moves to compute
	 * return best move
	 */
	movement AlphaBeta(int maxlevel);

	/**
	 * implement the minmax search for the best move
	 * cp: current player for the upcoming move
	 * mv: movement to be tested
	 * Fakeblob: state of the map before mv is applied
	 * A,B: initial alpha beta bracket
	 *return best score
	 */
	Sint32 AlphaBetaScore(int level, int maxlevel, Uint16 cp, movement mv,
			bidiarray<Sint16> Fakeblobs, Sint32 A, Sint32 B);

	/**
	 *return true if & only if value is more profitable to cp than bestvalue
	 */
	bool isBetter(Sint32 value, Sint32 bestvalue, Uint16 cp);
    
};

#endif
