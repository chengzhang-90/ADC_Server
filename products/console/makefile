
include $(HOME)/src/make.dep

CXXFLAGS = -g $(INCPATH) -z muldefs
CFLAGS = -g  $(INCPATH)
LIBS = -g $(ADCLIBS) $(APR_LIB) $(ORACLE_LIB)
LINKOBJ =  agm_call.o adc_public.o adc_203001.o
DELC= adc_203001.c adc_public.c

all: p_console

INCPATHPC=$(ADC_ROOT)/include
p_console: $(LINKOBJ)
	@echo "Compile The <"$@"> Start..."
	$(CXX) $(LINKFLAGS) -o $@ $(LINKOBJ) $(LIBS) -lsecu
	@mv -f $@ $(ADC_ROOT)/bin
	@echo "Compile The <"$@"> End!"
	@rm -f $(LINKOBJ) *.lis $(DELC) tp*
.SUFFIXES:.cpp .pc .ec

.pc.o:
	$(PROC) DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(INCPATHPC) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES LINE=YES PARSE=none iname=$< oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c
	rm -f $*.lis
	rm -f $*.c

.cpp.o:
	$(CXX) -I. $(CFLAGS) -c $<

.ec.o:
	$(EC) -I. $(CFLAGS) -c $<
	@rm -f $*.c

.c.o:
	$(CC) -I. $(CFLAGS)  -c $<

clean: 
	@rm -f $(LINKOBJ) *.lis $(DELC) adc_*.c tp*
