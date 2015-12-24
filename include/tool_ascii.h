/*
**    tool_ascii.h
*/
#ifndef _TOOL_ASCII_H_
#define _TOOL_ASCII_H_

#ifdef __cplusplus
extern "C" {
#endif

void           TOOL_ConvertAscii2Long( unsigned char *string, 
                                       unsigned long  *num,
                                     int length);
void           TOOL_ConvertLong2Ascii( unsigned char *string, 
                                       unsigned  long num, 
                                       int length );
void 	      TOOL_ConvertAscii2Int( unsigned char *pucString,
                                     unsigned int  *puiValue,
                                     int iLength );
void           TOOL_ConvertInt2Ascii( unsigned char *pucString,
                                      unsigned int  iValue,
                                      int iLength );


void           TOOL_AsciiIncrement(unsigned char *pucAscii, int iLength);

unsigned short TOOL_Host2NetworkShort( unsigned short usConvert );
unsigned long  TOOL_Host2NetworkLong( unsigned long ulConvert );
unsigned short TOOL_Network2HostShort( unsigned short usConvert );
unsigned long  TOOL_Network2HostLong( unsigned long ulConvert );

int            TOOL_ConvertAscii2BCDR( const char *epctAsc, char *opctBcd,  int iAscLen );
void           TOOL_ConvertAscii2BCD( unsigned char *pucInBuf, 
                                      unsigned char *pucOutBuf, 
                                      int iInBufLen );
void           TOOL_ConvertBCD2Ascii( unsigned char *pucSource, 
                                      int iSourceLen,
                                      unsigned char *pucDest );
void           TOOL_ConvertAscii2EBCDIC( char *pcBufAscii, 
                                         int iLength,
                                         char *pcBufEBCDIC );
void           TOOL_ConvertEBCDIC2Ascii( char *pcEBCDICBuf, 
                                         int iEBCDICLen,
                                        char *pcAsciiBuf);
unsigned char  TOOL_ConvertAscii2Binary( char *pcString );
void           TOOL_ConvertByte2Ascii( unsigned char ucByte, 
                                      char *pcOut );
int            TOOL_BitExist( int iBitNo, unsigned char *pbyBitMap );



#ifdef __cplusplus
}
#endif

#endif



/*
**   end of file tool_ascii.h
*/
