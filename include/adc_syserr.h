#ifndef __ADC_SYSTEM_ERROR_H__
#define __ADC_SYSTEM_ERROR_H__

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define SQLNOTFOUND 1403
/**
  * ϵͳ����
  */
static const int READ_OPEN_FILE_ERROR  = 101;
static const int WRITE_OPEN_FILE_ERROR = 102;
static const int CREAT_FILE_ERROR      = 103;
static const int NO_REQUIRED_FIELD     = 104;

/**
  * ���ݿ����
  */
static const int DB_SELECT_NOT_FOUND = 201;
static const int DB_SELECT_ERROR     = 202;
static const int DB_INSERT_ERROR     = 203;
static const int DB_UPDATE_ERROR     = 204;
static const int DB_DELETE_ERROR     = 205;
static const int DB_OPEN_CUR_ERROR   = 206;


/*
** Ӧ���๫�������붨�壬һ��Ӧ����ǩԼ����ҳʽ��ѯ��
*/

static const int  DATA_REPEAT            =    301;  /*�����ظ�                */
static const int  FILE_END               =    302;  /*���ļ�β                */
static const int  SYS_DATA_ERROR         =    303;  /*ϵͳ���ݴ���            */
static const int  NO_DEL                 =    304;  /*��ɾ��                  */
static const int  DELORG_IS_REQ          =    305;  /*ɾ��������ǩԼ������һ��*/

static const int  PROD_CODE_NOT_FOUNT    =    306; /*ɾ��������ǩԼ������һ�� */
static const int  VISA_ORG_NOT_OPEN_ORG  =    308; /*����������ǩԼ������ͬ   */
static const int  CUSTNO_NOT_FOUND       =    307; /*û���ҵ���¼             */
static const int  INTE_ACCT_TOO_SHORT    =    310; /*�ڲ��ʺ�λ������         */
static const int  BAS_TYPE_ERROR         =    311; /*�ʺ����ʹ���             */
static const int  PROD_CODE_ERROR        =    312; /*��Ʒ�����               */
static const int  CURR_TYPE_ERROR        =    313; /*���ִ���                 */
static const int  ENDNO_STRNO_ERROR      =    314; /*��ʼ��,��ֹ�������      */
static const int  FLW_STATUS_ERROR       =    315; /*ƾ֤״̬��               */
static const int  DB_SELECT_FOUND        =    316; /*������                   */
static const int  PZ_CLS_ERROR           =    317; /*�������                 */
static const int  DATA_NUM_ERROR         =    318; /*ƾ֤��������             */
static const int  PZ_ORG_ERROR           =    319; /*ƾ֤��Դ����             */
static const int  PZ_TYPE_ERROR          =    320; /*ƾ֤���ʹ���             */
static const int  PZ_GET_OUT_ERROR       =    321; /*ƾ֤������ */
static const int  UPDATE_YW_INSTRUCT_ERROR =  322; /*����ָ������ */

static const int  INSERT_CUST_VISA_ERROR   =  400; /*����ǩԼ��ʧ��           */
static const int  INSERT_PUB_CUST_INFO_ERROR= 401; /*����ͻ�������ʧ��       */
static const int  INSERT_ABL_ERROR          = 402; /*�������ϱ�ʧ��           */
static const int  INSERT_OLDC_ERROR         = 403; /*������;��ʧ��           */
static const int  INSERT_FLW_ERROR          = 404; /*������ת��ʧ��           */
static const int  UPDATE_CUST_VISA_ERROR    =405; /*����ǩԼ�����           */
static const int  UPDATE_UNIT_CTL_ERROR     =406; /*���µ�λ���Ʊ����       */
static const int  UPDATE_PZBAL_ERROR        =407; /*�����������           */
static const int  UPDATE_PZ_OCT_ERROR       =408; /*����ƾ֤�������       */
static const int  UPDATE_FLW_ERROR          =409; /*����ƾ֤��ת�����       */

static const int  PROD_CODE_IS_NULL         =500; /*��Ʒ����Ϊ��             */
static const int  UNIT_CODE_IS_NULL         =501; /*��λ���Ϊ��             */
static const int  PAPER_NO_IS_NULL          =502; /*֤������Ϊ��             */
static const int  PAPER_TYPE_IS_NULL        =503; /*֤������Ϊ��             */
static const int  CURR_TYPE_IS_NULL         =504; /*����Ϊ��                 */
static const int  BAS_ACCT_IS_NULL          =505; /*�ʺ�Ϊ��                 */
static const int  INTE_ACCT_IS_NULL         =506; /*�ڲ��ʺ�Ϊ��             */
static const int  TX_ORG_IS_NULL            =507; /*��������Ϊ��             */
static const int  PZ_TYPE_IS_NULL           =508; /*ƾ֤����Ϊ��             */
static const int  ORG_TELLER_IS_NULL        =509; /*�����ź͹�Ա���ܶ���     */
static const int  PZ_NUM_IS_NULL            =510; /*ƾ֤����Ϊ��             */
static const int  PZ_STRNO_IS_NULL          =511; /*ƾ֤��ʼ��Ϊ��           */
static const int  CPIC_NO_COMMISION         =512; /*�����ܾ� */ 
static const int  CPIC_NO_NEED_REVERSE      =513; /*�ܾ�����*/ 
static const int  CPIC_CHECK_ACCT_ERROR     =514; /*���ʳ���*/


static const int PUB_BASE                       = 800;
static const int PUB_NOT_FOUND_VISA_ID          = 801;
static const int PUB_NOT_FOUND_BANK_NO          = 802;

#endif
