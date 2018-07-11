apue tcp  licheng
change uptime to time_out
success

基于tcp_self_ok更改
将计算机内通信改为tx2和PC之间的通信



nvidia@tegra-ubuntu:~/Documents/smartcable/network/tcp_pc_to_tx2$ ps -ef | grep 2345
nvidia    6562  4423  0 20:12 ?        00:00:00 ./tcp_server 2345
nvidia    6658  6465  0 20:16 pts/2    00:00:00 grep --color=auto 2345
能够实现IMU数据的跨主机传输，并且双方都能存文件

2346端口

