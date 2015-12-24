include $(ADC_ROOT)/src/make.dep

all: libadcbase.a adc_cmd 

OBJS1 = adc_cmd.o
OBJS2 = adc_handle.o adc_hooks.o adc_base.o adc_global.o

adc_cmd: $(OBJS1)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS1) -L$(ADC_LIB_DIR) -lbase $(APR_LIB) -lpthread
	@mv $@ $(ADC_BIN_DIR)
	@rm -f $(OBJS1)
	@echo "Make [$@] Success!"

libadcbase.a: $(OBJS2)
	$(AR) $@ $(OBJS2)
	mv $@ $(ADC_LIB_DIR)
	@rm -f $(OBJS2)
	@echo "Make [$@] Success!"

.SUFFIXES:.c .cpp .ecpp .pc .ec

.ecpp.o:
	$(ECXX) $(CFLAGS) $(FLAG) $(INCPATH) $? -c
	rm -f $*.C

.pc.o:
	$(PROC) DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(ADC_INCL_DIR) INCLUDE=$(APR_INCL_DIR) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=NO iname=$*.pc oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c
	rm -f $*.c $*.lis

.ec.o:
	$(EC) $(CFLAGS) $(FLAG) -g $(INCPATH) $? -c
	rm -f $*.c

.cpp.o:
	$(CXX) $(CFLAGS) $(FLAG) -g $(INCPATH) -c $?

.c.o:
	$(CC) $(CFLAGS) $(FLAG) -g $(INCPATH) -c $?

clean:
	rm -f *.o *.csc *.lis
