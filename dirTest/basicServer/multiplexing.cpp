#include "ServerTool.hpp"

extern int fdMax;

int	main (void) {
	char	port[] = "1600";
	int		client_fd;
	int		server_fd;
	fd_set	readFds, writeFds, tmpReadFds, tmpWriteFds;
	std::string	reqMsg;
	struct timeval	timeOut;
	timeOut.tv_sec = 1;
	timeOut.tv_usec = 0;

	server_fd = initServer(port);
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	if (server_fd < 0)
		return 1;
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	fdSet(server_fd, readFds);
	while (true) {
		tmpReadFds = readFds;	// because select will modified fd_set
		tmpWriteFds = writeFds;	// because select will modified fd_set
		std::cout << GRN << "Start select..." << RESET << std::endl;
		// select will make system motoring three set, block until some fd ready
		if (select(fdMax + 1, &tmpReadFds, &tmpWriteFds, NULL, &timeOut) == -1)
			return (perror("select"), 4);
		for (int fd = 0; fd <= fdMax; fd++) {
			// after using select modified fd set, Now can use FD_ISSET to test if fd still present in sets
			if (FD_ISSET(fd, &tmpReadFds)) {
				if (fd == server_fd) { // handle new connection
					client_fd = acceptConnection(server_fd);
					if (client_fd == 0) // can't accept connection
						continue;
					fcntl(client_fd, F_SETFL, O_NONBLOCK); // set non-blocking i/o on fd
					fdSet(client_fd, readFds);
				}
				else { // handle data from client
					receiveRequest(fd, reqMsg);
					std::cout << GRN << "reach EOF" << RESET << std::endl;
					fdClear(fd, readFds);
					fdSet(client_fd, writeFds);
				}
			}
			else if (FD_ISSET(fd, &tmpWriteFds)) { // send data back to client
				std::string	resMsg;
				readFile("text.txt", resMsg);
				sendReponse(fd, resMsg);
				fdClear(fd, writeFds);
				close(fd);
				std::cout << GRN << "Close connection" << RESET << std::endl;
			}
		}
		sleep(2);
	}
	return 0;
}