#ifndef __SHMEM_H
#define __SHMEM_H

#include "common.h"
#include "mvt.h"

/**
 * Share memory between 2 process (blobwar and launchStrategy).
 */

//! should be called first
//! if init_mvt is true, the mvt will be set to 0,0,0,0
void shmem_init(bool init_mvt=false);

//! return the last save mvt
mvt shmem_get();

//! save a new mvt
void shmem_set(mvt& m);


#endif
