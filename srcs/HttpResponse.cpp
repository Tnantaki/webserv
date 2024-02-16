#include "HttpResponse.hpp"

// std::string	HttpResponse::_encodeURL(std::string uri) {
// 	for (int i = 0; i < uri.length(); i++) {
// 		if (uri[i] == '%') {
// 			if (!std::isdigit(uri[i]))
// 				throw HttpResponse::ReqException();
// 			else
// 		}
// 	}
// }

HttpResponse::HttpResponse(Server & serv, httpReq & req) {
	// _allowMethod.push_back("GET");
	// _allowMethod.push_back("POST");
	// _allowMethod.push_back("HEAD");
	_req.serv = serv;
	_req.cliIPaddr = "";		// Can't find way
	_req.method = req.method;
	_req.uri = req.srcPath;
	_req.version = req.version;
	_req.headers = req.headers;
	_req.contentLength = _findContent(_req.headers, "Content-Length");
	_req.contentType = _findContent(_req.headers, "Content-Type");
	std::string	uriEncode = _req.uri; // TODO : have to encode uri first
	_req.path = "";
	_req.pathInfo = "";
	_req.queryStr = "";
	_req.body = req.body;
}

std::string	HttpResponse::createResponse(void) {
	// _checkRequest();
	// TODO : match location in configfile
	if (_isCgi(_req.path)) {
		CgiHandler	cgi;

		_body = cgi.execCgiScript(_req, _status);
	}
	else {
		// _request.srcPath += "/index.html";
		_readFile(_req.path, _body);
	}
	_createHeader();
	return _header + CRLF + _body;
}

bool	HttpResponse::_createHeader(void) {
	_contentType = _getContentType(_req.path);
	_contentLength = _getContentLength();
	_date = _getDate();
	_header = _getStatusLine(_status);
	_header += _contentLength;
	_header += _contentType;
	_header += _date;
	return true;
}

bool	HttpResponse::_checkRequest(void) {
	if (!_checkMethod("GET")) { // TODO : get from request
		_status = 405;
		return false;
	}
	if (!_checkVersion("HTTP/1.1")) { // TODO get from request
		_status = 505;
		return false;
	}
	return true;
}

void	HttpResponse::prtResponse(void) {
	std::cout << _contentType;
	std::cout << _contentLength;
	std::cout << _date;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << _body;
}

// ************************************************************************** //
// -------------------------- Check Header Fields --------------------------- //
// ************************************************************************** //

bool	HttpResponse::_checkMethod(std::string method) {
	for (size_t i = 0; i < _allowMethod.size(); i++) {
		if (method.compare(_allowMethod[i]) == 0)
			return true;
	}
	return false;
}

bool	HttpResponse::_checkVersion(std::string version) {
	if (version.compare(HTTP_VERS) == 0)
		return true;
	return false;
}

// ************************************************************************** //
// ------------------------- Response Header Fields ------------------------- //
// ************************************************************************** //

std::string	HttpResponse::_getStatusLine(short int & statusCode) {
	std::string	httpVer = HTTP_VERS;
	std::string	httpStatCode = numToStr(statusCode);
	std::string httpStatText = _getStatusText(statusCode);
	return httpVer + " " + httpStatCode + " " + httpStatText + CRLF;
}

std::string	HttpResponse::_getContentLength(void) {
	std::string	contentLength = "Content-Length: ";
	size_t	length = _body.length();
	return contentLength + numToStr(length) + CRLF;
}

std::string	HttpResponse::_getContentType(std::string & path) {
	std::string	contentType = "Content-Type: ";
	size_t	index = path.find_last_of(".");
	if (index != std::string::npos) {
		std::string	ext = path.substr(index + 1);
		return contentType + _req.serv.getMimeType(ext) + CRLF;
	}
	return contentType + _req.serv.getMimeType("default") + CRLF;
}

std::string	HttpResponse::_getDate(void) {
	std::string	date = "Date: ";
	std::time_t	currentTime;
	struct tm	*gmTime;
	char		buffer[30];

	std::time(&currentTime);		// get current time.
	gmTime = gmtime(&currentTime);	// convert to tm struct GMT

	// Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", gmTime);
	return date + buffer + CRLF;
}

// ************************************************************************** //
// ----------------------------- Body Messages ------------------------------ //
// ************************************************************************** //

bool	HttpResponse::_readFile(std::string & fileName, std::string & buffer) {
	std::ifstream	inFile;
	int				length;

	inFile.open(fileName.c_str());		// Convert string to char* by c_str() function
	if (!inFile.is_open()) {
		if (errno == ENOENT) {	// 2 No such file or directory : 404
			_status = 404;
			return false;
		}
		if (errno == EACCES) { // 13 Permission denied : 403
			_status = 403;
			return false;
		}
		// EMFILE, ENFILE : Too many open file, File table overflow
		// Server Error, May reach the limit of file descriptors : 500
		// std::cerr << "Internal Server Error" << std::endl;
		_status = 500;
		return (false);
	} // : TODO Check it is a file not a dir if dir are there autoindex
	inFile.seekg(0, inFile.end);		// Set position to end of the stream
	length = inFile.tellg();			// Get current position
	inFile.seekg(0, inFile.beg);		// Set position back to begining of the stream
	buffer.resize(length);
	inFile.read(&buffer[0], length);	// Read all data in inFile to Buffer
	inFile.close();						// Close inFile
	_status = 200;
	return (true);
}

bool	HttpResponse::_isCgi(std::string & path) {
	size_t	index = path.find_last_of(".");
	if (index != std::string::npos) {
		std::string	ext = path.substr(index + 1);
		if (ext == "sh")
			return true;
	}
	return false;
}

std::string	HttpResponse::_getStatusText(short int & statusCode) {
	switch (statusCode)
	{
	case 200:
		return "OK";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 505:
		return "HTTP Version Not Supported";
	default:
		return "Undefined";
	}
}

// ************************************************************************** //
// ---------------------------- Parsing Request ----------------------------- //
// ************************************************************************** //

std::string	HttpResponse::_findContent(std::map<std::string, std::string> & map, std::string const & content) {
	std::map<std::string, std::string>::const_iterator	it;
	std::string	value = "";

	it = map.find(content);
	if (it != map.end()) {
		value = it->second;
		map.erase(it);
	}
	return value;
}
