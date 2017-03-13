#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include "make_log.h"

#define FDFS_TEST_MODULE "test"
#define FDFS_TEST_PROC  "fdfs"

#define FILE_ID_LEN  (256)


/* -------------------------------------------*/
/**
 * @brief  长传fdfs 文件的 接口
 *
 * @param filename IN
 * @param fileid OUT
 *
 * @returns   
 *    0 succ, - 1 fail
 */
/* -------------------------------------------*/
int fdfs_upload_file(char *filename , char *fileid)
{
    int retn = 0;
    int pfd[2];
    pid_t pid;

    //创建管道
    retn = pipe(pfd);
    if (retn < 0) {
        perror("pipe error");
        return -1;

    }


    pid  = fork();
    if (pid < 0) {
        perror("fork error");
        return -1;
    }
    if (pid == 0) {
        //child

        //关闭读端
        close(pfd[0]);

        //重定向 stdout --> fd[1]
        dup2(pfd[1], STDOUT_FILENO);

        //exec fdfs_upload_file
        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf", filename, NULL);
        LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "exec fdfs_upload_file error");
        return -1;
    }
    else {
        //parent

        //关闭写端
        close(pfd[1]);

        //wait
        wait(NULL);

        //从 fd[0] 度数据 
        read(pfd[0], fileid, FILE_ID_LEN);

        fileid[strlen(fileid) - 1] = '\0';



        //close(读端)
        close(pfd[0]);
    }

    return 0;
}


int main(int argc, char *argv[])
{
    char fileid[FILE_ID_LEN] = {0};

    fdfs_upload_file(argv[1], fileid);

    //记录log中
    LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "filename = [%s], fileid=[%s]", argv[1], fileid);

	return 0;
}
