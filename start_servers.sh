#!/bin/bash
#mysql设置
mysql_mysqlexe="mysql"          #mysql客户端命令
mysql_host="rds.hiczp.com"      #mysql主机
mysql_port="3306"               #mysql端口
mysql_user="shadowsocks"        #mysql用户名
mysql_password="123456789"      #mysql密码
mysql_database="shadowsocks"    #mysql数据库
mysql_table="free_user"         #mysql数据表
mysql_refresh_time="60"         #刷新时间
#column设置
column_port="port"
column_password="passwd"
column_encrypt_method="encrypt_method"
column_udp_relay="udp_relay"
column_fast_open="fast_open"
column_group="group"
column_enable="enable"
#server设置
server_bind_ip="0.0.0.0"
server_group="free_user"
#ss-server -s 0.0.0.0 -p 8989 -k 1234 -m aes-256-cfb &
