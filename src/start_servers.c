#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <string.h>

struct Mysql_Config
{
    char mysql_host[256];
    char mysql_user[64];
    char mysql_password[64];
    char mysql_database[64];
    char mysql_table[64];
    int refresh_time;
};

struct Server_Config
{
    int port;
    char password[16];
    char encrypt_method[32];
    bool udp_relay;
    bool fast_open;
};

bool read_config(struct Mysql_Config *config)
{
    FILE *fp;
    fp=fopen("config","r");
    if(fp==NULL) return false;
    fscanf(fp,"%s",config->mysql_host);
    fscanf(fp,"%s",config->mysql_user);
    fscanf(fp,"%s",config->mysql_password);
    fscanf(fp,"%s",config->mysql_database);
    fscanf(fp,"%s",config->mysql_table);
    fscanf(fp,"%d",&config->refresh_time);
    return true;
}

MYSQL_RES *read_mysql(struct Mysql_Config *mysql_config)
{
    MYSQL mysql_connection;
    MYSQL_RES *mysql_result;
    char query[256]="select port,password,encrypt_method,udp_relay,fast_open from ";
    strcat(query,mysql_config->mysql_table);
    mysql_init(&mysql_connection);
    mysql_real_connect(&mysql_connection, mysql_config->mysql_host, mysql_config->mysql_user, mysql_config->mysql_password,
                       mysql_config->mysql_database, 0, NULL, 0);
    if(&mysql_connection==NULL){
        return NULL;
    }
    mysql_query(&mysql_connection,query);
    mysql_result=mysql_store_result(&mysql_connection);
    mysql_close(&mysql_connection);
    return mysql_result;
}

void create_server(struct Server_Config *server_config)
{
    int pid;
    char port[5];
    sprintf(port, "%d", server_config->port);
    char *exec_path = "/usr/local/bin/ss-server";
    char *exec_arg[] = {"ss-server", "-s", "0.0.0.0", "-p", port, "-k", server_config->password, "-m", server_config->encrypt_method, NULL};
    char *exec_envp[] = {NULL};
    pid = fork();
    if (pid == 0) {
        close(0);   //关闭输入输出
        close(1);
        close(2);
        execve(exec_path, exec_arg, exec_envp);
    }
    else if (pid > 0)
        printf("创建进程 pid=%d port=%d password=%s encrypt_method=%s\n",pid,server_config->port,server_config->password,server_config->encrypt_method);
    else
        printf("fork失败\n");
}

int main() {
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;
    struct Mysql_Config mysql_config;
    struct Server_Config server_config;
    printf("载入配置文件中...\n");
    if (read_config(&mysql_config))
        printf("载入配置文件成功\n");
    else {
        printf("载入配置文件失败\n");
        exit(-1);
    }
    while (true)
    {
        mysql_result=read_mysql(&mysql_config);
        if(mysql_result)
        {
            while(mysql_row=mysql_fetch_row(mysql_result))
            {
                server_config.port=atoi(mysql_row[0]);
                strcpy(server_config.password,mysql_row[1]);
                stpcpy(server_config.encrypt_method,mysql_row[2]);
                server_config.udp_relay=atoi(mysql_row[3]);
                server_config.fast_open=atoi(mysql_row[4]);
                create_server(&server_config);
            }
        }
        else
            printf("数据库连接失败,重新连接在 %d 秒后\n",mysql_config.refresh_time);
        free(mysql_result);
        sleep(mysql_config.refresh_time);
    }

    return 0;
}
