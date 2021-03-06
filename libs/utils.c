#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>

#include "utils.h"

int read_stdin(char *buffer){

    static int head=0;
    int header_readed=0;
    int answer_read = read(STDIN_FILENO,&buffer[head],1);
    if (answer_read == -1)
    {
        if (errno == EAGAIN)
        {
            usleep(500);
            return header_readed;
        }else
        {
            perror("read read_header");
            exit(1);
        }   
    }else
    {
        if (head >= MAX_MESSAGE_LEN || buffer[head] == 10)
        {
            buffer[head+1] = '\0';
            head = 0;
            answer_read = 0;
            header_readed = 1;
        }else
        {
            header_readed = 0;
        }
        head += answer_read;
    }
    
    
    return header_readed;
}

void stdout_black() {
    printf("\033[0;30m");
}

void stdout_red() {
    printf("\033[0;31m");
}

void stdout_green() {
    printf("\033[0;32m");
}

void stdout_yellow() {
    printf("\033[0;33m");
}

void stdout_blue() {
    printf("\033[0;34m");
}

void stdout_purple() {
    printf("\033[0;35m");
}

void stdout_cyan() {
    printf("\033[0;36m");
}

void stdout_white() {
    printf("\033[0;37m");
}


void str_overwrite_stdout() {
    stdout_cyan();
    printf("%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

pid_t skeleton_daemon(char* port) {
    pid_t pid;

    pid = fork();

    if (pid == 0) {

        int fd_null = open("/dev/null", O_RDONLY);
        if (fd_null == -1)
            return -1;
        
        int fd_log = open("./log.txt", O_CREAT | O_APPEND | O_WRONLY, 0777);
        if (fd_log == -1)
            return -1;
        
        int fd_err = open("./log_errors.txt", O_CREAT | O_APPEND | O_WRONLY, 0777);
        if (fd_err == -1)
            return -1;

        dup2(fd_null, STDIN_FILENO);
        dup2(fd_log,  STDOUT_FILENO);
        dup2(fd_err,  STDERR_FILENO);

        int err = execl("../server/bin/server_app", "server_app", port, (char*)NULL);

        return getpid();

        if (err != 0)
            return -1;
    } else {
        return pid;
    }

    return -1;
}