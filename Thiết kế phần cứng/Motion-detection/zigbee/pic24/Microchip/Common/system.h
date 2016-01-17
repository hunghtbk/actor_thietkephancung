#ifndef _SYSTEM_H
#define _SYSTEM_H
	#include "generic.h"

	#ifdef USE_MicroWaveS
	    volatile unsigned int mradar_count = 0;
		const unsigned long MWCOUNT = 20000;
		unsigned int see_dev = 0; 
		const unsigned int MAX_SEEDEV = 5;//max fix device

	#endif

#endif

