#ifndef __CONSOLE_PUBLIC_STRUCTURE_H__
#define __CONSOLE_PUBLIC_STRUCTURE_H__

EXEC SQL BEGIN DECLARE SECTION;

typedef struct taccount
{
      char  va_trn[6+1];                        /* ϵͳ���ٺ�|Ψһ��ʶһ�ʽ��� */
      char  acq_trn[6+1];                       /* ������ˮ */
      char  exchangeno[16+1];                   /* �ɿ���ˮ�� */
      char  agent_dept_code[11+1];              /* ���������ʶ�� */
      char  send_dept_code[11+1];               /* ���ͻ�����ʶ�� */
      char  hldist_code_stl[11+1];              /* ���ջ�����ʶ�� */
      char  srvist_code_stl[11+1];              /* ����������� */
      char  ctr_code_stl[11+1];                 /* �����������Ĵ��� */
      char  va_code_stl[12+1];                  /* ��ֵƽ̨���Ĵ��� */
      char  acq_code_stl[11+1];                 /* �յ��д��� */
      char  tran_acct[20+1];                    /* ���˺� */
      char  iss_code_stl[11+1];                 /* ����������ʶ�� */
      char  union_card_flag[1+1];               /* �Ƿ�������׼�� */
      char  credit_card_flag[2+1];              /* ����ǿ���־ */
      char  card_medium[1+1];                   /* ������ */
      char  card_info[24+1];                    /* ����Ʒ��ʶ��Ϣ */
      char  pay_card_num[19+1];                 /* ת������ */
      char  exchange_rate[8+1];                 /* �ֿ��˿��˻��� */
      char  accept_card_num[19+1];              /* ת�뿨�� */
      char  IC_card_code[1+1];                  /* IC���������� */
      char  card_sequence_num[3+1];             /* ��Ƭ���к� */
      char  pay_dept_code[11+1];                /* ת��������ʶ�� */
      char  accept_dept_code[11+1];             /* ת�������ʶ�� */
      char  paydate[14+1];                      /* ����ʱ�� */
      char  tran_dtm[10+1];                     /* ����ʱ�� */
      char  tran_amt[12+1];                     /* ���׽�� */
      char  tx_code[4+1];                       /* ���״��� IPER:���� IECR�����ѳ��� IPBI����ѯ */
      char  payid[2+1];                         /* ���������� */
      char  trade_region_flag[1+1];             /* ���׵����־ */
      char  area_flag[1+1];                     /* ͬ��|��ر�־ */
      char  srvmer_type[4+1];                   /* �̻����� */
      char  merchant_id[15+1];                  /* �̻����� */
      char  merchant_address[40+1];             /* �̻����Ƶ�ַ */ 
      char  hldmer_code_stl[20+1];              /* �����̻����� */ 
      char  srvmer_code_stl[20+1];              /* �����̻����� */ 
      char  connect_mer[15+1];                  /* �����̻���;�����̻��� */
      char  term_code[8+1];                     /* �ܿ����ն˱�ʶ�� */
      char  terminal_type[8+1];                 /* �ն����� */
      char  terminal_able[1+1];                 /* �ն˶�ȡ���� */
      char  postrace[6+1];                      /* pos�ն�ƾ֤�� */
      char  terminal_id[8+1];                   /* �����ն˺� */
      char  service_code[2+1];                  /* ����������� */
      char  service_point_form[3+1];            /* ��������뷽ʽ */ 
      char  acq_cost[12+1];                     /* Ӧ�������� */
      char  pay_cost[12+1];                     /* Ӧ�������� */
      char  turn_cost[12+1];                    /* ת�ӷ���� */
      char  installment_cost[12+1];             /* ���ڸ���������� */
      char  carder_trade_cost[12+1];            /* �ֿ��˽��������� */
      char  brand_cost[12+1];                   /* Ʒ�Ʒ���� */
      char  single_double[1+1];                 /* ��˫ת����־ */
      char  tran_curr[3+1];                     /* ���׻��� */
      char  acq_fee[12+1];                      /* Ӧ���ܷ��� */
      char  pay_fee[12+1];                      /* Ӧ���ܷ��� */
      char  orig_va_trn[6+1];                   /* ԭʼ���׵�ϵͳ���ٺ� */
      char  orig_trade_term_type[2+1];          /* ԭʼ�����ն����� */
      char  orig_acq_trn[6+1];                  /* ԭ������ˮ */
      char  orig_trade_index_num[12+1];         /* ԭʼ���׼����ο��� */
      char  orig_trade_date[10+1];              /* ԭʼ���׵�����ʱ�� */
      char  orig_trade_amount[12+1];            /* ԭʼ���׽�� */
      char  auth_code[6+1];                     /* ��ȨӦ���� */
      char  ref_no[12+1];                       /* �����ο��� */
      char  rep_code[3+1];                      /* ���׷����� */
      char  comp_flag[2+1];                     /* ��ɱ�ʶ 00:�ɹ�δ����02:�ɹ�������03:����04:֪ͨ��05:���ľܾ� */
      char  reckon_date[8+1];                   /* ������ʱ��*/
      char  record_id[6+1];                     /* �����ļ���¼�� */
      char  recon_flag[2+1];                    /* ���˱�� 00Ϊƥ�� */
      char  reckon_flag[2+1];                   /* ����״̬ Y or N */
      char  mistake_reason_code[4+1];           /* ���ԭ���ʶ�� */
      char  send_settlement_dept[11+1];         /* ���ͷ�������� */
      char  accept_settlement_dept[11+1];       /* ���ܷ�������� */
      char  sett_date[8+1];                     /* �������� */
      char  sett_flag[1+1];                     /* ������ Y or N */
      char  spare_field[90+1];                  /* �����ֶ� */
      char  tac_flag[1+1];                      /* TAC��־ */
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
