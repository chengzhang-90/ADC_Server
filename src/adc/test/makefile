include $(ADC_ROOT)/src/make.dep

all: adc_cli adc_cliya

OBJS1 = adc_client.o adc_cli.o ap_base.o 
OBJS2 = adc_client.o ap_base.o adc_cliya.o
adc_cli: $(OBJS1)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS1) -L$(ADC_LIB_DIR) -lbase $(APR_LIB) -lpthread
	mv $@ $(ADC_BIN_DIR)

adc_cliya: $(OBJS2)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS2) -L$(ADC_LIB_DIR) -lbase $(APR_LIB) -lpthread
	mv $@ $(ADC_BIN_DIR)

.SUFFIXES:.c .cpp .ecpp .pc .ec

.ecpp.o:
	$(ECXX) $(CFLAGS) $(FLAG) $(INCPATH) $? -c
	rm -f $*.C

.pc.o:
	#proc DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(ADC_INCL_DIR) INCLUDE=$(APR_INCL_DIR) sqlcheck=SEMANTICS userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=NO iname=$*.pc oname=$*.c
	proc DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(ADC_INCL_DIR) INCLUDE=$(APR_INCL_DIR) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=NO iname=$*.pc oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c
	rm -f $*.c $*.lis

.ec.o:
	$(EC) $(CFLAGS) $(FLAG) -g $(INCPATH) $? -c
	rm -f $*.c

.cpp.o:
	$(CXX) $(CFLAGS) $(FLAG) -g $(INCPATH) -c $?

.c.o:
	$(CXX) $(CFLAGS) $(FLAG) -g $(INCPATH) -c $?

clean:
	rm -f *.o *.csc *.lis
