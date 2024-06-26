#ifndef SERVER_HPP
# define SERVER_HPP

#define BODYBUFSIZE 1000000	// defautl

#define	METHOD_GET	0x1
#define	METHOD_POST 0x4
#define	METHOD_DEL	0x8

#define INITALL_METHOD(BF) BF |= -1
#define SET_METHOD(BF, N) BF |= N // 1.method from config	2.line 7/8/9/10 as a second parameter
#define CLR_METHOD(BF, N) BF &= ~N
#define IS_METHOD_SET(BF, N) BF & N

#include <fstream>		// open and read file
#include <netdb.h>		// for getaddrinfo
#include <cstring>		// for memset
#include "Utils.hpp"

struct return_t {
	bool		have;	// if there is a return 1 (true)
	short int	code;	// Status Code
	std::string	text;	// Option
};

struct Location
{
	bool						cgiPass;
	bool						autoIndex;
	uint16_t					allowMethod;
	uint64_t					cliBodySize;
	std::string					path;	// path
	std::string					root;
	std::vector<std::string>	index;
	return_t					retur;
};

class Server // don't forget to create vector to store this class as it may have more than one server in the file.
{
	private:
		std::map<std::string, std::string>	_mimeType; //! no need
		std::map<short int, std::string>	_errPageBody;

		void	_initErrPage(void);
		void	_initMineTypeDefault(void);
	public:
		int									sockFd; //! Pmos only
		bool								cgiPass; //? cgi_pass
		bool								autoIndex; //?autoindex
		uint16_t							allowMethod; //? limit_except (use SET_METHOD to store this one)
		uint64_t							cliBodySize; //?client_max_body_size
		std::string							name; //? server_name
		std::string							ipAddr; //? listen could be ip address or port
		std::string							port; //? listen
		std::string							root; //? root
		return_t							retur; //? return
		std::vector<std::string>			index; //? index
		std::vector<Location>				location; //? location
		std::map<short int, std::string>	errPage; //? error_page

		Server(void);
		~Server(void);

		void		clearLocation(void);
		void		prtServer(void);
		void		initErrPageBody(void);
		std::string	getMimeType(const std::string & extension) const;
		std::string	getErrPagePath(short int) const;
		std::string	getErrPageBody(short int) const;

		void	initMineTypeDefault() {
			_initMineTypeDefault();
		}
		void	initErrPage() {
			_initErrPage();
		}
};


void	printConfig(Server obj);
void	clearLocation(Location &locStruct);

#endif