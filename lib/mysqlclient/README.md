# libmysqlclient

版本 V18.0.0.0

来自于 mysql-5.5.48 .其中头文件是编译主程序需要的.libmysqlclient.so (linux X86_64)文件过大,这里就不提供源码了.其他架构的设备可能需要重新编译该库(同时需要glibc版本 2.14 以上才可以运行).

访问 [oracle网站](http://dev.mysql.com/downloads/mysql/5.5.html) 获得更多信息.

顺便一体,mysql的cmake文件有点复杂,我不知道如何仅编译libmysqlclient.so,整个mysql的编译在我的电脑上花了10分钟.

该链接库用于c语言访问mysql5.5(经测试,访问mysql5.6没有异常).
