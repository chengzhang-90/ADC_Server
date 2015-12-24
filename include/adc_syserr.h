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
  * 系统错误
  */
static const int READ_OPEN_FILE_ERROR  = 101;
static const int WRITE_OPEN_FILE_ERROR = 102;
static const int CREAT_FILE_ERROR      = 103;
static const int NO_REQUIRED_FIELD     = 104;

/**
  * 数据库错误
  */
static const int DB_SELECT_NOT_FOUND = 201;
static const int DB_SELECT_ERROR     = 202;
static const int DB_INSERT_ERROR     = 203;
static const int DB_UPDATE_ERROR     = 204;
static const int DB_DELETE_ERROR     = 205;
static const int DB_OPEN_CUR_ERROR   = 206;


/*
** 应用类公共错误码定义，一般应用于签约、多页式查询等
*/

static const int  DATA_REPEAT            =    301;  /*数据重复                */
static const int  FILE_END               =    302;  /*到文件尾                */
static const int  SYS_DATA_ERROR         =    303;  /*系统数据错误            */
static const int  NO_DEL                 =    304;  /*不删除                  */
static const int  DELORG_IS_REQ          =    305;  /*删除机构于签约机构不一样*/

static const int  PROD_CODE_NOT_FOUNT    =    306; /*删除机构与签约机构不一样 */
static const int  VISA_ORG_NOT_OPEN_ORG  =    308; /*开户机构和签约机构不同   */
static const int  CUSTNO_NOT_FOUND       =    307; /*没有找到记录             */
static const int  INTE_ACCT_TOO_SHORT    =    310; /*内部帐号位数不够         */
static const int  BAS_TYPE_ERROR         =    311; /*帐号类型错误             */
static const int  PROD_CODE_ERROR        =    312; /*产品代码错               */
static const int  CURR_TYPE_ERROR        =    313; /*币种错误                 */
static const int  ENDNO_STRNO_ERROR      =    314; /*起始号,中止号输入错      */
static const int  FLW_STATUS_ERROR       =    315; /*凭证状态错               */
static const int  DB_SELECT_FOUND        =    316; /*有数据                   */
static const int  PZ_CLS_ERROR           =    317; /*级别错误                 */
static const int  DATA_NUM_ERROR         =    318; /*凭证数量错误             */
static const int  PZ_ORG_ERROR           =    319; /*凭证来源错误             */
static const int  PZ_TYPE_ERROR          =    320; /*凭证类型错误             */
static const int  PZ_GET_OUT_ERROR       =    321; /*凭证付出错 */
static const int  UPDATE_YW_INSTRUCT_ERROR =  322; /*更新指令表错误 */

static const int  INSERT_CUST_VISA_ERROR   =  400; /*插入签约表失败           */
static const int  INSERT_PUB_CUST_INFO_ERROR= 401; /*插入客户公共表失败       */
static const int  INSERT_ABL_ERROR          = 402; /*插入作废表失败           */
static const int  INSERT_OLDC_ERROR         = 403; /*插入在途表失败           */
static const int  INSERT_FLW_ERROR          = 404; /*插入流转表失败           */
static const int  UPDATE_CUST_VISA_ERROR    =405; /*更新签约表错误           */
static const int  UPDATE_UNIT_CTL_ERROR     =406; /*更新单位控制表错误       */
static const int  UPDATE_PZBAL_ERROR        =407; /*更新余额表错误           */
static const int  UPDATE_PZ_OCT_ERROR       =408; /*更新凭证类别表错误       */
static const int  UPDATE_FLW_ERROR          =409; /*更新凭证流转表错误       */

static const int  PROD_CODE_IS_NULL         =500; /*产品代码为空             */
static const int  UNIT_CODE_IS_NULL         =501; /*单位编号为空             */
static const int  PAPER_NO_IS_NULL          =502; /*证件号码为空             */
static const int  PAPER_TYPE_IS_NULL        =503; /*证件类型为空             */
static const int  CURR_TYPE_IS_NULL         =504; /*币种为空                 */
static const int  BAS_ACCT_IS_NULL          =505; /*帐号为空                 */
static const int  INTE_ACCT_IS_NULL         =506; /*内部帐号为空             */
static const int  TX_ORG_IS_NULL            =507; /*机构代码为空             */
static const int  PZ_TYPE_IS_NULL           =508; /*凭证类型为空             */
static const int  ORG_TELLER_IS_NULL        =509; /*机构号和柜员不能都空     */
static const int  PZ_NUM_IS_NULL            =510; /*凭证数量为空             */
static const int  PZ_STRNO_IS_NULL          =511; /*凭证起始号为空           */
static const int  CPIC_NO_COMMISION         =512; /*操作拒绝 */ 
static const int  CPIC_NO_NEED_REVERSE      =513; /*拒绝撤消*/ 
static const int  CPIC_CHECK_ACCT_ERROR     =514; /*对帐出错*/


static const int PUB_BASE                       = 800;
static const int PUB_NOT_FOUND_VISA_ID          = 801;
static const int PUB_NOT_FOUND_BANK_NO          = 802;

#endif
