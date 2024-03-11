#ifndef SERVER_HPP
# define SERVER_HPP

#define HEADBUFSIZE 1000
#define BODYBUFSIZE 1000000	// defautl

#define	METHOD_GET	0x1
#define	METHOD_HEAD	0x2
#define	METHOD_POST 0x4
#define	METHOD_DEL	0x8

#define INIT_METHOD(BF) BF |= -1
#define SET_METHOD(BF, N) BF |= N
#define CLR_METHOD(BF, N) BF &= ~N
#define IS_METHOD_SET(BF, N) BF & N

#include <netdb.h>		// for getaddrinfo
#include <cstring>		// for memset
#include "Utils.hpp"

struct return_t {
	bool		have;
	short int	code;	// Status Code
	std::string	text;	// Option
};

struct Location
{
	std::string					path;	// path
	std::string					root;
	std::vector<std::string>	index;
	uint16_t					allowMethod;
	bool						autoIndex;
	size_t						cliBodySize;
	return_t					retur;
	bool						cgiPass;
};

class Server
{
	private:
		std::string							_name;
		std::string							_ipAddr;
		std::string							_port;
		std::string							_root;
		std::vector<std::string>			_index;
		std::vector<Location>				_location;
		std::map<std::string, std::string>	_mimeType;
	public:
		int									sockFd;
		size_t								cliBodySize;
		bool								autoIndex;
		bool								cgiPass;
		uint16_t							allowMethod;
		return_t							retur;
		std::map<short int, std::string>	errPage;

		Server(void);
		~Server(void) {}
		void	_initErrPage(void);
		void	_initMineTypeDefault(void);
		void	setName(std::string name);
		void	setIPaddr(std::string ipAddr);
		void	setPort(std::string port);
		void	setRoot(std::string root);
		void	setIndex(std::vector<std::string> index);
		void	setLocation(Location location);
		void	setMimeType(std::string key, std::string value);
		void	clearLocation(void);
		void	prtServer(void);

		std::string					getName(void) const;
		std::string					getIPaddr(void) const;
		std::string					getPort(void) const;
		std::string					getRoot(void) const;
		std::string					getMimeType(const std::string & extension) const;
		std::string					getErrPage(short int &) const;
		std::vector<std::string>	getIndex(void) const;
		std::vector<Location>		getLocation(void) const;
		struct sockaddr				getSockAddr(void) const;
};

#endif