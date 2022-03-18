
ifndef SUPPORTCMDS
SUPPORTCMDS:= all objs clean cleanall rebuild
export SUPPORTCMDS
endif

SUBDIRS1:= \
	./3rd \
	./base	\
	./tcp_base_on_asio	\
	./thread_pool	\
	
	
.PHONY: SUPPORTCMDS

$(SUPPORTCMDS):
	for subdir in $(SUBDIRS1); do \
		echo "Making " $$subdir $@; \
		(cd $$subdir && make $@ || exit "$$?"); \
	done;
