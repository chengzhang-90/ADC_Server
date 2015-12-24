#ifndef __HSM30_H__
#define __HSM30_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HSM_BUF_MAX_SIZE 2048
#define MAX_DATA_LEN (HSM_BUF_MAX_SIZE-50)


/* 错误码宏定义 */
#define    ERR_HSM_CONNT            -9001            /*连接加密机失败*/
#define    ERR_HSM_SEND             -9002            /*发送数据到加密机失败*/
#define    ERR_HSM_RECV             -9003            /*接收数据到加密机失败*/
#define    ERR_HSM_PARA             -9004           /*参数错误*/
#define    ERR_HSM_FAIL             -9005           /*调用加密机失败*/



/*获取工作密钥*/
typedef struct
{
    unsigned char uszHostIP[15+1];                  /*加密机地址*/
    unsigned int unPort;                            /*加密机端口*/
    unsigned char ucKeyLen;                         /*密钥长度*/
    unsigned int unZmkIndex;                        /*区域主密钥索引*/
}ST_HSM30_GENERATEWORKKEY_IN;


typedef struct
{
    unsigned char uszPinKeyByZmk[16+1];             /*PIN key 区域主密钥加密密文*/
    unsigned char uszMacKeyByZmk[16+1];             /*MAC key 区域主密钥加密密文*/
    unsigned char uszPinKeyByLmk[16+1];             /*PIN key 本地主密钥加密密文*/
    unsigned char uszMacKeyByLmk[16+1];             /*MAC key 本地主密钥加密密文*/
    unsigned char uszPinKeyCheckValue[8+1];         /*PIN KEY 校验值*/
    unsigned char uszMacKeyCheckValue[8+1];         /*MAC KEY 校验值*/
}ST_HSM30_GENERATEWORKKEY_OUT;

/*解密PIN*/
typedef struct
{
    unsigned char uszHostIP[15+1];                  /*加密机地址*/
    unsigned int unPort;                            /*加密机端口*/
    unsigned char ucKeyLen;                         /*密钥长度*/
    unsigned int unZmkIndex;                        /*区域主密钥索引*/
    unsigned char uszPinKeyByZmk[16+1];             /*PIN key 区域主密钥加密密文*/
    unsigned char uszAcctNo[19+1];                  /*主账号*/
    unsigned char uszPinBlock[8+1];                 /*PIN BLOCK*/
}ST_HSM30_DECRYPT_PIN_IN;

typedef struct
{
    unsigned char ucPinPlainLen;                    /*密钥明文长度*/
    unsigned char uszPinPlain[13+1];                    /*密钥明文*/
}ST_HSM30_DECRYPT_PIN_Out;


/*计算MAC*/
typedef struct
{
    unsigned char uszHostIP[15+1];                  /*加密机地址*/
    unsigned int unPort;                            /*加密机端口*/
    unsigned char ucMacKeyLen;                      /*mac 密钥长度*/
    unsigned int unZmkIndex;                        /*区域主密钥索引*/
    unsigned char uszMacKey[16+1];                  /*MAC key 区域主密钥加密密文*/
    unsigned int unDataLen;                         /*数据长度*/    
    unsigned char usData[HSM_BUF_MAX_SIZE];         /*数据*/
}ST_HSM30_GENERATE_MAC_IN;

typedef struct
{
    unsigned char uszMac[8+1];                      /*mac */
}ST_HSM30_GENERATE_MAC_OUT;



/*计算MAC*/
typedef struct
{
    unsigned char uszHostIP[15+1];                  /*加密机地址*/
    unsigned int unPort;                            /*加密机端口*/
    unsigned char ucMacKeyLen;                      /*mac 密钥长度*/
    unsigned int unZmkIndex;                        /*区域主密钥索引*/
    unsigned char uszMacKey[16+1];                  /*MAC key 区域主密钥加密密文*/
	unsigned char ucMode;							/*算法标识0x01:XOR 0x02: ANSI9.9 0x03:ANSI9.19*/
    unsigned int unDataLen;                         /*数据长度*/    
    unsigned char usData[HSM_BUF_MAX_SIZE];         /*数据*/
}ST_HSM30_GENERATE_MAC_EXT_IN;

typedef struct
{
    unsigned char uszMac[8+1];                      /*mac */
}ST_HSM30_GENERATE_MAC_EXT_OUT;


/*产生随机工作密钥*/
typedef struct
{
    unsigned char cmd[2];
    unsigned char KeyLen;
    unsigned char KeyType;
    unsigned char Index[2];
    unsigned char TMKLen; /*此域仅当Index == 0xFFFF时存在*/
    unsigned char TMK[1]; /*此域仅当Index == 0xFFFF时存在*/
} StruCmdD107In;

typedef struct
{
    unsigned char Code;
    unsigned char KeyLen;
    unsigned char Key[1]; /*CipherKeyUnderLMK + CipherKeyUnderTMK + KeyCheckValue*/
} StruCmdD107Out;


typedef struct
  {
      unsigned char cmd[2];
      unsigned char keylen;
      unsigned char key[1]; /* cipher Key(by lmk) + pinfomat + cpin + pan */
  }               StruCmdD126In;

  typedef struct
  {
      unsigned char code;
      unsigned char pinlen;
      unsigned char pin[13]; /* plain pin */
  }               StruCmdD126Out;


/*计算MAC*/
typedef struct
{
    unsigned char cmd[2];
    unsigned char flag;
    unsigned char keylen;
    unsigned char key[1]; /* cipher Key1(by lmk) + IV + datalen(int) +
                * data */
}StruCmdD132In;

typedef struct
{
    unsigned char code;
    unsigned char mac[8];
}StruCmdD132Out;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HSM30_H__ */

