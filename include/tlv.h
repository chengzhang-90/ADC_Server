#ifndef _TLV_H_
#define _TLV_H_

#define LEN_TAG	 4	/*Tag��󳤶�*/


typedef char TTag[LEN_TAG]; /*Tag����*/


/*Tag��ǩԼ������(��󳤶�Ϊ0ʱ��ʾ����Լ��)*/
typedef struct
{
    TTag Tag;   /*��ǩ*/
    int MinLen; /*��С����*/
    int MaxLen; /*��󳤶�*/
} TTagRestriction;

typedef struct
{
    TTag Tag;
    int offset;
    int len;
    char Restriction;
    int iNesting;
} TTagList;

#define TLVERR_NONE			0
#define TLVERR_BASE			0
#define TLVERR_INVALIDTAG		(TLVERR_BASE-1)		/*�Ƿ�Tag*/
#define TLVERR_INVALIDLENGTH		(TLVERR_BASE-2)		/*�Ƿ�Length*/
#define TLVERR_DECODEFAIL		(TLVERR_BASE-3)		/*TLV����ʧ��*/
#define TLVERR_TAGNOTFOUND		(TLVERR_BASE-4)		/*Tagδ�ҵ�*/
#define TLVERR_INVALIDTAGRESTRICTION	(TLVERR_BASE-5)		/*TagԼ���Ƿ�*/

/**
@brief		TLVģ���ʼ��

TR.TagΪȫ0ʱΪԼ�����Ľ�����־
@param		TTagRestriction		TR[]	TagԼ����
@return		TLVERR_xxx
 */
extern int TLV_Init(const TTagRestriction TR[]);

/**
@brief		TLV����

@param		char	*TLVStr		TLV��
@param		int		TLVStrLen	TLV����
@return		TLVERR_xxx
*/
extern int TLV_Decode(const char *TLVStr, int TLVStrLen);

/**
@brief		ȡ��ֵ(Value)

@param		TTag	Tag			��ǩ(Tag)
@param		char	*Value		ֵ(Value)(���)
@param		int		*Len		ֵ����(���)
@param		int		Offset		ƥ���Offset����ͬ��ǩ
@return		TLVERR_xxx
 */
extern int TLV_GetValue(TTag Tag, char *Value, int *Len, int Offset);

/**
@brief		ȡ��ģ���ֵ(Value)

@param		TTag	Tag			��ǩ(ģ��Tag)
@param		int		*Offset		��ģ���ֵ��ԭTLV����ƫ��
@param		int		*Len		ֵ����(���)
@return		TLVERR_xxx
 */
extern int TLV_GetTemplate(TTag Tag, int *offset, int *len);



/**
@brief		��ó���(Length)

@param		char	*TLVStr		TLV��
@param		int		TLVStrLen	TLV����
@param		int		*Len		���ȴ�(���)
@param		int		*LenLen		���ȴ�����(���)
@return		TLVERR_xxx
 */
extern int TLV_GetLength(const unsigned char *TLVStr, int TLVStrLen, int *Len, int *LenLen);



/**
@brief		��ñ�ǩ(Tag)

@param		(in)	char	*TLVStr		TLV��
@param		(in)	int		TLVStrLen	TLV����
@param		(out)	TTag	pTag		Tag(���)
@param		(out)	int		*TagLen		Tag����
@return		TLVERR_xxx
 */
extern int TLV_GetTag(const char *TLVStr, int TLVStrLen, TTag pTag, int *TagLen);

/**
@brief		ȡ�ñ�ǩ�б�

@param		TTagList	TagList		��ǩ�б�(���)
@param		int			*TagNum		��ǩ��
@return		��
 */
extern void TLV_GetTagList(TTagList TagList[], int *TagNum);


/*
@brief		�ж��ֽ�ĳλ�Ƿ�Ϊ1

@param		char	Byte	�����ֽ�
@param		char	Bit		λ(1-8)
@return		TRUE/FALSE
 */
extern int IsBitOn(char Byte, char Bit);

/**
@brief		TLV����(��֧��ģ��Ƕ��)

@param		char	*TLVStr		TLV��
@param		int		TLVStrLen	TLV����
@return		TLVERR_xxx
 */
extern int TLV_Decode_Script(char *TLVStr, int TLVStrLen);
extern int TLV_GetChildren(TTag Tag, TTagList TagList[], int iNesting);
extern int TLV_GetParent(TTag Tag, TTag ParentTag);
typedef struct
{
    TTag Tag;  /**��ǩ*/
    char cTagLen;/*��ǩ����*/
    char szLen[3]; /**���ݳ���*/
    char szData[256+1]; /*����*/
} STTLV; 


void InitTLVPack(void);

int AddTlvToList(const STTLV *pstTlv);

int SetTlv(const char *epczTag, int enTagLen, int enFieldLen, const char *psData, STTLV *pstTlv);

int PackTLV(char *psPackBuffer, int *pnPackLen);






#endif
