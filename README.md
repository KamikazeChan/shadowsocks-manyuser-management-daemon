# shadowsocks-manyuser-management-daemon
大致上是要做一个ss多用户管理程序，用来管理shadowsocks-libev。打算用c语言写，然而并不是很精通c语言。

于是挖个坑把自己埋了。

## 使用方法

首先需要安装 [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev) 作为ss服务端.

可以编译安装或使用现成的对应发行版的安装包,只要在终端内可以执行 "/usr/local/bin/ss-server" 就可以.

之后需要一个可以被正确访问的mysql服务器,设置好相关用户名,密码,数据库名,表名.

完成以上步骤之后再使用本程序,设置文件将会生成在执行目录下.设置文件比较简单,就不描述了.

本程序将会按照固定时间间隔访问mysql,获取指定组别的用户的信息,然后比对服务器上已有的进程.自动创建本地没有的进程,自动销毁再创建数据已经改变的进程(一个进程对应一个端口).从而达到配合前端使用的目的.

## 下载

下载请直接至 [RELEASES](https://github.com/czp3009/shadowsocks-manyuser-management-daemon/releases) 页面,如果该页面还没有东西,说明本程序还在测试阶段.

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.
