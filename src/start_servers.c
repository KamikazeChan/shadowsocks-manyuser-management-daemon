#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <string.h>
#include <sys/wait.h>
#include "iniparser.h"

struct Config_File
{
    //mysql_config
    char mysql_host[256];
    char mysql_user[64];
    char mysql_password[64];
    char mysql_database[64];
    char mysql_table[64];
    int refresh_time;
    //column_config
    char column_port[64];
    char column_password[64];
    char column_encrypt_method[64];
    char column_udp_relay[64];
    char column_fast_open[64];
    char column_group[64];
    //server_config
    char server_ip[32];
    char server_group[32];
}config_file;   //全局变量

struct Server_Info
{
    int port;
    char password[16];
    char encrypt_method[32];
    bool udp_relay;
    bool fast_open;
};

bool read_config()
{
    dictionary *ini;
    char *ini_name="config.ini";
    ini=iniparser_load(ini_name);
    iniparser_dump(ini, stderr);
    if(ini==NULL) return false;
    //mysql_config
    strcpy(config_file.mysql_host,iniparser_getstring(ini,"mysql_config:host","\0"));
    strcpy(config_file.mysql_user,iniparser_getstring(ini,"mysql_config:username","\0"));
    strcpy(config_file.mysql_password,iniparser_getstring(ini,"mysql_config:password","\0"));
    strcpy(config_file.mysql_database,iniparser_getstring(ini,"mysql_config:database","\0"));
    strcpy(config_file.mysql_table,iniparser_getstring(ini,"mysql_config:table","\0"));
    config_file.refresh_time=iniparser_getint(ini,"mysql_config:refresh_time",0);
    //column_config
    strcpy(config_file.column_port,iniparser_getstring(ini, "column_config:port", "\0"));
    strcpy(config_file.column_password, iniparser_getstring(ini, "column_config:password", "\0"));
    strcpy(config_file.column_encrypt_method, iniparser_getstring(ini, "column_config:encrypt_method", "\0"));
    strcpy(config_file.column_udp_relay,iniparser_getstring(ini,"column_config:udp_relay","\0"));
    strcpy(config_file.column_fast_open,iniparser_getstring(ini,"column_config:fast_open","\0"));
    strcpy(config_file.column_group,iniparser_getstring(ini,"column_config:group","\0"));
    //server_config
    strcpy(config_file.server_ip,iniparser_getstring(ini,"server_config:ip","\0"));
    strcpy(config_file.server_group,iniparser_getstring(ini,"server_config:group","\0"));
    iniparser_freedict(ini);
    return true;
}

void create_server(struct Server_Info *server_info)
{
    int pid;
    char port[5];
    sprintf(port, "%d", server_info->port);
    char *exec_path = "/usr/local/bin/ss-server";
    char *exec_arg[] = {"ss-server", "-s", config_file.server_ip, "-p", port, "-k", server_info->password, "-m", server_info->encrypt_method, NULL};
    char *exec_envp[] = {NULL};
    pid = fork();
    if (pid == 0) {
        close(0);   //关闭输入输出
        close(1);
        close(2);
        execve(exec_path, exec_arg, exec_envp);
    }
    else if (pid > 0)
        printf("创建进程 pid=%d port=%d password=%s encrypt_method=%s\n", pid, server_info->port, server_info->password,server_info->encrypt_method);
    else
        printf("fork失败\n");
}

int main() {
    struct Server_Info server_info;
    printf("载入配置文件中...\n");
    if (read_config())
        printf("配置文件载入成功\n");
    else
    {
        printf("配置文件载入失败\n");
        exit(-1);
    }

    while(true)    //不停访问数据库并创建进程
    {
        MYSQL mysql_connection;
        MYSQL_ROW mysql_row;
        MYSQL_RES *mysql_result;
        char query[256];
        sprintf(query,"select %s,%s,%s,%s,%s from %s where `%s`='%s'",config_file.column_port,config_file.column_password,config_file.column_encrypt_method,config_file.column_udp_relay,config_file.column_fast_open,config_file.mysql_table,config_file.column_group,config_file.server_group);
        mysql_init(&mysql_connection);
        if(!mysql_real_connect(&mysql_connection, config_file.mysql_host, config_file.mysql_user, config_file.mysql_password, config_file.mysql_database, 0, NULL, 0))
        {
            printf("数据库连接失败\n");
            goto error;
        }
        mysql_query(&mysql_connection,query);
        mysql_result=mysql_store_result(&mysql_connection);
        while(mysql_row=mysql_fetch_row(mysql_result))
        {
            server_info.port=atoi(mysql_row[0]);
            strcpy(server_info.password, mysql_row[1]);
            stpcpy(server_info.encrypt_method, mysql_row[2]);
            server_info.udp_relay=atoi(mysql_row[3]);
            server_info.fast_open=atoi(mysql_row[4]);

            int pid,port,control=0;
            char password[16],encrypt_method[32],ps_output[256];
            FILE *pp;
            pp=popen("ps -ef | grep ss-server | grep -v ps | grep -v grep | awk '{print $2,$12,$14,$16}'","r");
            while(fgets(ps_output,256,pp))
            {
                if(sscanf(ps_output,"%d %d %s %s\n",&pid,&port,password,encrypt_method)!=4)
                {
                    waitpid(pid,NULL,0);
                    printf("进程 pid=%d 崩溃,已被销毁\n",pid);
                    continue;
                }
                if(port==server_info.port)
                {
                    if(strcmp(password,server_info.password)==0&&strcmp(encrypt_method,server_info.encrypt_method)==0)
                    {
                        control=1;
                        break;
                    }
                    else
                    {
                        char command[32];
                        sprintf(command,"kill -9 %d",pid);
                        system(command);
                        waitpid(pid,NULL,0);
                        printf("为更新数据而销毁进程 pid=%d port=%d password=%s encrypt_method=%s\n", pid, port,password,encrypt_method);
                        break;
                    }
                }
            }
            if(control==0) create_server(&server_info);
            pclose(pp);
        }
        error:
        mysql_free_result(mysql_result);
        mysql_close(&mysql_connection);
        mysql_library_end();

        sleep(config_file.refresh_time);
    }
    return 0;
}