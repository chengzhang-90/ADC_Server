#ifndef __TOOL_DATETIME_H__
#define __TOOL_DATETIME_H__


#ifdef __cplusplus
extern "C" {
#endif


extern void TOOL_GetSystemYYYYMMDD( char *pcYYYYMMDD );
extern void TOOL_GetSystemYYMMDD( char *pcYYMMDD );
extern void TOOL_GetSystemYYMM( char *pcYYMM );
extern void TOOL_GetSystemMMDD( char *pcMMDD );
extern void TOOL_GetSystemMMDDhhmmss( char *pcMMDDhhmmss );
extern void TOOL_GetSystemhhmmss( char *pchhmmss );
extern void TOOL_GetSystemhhmm( char *pchhmm );
extern void TOOL_GetSystemYYYYMMDDhhmmss( char *pcYYYYMMDDhhmmss );
extern void TOOL_GetSystemYYMMDDhhmmss( char *pcYYMMDDhhmmss );
extern int TOOL_GetWeekofDay(char * epaczDate );
extern int TOOL_GetDateBefore(int eiDateNum ,char * epaczDate, char * opaczDate);




#ifdef __cplusplus
}
#endif


#endif

