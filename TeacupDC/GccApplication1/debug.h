#ifndef	_DEBUG_H
#define	_DEBUG_H

#include	<stdint.h>

#ifdef	DEBUG
    #define     DEBUG_LED_PIN 1
	#define		DEBUG_PID				1
	#define		DEBUG_DDA				2
	#define		DEBUG_POSITION	        4
	#define		DEBUG_ECHO			128
#else
	// by setting these to zero, the compiler should optimise the relevant code out
	#define		DEBUG_PID				0
	#define		DEBUG_DDA				2
	#define		DEBUG_POSITION	    4
	#define		DEBUG_ECHO			128
#endif


extern volatile uint8_t	debug_flags;

#endif	/* _DEBUG_H */
