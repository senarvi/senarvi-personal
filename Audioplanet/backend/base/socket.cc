//  backend/base/socket.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <cerrno>

#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>  // unlink

#include "backend/base/socket.h"
#include "backend/base/exceptions.h"
#include "backend/base/debug.h"

#include "backend/base/file.h"


namespace ap
{

//
// Socket class
//

    Socket::Socket(int protocolFamily)
    {
	fd_ = ::socket(protocolFamily, SOCK_STREAM, 0);
	if (fd_ == -1) throw SystemException(THIS_LOCATION);
    }


// Assign a local address to the socket
    void Socket::bind(Address * address, AddressLength addressLength) const
    {
	if (::bind(fd_, address, addressLength) != 0)
	    throw SystemException(THIS_LOCATION);
    }


// Make a connection to a remote socket
    int Socket::connect(Address * address, AddressLength addressLength) const
    {
	if (::connect(fd_, address, addressLength) != 0)
	    throw SystemException(THIS_LOCATION);
	return fd_;
    }


// Start listening for incoming connections
    void Socket::listen() const
    {
	if (::listen(fd_, 100) != 0)
	    throw SystemException(THIS_LOCATION);
    }


// Accept a pending connection
    int Socket::accept() const
    {
	int fd = ::accept(fd_, NULL, NULL);
	if (fd == -1) throw SystemException(THIS_LOCATION);
	return fd;
    }


//
// LocalSocket class
//


    LocalSocket::LocalSocket() : Socket(PF_LOCAL) {}


    LocalSocket::~LocalSocket()
    {
	unlink();
    }


    void LocalSocket::unlink() const
    {
	if (path_ == "") return;
	if (::unlink(path_.c_str()) != 0)
	    if (errno != ENOENT) throw SystemException(THIS_LOCATION);
    }


    void LocalSocket::bind(const std::string & path)
    {
	path_ = path;

	struct sockaddr_un address;
	address.sun_family = AF_LOCAL;
	strcpy(address.sun_path, path_.c_str());

	unlink();
	Socket::bind(reinterpret_cast<Address *>(&address), sizeof(address));
    }


    int LocalSocket::connect(const std::string & path) const
    {
	struct sockaddr_un address;
	address.sun_family = AF_LOCAL;
	std::strcpy(address.sun_path, path.c_str());

	return Socket::connect(reinterpret_cast<Address *>(&address), sizeof(address));
    }


//
// InetSocket class
//


    InetSocket::InetSocket() : Socket(PF_INET) {}


    void InetSocket::bind(uint32_t ipAddress, uint16_t tcpPort) const
    {
	trace();


	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = tcpPort;
	address.sin_addr.s_addr = ipAddress;

	Socket::bind(reinterpret_cast<Address *>(&address), sizeof(address));
    }


    int InetSocket::connect(uint32_t ipAddress, uint16_t tcpPort) const
    {
	trace();


	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = tcpPort;
	address.sin_addr.s_addr = ipAddress;

	return Socket::connect(reinterpret_cast<Address *>(&address), sizeof(address));
    }

}  // namespace ap
