#ifndef __STRATEGY_H
#define __STRATEGY_H

#include "common.h"
#include "bidiarray.h"
#include "mvt.h"



class Strategy {

private:
    //! array containing all blobs on the board
    bidiarray<Sint16> _blobs;
    //! an array of booleans indicating for each cell whether it is a hole or not.
    const bidiarray<bool>& _holes;
    //! Current player
    Uint16 _current_player;
    
    //! Call this function to save your best mvts.
    //! Multiple call can be done each turn,
    //! Only the last mvt saved will be used.
    void (*_saveBestMove)(mvt&);

public:
        // Constructor from a current situation
    Strategy (bidiarray<Sint16>& blobs, 
              const bidiarray<bool>& holes,
              const Uint16 current_player,
              void (*saveBestMove)(mvt&))
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
         * Apply a mvt to the current state of blobs
         * Assumes that the mvt is valid
         */
    void applyMove (const mvt& mv);

        /**
         * Compute the vector containing every possible moves
         */
    vector<mvt>& computeValidMoves (vector<mvt>& valid_moves) const;

        /**
         * Estimate the score of the current state of the game
         */
    Sint32 estimateCurrentScore () const;

        /**
         * Find the best mvt.
         */
    void computeBestMove ();
    
    
};

#endif
