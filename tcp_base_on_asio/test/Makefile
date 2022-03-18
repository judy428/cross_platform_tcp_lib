
SUBDIRS = ./tcpclient/

define fun1
	echo "Making $(1) in $(SUBDIRS)" 
	for subdir in $(SUBDIRS); do \
		(cd $$subdir && make $(1)); \
	done;
endef

all:
# g++ -g -Wall -pthread -std=c++11 -fpic -DBOOST_SYSTEM_NO_DEPRECATED -Wl,-rpath=./ *.cpp -o server.exe -L../../../out/lib/debug -lboost_system-mt -lboost_thread -lbase  -lcommon -lrt

# g++ -g -Wall -pthread -std=c++11 -fpic -DBOOST_SYSTEM_NO_DEPRECATED -Wl,-rpath=./ tcp_server/*.cpp -o server.exe -L../../../out/lib/debug -lboost_system-mt -lboost_thread -lbase -pthread -lrt

	g++ -g -Wall -pthread -std=c++11 -fpic -DBOOST_SYSTEM_NO_DEPRECATED -Wl,-rpath=./ tcp_server/*.cpp -o server.exe -L../../out/lib/debug -lboost_system-mt -lboost_thread -lbase -ltcpAsio -lpthread -lrt

	g++ -g -Wall -pthread -std=c++11 -fpic -Wl,-rpath=./ tcp_client/*.cpp -o client.exe -I../../out/include/base/ -L../../out/lib/debug -lboost_system-mt -lboost_thread -lbase -ltcpAsio -pthread -lrt
##	$(call fun1,all)

clean:
	rm -rf *.out
	rm -rf *.exe
#	 $(call fun1,clean)
cleanall:
	rm -rf *.out
	rm -rf *.exe
	rm -rf *.so*
	$(call fun1,cleanall)
