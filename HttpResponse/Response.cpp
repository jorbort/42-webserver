#include "Response.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "../Includes/Logger.hpp"

Response::Response(HttpRequest &request, ServerConfigs *server) {
	this->method = getMethod(request._method);
	this->uri = strdup(request._URI.c_str());
	this->extension = getExtension(this->uri);
	this->_statusCode = 200;
	this->_isCGI = false;
	this->_CGIhandler = NULL;
	this->server = server;
	this->_headers = request.getHeaders();
	parseRequestBody(request._body);
	setDefaultErrorBody();
	initStatusPageMap();
}

Response::~Response() {
	free(this->uri);
	free(this->extension);
	free(this->_requestContent);
	delete(this->_CGIhandler);
}

std::string	Response::createResponse() {
	if(this->server->getClientMaxBodySize() < this->_contentLength){
		this->_statusCode = 413;
		this->_body = readBody(this->getStatusPage().c_str());
		std::string	response;
		this->_contentLength = this->_body.size();
		response = addStatusLine(this->_statusCode);
		response += addDateHeader();
		response += addContentTypeHeader(HTML);
		response += addContentLengthHeader(this->_contentLength);
		if (_headers.find("Connection") != _headers.end() && _headers["Connection"] == "keep-alive")
			response += connectionKeepAlive();
		response += "\r\n";
		response += this->_body;
		return response;
	}
    if (!strcmp(this->uri, "/")){
        free(this->uri);
        this->uri = strdup(this->server->getRoot().c_str());
    }
	else if(this->uri[0] == '/'){
		std::string tmp = this->uri;
		tmp = tmp.substr(1);
		free(this->uri);
		this->uri = strdup(tmp.c_str());
	}
	if (!isURIAcceptable(this->uri))
		return errorResponse();
	if (isCGI(this->extension)) {
		if (!isProcessableCGI(this->extension, server->getLocation("./cgi-bin")->cgiExtensions)){
			return errorResponse();
		}
		if (method != GET && method != POST) {
			this->_statusCode = 405;
			return errorResponse();
		}
		this->_isCGI = true;
		this->_CGIhandler = new CGIHandler(*this);
		std::string body(_requestContent);
		if ((this->_statusCode = this->_CGIhandler->handleCGI(body)) != 200){
			return errorResponse();
		}
	}
	if (method == GET) {
		std::string	response;

		if (this->_isCGI){
			this->_body = readBody(this->_CGIhandler->fd);
		}
		else{
			this->_body = readBody(this->uri);
		}
		this->_contentLength = this->_body.size();
		response = addStatusLine(this->_statusCode);
		response += addDateHeader();
		response += addContentTypeHeader(HTML);
		response += addContentLengthHeader(this->_contentLength);
		if(_headers.find("Connection") != _headers.end() && _headers["Connection"] == "keep-alive")
			response += connectionKeepAlive();
		if(_headers.find("Cookie") != _headers.end()){
			response += addCookieHeader(_headers["Cookie"]);
		}
		response += "\r\n";
		response += this->_body;
		return response;
	}
	else if (method == POST) {
		std::string	response;
		if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos){
			std::string filename = "";
			_requestContent = getContentDisposition(_requestContent, filename);
			getFormUri(filename);
		}
		if (this->_isCGI)
			this->_body = readBody(this->_CGIhandler->fd);
		else if (this->_statusCode == 500){
				this->_body = readBody(this->getStatusPage().c_str());
			}
		else
			this->_body = writeContent(this->uri);
		this->_contentLength = this->_body.size();
		response = addStatusLine(this->_statusCode);
		response += addDateHeader();
		response += addContentTypeHeader(HTML);
		response += addContentLengthHeader(this->_contentLength);
		if (_headers.find("Connection") != _headers.end() && _headers["Connection"] == "keep-alive")
			response += connectionKeepAlive();
		if(_headers.find("Cookie") != _headers.end()){
			response += addCookieHeader(_headers["Cookie"]);
		}
		response += "\r\n";
		response += this->_body;
		return response;
	}
	else if (method == DELETE) {
		std::string response;
		if (!isUriInServer(this->location.c_str()) || !isMethodAllowed("DELETE",this->location.c_str())){
			this->_statusCode = 403;
			this->_body = readBody(this->getStatusPage().c_str());
		}
		else
			this->_body = deleteContent(this->uri);
		this->_contentLength = this->_body.size();
		response = addStatusLine(this->_statusCode);
		response += addDateHeader();
		response += addContentTypeHeader(HTML);
		response += addContentLengthHeader(this->_contentLength);
		if (_headers.find("Connection") != _headers.end() && _headers["Connection"] == "keep-alive"){
			response += connectionKeepAlive();
		}
		if(_headers.find("Cookie") != _headers.end()){
			response += addCookieHeader(_headers["Cookie"]);
		}
		response += "\r\n";
		response += this->_body;
		return response;

	}
	this->_statusCode = 405;
	return errorResponse();
}

