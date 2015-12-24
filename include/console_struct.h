#ifndef __CONSOLE_PUBLIC_STRUCTURE_H__
#define __CONSOLE_PUBLIC_STRUCTURE_H__

EXEC SQL BEGIN DECLARE SECTION;

typedef struct taccount
{
      char  va_trn[6+1];                        /* 系统跟踪号|唯一标识一笔交易 */
      char  acq_trn[6+1];                       /* 受理流水 */
      char  exchangeno[16+1];                   /* 缴款流水号 */
      char  agent_dept_code[11+1];              /* 代理机构标识码 */
      char  send_dept_code[11+1];               /* 发送机构标识码 */
      char  hldist_code_stl[11+1];              /* 接收机构标识码 */
      char  srvist_code_stl[11+1];              /* 服务机构代码 */
      char  ctr_code_stl[11+1];                 /* 银联交换中心代码 */
      char  va_code_stl[12+1];                  /* 增值平台中心代码 */
      char  acq_code_stl[11+1];                 /* 收单行代码 */
      char  tran_acct[20+1];                    /* 主账号 */
      char  iss_code_stl[11+1];                 /* 发卡机构标识码 */
      char  union_card_flag[1+1];               /* 是否银联标准卡 */
      char  credit_card_flag[2+1];              /* 借贷记卡标志 */
      char  card_medium[1+1];                   /* 卡介质 */
      char  card_info[24+1];                    /* 卡产品标识信息 */
      char  pay_card_num[19+1];                 /* 转出卡号 */
      char  exchange_rate[8+1];                 /* 持卡人扣账汇率 */
      char  accept_card_num[19+1];              /* 转入卡号 */
      char  IC_card_code[1+1];                  /* IC卡条件代码 */
      char  card_sequence_num[3+1];             /* 卡片序列号 */
      char  pay_dept_code[11+1];                /* 转出机构标识码 */
      char  accept_dept_code[11+1];             /* 转入机构标识码 */
      char  paydate[14+1];                      /* 交易时间 */
      char  tran_dtm[10+1];                     /* 传输时间 */
      char  tran_amt[12+1];                     /* 交易金额 */
      char  tx_code[4+1];                       /* 交易代码 IPER:消费 IECR：消费撤消 IPBI：查询 */
      char  payid[2+1];                         /* 交易类型码 */
      char  trade_region_flag[1+1];             /* 交易地域标志 */
      char  area_flag[1+1];                     /* 同城|异地标志 */
      char  srvmer_type[4+1];                   /* 商户类型 */
      char  merchant_id[15+1];                  /* 商户代码 */
      char  merchant_address[40+1];             /* 商户名称地址 */ 
      char  hldmer_code_stl[20+1];              /* 接入商户代码 */ 
      char  srvmer_code_stl[20+1];              /* 服务商户代码 */ 
      char  connect_mer[15+1];                  /* 受理商户号;接入商户号 */
      char  term_code[8+1];                     /* 受卡机终端标识码 */
      char  terminal_type[8+1];                 /* 终端类型 */
      char  terminal_able[1+1];                 /* 终端读取能力 */
      char  postrace[6+1];                      /* pos终端凭证号 */
      char  terminal_id[8+1];                   /* 受理终端号 */
      char  service_code[2+1];                  /* 服务点条件码 */
      char  service_point_form[3+1];            /* 服务点输入方式 */ 
      char  acq_cost[12+1];                     /* 应收手续费 */
      char  pay_cost[12+1];                     /* 应付手续费 */
      char  turn_cost[12+1];                    /* 转接服务费 */
      char  installment_cost[12+1];             /* 分期付款附加手续费 */
      char  carder_trade_cost[12+1];            /* 持卡人交易手续费 */
      char  brand_cost[12+1];                   /* 品牌服务费 */
      char  single_double[1+1];                 /* 单双转换标志 */
      char  tran_curr[3+1];                     /* 交易货币 */
      char  acq_fee[12+1];                      /* 应收总费用 */
      char  pay_fee[12+1];                      /* 应付总费用 */
      char  orig_va_trn[6+1];                   /* 原始交易的系统跟踪号 */
      char  orig_trade_term_type[2+1];          /* 原始交易终端类型 */
      char  orig_acq_trn[6+1];                  /* 原受理流水 */
      char  orig_trade_index_num[12+1];         /* 原始交易检索参考号 */
      char  orig_trade_date[10+1];              /* 原始交易的日期时间 */
      char  orig_trade_amount[12+1];            /* 原始交易金额 */
      char  auth_code[6+1];                     /* 授权应答码 */
      char  ref_no[12+1];                       /* 检索参考号 */
      char  rep_code[3+1];                      /* 交易返回码 */
      char  comp_flag[2+1];                     /* 完成标识 00:成功未冲正02:成功被冲正03:冲正04:通知类05:中心拒绝 */
      char  reckon_date[8+1];                   /* 入库对账时间*/
      char  record_id[6+1];                     /* 银联文件记录号 */
      char  recon_flag[2+1];                    /* 对账标记 00为匹配 */
      char  reckon_flag[2+1];                   /* 对账状态 Y or N */
      char  mistake_reason_code[4+1];           /* 差错原因标识码 */
      char  send_settlement_dept[11+1];         /* 发送方清算机构 */
      char  accept_settlement_dept[11+1];       /* 接受方清算机构 */
      char  sett_date[8+1];                     /* 清算日期 */
      char  sett_flag[1+1];                     /* 清算标记 Y or N */
      char  spare_field[90+1];                  /* 备用字段 */
      char  tac_flag[1+1];                      /* TAC标志 */
}TACCOUNT,*PTACCOUNT;


typedef struct _web_detail
{
     int      m_nPtSerial;
     char     m_sPtDate[9];
     char     m_sHoTxCode[13];
     char     m_sAuthNo[13];
     char     m_sRefnum[21];
     char     m_sCardNo[33];
     char     m_sTxTeller[21];
     double   m_fTxAmt;
     int      m_nTxScore;
     char     m_sRetCode[11];
     char     m_sRetMsg[101];
     char     m_sIssOrgid[21];
     char     m_sServOrgid[21];
     char     m_sSysDate[9];
     char     m_sSysTime[7];
     char     m_sSettleDate[9];
     char     m_sSettleTime[7];
     int      m_nNodeId;
     char     m_sStatus[2];
     int      m_nOldPtSerial;
     char     m_sOldPtDate[9];
     char     m_sDac[17];
}T_SVC_WebDetail;
typedef struct _web_detail_ind
{
     short   m_nPtSerial;
     short   m_sPtDate;
     short   m_sHoTxCode;
     short   m_sAuthNo;
     short   m_sRefnum;
     short   m_sCardNo;
     short   m_sTxTeller;
     short   m_fTxAmt;
     short   m_nTxScore;
     short   m_sRetCode;
     short   m_sRetMsg;
     short   m_sIssOrgid;
     short   m_sServOrgid;
     short   m_sSysDate;
     short   m_sSysTime;
     short   m_sSettleDate;
     short   m_sSettleTime;
     short   m_nNodeId;
     short   m_nStatus;
     short   m_nOldPtSerial;
     short   m_sOldPtDate;
     short   m_sDac;
}T_SVC_WebDetailInd;

EXEC SQL END DECLARE SECTION;

#endif
