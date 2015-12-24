include $(ADC_ROOT)/src/make.dep

all: dayend

OBJS=dayend.o
LIBS = -L$(ADC_ROOT)/lib  -lcommon -lbase

dayend: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(ORACLE_LIB)
	mv -f dayend $(ADC_ROOT)/bin

.SUFFIXES:.pc .c

.pc.o:
	$(PROC) THREADS=NO DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(ADC_INCL_DIR) INCLUDE=$(APR_INCL_DIR) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=NO iname=$< oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c 
	rm -f $*.c $*.lis

.c.o:
	$(CC) $(FLAG) $(CFLAG) $(INCPATH) -c $<

clean:
	rm -f *.o
	rm -f tp*
	rm -f dayend.c
	rm -f dayend.lis
