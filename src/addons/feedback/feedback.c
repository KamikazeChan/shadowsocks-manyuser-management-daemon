#include <stdio.h>
#include <stdbool.h>
#include "iniparser.h"
#include "mysql/mysql.h"

struct Config_File
{
    //mysql_config
    char mysql_host[256];
    char mysql_user[64];
    char mysql_password[64];
    char mysql_database[64];
    char mysql_table[64];
    //column_config
    char column_ip[32];
    char column_net_load_5min[32];
    char column_net_load_10min[32];
    char column_net_load_15min[32];
    //server_config
    char server_ip[32];
    char server_net_dev[32];
}config_file;   //全局变量

struct Net_Load
{
    long long load[4];
    int point;
}net_load;

bool create_config_file()
{
    FILE *fp;
    fp=fopen("config.ini","w");
    if(fp==NULL)
        return false;
    fprintf(fp,"#\n"
            "# 这是feedback的配置文件\n"
            "#\n"
            "\n"
            "[mysql_config]\n"
            "\n"
            "host = rds.hiczp.com ;\n"
            "username = shadowsocks ;\n"
            "password = 123456789 ;\n"
            "database = shadowsocks ;\n"
            "table = node_free ;\n"
            "\n"
            "\n"
            "[column_config]\n"
            "\n"
            "ip = ip ;\n"
            "net_load_5min = net_load_5min ;\n"
            "net_load_10min = net_load_10min ;\n"
            "net_load_15min = net_load_15min ;\n"
            "\n"
            "\n"
            "[server_config]\n"
            "\n"
            "ip = 1.1.1.1 ;\n"
            "net_dev = wlan0 ;");
    fclose(fp);
    return true;
}

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
    //column_config
    strcpy(config_file.column_ip,iniparser_getstring(ini, "column_config:ip", "\0"));
    strcpy(config_file.column_net_load_5min, iniparser_getstring(ini, "column_config:net_load_5min", "\0"));
    strcpy(config_file.column_net_load_10min, iniparser_getstring(ini, "column_config:net_load_10min", "\0"));
    strcpy(config_file.column_net_load_15min,iniparser_getstring(ini,"column_config:net_load_15min","\0"));
    //server_config
    strcpy(config_file.server_ip,iniparser_getstring(ini,"server_config:ip","\0"));
    strcpy(config_file.server_net_dev,iniparser_getstring(ini,"server_config:net_dev","\0"));
    iniparser_freedict(ini);
    return true;
}

int main()
{
    if (read_config())
        printf("Loaded config file\n");
    else
    {
        if(create_config_file())
        {
            printf("Created config file\n");
            read_config();
        }
        else
        {
            printf("Create config file failed\n");
            exit(-1);
        }
    }
    while(true)    //不停访问数据库并创建进程
    {
        FILE *pp;
        char pp_output[256];
        char command[256];
        sprintf(command,"cat /proc/net/dev | grep %s | awk '{print $10}'",config_file.server_net_dev);
        pp=popen(command,"r");
        if(!fgets(pp_output,256,pp))
            goto error;
        sscanf(pp_output,"%lld\n",&net_load.load[net_load.point]);
        pclose(pp);
        //printf("流量记录值 %lld %lld %lld %lld\n",net_load.load[0],net_load.load[1],net_load.load[2],net_load.load[3]);
        long long net_load_5,net_load_10,net_load_15;
        switch (net_load.point)
        {
            case 0:
            {
                net_load_5=net_load.load[0]-net_load.load[3];
                net_load_10=net_load.load[0]-net_load.load[2];
                net_load_15=net_load.load[0]-net_load.load[1];
            }
            break;
            case 1:
            {
                net_load_5=net_load.load[1]-net_load.load[0];
                net_load_10=net_load.load[1]-net_load.load[3];
                net_load_15=net_load.load[1]-net_load.load[2];
            }
            break;
            case 2:
            {
                net_load_5=net_load.load[2]-net_load.load[1];
                net_load_10=net_load.load[2]-net_load.load[0];
                net_load_15=net_load.load[2]-net_load.load[3];
            }
            break;
            case 3:
            {
                net_load_5=net_load.load[3]-net_load.load[2];
                net_load_10=net_load.load[3]-net_load.load[1];
                net_load_15=net_load.load[3]-net_load.load[0];
            }
            break;
        }
        if(net_load.point<3)
            net_load.point++;
        else
            net_load.point=0;
        printf("Net load: %lld KB/S %lld KB/S %lld KB/S\n",net_load_5/1024/300,net_load_10/1024/600,net_load_15/1024/900);
        //回传到数据库
        MYSQL mysql_connection;
        MYSQL_ROW mysql_row;
        MYSQL_RES *mysql_result;
        char query[256];
        mysql_init(&mysql_connection);
        if (!mysql_real_connect(&mysql_connection, config_file.mysql_host, config_file.mysql_user, config_file.mysql_password, config_file.mysql_database, 0, NULL, 0)) {
            printf("Connect to database failed\n");
            goto error;
        }
        sprintf(query,"UPDATE `%s` SET `net_load_5min` ='%lld KB/S',`net_load_10min` ='%lld KB/S',`net_load_15min` ='%lld KB/S' WHERE `ip` ='%s'",config_file.mysql_table,net_load_5/1024/300,net_load_10/1024/600,net_load_15/1024/900,config_file.server_ip);
        mysql_query(&mysql_connection, query);
        error:
        mysql_close(&mysql_connection);
        mysql_library_end();    //这一步绝对不能少,否则产生内存泄露
        sleep(300);
    }
    return 0;
}
