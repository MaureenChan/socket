#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char*message)
{
	fputs(message, stdout);
	fputc('\n', stdout);
	exit(1);
}
