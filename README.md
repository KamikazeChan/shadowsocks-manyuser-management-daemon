# shadowsocks-manyuser-management-daemon

## 介绍

这是一个用于管理c语言编写的 shadowsocks 服务端 shadowsocks-libev 的管理程序,可以访问数据库读取数据,并创建多个本地进程,实现多用户功能.

## 功能

自动访问数据库并创建对应服务端(一个服务端一个端口)

自动创建新增数据库记录对应的服务端进程

自动销毁并重新创建数据(密码,加密方式)出现变化的端口对应的服务端进程

自动销毁服务端崩溃产生的僵尸进程

可根据数据库中 group 字段的值来选择性打开端口,达到使用同一个数据表但每台服务器打开端口不一样的效果.

加密方式的值将放到数据库中,可以进行修改,而不是全部采用统一加密方式.

数据库设计大体上兼容 ss-panel , ss-panel 没有的功能我可能考虑开发 whmcs 插件来完全配套本程序(自动化shadowsocks销售).

## 使用方法

首先需要安装 [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev) 作为ss服务端.

不同版本可能出现问题,因此推荐直接编译安装这里自带的shadowsocks-libev-2.4.5,可能会有依赖缺失,请自行安装.

```bash
sudo apt-get install git make gcc g++ build-essential autoconf libtool libssl-dev
git clone https://github.com/czp3009/shadowsocks-manyuser-management-daemon.git
cd shadowsocks-manyuser-management-daemon
cd shadowsocks-libev-2.4.5
./configure
sudo make && make install
```

安装完毕shadowsocks-libev之后输入 ss-server --help ,出现帮助即说明安装成功.

本程序需要额外的动态链接库 [libmysqlclient18](https://github.com/czp3009/shadowsocks-manyuser-management-daemon/tree/master/lib/mysqlclient) ,请先满足这个依赖.

完成以上步骤之后编译本程序(假设工作目录在编译 ss-libev 完成后没有改变).

```bash
sudo apt-get install cmake
cd ..
cd build
cmake ..
make
```

编译完毕后运行程序

```bash
cd ..
cd bin
./start_servers
```

程序配置文件将生成在主程序同级目录下,名为 config.ini ,设置比较简单,这里不做赘述.

## 尚在实现中

使用更优算法来管理进程

实现用户自助设置 udp_relay 和 fast_open

whmcs插件和页面

交互式控制台

配置文件重载功能

## 下载

下载请直接至 [RELEASES](https://github.com/czp3009/shadowsocks-manyuser-management-daemon/releases)

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.
