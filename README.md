# FT_IRC - Internet Relay Chat

## Description

This is an implementation of an IRC (Internet Relay Chat) server in C++98. The server is capable of handling multiple client connections simultaneously, using non-blocking I/O operations and TCP/IP for communication.

**Please note that only the server part is implemented - you'll need to connect using the IRC client IRSII or with netcat or telnet.**

## Features

- Support for multiple concurrent client connections
- Non-blocking I/O operations
- TCP/IP (v4 or v6) for client-server communication
- Authentication, setting a nickname and username
- Joining a channel
- Sending and receiving private messages
- All messages sent from one client to a channel are forwarded to every other client that joined the channel
- Two types of users: operators and regular users
- Channel operators have specific commands:

## Getting Started

### Prerequisites

Ensure you have a C++98 compiler installed on your machine.

### Building

Compile the source code with your C++98 compiler.

### Running

After compiling, run the server with the following command:

```
./ircserv <port> <password>
```

`port`: The port number on which your IRC server will listen for incoming IRC connections.

`password`: The connection password. This will be needed by any IRC client that tries to connect to your server.

### Connecting

This server will work with IRSSI client and with netcat 