Response::Method	Response::getMethod(const std::string &method) {
	if (method.compare("GET") == 0)
		return GET;
	else if (method.compare("POST") == 0)
		return POST;
	else if (method.compare("DELETE") == 0)
		return DELETE;
	else
		return UNKNOWN;
}

char *	Response::getExtension(const char *uri) {
	const char *tmp;

	if ((tmp = strrchr(uri, '.')) == NULL)
		return NULL;
	return (strdup(tmp + 1));
}

char * Response::getContentDisposition(char *reqBody, std::string &fileName){
	std::string bodyStr = reqBody;
	size_t pos = 0;
	size_t endPos = bodyStr.find("\r\n\r\n");
	std::string headerPart = bodyStr.substr(pos, endPos);
	std::string tempBody = bodyStr.substr(endPos + 4);
	fileName = headerPart.substr(headerPart.find("Content-Disposition") + 19, headerPart.find("\r\n") - headerPart.find("Content-Disposition"));
	free(reqBody);
	this->_requestContentLength = tempBody.size();
	return strdup(tempBody.c_str());
	
}

void Response::getFormUri(std::string str){
	if(str.empty()){
		this->_statusCode = 500;
		return;
	}
	std::string url = this->uri;
	std::string fileName;
	std::size_t pos = str.find("filename=");
	if (pos != std::string::npos){
		pos += 9;
		if (str[pos] == '\"'){
			pos++;
		}
		std::size_t endPos = str.find("\"", pos);
		if (endPos != std::string::npos){
			fileName = str.substr(pos, endPos - pos);
		}
	}else{
		std::size_t endPos = str.find(";", pos);
		if (endPos == std::string::npos){
			endPos = str.size();
		}
		fileName = str.substr(pos, endPos - pos);
	}
	std::string fullPath = url + "/" + fileName;
	free(this->uri);
	this->uri = strdup(fullPath.c_str());
}

void	Response::parseRequestBody(const std::vector<char> &rqBody) {
	_requestContent = (char *)malloc(sizeof(char) * (rqBody.size() + 1));
	_requestContentLength = rqBody.size();
	for (int i = 0; i < _requestContentLength; i++)
		_requestContent[i] = rqBody[i];
	_requestContent[_requestContentLength] = '\0';
}

std::string addCookieHeader(std::string cookie){
	std::string header = "Set-Cookie: ";
	while (header << getline(cookie, ";")){
		header += "\r\nSet-Cookie: ";
	}
	std::cout << "header: " << header << std::endl;
	return header;
}

void	Response::setDefaultErrorBody() {
	this->_defaultErrorBody = "<html>\n";
	this->_defaultErrorBody += "  <body>\n";
	this->_defaultErrorBody += "    <h1>\n";
	this->_defaultErrorBody += "      500 Internal Server Error\n";
	this->_defaultErrorBody += "    </h1>\n";
	this->_defaultErrorBody += "  </body>\n";
	this->_defaultErrorBody += "</html>";
}

