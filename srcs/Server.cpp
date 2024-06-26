// #include "Server.hpp"
#include "../includes/Server.hpp"

Server::Server(void) {
	_initMineTypeDefault();
	_initErrPage();
	name = "localhost";				// default : null
	ipAddr = "0.0.0.0";				// default : any addr
	port = "80";					// default : 80
	root = "html";					// default : html directory
	index.push_back("index.html");	// default : index.html
	cliBodySize = BODYBUFSIZE;		// dafault : 1m byte
	INITALL_METHOD(allowMethod);	// default : allow all method
	autoIndex = 0;					// defualt : off
	retur.have = 0;					// default : --
	retur.code = 0;
	retur.text = "";
	cgiPass = 0;
}

Server::~Server(void) {}

void	Server::initErrPageBody(void) {
	std::ifstream		inputFile;
	std::stringstream	buffer;

	std::map<short int, std::string>::iterator	it;
	for (it = errPage.begin(); it != errPage.end(); it++) {
		inputFile.open(it->second.c_str(), std::ifstream::in);
		if (inputFile.is_open()) {
			buffer << inputFile.rdbuf();
			inputFile.close();
			_errPageBody[it->first] = buffer.str();
			buffer.str("");
			buffer.clear();
		}
		else
			_errPageBody[it->first] = "Something went wrong";
	}
}

// ************************************************************************** //
// --------------------------------- Getter --------------------------------- //
// ************************************************************************** //

std::string	Server::getMimeType(const std::string & extension) const {
	std::map<std::string, std::string>::const_iterator	it;
	it = _mimeType.find(extension);
	if (it != _mimeType.end())
		return it->second;
	it = _mimeType.find("default");
	return it->second;
}

std::string	Server::getErrPagePath(short int errCode) const {
	std::map<short int, std::string>::const_iterator	it;
	it = errPage.find(errCode);
	if (it != errPage.end())
		return it->second;
	return "";
}

std::string	Server::getErrPageBody(short int errCode) const {
	std::map<short int, std::string>::const_iterator	it;
	it = _errPageBody.find(errCode);
	if (it != _errPageBody.end())
		return it->second;
	return "Something went wrong";
}

void	Server::prtServer(void) {
	std::cout << "server_name : " << MAG << name << "|" << RESET << std::endl;
	std::cout << "IP address  : " << MAG << ipAddr << "|" << RESET << std::endl;
	std::cout << "Port        : " << MAG << port << "|" << RESET << std::endl;
	std::cout << "Root        : " << MAG << root << "|" << RESET << std::endl;
	std::cout << "index       : ";
	for (size_t i = 0; i < index.size(); i++)
		std::cout << MAG << index[i] << ", ";
	std::cout << RESET << std::endl;
	std::cout << "CliBodySize : " << MAG << cliBodySize << RESET << std::endl;
	for (size_t i = 0; i < location.size(); i++) {
		std::cout << "*** Location[" << i << "] ***" << std::endl;
		std::cout << "path        : " << MAG << location[i].path << RESET << std::endl;
		std::cout << "Root        : " << MAG << location[i].root << RESET << std::endl;
		std::cout << "index       : ";
		for (size_t j = 0; j < location[i].index.size(); j++)
			std::cout << MAG << location[i].index[j] << ", ";
		std::cout << RESET << std::endl;
		std::cout << "allowMethod : " << MAG << location[i].allowMethod << RESET << std::endl;
		std::cout << "autoIndex   : " << MAG << location[i].autoIndex << RESET << std::endl;
		std::cout << "cliBodySize : " << MAG << location[i].cliBodySize << RESET << std::endl;
		std::cout << "return      : " << MAG << location[i].retur.have << ", " << location[i].retur.code << ", " << location[i].retur.text << RESET << std::endl;
		std::cout << "cgiPass     : " << MAG << location[i].cgiPass << RESET << std::endl;
	}
}

void	Server::clearLocation(void) {
	location.clear();
}

// ************************************************************************** //
// ------------------------------- Initialize ------------------------------- //
// ************************************************************************** //

void	Server::_initErrPage(void) {
	errPage[400] = "docs/errorPage/400.html";
	errPage[403] = "docs/errorPage/403.html";
	errPage[404] = "docs/errorPage/404.html";
	errPage[405] = "docs/errorPage/405.html";
	errPage[411] = "docs/errorPage/411.html";
	errPage[413] = "docs/errorPage/413.html";
	errPage[500] = "docs/errorPage/500.html";
	errPage[501] = "docs/errorPage/501.html";
	errPage[502] = "docs/errorPage/502.html";
	errPage[503] = "docs/errorPage/503.html";
	errPage[504] = "docs/errorPage/504.html";
	errPage[505] = "docs/errorPage/505.html";
}

