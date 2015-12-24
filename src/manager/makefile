include $(ADC_ROOT)/src/make.dep

all: cmdsvr nlmanager

BASEOBJ=adc_base.o
CMDOBJ=adc_cmdsvr.o
ANYOBJ=adc_manager.o

LIBS=-L$(ADC_LIB_DIR) -lbase $(APR_LIB) $(XML2_LIB)

cmdsvr: $(CMDOBJ) $(BASEOBJ)
	$(CXX) $(LDFLAGS) -o $@ $(CMDOBJ) $(BASEOBJ) $(LIBS)
	@mv $@ $(ADC_BIN_DIR)
	@echo "Make [$@] Success!"

nlmanager: $(ANYOBJ) $(BASEOBJ)
	$(CXX) $(LDFLAGS) -o $@ $(ANYOBJ) $(BASEOBJ) $(LIBS)
	@mv $@ $(ADC_BIN_DIR)
	@echo "Make [$@] Success!"

.SUFFIXES:.cpp

.cpp.o: 
	$(CXX) $(CFLAGS) $(FLAG) $(INCPATH) -g -c $<

clean:
	rm -f *.o

