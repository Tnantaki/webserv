#include <fcntl.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	// use ls command to see socket file.
	char * mysocketpath = "mysock.sock";
	struct sockaddr_un namesock;
	int fd;
	namesock.sun_family = AF_UNIX;
	strncpy(namesock.sun_path, (char *)mysocketpath, sizeof(namesock.sun_path));
	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	bind(fd, (struct sockaddr *) &namesock, sizeof(struct sockaddr_un));
	close(fd);
	return 0;
}