# libmysqlclient

版本 V18.0.0

来自于 mysql-5.5.48 .其中头文件是编译主程序需要的.

如果你的系统中没有 libmysqlclient18.0.0 ,你需要安装它(同时需要glibc版本 2.14 以上才可以运行).

```bash
sudo apt-get install libmysqlclient18
```

本目录下带有一份适用于amd64设备的libmysqlclient.so.18.0.0,可以直接放入如下路径 /usr/lib/x86_64-linux-gnu/libmysqlclient.so.18 必要时设定ln -s.

访问 [Oracle官网](http://dev.mysql.com/downloads/mysql/5.5.html) 获得更多信息.

该链接库用于c语言访问mysql5.5(经测试,访问mysql5.6没有异常).
