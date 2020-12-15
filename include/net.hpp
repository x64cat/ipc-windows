namespace net
{
	bool Get(std::string& buffer, SOCKET &sock);
	bool Send(std::string buffer, SOCKET &sock, bool echo);
};