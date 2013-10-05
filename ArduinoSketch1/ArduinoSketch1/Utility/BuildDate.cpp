#ifndef BUILDINFO_H_ 
#include "buildinfo.h" 
#endif 
 
/************************************************************  
DO NOT MODIFY 
Automatically Generated On 2013-09-28 11-10-PM by pre-build98 
*************************************************************/ 
 
#if defined(BUILDINFO_RAM) 
static const char* BUILD_DATE = "2013-09-28 11-10-PM"; 
#elif defined(BUILDINFO_EEMEM) 
static const char BUILD_DATE[HeaderMsgSize] EEMEM = "2013-09-28 11-10-PM"; 
#elif defined(BUILDINFO_PROGMEM) 
static const char BUILD_DATE[] PROGMEM = "2013-09-28 11-10-PM"; 
#endif 
 
/* Return the header message */  
void GetBuildDate( void *buffer, size_t bufferSize )  
{   
#if defined(BUILDINFO_RAM) 
	memcpy( buffer, BUILD_DATE, bufferSize ); 
#elif defined(BUILDINFO_EEMEM) 
   eeprom_read_block(buffer,BUILD_DATE,bufferSize); 
#elif defined(BUILDINFO_PROGMEM) 
   memcpy_P(buffer,BUILD_DATE,bufferSize); 
#endif    
}   
