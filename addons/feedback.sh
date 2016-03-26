#!/bin/bash
#mysql设置
mysql_mysqlexe="mysql"          #mysql客户端命令
mysql_host="rds.hiczp.com"      #mysql主机
mysql_port="3306"               #mysql端口
mysql_user="shadowsocks"        #mysql用户名
mysql_password="123456789"      #mysql密码
mysql_database="shadowsocks"    #mysql数据库
mysql_table="node_free"         #mysql数据表
#column设置
column_ip="ip"
column_net_load_5min="net_load_5min"
column_net_load_10min="net_load_10min"
column_net_load_15min="net_load_15min"
#server设置
server_ip="1.1.1.1"             #服务器ip
server_net_device="wlan0"       #外网网卡
#初始化变量
net_load_5min=0
net_load_10min=0
net_load_15min=0

#工作循环
while [[ true ]]; do
    #打印时间
    printf "[`date`]\n"
    #获得出流量
    transmit_now=`cat /proc/net/dev | grep ${server_net_device} | awk '{print $10}'`
    #计算平均负载
    printf "Net load history(5min,10min,15min): %d %d %d\n" $transmit_5min_ago $transmit_10min_ago $transmit_15min_ago
    if [[ -n $transmit_15min_ago ]]; then
        net_load_15min=`expr \( $transmit_now - $transmit_15min_ago \) / 900`
    fi
    if [[ -n $transmit_10min_ago ]]; then
        net_load_10min=`expr \( $transmit_now - $transmit_10min_ago \) / 600`
    fi
    if [[ -n $transmit_5min_ago ]]; then
        net_load_5min=`expr \( $transmit_now - $transmit_5min_ago \) / 300`
    fi
    #记录出流量
    transmit_15min_ago=$transmit_10min_ago
    transmit_10min_ago=$transmit_5min_ago
    transmit_5min_ago=$transmit_now
    #输出到屏幕
    printf "Net load average(5min,10min,15min): %d %d %d\n" $net_load_5min $net_load_10min $net_load_15min
    #回馈到数据库,回馈值单位为byte
    command="UPDATE \`${mysql_table}\` SET \`${column_net_load_5min}\` ='${net_load_5min}',\`${column_net_load_10min}\` ='${net_load_10min}',\`${column_net_load_15min}\` ='${net_load_15min}' WHERE \`${column_ip}\` ='${server_ip}'"
    $mysql_mysqlexe -h $mysql_host -P $mysql_port -u $mysql_user -p$mysql_password -D $mysql_database -e "$command"
    #每五分钟执行一次
    sleep 5m
done
