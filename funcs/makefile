#Makefile Created By ADP 1.2
include $(ADC_ROOT)/src/make.dep

INCPATH = -I$(ADC_ROOT)/include

LINKOBJ = pub_common.o myftpcli.o easysock.o

all: $(ADC_ROOT)/lib/libcommon.a clean

$(ADC_ROOT)/lib/libcommon.a: $(LINKOBJ)
	ar rv $@ $(LINKOBJ)
	@rm -f tp??????

.SUFFIXES:.ec .c .pc

.pc.o:
	$(PROC) DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(ADC_INCL_DIR) INCLUDE=$(APR_INCL_DIR) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=NO LINE=YES iname=$*.pc oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c
	rm -f $*.c $*.lis
.ec.o:
	$(EC) $(CFLAGS) $(INCPATH) -c $<
	@rm -f $*.c
.c.o:
	$(CC) $(CFLAGS) $(FLAG) $(INCPATH) -c $<
clean:
	rm -f $(LINKOBJ)
