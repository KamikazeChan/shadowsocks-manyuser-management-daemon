# shadowsocks-manyuser-management-daemon

## 介绍

用于管理 shadowsocks-libev 服务端进程

## 使用方法

首先需要安装 [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev) 作为ss服务端.

不同版本可能出现问题,因此推荐直接编译安装这里自带的 shadowsocks-libev-2.4.5 ,可能会有依赖缺失,请自行安装.

```bash
sudo apt-get install git make gcc g++ build-essential autoconf libtool libssl-dev unzip
git clone https://github.com/czp3009/shadowsocks-manyuser-management-daemon.git
unzip shadowsocks-libev-2.4.5.zip
cd shadowsocks-libev-2.4.5
./configure
make
sudo make install
```

安装完毕shadowsocks-libev之后输入 ss-server --help ,出现帮助即说明安装成功.

之后运行程序,正确运行本程序你需要一个配置正确的 [mysql数据表](https://github.com/czp3009/shadowsocks-manyuser-management-daemon/tree/master/mysql_backup_file) .

```bash
cd ..
chmod 740 ./start_servers.sh
./start_servers.sh
```

## 下载

下载请直接至 [RELEASES](https://github.com/czp3009/shadowsocks-manyuser-management-daemon/releases)

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.
