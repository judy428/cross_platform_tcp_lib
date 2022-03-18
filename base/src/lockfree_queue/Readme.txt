#define USE_LOCK
开启spinlock锁，多生产者多消费者场景
#define USE_MB
开启Memory Barrier
#define USE_POT
开启队列大小的2的幂对齐

多线程场景下，需要定义USE_LOCK宏，开启锁保护。


测试结果：
[root@localhost demo]# ./LockFreeQueueTest_mult.exe 
producer tid=139885003298560 1.709195 MB/s 13178.098190 msg/s elapsed= 79.569600 size= 1048576
consumer tid=139884986513152 1.678689 MB/s 12942.891769 msg/s elapsed= 81.015589 size= 1048576
producer tid=139885011691264 1.497569 MB/s 11546.433145 msg/s elapsed= 90.813846 size= 1048576
consumer tid=139884994905856 1.497570 MB/s 11546.440632 msg/s elapsed= 90.813787 size= 1048576

[root@localhost demo]# ./LockFreeQueueTest_mult.exe 
producer tid=139709312734976 0.810140 MB/s 6246.271679 msg/s elapsed= 167.872301 size= 1048576
consumer tid=139709287556864 0.740020 MB/s 5705.644610 msg/s elapsed= 183.778709 size= 1048576
producer tid=139709304342272 0.733638 MB/s 5656.435425 msg/s elapsed= 185.377525 size= 1048576
consumer tid=139709295949568 0.733638 MB/s 5656.435578 msg/s elapsed= 185.377520 size= 1048576

[root@localhost demo]# ./LockFreeQueueTest_mult.exe 
producer tid=140398422165248 2.868052 MB/s 22113.015072 msg/s elapsed= 47.418952 size= 1048576
consumer tid=140398413772544 2.800424 MB/s 21591.596517 msg/s elapsed= 48.564079 size= 1048576
producer tid=140398430557952 2.117407 MB/s 16325.455611 msg/s elapsed= 64.229509 size= 1048576
consumer tid=140398405379840 2.117410 MB/s 16325.479791 msg/s elapsed= 64.229414 size= 1048576

[root@localhost demo]# ./LockFreeQueueTest_mult.exe 
producer tid=139774062634752 1.491592 MB/s 11500.347954 msg/s elapsed= 91.177763 size= 1048576
consumer tid=139774045849344 1.449125 MB/s 11172.925539 msg/s elapsed= 93.849726 size= 1048576
producer tid=139774071027456 1.446062 MB/s 11149.308172 msg/s elapsed= 94.048526 size= 1048576
consumer tid=139774054242048 1.446066 MB/s 11149.338047 msg/s elapsed= 94.048274 size= 1048576

[root@localhost demo]# ./LockFreeQueueTest.exe 
producer tid=140027113031424 797.027410 MB/s 6145175.098058 msg/s elapsed= 0.170634 size= 1048576
consumer tid=140027104638720 797.093120 MB/s 6145681.734612 msg/s elapsed= 0.170620 size= 1048576

[root@localhost demo]# ./LockFreeQueueTest.exe
producer tid=140518792455936 759.530114 MB/s 5856066.531967 msg/s elapsed= 0.179058 size= 1048576
consumer tid=140518784063232 759.593832 MB/s 5856557.812062 msg/s elapsed= 0.179043 size= 1048576

其中：
    LockFreeQueueTest_mult为：两个线程读，两个线程写
    LockFreeQueueTest：单线程读写
结论：多线程读写，均需要加锁，尽量不用多线程

修改：多线程下，可以使用__sync_fetch_and_add()函数，不用加锁；此时m_tail和m_head一直递增，不用判断是否满或者空，但是可能会覆盖之前没有来得及读出来的数据
