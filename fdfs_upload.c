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
 * @brief  ����fdfs �ļ��� �ӿ�
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

    //�����ܵ�
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

        //�رն���
        close(pfd[0]);

        //�ض��� stdout --> fd[1]
        dup2(pfd[1], STDOUT_FILENO);

        //exec fdfs_upload_file
        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf", filename, NULL);
        LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "exec fdfs_upload_file error");
        return -1;
    }
    else {
        //parent

        //�ر�д��
        close(pfd[1]);

        //wait
        wait(NULL);

        //�� fd[0] ������ 
        read(pfd[0], fileid, FILE_ID_LEN);

        fileid[strlen(fileid) - 1] = '\0';



        //close(����)
        close(pfd[0]);
    }

    return 0;
}


int main(int argc, char *argv[])
{
    char fileid[FILE_ID_LEN] = {0};

    fdfs_upload_file(argv[1], fileid);

    //��¼log��
    LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "filename = [%s], fileid=[%s]", argv[1], fileid);

	return 0;
}
