#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdio.h>   
#include <stdlib.h>
#include <zlib.h>
#include <locale.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <time.h>
#include <sys/time.h>
#include <iconv.h>
#include "log.h"
#include "pub_base.h"
#include "stdsoap2.h"
#include "apaylibH.h"
#include "apaylib.nsmap"
#include "apyfunc.h"


static int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
    iconv_t cd;
    int rc;
    char **pin=&inbuf;
    char **pout=&outbuf;

    size_t iInLen;
    size_t iOutLen;

    iInLen = inlen;
    iOutLen =outlen;
    
    cd =iconv_open(to_charset,from_charset);
    if(cd==0) return -1;
    memset(outbuf,0,outlen);
    //rc=iconv(cd,pin,&inlen,pout,&outlen);
    rc=iconv(cd, pin, &iInLen, pout, &iOutLen);
    //return -1;
    iconv_close(cd);

    return rc;
}

static int GbToUtf(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
  return    code_convert("gb2312","utf-8" ,inbuf,inlen,outbuf,outlen);

}


static int UtfToGb(char *inbuf,int inlen,char *outbuf,int outlen)  //g2u
{
   return  code_convert("utf-8" ,"gb2312",inbuf,inlen,outbuf,outlen);

}


static int proSetCouponList(char *pszList,int *pnListLen, 
	struct coupon__couponRsp *pstCoupon, int nCouponNum, 
	int nCouponMax, char cIsLastFlag)
{
	char szTmp[1024+1];
	int nLen=0;
	int i=0;
	char szAmount[10+1];
	char szBuffer[50+1];

	*pnListLen = 0;
	
	memset(szTmp, 0, sizeof(szTmp));
	/*
	
	1	券总数	N	2	该手机号券总数
	"10":10张券 
	2	券明细条数	N	1	'1'：只有一条券信息 
	3	有无后续明细标志	N	1	'0'：无
	'1'：有 
	4	明细长度	N	3	"100":明细长度100个字节 
	5	明细内容	ANS 0...999 	
	*/
	sprintf(szTmp, "%02d", nCouponMax);
	nLen +=2;
	sprintf(szTmp+nLen, "%d", nCouponNum%10);
	nLen +=1;
	szTmp[nLen] = cIsLastFlag;
	nLen +=1;
	/*明细长度*/
	sprintf(szTmp+nLen, "%03d", 48*nCouponNum);
	nLen += 3;
	/*明细内容*/
	/*
	
	1	发券方标识	BCD 1	0x00：自发券
	0x01：百度券
	0x02：12580
	0x03．..	券显示采用2个汉字，简称：
	00-显示"梅泰诺"；01-显示"百度"；02-显示"移动"。
	2	券类型	BCD 1	0x00：团购券
	0x01：代金券（赠领）
	0x02：代金券（购买）	
	3	券码	ANS 20	补位规则：右补0x00	
	4	购券金额	BCD 5	后两代表角、分。补位规则：高位补0x00。	针对00，02类型电子券。
	5	验密模式	BCD 1	0x00：不验密。
	0x01：验密。	
	*/
	LOG4C(( LOG_DEBUG, "nCouponNum[%d]", nCouponNum));
	for(i=0; i<nCouponNum; i++)
	{

		LOG4C(( LOG_DEBUG, "iss_USCOREtype[%s]", (pstCoupon+i)->iss_USCOREtype));
		LOG4C(( LOG_DEBUG, "type[%s]", (pstCoupon+i)->type));
		LOG4C(( LOG_DEBUG, "coupon_USCOREid[%s]", (pstCoupon+i)->coupon_USCOREid));
		LOG4C(( LOG_DEBUG, "validate[%s]", (pstCoupon+i)->validate));
		if((pstCoupon+i)->ratio_USCOREamt != NULL)
		{
			LOG4C(( LOG_DEBUG, "ratio_USCOREamt[%ld]", *(pstCoupon+i)->ratio_USCOREamt));
		}
		TOOL_ConvertAscii2BCD((pstCoupon+i)->iss_USCOREtype,szTmp+nLen,2);
		nLen +=1;
		TOOL_ConvertAscii2BCD((pstCoupon+i)->type,szTmp+nLen,2);
		nLen +=1;
		strncpy(szTmp+nLen, (pstCoupon+i)->coupon_USCOREid, 20);
		nLen +=20;
		memset(szAmount, 0, sizeof(szAmount));
		
		if((pstCoupon+i)->ratio_USCOREamt != NULL)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szAmount, "%010ld", *(pstCoupon+i)->ratio_USCOREamt);
			
			LOG4C(( LOG_DEBUG, "ratio_USCOREamtaa[%s]", szAmount));
			TOOL_ConvertAscii2BCD(szAmount,szTmp+nLen,10);
		}
		nLen += 5;
		TOOL_ConvertAscii2BCD((pstCoupon+i)->validate,szTmp+nLen,2);
		nLen += 1;	

		memset(szBuffer, 0, sizeof(szBuffer));
		
		UtfToGb((pstCoupon+i)->coupon_USCOREname,strlen((pstCoupon+i)->coupon_USCOREname),szBuffer,sizeof(szBuffer)-1);	
		LOG4C(( LOG_DEBUG, "coupon_USCOREname[%s]", szBuffer));
		sprintf(szTmp+nLen, "%-20.20s", szBuffer);
		//strncpy(szTmp+nLen, (pstCoupon+i)->coupon_USCOREname, 20);
		nLen +=20;
	}
	
	memcpy(pszList, szTmp, nLen);
	*pnListLen = nLen;
	return 0;
}


