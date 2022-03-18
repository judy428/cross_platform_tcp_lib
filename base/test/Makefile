
SUBDIRS = ./tcpserver

define fun1
	echo "Making $(1) in $(SUBDIRS)" 
	for subdir in $(SUBDIRS); do \
		(cd $$subdir && make $(1)); \
	done;
endef

all:
#	g++ -g -Wall -pthread -std=c++11 -fpic -Wl,-rpath=./ -L../../out/lib/debug -lbase tcp_client.cpp -o client.exe
	g++ --std=c++11 RingBufferTest.cpp -o RingBufferTest.exe -pthread
	g++ --std=c++11 -O3 LockFreeQueueTest.cpp -o LockFreeQueueTest.exe -lrt -pthread -L../../out/lib/debug -lbase
	g++ --std=c++11 -O3 -D USE_LOCK LockFreeQueueTest_mult.cpp -o LockFreeQueueTest_mult.exe -lrt -pthread -L../../out/lib/debug -lbase
	g++ -g -Wall -pthread -std=c++11 -Wl,-rpath=./ spdlog.cpp -o spdlog.exe -lrt -pthread -L../../out/lib/debug -lbase -I../../3rd/spdlog/include
	g++ -g -Wall -pthread -std=c++11 -Wl,-rpath=./ spdlog2.cpp -o 2spdlog.exe -lrt -pthread -L../../out/lib/debug -lbase -I../../3rd/spdlog/include
	
#	$(call fun1,all)

clean:
	rm -rf *.out
	rm -rf *.exe
#	$(call fun1,clean)
cleanall:
	rm -rf *.out
	rm -rf *.exe
	rm -rf *.so*
####	$(call fun1,cleanall)