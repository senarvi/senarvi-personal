//  backend/base/socket.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_SOCKET_H
#define BACKEND_BASE_SOCKET_H


#include <string>
#include <stdint.h>

#include <sys/socket.h>


namespace ap
{

	class Socket
	{
	public:
		// Start listening for incoming connections
		void listen() const;

		// Accept a pending connection
		int accept() const;


	protected:
		typedef struct sockaddr Address;
		typedef socklen_t AddressLength;


		Socket(int protocolFamily);

		// Assign a local address to the socket
		void bind(Address * address, AddressLength addressLength) const;

		// Make a connection to a remote socket
		int connect(Address * address, AddressLength addressLength) const;


	private:
		int fd_;
	};


	class LocalSocket : public Socket
	{
	public:
		inline LocalSocket();
		~LocalSocket();

		void bind(const std::string & path);
		int connect(const std::string & path) const;


	private:
		std::string path_;

		void unlink() const;
	};


	class InetSocket : Socket
	{
	public:
		inline InetSocket();

		void bind(uint32_t ipAddress, uint16_t tcpPort) const;
		int connect(uint32_t ipAddress, uint16_t tcpPort) const;
	};

}  // namespace ap


#endif  // BACKEND_BASE_SOCKET_H

// Local Variables:
// mode:C++
// End:
