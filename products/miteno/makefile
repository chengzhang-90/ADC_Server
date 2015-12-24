
include $(ADC_ROOT)/src/make.dep

CXXFLAGS = -g $(INCPATH) -z muldefs
CFLAGS = -g  $(INCPATH)
LIBS = -g $(ADCLIBS) $(APR_LIB) $(ORACLE_LIB)  $(APAY_LIB) $(ICONV_LIB)
LINKOBJ =  agm_call.o adc_public.o adc_dac.o adc_211003.o adc_211004.o adc_211005.o adc_211006.o adc_211007.o \
     adc_211008.o adc_211009.o adc_211010.o  adc_211011.o adc_211012.o  adc_211013.o adc_211014.o adc_211015.o \
     adc_211016.o  adc_211017.o adc_211018.o adc_211019.o adc_211022.o adc_211023.o adc_211024.o adc_211025.o \
     adc_211026.o


all: p_miteno

INCPATHPC=$(ADC_ROOT)/include
p_miteno: $(LINKOBJ)
	@echo "Compile The <"$@"> Start..."
	$(CXX) $(LINKFLAGS) -o $@ $(LINKOBJ) $(LIBS) -lsecu
	@mv -f $@ $(ADC_ROOT)/bin
	@echo "Compile The <"$@"> End!"
	rm -f tp*
	
.SUFFIXES:.cpp .pc .ec

.pc.o:
	$(PROC) DEFINE=_LARGEFILE64_SOURCE INCLUDE=$(INCPATHPC) sqlcheck=SYNTAX userid=$(DBUSER)/$(DBPASS) DEF_SQLCODE=YES PARSE=none iname=$< oname=$*.c
	$(CC) -D_LARGEFILE64_SOURCE $(CFLAGS) $(FLAG) -I. $(INCPATH) -I$(ORACLE_HOME)/include -g -c $*.c
	rm -f $*.c $*.lis

.cpp.o:
	$(CXX) -I. $(CFLAGS) -c $<

.ec.o:
	$(EC) -I. $(CFLAGS) -c $<
	@rm -f $*.c

.c.o:
	$(CC) -I. $(CFLAGS)  -c $<

clean: 
	@rm -f $(LINKOBJ)  *.lis  adc_*.c tp*
