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
#include <stdbool.h>

#include <X11/Xlib.h>

char *dropboxstates[4] = {"Downloading", "Uploading", "Syncing", "Indexing"};

char *tzamsterdam = "Europe/Amsterdam";
static Display *dpy;
static int sockfd;
struct passwd *p;


void error(const char *msg){
	perror(msg);
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
		error("ERROR: localtime");

	if (!strftime(buf, 19, fmt, timtm))
		error("ERROR: strftime == 0\n");
}

void 
connectdropbox(){
	struct sockaddr_un local;
	socklen_t len;

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, "/home/jan/.dropbox/command_socket");
	len = strlen(local.sun_path)+sizeof(local.sun_family);

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR: Opening socket");

	struct timeval tv = {3, 0};
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)) < 0 ||
		setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval)) < 0)
		error("ERROR: Setting timeout for Dropbox socket connection");
	if (connect(sockfd, (struct sockaddr *)&local, len) == -1){
		sleep(5);
		if (connect(sockfd, (struct sockaddr *)&local, len) == -1)
			error("ERROR: Connecting Dropbox socket");
	}
}

void 
getdropboxstatus(char *status){
	int i, t;
	char command[100] = "get_dropbox_status\ndone\n";
	bool sync = false;
	char buf[200];

	if (send(sockfd, command, strlen(command), 0) == -1)
		error("ERROR: Sending Dropbox status request");
	if ((t = recv(sockfd, buf, 199, 0))>0){
		buf[t] = '\0';
		for (i = 0; i < 4; i++) {
			if (strstr(buf, dropboxstates[i])) sync=true;	
		}
		if (sync) strcpy(status, "sync");
		else strcpy(status, "idle");
	} else {
		if (t<0) error("ERROR: Receiving message from Dropbox");
		else error("ERROR: Dropbox server closed connection");
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
	char dropbox[5];

	if ((p = getpwuid(1)) == NULL)
		error("ERROR: Getting pwuid");

	connectdropbox();

	if (!(dpy = XOpenDisplay(NULL)))
		error("ERROR: dwmstatus: cannot open display.\n");

	for (;;sleep(5)) {
		mktimes(tmams, "%H:%M", tzamsterdam);
		getdropboxstatus(dropbox);

		sprintf(status, "Drpobox: %s | Time: %s", dropbox, tmams);
		setstatus(status);
	}

	XCloseDisplay(dpy);

	return 0;
}

