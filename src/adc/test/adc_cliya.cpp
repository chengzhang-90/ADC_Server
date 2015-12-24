#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include "udo.h"
#include "adc.h"

#define MAXPROCS 500

double GetCurrentMillTime()
{
    struct timeval  tv ;
    double   millnum;
    double   dTmp1;
    double   dTmp2;

    gettimeofday (&tv, NULL );

     dTmp1 = tv.tv_sec;

	millnum = dTmp1 * 1000 + tv.tv_usec/1000 ;
    return( millnum );
}



#ifdef SCO
int strncasecmp( char *s1, char *s2, int len )
{
   int i = 0;
   char l, r;

   for( i = 0; i < len && *(s1+i) && *(s2+i); i++ )
   {
      l = toupper( *(s1+i) );
      r = toupper( *(s2+i) );
      if( l != r )
      {
         return l - r;
      }
   }

   l = toupper( *(s1+i) );
   r = toupper( *(s2+i) );
   if( i < len )
   {
      return l - r;
   }

   return 0;
}

int strcasecmp( char *s1, char *s2 )
{
   int i = 0;
   char l, r;

   for( i = 0; *(s1+i) && *(s2+i); i++ )
   {
      l = toupper( *(s1+i) );
      r = toupper( *(s2+i) );
      if( l != r )
      {
         return l - r;
      }
   }

   l = toupper( *(s1+i) );
   r = toupper( *(s2+i) );

   return l - r;
}
#endif
double readDouble( char *p )
{
   int i = 0;
   char val[20];

   while( p[i] == ' ' ) i++;

   while( isdigit( p[i] ) || p[i] == '.' )
   {
      i++;
   }

   memset( val, 0, sizeof(val) );
   memcpy( val, p, i );

   return atof(val);
}

long readLong( char *p )
{
   int i = 0;
   char val[20];

   while( p[i] == ' ' ) i++;

   while( isdigit( p[i] ) )
   {
      i++;
   }

   memset( val, 0, sizeof(val) );
   memcpy( val, p, i );

   return atol(val);
}

int readKeyword( char **p )
{
   char key[30];
   int i=0;
   char *lp = *p;

   memset( key, 0, sizeof(key) );
   while( islower(lp[i]) || isupper(lp[i]) ) i++;

   if( i > 10 )
   {
      return -1;
   }

   memcpy( key, lp, i );
   *p = lp+i;

   if( strcasecmp( key, "char" ) == 0 )
   {
      return UDT_STRING;
   }
   else if( strcasecmp( key, "short" ) == 0 )
   {
      return UDT_SHORT;
   }
   else if( strcasecmp( key, "int" ) == 0 )
   {
      return UDT_INT;
   }
   else if( strcasecmp( key, "long" ) == 0 )
   {
      return UDT_LONG;
   }
   else if( strcasecmp( key, "float" ) == 0 )
   {
      return UDT_FLOAT;
   }
   else if( strcasecmp( key, "double" ) == 0 )
   {
      return UDT_DOUBLE;
   }
   else
   {
      return -1;
   }
}

void readDataFromFile( UDP req, char *filename )
{
   FILE *fp;
   char line[2048], *p1, *p2, *p3;
   int len, fldid, lineNum=1, fldtype;
   short  sVal;
   int    iVal;
   long   lVal;
   float  fVal;
   double dVal;

   fp = fopen( filename, "r" );
   if( fp == NULL )
   {
      printf( "open data file %s error : %s\n", filename, strerror(errno) );
      exit( 0 );
   }

   memset( line, 0, sizeof(line) );
   while( fgets( line, sizeof(line), fp ) != NULL )
   {
      fldid = (int)readLong(line);
      p2 = strchr( line, '=' );
      p1 = p2 + 1;
      while( *p1 == ' ' ) p1++;

      fldtype = readKeyword( &p1 );
      switch( fldtype )
      {
      case UDT_STRING:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p1 = p2 + 1;
         p2 = strchr( p1, '"' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is '\"' \n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, '"' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, not enought char '\"' \n", lineNum );
            exit( 0 );
         }
         len = p3-p2-1;
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         UDO_AddField( req, fldid, UDT_STRING, p2+1, len );
         break;
      case UDT_SHORT:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         sVal = (short)readLong( p2+1 );
         UDO_AddField( req, fldid, UDT_SHORT, (char*)&sVal, len );
         break;
      case UDT_INT:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         iVal = (int)readLong( p2+1 );
         UDO_AddField( req, fldid, UDT_INT, (char*)&iVal, len );
         break;
      case UDT_LONG:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         lVal = readLong( p2+1 );
         UDO_AddField( req, fldid, UDT_LONG, (char*)&lVal, len );
         break;
      case UDT_FLOAT:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         fVal = (float)readDouble( p2+1 );
         UDO_AddField( req, fldid, UDT_FLOAT, (char*)&fVal, len );
         break;
      case UDT_DOUBLE:
         p2 = strchr( p1, '(' );
         if( p2 == NULL )
         {
            printf( "LINE %d Invalid format, expected char is (\n", lineNum );
            exit( 0 );
         }
         p3 = strchr( p2+1, ')' );
         if( p3 == NULL )
         {
            printf( "LINE %d Invalid format, needed char is )\n", lineNum );
            exit( 0 );
         }
         dVal = readDouble( p2+1 );
         UDO_AddField( req, fldid, UDT_DOUBLE, (char*)&dVal, len );
         break;
      default:
         printf( "LINE %d Invalid datatype\n", lineNum );
         exit( 0 );
      }
      memset( line, 0, sizeof(line) );
      lineNum++;
   }

   fclose( fp );
}