void	Response::initStatusPageMap() {
	std::map<int,std::string>::iterator it = this->server->_errorPages.begin();
	this->_errorPageMap[200] = "docs/web/status_pages/200.html";
	this->_errorPageMap[201] = "docs/web/status_pages/201.html";
	this->_errorPageMap[400] = "docs/web/status_pages/400.html";
	this->_errorPageMap[403] = "docs/web/status_pages/403.html";
	this->_errorPageMap[404] = "docs/web/status_pages/404.html";
	this->_errorPageMap[405] = "docs/web/status_pages/405.html";
	this->_errorPageMap[413] = "docs/web/status_pages/413.html";
	this->_errorPageMap[500] = "docs/web/status_pages/500.html";
	this->_errorPageMap[502] = "docs/web/status_pages/502.html";
	for (;it != this->server->_errorPages.end(); ++it){
		this->_errorPageMap[it->first] = it->second;	
	}
}

std::string Response::getDirName(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
		this->location = path.substr(0,pos);
        return path.substr(0, pos);
    } else {
        return ".";
    }
}

bool	Response::isURIAcceptable(const char *uri) {
	struct stat info;
	if (stat(uri, &info) != 0) {
		if (errno == ENOENT) {
            std::string path_str(uri);
            std::string dir = getDirName(path_str);
			if (stat(dir.c_str(), &info) != 0) {
				this->_statusCode = 404;
				return false;
			} else if (method != POST) {
				this->_statusCode = 404;
				return false;
			} else {
				this->_statusCode = 200;
				return true;
			}
		}
	} else if (info.st_mode & S_IFDIR) {
		this->location = uri;
		//Given uri points to directory.
		if (method == DELETE) {
			this->_statusCode = 405;
			return false;
		}
		else if(method == POST && _headers["Content-Type"].find("multipart/form-data") != std::string::npos){
			if (isUriInServer(uri)){
				if(isMethodAllowed("POST", uri)){
					this->_statusCode = 200;
					return true;
				}
			}
			this->_statusCode = 403;
			return false;
		}
		else if (method == GET) {
			if (!strcmp(uri, server->getRoot().c_str())){
			    this->_statusCode = 200;
				return true;
			}
			else{
				if (isUriInServer(uri)){
				    if(isMethodAllowed("GET", uri) && isAutoIndex(uri)){
						this->_statusCode = 200;
						return true;
					}
				}
				this->_statusCode = 403;
				return false;
			}
		}
		else {
			this->_statusCode = 405;
			return false;
		}
	}
	std::string path_str(uri);
    std::string dir = getDirName(path_str);
	this->_statusCode = 200;
	return true;
}

bool Response::isAutoIndex(std::string location){
	Location *locationObj = server->getLocation(location);
	if (locationObj){
		return (locationObj->isAutoindex() > 0 ? true : false);
	}
	return false;
}

bool Response::isMethodAllowed(std::string method, std::string location){
	Location *locationObj = server->getLocation(location);
	if (locationObj){
		std::vector<std::string>::iterator it = locationObj->getMethods();
		for (; it != locationObj->allowed_methods.end(); it++){
			if (!method.compare(*it))
				return true;
		}
	}
	return false;

}


bool Response::isUriInServer(const char *uri){
	Location *location = server->getLocation(uri);

	if (location == NULL)
		return false;
	return true;
}

bool	Response::isCGI(const char *extension) {
	if (extension == NULL)
		return false;
	std::vector<std::string> cgiExtensions ;
	cgiExtensions.push_back(".cgi");
    cgiExtensions.push_back(".pl");
    cgiExtensions.push_back(".py");
    cgiExtensions.push_back(".sh");
    cgiExtensions.push_back(".exe");
    cgiExtensions.push_back(".php");
    cgiExtensions.push_back(".rb");
    cgiExtensions.push_back(".tcl");
	std::string ext = ".";
	ext += extension;
	if (std::find(cgiExtensions.begin(), cgiExtensions.end() , ext) != cgiExtensions.end()){
		return true;
	} else {
		return false;
	}
}

