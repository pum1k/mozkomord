# print prefix
PRINT := [MAIN]:

MAKE_TARGET :=
debug:    MAKE_TARGET := debug
clean:    MAKE_TARGET := clean
cleanall: MAKE_TARGET := cleanall

.PHONY: all cli gui debug clean cleanall

TASKS := cli
# TASKS += gui

all: $(TASKS)
	@echo \
################################################################################\
& echo \
################################ ALL TASKS DONE ################################\
& echo \
################################################################################

cli:
	@echo $(PRINT)Launching CLI Makefile...
	@$(MAKE) -f Makefile-cli $(MAKE_TARGET)
	@echo $(PRINT)CLI done.

# gui:
# 	@echo $(PRINT)Launching GUI Makefile...
# 	@$(MAKE) -f Makefile-gui $(MAKE_TARGET)
# 	@echo $(PRINT)GUI done.

debug: all

clean: all

cleanall: all
