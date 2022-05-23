# 课程项目：分布式文件系统的元数据管理模块

开设一个分布式元数据管理服务集群和一个基于 POSIX 接口的客户端。

客户端支持对元数据进行操作/修改（mkdir, create file, reade, rm file 等）

客户端支持元数据查询命令(cd, ls, stat)

元数据的分布式储存：元数据被分布在不同的元数据服务器上

## Compile

```shell
mkdir build
cd build
cmake ..
make
```



## Usage

```shell
./server
./client
```
The above two commands can show the help information.

```eg:
bash run.sh
cd build
./server -a 127.0.0.1 -b 1111 -i 127.0.0.1 -p 1111 -n 2 #创建一个IP为127.0.0.1 port为1111的master服务器,并且期望有两台slave服务器作为它的子结点
./server -a 192.168.1.120 -b 2222 -i 127.0.0.1 -p 1111 -n 2 #创建第一个slave服务器连接master
./server -a 192.168.1.121 -b 3333 -i 127.0.0.1 -p 1111 -n 2 #创建第二个slave服务器连接master
./client -i 127.0.0.1 -p 1111 #client连接master server
```



## Supported Commands

```shell
pwd
mkdir "xxx"
rm "xxx"
rm -r "xxx"
ls ""/"xxx"
ls -r ""/"xxx"
touch "xxx"
mv "xxx" "yyy"
stat "xxx"
serverstats
```

Type the above commands in clients.
