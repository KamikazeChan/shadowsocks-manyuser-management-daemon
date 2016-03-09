#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid;
    int i;
    char *exec_path = "/usr/local/bin/ss-server";
    char *exec_arg[] = {"ss-server", "-s", "0.0.0.0", "-p", "10000", "-k", "1234", "-m", "aes-256-cfb", NULL};
    char *exec_envp[] = {NULL};
    for (i = 1; i <= 5; i++) {
        pid = fork();
        if (pid == 0) {
            close(0);   //关闭输入输出
            close(1);
            close(2);
            execve(exec_path, exec_arg, exec_envp);
        }
        else if (pid > 0) printf("已生成子进程 pid = %d\n", pid);
        else printf("fork失败\n");
    }

    printf("sleeping....\n");
    sleep(1000);
    return 0;
}
