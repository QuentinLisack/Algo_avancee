#ifndef __mvt_H
#define __mvt_H
#include<SDL.h>


/** mvt class
 */
struct mvt
{
    mvt(){}
    
    mvt(const mvt & mv)
            :ox(mv.ox), oy(mv.oy), nx(mv.nx), ny(mv.ny) {}
    mvt(const Uint8 oldx, const Uint8 oldy, const Uint8 newx, const Uint8 newy)
            : ox(oldx),oy(oldy),nx(newx),ny(newy){}
    
    mvt& operator=(const mvt& mv)
        {ox=mv.ox;oy=mv.oy;nx=mv.nx;ny=mv.ny; return *this;}
        
    
    Uint8 ox;
    Uint8 oy;
    Uint8 nx;
    Uint8 ny;
};

#endif
