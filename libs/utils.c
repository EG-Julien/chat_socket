#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"

void exit_if(int condition, const char *prefix)
{
    if (condition) {
        perror(prefix);
        exit(1);
    }
}

void fill_array(char *str, int size) {
    for (int i = 0; i < size; i++)
    {
        str[i] = 0;
    }
}

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

void escape_string(char *str, int size, char escape, char replace) {
    for (int i = 0; i < size; i++)
    {
        if (str[i] == escape) {
            str[i] = replace;
        }
    }
    
}