int doCardCouponQuery(ST_CARD_COUPON_QUREY_REQ *pstReq, ST_CARD_COUPON_QUREY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponSerachByPanReq request;
	struct coupon__couponSearchByPanResponse response;
	struct coupon__couponSearchByPanRsp * pstGsoapRsp=NULL;
	struct coupon__couponRsp * pstCoupon=NULL;	
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.amount = &lAmount;
  	request.pan = pstReq->szPan;
	
	nRet = soap_call_coupon__couponSearchByPan(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstGsoapRsp = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstGsoapRsp->rspdesc != NULL)
	{
		UtfToGb(pstGsoapRsp->rspdesc,strlen(pstGsoapRsp->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstGsoapRsp->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstGsoapRsp->rspcode);
		strcpy(pstRsp->szRespInfo, szRestInfo);
		goto END;
	}
	strcpy(pstRsp->szRespInfo, szRestInfo);
	pstCoupon = pstGsoapRsp->counpon_USCORElist;
	/*判断券的数量*/
	if(pstGsoapRsp->__sizecounpon_USCORElist > 0)
	{
		proSetCouponList(pstRsp->szCouponList,&pstRsp->nCouponListLen, \
			pstCoupon,pstGsoapRsp->__sizecounpon_USCORElist,\
			pstGsoapRsp->__sizecounpon_USCORElist, '0');
	}
	else
	{	
		pstRsp->nResp = 1;
		strcpy(pstRsp->szRespInfo, "无优惠券");
	}

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}

int doCardCouponAccept(ST_CARD_COUPON_ACCEPT_REQ *pstReq, ST_CARD_COUPON_ACCEPT_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponPanExchangeReq request;
	struct coupon__couponPanExchangeResponse response;
	struct coupon__couponPanExchangeRsp * pstReturn=NULL;
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);

	request.trans_USCORECode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.amount = &lAmount;
	request.coupon_USCOREcode = pstReq->szCouponID;
	request.validateCode = pstReq->szPin;
  	request.pan = pstReq->szPan;

    LOG4C(( LOG_DEBUG, "doCardCouponAccept"));
    LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
    LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
    LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
    LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
    LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
    LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
    LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
    LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
    LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
    LOG4C(( LOG_DEBUG, "coupon_USCOREcode[%s]", request.coupon_USCOREcode));
    LOG4C(( LOG_DEBUG, "validateCode[%s]", request.validateCode));
    LOG4C(( LOG_DEBUG, "pan[%s]", request.pan));

	nRet = soap_call_coupon__couponPanExchange(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	lAmount = *pstReturn->ratio_USCOREamount;
	sprintf(pstRsp->szReAmount, "%012ld", lAmount);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}


int doTelCouponQuery(ST_TEL_COUPON_QUREY_REQ *pstReq, ST_TEL_COUPON_QUREY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponSerachByMobileReq request;
	struct coupon__couponSearchByMobileResponse response;
	struct coupon__couponSearchByMobileRsp * pstReturn=NULL;
	struct coupon__couponRsp * pstCoupon=NULL;	
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.amount = &lAmount;
  	request.mobile = pstReq->szTelno;

	LOG4C(( LOG_DEBUG, "doTelCouponQuery"));
	 LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	 LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	 LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	 LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	 LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	 LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	 LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	 LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	 LOG4C(( LOG_DEBUG, "validateCode[%s]", request.mobile));

	
	nRet = soap_call_coupon__couponSearchByMobile(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strcpy(pstRsp->szRespInfo, szRestInfo);
	pstCoupon = pstReturn->counpon_USCORElist;

	/*判断券的数量*/
	if(pstReturn->__sizecounpon_USCORElist > 0)
	{
		proSetCouponList(pstRsp->szCouponList,&pstRsp->nCouponListLen, \
			pstCoupon,pstReturn->__sizecounpon_USCORElist,\
			pstReturn->__sizecounpon_USCORElist, '0');
	}
	else
	{	
		pstRsp->nResp = 1;
		strcpy(pstRsp->szRespInfo, "无优惠券");
	}
END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


int doTelCouponAccept(ST_TEL_COUPON_ACCEPT_REQ *pstReq, ST_TEL_COUPON_ACCEPT_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponMobileExchangeReq request;
	struct coupon__couponMobileExchangeResponse response;
	struct coupon__couponMobileExchangeRsp * pstReturn=NULL;
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);

	request.trans_USCORECode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.amount = &lAmount;
	request.coupon_USCOREcode = pstReq->szCouponID;
	request.validateCode = pstReq->szPin;
  	request.mobile = pstReq->szTelno;

    LOG4C(( LOG_DEBUG, "doCardCouponAccept"));
    LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
    LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
    LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
    LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
    LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
    LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
    LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
    LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
    LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
    LOG4C(( LOG_DEBUG, "coupon_USCOREcode[%s]", request.coupon_USCOREcode));
    LOG4C(( LOG_DEBUG, "validateCode[%s]", request.validateCode));
    LOG4C(( LOG_DEBUG, "mobile[%s]", request.mobile));

	nRet = soap_call_coupon__couponMobileExchange(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}

	LOG4C(( LOG_DEBUG, "sys_USCOREdate[%s]", pstReturn->sys_USCOREdate));
	LOG4C(( LOG_DEBUG, "sys_USCOREtime[%s]",pstReturn->sys_USCOREtime));
	LOG4C(( LOG_DEBUG, "ratio_USCOREamount[%f]",*pstReturn->ratio_USCOREamount));

	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	lAmount = *pstReturn->ratio_USCOREamount;
	sprintf(pstRsp->szReAmount, "%012ld", lAmount);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}




int doIDCouponQuery(ST_ID_COUPON_QUREY_REQ *pstReq, ST_ID_COUPON_QUREY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponSingleReq request;
	struct coupon__couponSingleResponse response;
	struct coupon__couponSingleRsp * pstReturn=NULL;
	struct coupon__couponRsp * pstCoupon=NULL;	
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	
	request.trans_USCORECode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.amount = &lAmount;
  	request.coupon_USCOREid = pstReq->szCouponID;

	LOG4C(( LOG_DEBUG, "doIDCouponQuery"));
	 LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
	 LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	 LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	 LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	 LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	 LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	 LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	 LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	 LOG4C(( LOG_DEBUG, "coupon_USCOREid[%s]", request.coupon_USCOREid));

	nRet = soap_call_coupon__couponSingle(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strcpy(pstRsp->szRespInfo, szRestInfo);
	pstCoupon = pstReturn->counpon_USCORElist;

	/*判断券的数量*/
	if(pstReturn->__sizecounpon_USCORElist > 0)
	{
		proSetCouponList(pstRsp->szCouponList,&pstRsp->nCouponListLen, \
			pstCoupon,pstReturn->__sizecounpon_USCORElist,\
			pstReturn->__sizecounpon_USCORElist, '0');
	}
	else
	{	
		pstRsp->nResp = 1;
		strcpy(pstRsp->szRespInfo, "无优惠券");
	}
END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}




int doIDCouponAccept(ST_ID_COUPON_ACCEPT_REQ *pstReq, ST_ID_COUPON_ACCEPT_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__couponIdExchangeReq request;
	struct coupon__couponCodeExchangeResponse response;
	struct coupon__couponIdExchangeRsp * pstReturn=NULL;
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);

	request.trans_USCORECode = pstReq->szTransCode;
  	request.merch_USCOREid = pstReq->szMerchantID;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;
  	request.trace_USCOREno = pstReq->szTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.amount = &lAmount;
	request.coupon_USCOREcode = pstReq->szCouponID;
	request.validateCode = pstReq->szPin;

    LOG4C(( LOG_DEBUG, "doCardCouponAccept"));
    LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
    LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
    LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
    LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
    LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
    LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
    LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
    LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
    LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
    LOG4C(( LOG_DEBUG, "coupon_USCOREcode[%s]", request.coupon_USCOREcode));
    LOG4C(( LOG_DEBUG, "validateCode[%s]", request.validateCode));
	
	nRet = soap_call_coupon__couponCodeExchange(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	lAmount = *pstReturn->ratio_USCOREamount;
	sprintf(pstRsp->szReAmount, "%012ld", lAmount);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}


int doVoidAccept(ST_VOID_COUPON_ACCEPT_REQ *pstReq, ST_VOID_COUPON_ACCEPT_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__withdrawReq request;
	struct coupon__couponWithdrawResponse response;
	struct coupon__withdrawRsp * pstReturn=NULL;
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
	soap_set_mode(&soap,SOAP_C_UTFSTRING);
	soap_set_namespaces(&soap, apaylib_namespaces);
	
	memset((char *)&request, 0, sizeof(request));
	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);

	request.trans_USCORECode = pstReq->szTransCode;
	request.merch_USCOREid = pstReq->szMerchantID;
	request.term_USCOREid =  pstReq->szTermID;
	request.trans_USCOREdate = pstReq->szDate;
	request.trans_USCOREtime = pstReq->szTime;
	request.trace_USCOREno = pstReq->szTrace;
	request.batch_USCOREno = pstReq->szBatchNo;
	request.ref_USCOREnum = pstReq->szRefNum;
	request.amount = &lAmount;
	request.coupon_USCOREcode = pstReq->szCouponID;
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;
	request.orig_USCOREref_USCOREnum = pstReq->szOldRefNum;

	LOG4C(( LOG_DEBUG, "doCardCouponAccept"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	LOG4C(( LOG_DEBUG, "coupon_USCOREcode[%s]", request.coupon_USCOREcode));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	LOG4C(( LOG_DEBUG, "orig_USCOREref_USCOREnum[%s]", request.orig_USCOREref_USCOREnum));
	
	nRet = soap_call_coupon__couponWithdraw(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1); 
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}

	LOG4C(( LOG_DEBUG, "couponWithdraw[%s][%s]", pstReturn->rspcode,szRestInfo));

	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	lAmount = *pstReturn->ratio_USCOREamount;
	sprintf(pstRsp->szReAmount, "%012ld", lAmount);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}



int doReversal(ST_REVERSAL_COUPON_ACCEPT_REQ *pstReq, ST_REVERSAL_COUPON_ACCEPT_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__backChargeReq request;
	struct coupon__chargeBackResponse response;
	struct coupon__backChargeRsp * pstReturn=NULL;
	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
	soap_set_mode(&soap,SOAP_C_UTFSTRING);
	soap_set_namespaces(&soap, apaylib_namespaces);
	
	memset((char *)&request, 0, sizeof(request));
	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);

	request.trans_USCORECode = pstReq->szTransCode;
	request.merch_USCOREid = pstReq->szMerchantID;
	request.term_USCOREid =  pstReq->szTermID;
	request.trans_USCOREdate = pstReq->szDate;
	request.trans_USCOREtime = pstReq->szTime;
	request.trace_USCOREno = pstReq->szTrace;
	request.batch_USCOREno = pstReq->szBatchNo;
	request.ref_USCOREnum = pstReq->szRefNum;
	request.Orig_USCOREtrans_USCORECode = pstReq->szOldTransCode;
	request.amount = &lAmount;
	request.coupon_USCOREcode = pstReq->szCouponID;
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;

	LOG4C(( LOG_DEBUG, "doCardCouponAccept"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "Orig_USCOREtrans_USCORECode[%s]", request.Orig_USCOREtrans_USCORECode));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	LOG4C(( LOG_DEBUG, "coupon_USCOREcode[%s]", request.coupon_USCOREcode));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	
	nRet = soap_call_coupon__chargeBack(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1); 
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}

int doTermLogin(ST_TERM_LOGIN_REQ *pstReq, ST_TERM_LOGIN_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct coupon__sginReq request;
	struct coupon__sginResponse response;
	struct coupon__sginRsp * pstReturn=NULL;
	char szRestInfo[100+1];
	
	soap_init(&soap);
	soap_set_mode(&soap,SOAP_C_UTFSTRING);
	soap_set_namespaces(&soap, apaylib_namespaces);
	
	memset((char *)&request, 0, sizeof(request));
	memset((char *)&response, 0, sizeof(response));

	request.trans_USCORECode = pstReq->szTransCode;
	request.merch_USCOREid = pstReq->szMerchantID;
	request.term_USCOREid =  pstReq->szTermID;
	request.trans_USCOREdate = pstReq->szDate;
	request.trans_USCOREtime = pstReq->szTime;
	request.trace_USCOREno = pstReq->szTrace;
	request.batch_USCOREno = pstReq->szBatchNo;


	LOG4C(( LOG_DEBUG, "doTermLogin"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCORECode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));

	nRet = soap_call_coupon__sgin(&soap, getenv( "APAY_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;
	LOG4C(( LOG_DEBUG, "rspcode[%s]",pstReturn->rspcode));

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1); 
		strcpy(pstRsp->szRespInfo, szRestInfo);
	
		LOG4C(( LOG_DEBUG, "szRestInfo[%s]",szRestInfo));
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}

	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	
	memset(szRestInfo, 0, sizeof(szRestInfo));
	
	UtfToGb(pstReturn->merchName,strlen(pstReturn->merchName),szRestInfo,sizeof(szRestInfo)-1); 
	strcpy(pstRsp->szShopName, szRestInfo);
	LOG4C(( LOG_DEBUG, "szShopName[%s]", szRestInfo));
	strncpy(pstRsp->szParm, pstReturn->param, 512);
#if 0
	strncpy(pstRsp->szParmVersion, pstReturn->term_USCOREversion, 20);
#endif

	LOG4C(( LOG_DEBUG, "sys_USCOREdate[%s]", pstReturn->sys_USCOREdate));
	LOG4C(( LOG_DEBUG, "sys_USCOREtime[%s]",pstReturn->sys_USCOREtime));
#if 0

	LOG4C(( LOG_DEBUG, "param[%s]",pstReturn->param));
	LOG4C(( LOG_DEBUG, "term_USCOREversion[%s]",pstReturn->term_USCOREversion));
#endif

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
	return 0;
}




int doWxPreCreate(ST_WX_PRE_CREATE_REQ *pstReq, ST_WX_PRE_CREATE_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__unifiedorderReq request;
	struct wechat__unifiedorderResponse response;
	struct wechat__unifiedorderRsp * pstReturn=NULL;



	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
  	request.amount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxPreCreate"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));

	nRet = soap_call_wechat__unifiedorder(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	strcpy(pstRsp->szCodeUrl, pstReturn->code_USCOREurl);
	strcpy(pstRsp->szOutTradeNo, pstReturn->out_USCOREtrade_USCOREno);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}

int doWxPay(ST_WX_PAY_REQ *pstReq, ST_WX_PAY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__micropayReq request;
	struct wechat__micripayResponse response;
	struct wechat__micropayRsp * pstReturn=NULL;



	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
  	request.amount = &lAmount;
	request.auth_USCOREcode = pstReq->szAuthCode;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxPay"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "auth_USCOREcode[%s]", request.auth_USCOREcode));

	nRet = soap_call_wechat__micripay(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	strcpy(pstRsp->szOutTradeNo, pstReturn->out_USCOREtrade_USCOREno);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


int doWxOrderQuery(ST_WX_ORDER_QUERY_REQ *pstReq, ST_WX_ORDER_QUERY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__orderQueryReq request;
	struct wechat__orderqueryResponse response;
	struct wechat__orderQueryRsp * pstReturn=NULL;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.out_USCOREtrade_USCOREno = pstReq->szOutTradeNo;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxOrderQuery"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "out_USCOREtrade_USCOREno[%s]", request.out_USCOREtrade_USCOREno));

	nRet = soap_call_wechat__orderquery(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strcpy(pstRsp->szOutTradeNo, pstReturn->out_USCOREtrade_USCOREno);
	strcpy(pstRsp->szRefNum, pstReturn->order_USCOREref_USCOREnum);
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}

int doWxOrderReversal(ST_WX_ORDER_REVERSAL_REQ *pstReq, ST_WX_ORDER_REVERSAL_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__reversalReq request;
	struct wechat__reversalResponse response;
	struct wechat__reversalRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.amount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;
	request.out_USCOREtrade_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxOrderReversal"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	LOG4C(( LOG_DEBUG, "out_USCOREtrade_USCOREno[%s]", request.out_USCOREtrade_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_wechat__reversal(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}

int doWxOrderRefund(ST_WX_ORDER_REFUND_REQ *pstReq, ST_WX_ORDER_REFUND_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__refundReq request;
	struct wechat__refundResponse response;
	struct wechat__refundRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.refund_USCOREamount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.out_USCOREtrade_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxOrderRefund"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "out_USCOREtrade_USCOREno[%s]", request.out_USCOREtrade_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_wechat__refund(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}



int doWxOrderVoid(ST_WX_ORDER_VOID_REQ *pstReq, ST_WX_ORDER_VOID_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct wechat__cancelReq request;
	struct wechat__cancelResponse response;
	struct wechat__cancelRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.amount = &lAmount;
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.out_USCOREtrade_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doWxOrderVoid"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	LOG4C(( LOG_DEBUG, "out_USCOREtrade_USCOREno[%s]", request.out_USCOREtrade_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_wechat__cancel(&soap, getenv( "APAY_WX_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}

	LOG4C(( LOG_DEBUG, "rspcode[%s]",pstReturn->rspcode));
	LOG4C(( LOG_DEBUG, "szRespInfo[%s]",pstRsp->szRespInfo));

	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


int doBaiduPay(ST_BAIDU_PAY_REQ *pstReq, ST_BAIDU_PAY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct baidu__scanPayReq request;
	struct baidu__scanpayResponse response;
	struct baidu__scanPayRsp * pstReturn=NULL;



	LONG64 lAmount=0;
	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
  	request.amount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.pay_USCOREcode = pstReq->szAuthCode;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;


	LOG4C(( LOG_DEBUG, "doBaiduPay"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "amount[%ld]", *request.amount));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "pay_USCOREcode[%s]", request.pay_USCOREcode));

	nRet = soap_call_baidu__scanpay(&soap, getenv( "APAY_BAIDU_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);
	strcpy(pstRsp->szOutTradeNo, pstReturn->order_USCOREno);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}



int doBaiduOrderQuery(ST_BAIDU_ORDER_QUERY_REQ *pstReq, ST_BAIDU_ORDER_QUERY_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct baidu__orderQueryReq request;
	struct baidu__orderqueryResponse response;
	struct baidu__orderQueryRsp * pstReturn=NULL;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.order_USCOREno = pstReq->szOutTradeNo;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doBaiduOrderQuery"));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "out_USCOREtrade_USCOREno[%s]", request.order_USCOREno));

	nRet = soap_call_baidu__orderquery(&soap, getenv( "APAY_BAIDU_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strcpy(pstRsp->szOutTradeNo, pstReturn->order_USCOREno);
	strcpy(pstRsp->szRefNum, pstReturn->order_USCOREref_USCOREnum);
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}

int doBaiduOrderVoid(ST_BAIDU_ORDER_VOID_REQ *pstReq, ST_BAIDU_ORDER_VOID_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct baidu__cancelReq request;
	struct baidu__cancelResponse response;
	struct baidu__cancelRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.amount = &lAmount;
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.order_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doBaiduOrderVoid"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	LOG4C(( LOG_DEBUG, "order_USCOREno[%s]", request.order_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_baidu__cancel(&soap, getenv( "APAY_BAIDU_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}

	LOG4C(( LOG_DEBUG, "rspcode[%s]",pstReturn->rspcode));
	LOG4C(( LOG_DEBUG, "szRespInfo[%s]",pstRsp->szRespInfo));

	
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


int doBaiduOrderRefund(ST_BAIDU_ORDER_REFUND_REQ *pstReq, ST_BAIDU_ORDER_REFUND_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct baidu__refundReq request;
	struct baidu__refundResponse response;
	struct baidu__refundRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.refund_USCOREamount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.order_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doBaiduOrderRefund"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "order_USCOREno[%s]", request.order_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_baidu__refund(&soap, getenv( "APAY_BAIDU_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


int doBaiduOrderReversal(ST_BAIDU_ORDER_REVERSAL_REQ *pstReq, ST_BAIDU_ORDER_REVERSAL_RSP *pstRsp)
{
	int nRet;
	struct soap soap;	
	struct baidu__reversalReq request;
	struct baidu__reversalResponse response;
	struct baidu__reversalRsp * pstReturn=NULL;
	LONG64 lAmount=0;

	char szRestInfo[100+1];
	
	soap_init(&soap);
  	soap_set_mode(&soap,SOAP_C_UTFSTRING);
  	soap_set_namespaces(&soap, apaylib_namespaces);
	
  	memset((char *)&request, 0, sizeof(request));
  	memset((char *)&response, 0, sizeof(response));

	lAmount = atol(pstReq->szAmount);
	request.amount = &lAmount;
  	request.batch_USCOREno = pstReq->szBatchNo;
  	request.merch_USCOREid = pstReq->szMerchantID;	
	request.orig_USCOREtrace_USCOREno = pstReq->szOldTrace;
	request.order_USCOREno = pstReq->szOutTradeNo;
	request.ref_USCOREnum = pstReq->szRefNum;
  	request.term_USCOREid =  pstReq->szTermID;
  	request.trace_USCOREno = pstReq->szTrace;	
	request.trans_USCOREcode = pstReq->szTransCode;
  	request.trans_USCOREdate = pstReq->szDate;
  	request.trans_USCOREtime = pstReq->szTime;

	LOG4C(( LOG_DEBUG, "doBaiduOrderReversal"));
	LOG4C(( LOG_DEBUG, "amount[%ld]", lAmount));	
	LOG4C(( LOG_DEBUG, "batch_USCOREno[%s]", request.batch_USCOREno));
	LOG4C(( LOG_DEBUG, "merch_USCOREid[%s]", request.merch_USCOREid));
	LOG4C(( LOG_DEBUG, "orig_USCOREtrace_USCOREno[%s]", request.orig_USCOREtrace_USCOREno));
	LOG4C(( LOG_DEBUG, "order_USCOREno[%s]", request.order_USCOREno));
	LOG4C(( LOG_DEBUG, "ref_USCOREnum[%s]", request.ref_USCOREnum));
	LOG4C(( LOG_DEBUG, "term_USCOREid[%s]", request.term_USCOREid));
	LOG4C(( LOG_DEBUG, "trace_USCOREno[%s]", request.trace_USCOREno));
	LOG4C(( LOG_DEBUG, "trans_USCORECode[%s]", request.trans_USCOREcode));
	LOG4C(( LOG_DEBUG, "trans_USCOREdate[%s]", request.trans_USCOREdate));
	LOG4C(( LOG_DEBUG, "trans_USCOREtime[%s]", request.trans_USCOREtime));

	nRet = soap_call_baidu__reversal(&soap, getenv( "APAY_BAIDU_SOAP_ENDPOINT"), NULL, &request, &response);
	if(nRet != SOAP_OK)
	{		
		pstRsp->nResp = -1;
		strcpy(pstRsp->szRespInfo, "系统故障");
		
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);		
		return -1;
	}

	pstReturn = response.return_;

	memset(szRestInfo, 0, sizeof(szRestInfo));
	if(pstReturn->rspdesc != NULL)
	{
		UtfToGb(pstReturn->rspdesc,strlen(pstReturn->rspdesc),szRestInfo,sizeof(szRestInfo)-1);	
		strcpy(pstRsp->szRespInfo, szRestInfo);
	}
	else
	{
		strcpy(pstRsp->szRespInfo, "梅泰诺核心故障");
		pstRsp->nResp = -1;
		goto END;
	}
	if(atoi(pstReturn->rspcode) != 0)
	{
		pstRsp->nResp = atoi(pstReturn->rspcode);
		goto END;
	}
	strncpy(pstRsp->szHostDate, pstReturn->sys_USCOREdate, 8);
	strncpy(pstRsp->szHostTime, pstReturn->sys_USCOREtime, 6);

END:
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	return 0;
}


