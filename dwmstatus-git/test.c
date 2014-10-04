#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void error(const char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, const char *argv[])
{
	int socketfd, t, flags;
	struct sockaddr_un local;
	socklen_t len;

	char *command = "get_dropbox_status\ndone\n";
	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socketfd <0)
		error("ERROR opening socket");
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, "/home/jan/.dropbox/command_socket");
	len = strlen(local.sun_path)+sizeof(local.sun_family);
	struct timeval tv = {3, 0};
	if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)) < 0 ||
			setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval)) < 0)
		error("ERROR setting timeout");
	if (connect(socketfd, (struct sockaddr *)&local, len) == -1)
		error("ERROR connecting");
	char str[100] = "get_dropbox_status\ndone\n";
	if (send(socketfd, str, strlen(str), 0)==-1)
		error("ERROR sending");
	if((t=recv(socketfd, str, 100, 0))>0){
		str[t] = '\0';
		printf("%s", str);
	} else {
		if (t<0) error("ERROR receiving");
		else printf("Server closed connection");
		exit(1);
	}
	return 0;
}
