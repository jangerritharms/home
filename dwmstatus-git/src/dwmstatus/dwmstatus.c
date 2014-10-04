#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/time.h>

#include <X11/Xlib.h>

char *tzamsterdam = "Europe/Amsterdam";
static Display *dpy;
static int sockfd;
struct passwd *p;


void error(const char *msg){
	perror(strcat("ERROR: ",msg));
	exit(1);
}

void
mktimes(char *buf, char *fmt, char *tzname)
{
	time_t tim;
	struct tm *timtm;

	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL)
		error("localtime");

	if (!strftime(buf, 19, fmt, timtm))
		error("strftime == 0\n");
}

void 
connectdropbox(){
	struct sockaddr_un local;
	socklen_t len;

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, "/home/jan/.dropbox/command_socket");
	printf("%s\n", local.sun_path);
	len = strlen(local.sun_path)+sizeof(local.sun_family);

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Opening socket");

	struct timeval tv = {3, 0};
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)) < 0 ||
		setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval)) < 0)
		error("Setting timeout for Dropbox socket connection");
	if (connect(sockfd, (struct sockaddr *)&local, len) == -1)
		error("Connecting Dropbox socket");
}

void 
getdropbox(){
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
	}}

void 
getdropboxstatus(char *status){
	int t;
	char command[100] = "get_dropbox_status\ndone\n";

	if (send(sockfd, command, strlen(command), 0) == -1)
		error("Sending Dropbox status request");
	if ((t = recv(sockfd, status, 40, 0))>0){
		status[t] = '\0';
	} else {
		if (t<0) error("Receiving message from Dropbox");
		else error("Dropbox server closed connection");
	}	
}

void
setstatus(char *str)
{
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

int
main(void)
{
	char status[100];
	char tmams[20];
	char dropbox[40];

	if ((p = getpwuid(1)) == NULL)
		error("Getting pwuid");

	/* getdropbox(); */
	connectdropbox();

	if (!(dpy = XOpenDisplay(NULL)))
		error("dwmstatus: cannot open display.\n");

	for (;;sleep(1)) {
		mktimes(tmams, "%H:%M:%S", tzamsterdam);
		getdropboxstatus(dropbox);

		sprintf(status, "Drpobox: %s | Time: %s", dropbox, tmams);
		setstatus(status);
	}

	XCloseDisplay(dpy);

	return 0;
}