bool	Response::isProcessableCGI(const char *extension, std::vector<std::string> cgiextensions) {
	std::string ext = ".";
	ext += extension;

	if (extension == NULL) {
		this->_statusCode = 500;
		return false;
	}
	if (std::find(cgiextensions.begin(), cgiextensions.end(), ext) != cgiextensions.end())
		return true;
	else {
		this->_statusCode = 502;
		return false;
	}
}

std::string	Response::errorResponse() {
	this->_body = readBody(getStatusPage().c_str());
	this->_contentLength = _body.size();

	std::string	response;

	response = addStatusLine(this->_statusCode);
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(this->_contentLength);
	response += "\r\n";
	response += this->_body;
	return response;
}

std::string Response::getStatusPage() {
	return this->_errorPageMap[this->_statusCode];
}

std::string Response::readBody(const char *path) {
	int	fd;
	std::string filePath = "";
	if(!strcmp(path,this->server->getRoot().c_str())){
		filePath = path;
		filePath.append(server->getIndex());
	}
	else{

		filePath = path;
	}
	struct stat info;
	if (stat(filePath.c_str(), &info) == 0 && (info.st_mode & S_IFDIR)) {
        if (isAutoIndex(path)) {
            return generateAutoIndex(path);
        }
    }
	if ((fd = open(filePath.c_str(), O_RDONLY)) < 0) {
			switch (errno) {
			case ENOENT:
				this->_statusCode = 404;
				break;
			case EACCES:
				this->_statusCode = 403;
				break;
			default:
				this->_statusCode = 500;
		}
		if ((fd = open(getStatusPage().substr(1).c_str(), O_RDONLY)) < 0){
			return this->_defaultErrorBody;
		}
	}
	return this->readBody(fd);
}

std::string Response::generateAutoIndex(std::string  path){
    DIR *dir;
    struct dirent *ent;
    std::stringstream body;

    body << "<ul>";

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (std::strcmp(ent->d_name, ".") != 0 && std::strcmp(ent->d_name, "..") != 0) {
                std::string fullPath = path.substr(path.rfind("/") +1) + "/" + ent->d_name;
                body << "<li><a href=\"" << fullPath << "\">" << ent->d_name << "</a></li>";
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Could not open directory: " << path << std::endl;
    }

    body << "</ul>";

    return body.str();
}

std::string	Response::readBody(const int &fd) {
	std::string body;
	char		buffer[1024];
	ssize_t		bytesRead;

	while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		body.append(buffer);
		buffer[0] = '\0';
	}
	return body;
}

std::string	Response::writeContent(const char *path) {
	int fd = -1;

	if (access(path, F_OK) < 0) {
		fd = open(path, O_WRONLY | O_CREAT, 0664);	
		if (fd < 0 || write(fd, this->_requestContent, this->_requestContentLength) < 0)
			this->_statusCode = 500;
		this->_statusCode = 201;
	} else if (access(path, W_OK) < 0) {
		this->_statusCode = 403;
	} else {
		fd = open(path, O_WRONLY | O_TRUNC);
		if (fd < 0 ||  write(fd, this->_requestContent, this->_requestContentLength) < 0)
			this->_statusCode =500;
		this->_statusCode = 200;
	}
	close(fd);
	return readBody(this->getStatusPage().c_str());
}

std::string Response::deleteContent(const char *path) {
	if (access(path, F_OK) < 0) {
		this->_statusCode = 404;
	} else {
		if (unlink(path) < 0)
			this->_statusCode = 403;
		else
			this->_statusCode = 200;
	}
	return readBody(this->getStatusPage().c_str());
}
