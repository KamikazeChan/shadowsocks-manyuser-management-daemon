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
mysql_temp_file="/tmp/mysqltemp"    #mysql查询结果暂存文件
#column设置
column_port="port"
column_password="passwd"
column_encrypt_method="encrypt_method"
column_udp_relay="udp_relay"
column_fast_open="fast_open"
column_group="group"
column_enable="enable"
#server设置
server_bind_ip="0.0.0.0"        #监听地址
server_group="free_user"        #服务器用户组
#ss-server设置
ss_ssexe="ss-server"            #ss-server命令

#工作循环
while [[ true ]]; do
    #ss-server -s 0.0.0.0 -p 8989 -k 1234 -m aes-256-cfb &
    #访问mysql
    #SELECT `ip`,`passwd`,`encrypt_method`,`udp_relay`,`fast_open` FROM `free_user` WHERE `group`='free_user' ORDER BY `port`;
    printf "[`date`]\n"
    mysql_command="SELECT \`${column_port}\` ,\`${column_password}\` ,\`${column_encrypt_method}\` ,\`${column_udp_relay}\` ,\`${column_fast_open}\`  FROM \`${mysql_table}\` WHERE \`${column_group}\` = '${server_group}' ORDER BY \`${column_port}\`"
    $mysql_mysqlexe -h $mysql_host -P $mysql_port -u $mysql_user -p$mysql_password -D $mysql_database -e "$mysql_command" -N > $mysql_temp_file

    while read line
    do
        ss_command="`echo $line |
        awk -v ss_ssexe=${ss_ssexe} -v server_bind_ip=${server_bind_ip} '{
        printf("%s -s %s -p %s -k %s -m %s",ss_ssexe,server_bind_ip,$1,$2,$3)
        if($4==1) printf(" -u")
        if($5==1) printf(" --fast-open")
        }'`"
        ss_command_result=`ps -ef | grep "${ss_command}" | grep -v ps | grep -v grep | sort`
        ss_port=`echo $line | awk '{print $1}'`
        ss_port_result=`ps -ef | grep $ss_ssexe | grep "$ss_port -k" | grep -v ps | grep -v grep | sort`
        if [[ ! -n $ss_port_result ]]; then
            $ss_command &
        else
            if [[ "$ss_command_result" != "$ss_port_result" ]]; then
                ss_pid=`echo $ss_port_result | awk '{print $2}'`
                kill $ss_pid
                printf "killed process pid=%s port=%s\n" $ss_pid $ss_port
                $ss_command &
            fi
        fi
    done < $mysql_temp_file

    sleep $mysql_refresh_time
    #sleep 10
done
