#ifndef __TRANS_CODE_H__
#define __TRANS_CODE_H__

#define SNDLEN			8192		
#define RCVLEN			8192	
#define SNDHDLEN		287

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define BS          '\010'
#define BACKSPACE   '\010'

#define HT          '\011'
#define TAB         '\011'

#define LF          '\012'

#define VT          '\013'

#define FF          '\014'

#define CR          '\015'
#define RET         '\015'
#define RETURN      '\015'
#define NUL         '\0'

#define BEL         '\007'
#define BELL        '\007'

#define CAN         '\030'

#define EM          '\031'

#define SUB         '\032'

#define ESC         '\033'
#define ESCAPE      '\033'

#define SP          '\040'
#define SPACE       '\040'
#define BLANK       '\040'
#define DEL         '\177'
#define DELETE      '\177'

#define QX          ")%"
#define WX          ")W"
#define TX          ")G"
#define UX          ")O"
#define JX          ")?"
#define KX          ")7"
#define LX          ")3"
#define XX          ")'"
#define VX          ")_"
#define NX          ")o"
#define MX          ");"


#define QY          ")$"
#define WY          ")P"
#define TY          ")@"
#define UY          ")H"
#define JY          ")<"
#define KY          ")4"
#define LY          ")0"
#define XY          ")&"
#define VY          ")X"
#define NY          ")`"
#define MY          ")8"

#define YES         'y'
#define NO          'n'
#define ERR         (-1)
#define ARG_ERR     (-2)
#define ENGLISH     0
#define CHINESE     1
#define CONSOLE     2

#define prgname     "scus"

/* message line */
#define MSGLINE     21
#define MSGCOLUMN   11

/* function key line */
#define FTKEYLINE   23

#define HEADLINE1   0
#define HEADLINE2   1

#define LEFT        1
#define RIGHT       0
typedef struct
 {
    unsigned char first_byte;
    unsigned char second_byte;
 } CH_WORD;

char ISPWD[SNDLEN+1];
unsigned char g_compress_data[254];
unsigned short g_data_len;

#ifdef __cplusplus
extern "C"
{
#endif 

 extern int EBCtoASC(unsigned char *in_EBC_str,unsigned char *out_ASC_str, \
              int EBC_str_len);
 extern int ASCtoEBC(unsigned char *in_ASC_str,unsigned char *out_EBC_str, \
             int ASC_str_len);
 extern int ASCtoEBC1(unsigned char *in_ASC_str,unsigned char *out_EBC_str, \
              int ASC_str_len);
 extern int Turn_ASCtoEBC_Str(unsigned char *in_ASC_str,int ASC_str_len,\
              int EBC_str_len);
 extern void IC_ASCtoEBC(unsigned char *in_ASC_str,unsigned char *out_EBC_str, \
                 int ASC_str_len);
 extern unsigned char SB_ASCIItoEBCDIC(unsigned char in_ASC_byte_value);
 extern unsigned char SB_EBCDICtoASCII(unsigned char in_EBC_byte_value);
 extern CH_WORD DB_ASCIItoEBCDIC_01(CH_WORD ASC_WORD);
 extern CH_WORD DB_EBCDICtoASCII_01( CH_WORD EBC_WORD);
 extern CH_WORD DB_ASCIItoEBCDIC_02(CH_WORD ASC_WORD);
 extern CH_WORD DB_EBCDICtoASCII_02( CH_WORD EBC_WORD);

extern void DspMsg(int , char *);
extern void InTrimStr(char *,char *);

#ifdef __cplusplus
}
#endif

#endif