void	Server::_initMineTypeDefault(void) {
	// Text Type
	_mimeType["default"] = "text/plain";
	_mimeType["html"] = "text/html";
	_mimeType["htm"] = "text/html";
	_mimeType["shtml"] = "text/html";
	_mimeType["css"] = "text/css";
	_mimeType["xml"] = "text/xml";
	_mimeType["mml"] = "text/mathml";
	_mimeType["txt"] = "text/plain";
	_mimeType["jad"] = "text/vnd.sun.j2me.app-descriptor";
	_mimeType["wml"] = "text/vnd.wap.wml";
	_mimeType["htc"] = "text/x-component";
	// Images Type
	_mimeType["gif"] = "image/gif";
	_mimeType["jpeg"] = "image/jpeg";
	_mimeType["jpg"] = "image/jpeg";
	_mimeType["png"] = "image/png";
	_mimeType["tif"] = "image/tiff";
	_mimeType["tiff"] = "image/tiff";
	_mimeType["wbmp"] = "image/vnd.wap.wbmp";
	_mimeType["ico"] = "image/x-icon";
	_mimeType["jng"] = "image/x-jng";
	_mimeType["bmp"] = "image/x-ms-bmp";
	_mimeType["svg"] = "image/svg+xml";
	_mimeType["svgz"] = "image/svg+xml";
	_mimeType["webp"] = "image/webp";
	// application it is a top-level media type (not Text or Multimedia)
	_mimeType["js"] = "application/javascript";
	_mimeType["atom"] = "application/atom+xml";
	_mimeType["rss"] = "application/rss+xml";
	_mimeType["woff"] = "application/font-woff";
	_mimeType["jar"] = "application/java-archive";
	_mimeType["war"] = "application/java-archive";
	_mimeType["ear"] = "application/java-archive";
	_mimeType["json"] = "application/json";
	_mimeType["hqx"] = "application/mac-binhex40";
	_mimeType["doc"] = "application/msword";
	_mimeType["pdf"] = "application/pdf";
	_mimeType["ps"] = "application/postscript";
	_mimeType["eps"] = "application/postscript";
	_mimeType["ai"] = "application/postscript";
	_mimeType["rtf"] = "application/rtf";
	_mimeType["m3u8"] = "application/vnd.apple.mpegurl";
	_mimeType["xls"] = "application/vnd.ms-excel";
	_mimeType["eot"] = "application/vnd.ms-fontobject";
	_mimeType["ppt"] = "application/vnd.ms-powerpoint";
	_mimeType["wmlc"] = "application/vnd.wap.wmlc";
	_mimeType["kml"] = "application/vnd.google-earth.kml+xml";
	_mimeType["kmz"] = "application/vnd.google-earth.kmz";
	_mimeType["7z"] = "application/x-7z-compressed";
	_mimeType["cco"] = "application/x-cocoa";
	_mimeType["jardiff"] = "application/x-java-archive-diff";
	_mimeType["jnlp"] = "application/x-java-jnlp-file";
	_mimeType["run"] = "application/x-makeself";
	_mimeType["pl"] = "application/x-perl";
	_mimeType["pm"] = "application/x-perl";
	_mimeType["prc"] = "application/x-pilot";
	_mimeType["pdb"] = "application/x-pilot";
	_mimeType["rar"] = "application/x-rar-compressed";
	_mimeType["rpm"] = "application/x-redhat-package-manager";
	_mimeType["sea"] = "application/x-sea";
	_mimeType["swf"] = "application/x-shockwave-flash";
	_mimeType["sit"] = "application/x-stuffit";
	_mimeType["tcl"] = "application/x-tcl";
	_mimeType["tk"] = "application/x-tcl";
	_mimeType["der"] = "application/x-x509-ca-cert";
	_mimeType["pem"] = "application/x-x509-ca-cert";
	_mimeType["crt"] = "application/x-x509-ca-cert";
	_mimeType["xpi"] = "application/x-xpinstall";
	_mimeType["xhtml"] = "application/xhtml+xml";
	_mimeType["xspf"] = "application/xspf+xml";
	_mimeType["zip"] = "application/zip";
	_mimeType["bin"] = "application/octet-stream";
	_mimeType["exe"] = "application/octet-stream";
	_mimeType["dll"] = "application/octet-stream";
	_mimeType["deb"] = "application/octet-stream";
	_mimeType["dmg"] = "application/octet-stream";
	_mimeType["iso"] = "application/octet-stream";
	_mimeType["img"] = "application/octet-stream";
	_mimeType["msi"] = "application/octet-stream";
	_mimeType["msp"] = "application/octet-stream";
	_mimeType["msm"] = "application/octet-stream";
	_mimeType["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	_mimeType["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	_mimeType["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	// Audio Type
	_mimeType["mid"] = "audio/midi";
	_mimeType["midi"] = "audio/midi";
	_mimeType["kar"] = "audio/midi";
	_mimeType["mp3"] = "audio/mpeg";
	_mimeType["ogg"] = "audio/ogg";
	_mimeType["m4a"] = "audio/x-m4a";
	_mimeType["ra"] = "audio/x-realaudio";
	// Video Type
	_mimeType["3gpp"] = "video/3gpp";
	_mimeType["3gp"] = "video/3gpp";
	_mimeType["ts"] = "video/mp2t";
	_mimeType["mp4"] = "video/mp4";
	_mimeType["mpeg"] = "video/mpeg";
	_mimeType["mpg"] = "video/mpeg";
	_mimeType["mov"] = "video/quicktime";
	_mimeType["webm"] = "video/webm";
	_mimeType["flv"] = "video/x-flv";
	_mimeType["m4v"] = "video/x-m4v";
	_mimeType["mng"] = "video/x-mng";
	_mimeType["asx"] = "video/x-ms-asf";
	_mimeType["asf"] = "video/x-ms-asf";
	_mimeType["wmv"] = "video/x-ms-wmv";
	_mimeType["avi"] = "video/x-msvideo";
}
