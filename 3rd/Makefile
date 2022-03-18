ifndef SUPPORTCMDS
SUPPORTCMDS:= all objs clean cleanall rebuild
export SUPPORTCMDS
endif

.PHONY: SUPPORTCMDS

$(SUPPORTCMDS): 
	@echo boost libhv libspdlog ;\
	$(warning $(shell sh ./spdlog.sh $@))
	$(warning $(shell sh ./boost.sh $@))