int main( int argc, char *argv[])
{
   char req[204800], res[204800];
   int a, code;
	int  iActProc[MAXPROCS], ProcStatus, iLoop, i;
	int  iProcNum, iTxNum;
	char aczTransFile[ 10+1 ], CommMode;
	int  iRet;
	int  iSucNum, icFailNum, iApErrNum;
	int  iSumSucNum , iSumCFailNum , iSumApErrNum;

	double lBegTime, lEndTime, lLastTime, lMaxTime, lMinTime, lTotalTime  ;
	double lAllMaxTime, lAllMinTime, iSumTotalTime;
   int  iCliSerial;

	FILE * fp;
	char FileName[ 20 ] ; 
	char aczCmd[ 30 ] , aczLineBuf[256] ;


   if( argc != 8)
   {
      printf( "Usage : %s <call name> <ip> <port> <data file> 并发进程数 单进程交易数 交易机构号 \n", argv[0] );
      return 0;
   }
   if (apr_initialize() != APR_SUCCESS) {
        fprintf(stderr, "Something went wrong\n");
        exit(-1);
   }
   atexit(apr_terminate);

   AdpClient client( argv[2], atoi(argv[3]) );

   client.init( "aaa" );

   UDO_Init( req, sizeof(req) );
   UDO_Init( res, sizeof(res) );

   readDataFromFile( req, argv[4] );

   UDO_Print( req );

	iProcNum = atoi (argv[5]);
	iTxNum = atoi (argv[6]) ;

	lBegTime = GetCurrentMillTime();	

	iCliSerial = 1;/*发送的交易流水*/

	for ( iLoop = 0 ; iLoop < iProcNum ; iLoop ++ ) 
	{
 
		/*iProcNum并发进程数*/
		if ( ( iActProc[ iLoop ] = fork( ) ) == 0 )
		{
			lMaxTime = 0;
			lMinTime = 100000;

			iSucNum    = 0 ;
			icFailNum  = 0 ;
		    iApErrNum = 0 ;
			lTotalTime = 0;

			/*iTxNum每个子进程发送交易数*/
			for ( i = 0 ; i < iTxNum ; i ++ ) 
			{
			        lBegTime = GetCurrentMillTime();	
				printf("------Do Trans------\n\n");
				
            iRet = client.call( argv[1],argv[1],req, res, 100 );
			        lEndTime = GetCurrentMillTime();	
				if (iRet == 0)  
				{
			   		if ( lLastTime > lMaxTime ) lMaxTime = lLastTime ;
			   		if ( lLastTime < lMinTime ) lMinTime = lLastTime ;
                                        /*该子进程下完全成功交易时间*/
					lTotalTime += lLastTime ;
				}

				if  (  iRet == 0 )  iSucNum ++ ;
				if	( iRet == -1 ) icFailNum ++ ; 
				if	( iRet == -2 ) iApErrNum ++ ; 

				iCliSerial = iCliSerial + 1;
				/*lTotalTime += lLastTime ;*/
			}

			memset( FileName , 0 ,sizeof ( FileName ) );
			sprintf( FileName , "%s/cli/log/%d" ,getenv("HOME") , getpid( ) );

			/* 记录运行结果 此处统计的是每个子进程的运行结果*/
			if( ( fp = fopen ( FileName , "w+" ) ) == NULL ) 
			{
				perror( "open file for write : " ) ;
				printf("-----------lp-----------\n\n");
				fprintf ( stderr , "filename [%s] \n" , FileName );
				exit( -1 ) ;
			}

		 fprintf ( fp , "%d %d %d %.0lf %.0lf %.0lf  \n", iSucNum , icFailNum , iApErrNum , lMaxTime , lMinTime , lTotalTime   ) ;
		/*成功交易数  通讯失败笔数  调用成功但交易失败*/
		/*成功交易最大时间    最小时间    成功交易总时间*/

			fflush( fp ) ;
			fclose( fp ) ;

			exit ( 0 ) ;
		}
		if ( iActProc[ iLoop ] < 0 )
			perror( " Fork Error : " ) ;

		iCliSerial = iCliSerial + iTxNum ;/*每个子进程的流水区间为iTxNum*/
	}


	/* 等待子进程结束 */
	for ( iLoop = 0 ; iLoop < iProcNum ; iLoop ++ ) 
	{ 
		if ( ( ProcStatus = waitpid ( iActProc[ iLoop ] , NULL , 0 ) ) != iActProc[ iLoop ] )  
		{
			fprintf ( stderr , " child process error ! \n " );
			perror ( " waiting child end error : " ) ;
		} 
	}

    lEndTime = GetCurrentMillTime();	

    lLastTime = lEndTime - lBegTime ;
    printf("\n b[%016.0lf] e[%016.0lf]\n",lEndTime,lBegTime);
	

	/*  统计运行结果 */
	iSumSucNum    = 0 ;
	iSumCFailNum  = 0 ;
	iSumApErrNum = 0 ;
	iSumTotalTime  = 0 ;
	
	lAllMaxTime = 0 ;
	lAllMinTime = 1000000 ;

	for ( iLoop = 0 ; iLoop < iProcNum ; iLoop ++ )
	{
			memset( FileName , 0 ,sizeof ( FileName ) );
			sprintf( FileName , "%s/cli/log/%d" ,getenv("HOME") , iActProc[iLoop ] );

			/* 记录运行结果 */
			if( ( fp = fopen ( FileName , "r+" ) ) == NULL ) 
			{
				perror( "open file : " ) ;
				printf("-------------lp2-------------\n\n");
				fprintf ( stderr , "filename [%s] \n", FileName ) ;
			        continue ;
			}
			fgets ( aczLineBuf , sizeof( aczLineBuf ) , fp ) ;
			sscanf ( aczLineBuf , "%d %d %d %lf %lf %lf", 
&iSucNum , &icFailNum , &iApErrNum , &lMaxTime , &lMinTime ,&lTotalTime ) ;

			iSumSucNum += iSucNum ;
			iSumCFailNum += icFailNum ;
			iSumApErrNum += iApErrNum ;
			iSumTotalTime += lTotalTime ;

			/***printf("\n\n pre iSumTotalTime[%.0lf]\n\n", iSumTotalTime);

			printf("\n\naczLineBuf[%s]\n\n", aczLineBuf);

                        printf("\n\niSucNum[%d]", iSucNum);
                        printf("\n\nicFailNum[%d]", icFailNum);
                        printf("\n\niApErrNum[%d]", iApErrNum);
                        printf("\n\nlMaxTime[%.0lf]", lMaxTime);
                        printf("\n\nlMinTime[%.0lf]", lMinTime);
                        printf("\n\nlTotalTime[%.0lf]", lTotalTime);
                        printf("\n\niSumTotalTime[%.0lf]", iSumTotalTime);***/
			
			if ( lAllMaxTime < lMaxTime ) lAllMaxTime = lMaxTime ;
			if ( lAllMinTime > lMinTime ) lAllMinTime = lMinTime ;

			fclose( fp ) ;
	}
	fprintf ( stderr , " ================汇总统计=======================\n" );  
	fprintf ( stderr , "             交易总笔数 [ %d ]\n" , iProcNum * iTxNum );
	fprintf ( stderr , "             完全成功交易总笔数 [ %d ]\n" , iSumSucNum  );
	fprintf ( stderr , "             交易调用成功总笔数[ %d ]\n" , iSumSucNum+iSumApErrNum  );
	fprintf ( stderr , "             交易通讯失败总笔数[ %d ]\n" , iSumCFailNum  );
	fprintf ( stderr , "             交易总时间   [ %.3lf ]\n" , lLastTime * 1.0 / 1000  );
	fprintf ( stderr , "             交易处理吞吐量    [ %.3lf ]\n" ,
			 1/(lLastTime * 1.0 / 1000 / (iProcNum * iTxNum)) );

	if ( iSumSucNum != 0 )
	{
		fprintf ( stderr , "             完全成功交易平均时间 [ %.3lf ]\n" ,
			 iSumTotalTime * 1.0 / 1000 / iSumSucNum );
		fprintf ( stderr , "             完全成功交易最大时间 [ %.3lf ]\n" ,
			 lAllMaxTime * 1.0 / 1000  );
		fprintf ( stderr , "             完全成功交易最小时间 [ %.3lf ]\n" ,
			 lAllMinTime * 1.0 / 1000  );
	}
	else
	fprintf (stderr , " ################无成功的交易 ############ \n"); 
	exit(0);



#if 0
   a = client.call( argv[1],argv[1],req, res, 100 );
   printf( "======================================\n" );
   printf( "call return %d", a );
   if( a != 0 )
   {
      code = client.getError();
      if( code == 9 )
      {
         printf( " and server coredump" );
      }
      else if( code == 10 )
      {
         printf( " and server no such function" );
      }
      else if( code == 6 || code == 2 )
      {
         printf( " and server not started" );
      }
      else
      {
         printf( " error code is %d", code );
      }
      printf( " or %s\n", strerror(errno) );
      return 2;
   }
   printf( " success\n" );
   printf( "======================================\n" );

   UDO_Print( res );
#endif

   return -1;
}
