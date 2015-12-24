#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "udo.h"
#include "adc.h"

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

   if( argc != 5)
   {
      printf( "Usage : %s <call name> <ip> <port> <data file> \n", argv[0] );
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

   return -1;
}
