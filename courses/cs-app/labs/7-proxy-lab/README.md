# CS:APP Proxy Lab

 The proxy server acts as an intermediary between clients and servers, forwarding client requests to the appropriate servers and returning the server's response to the clients. This allows the proxy server to provide caching, filtering, and other functionality.

## Files Included
The following files are included in the repository:

- `proxy.c`: The main source file that contains the implementation of the proxy server.
- `cache.c`: A source file that implements caching functions used by the proxy server.
- `csapp.c`: A source file that implements various helper functions used by the proxy server.
- `Makefile`: A makefile that builds the proxy program and provides additional commands for cleaning and packaging.
- `port-for-user.pl`: A Perl script that generates a random port for a specific user.
- `free-port.sh`: A shell script that identifies an unused TCP port that can be used for the proxy server.
- `driver.sh`: An autograder script for Basic, Concurrency, and Cache testing.
- `nop-server.py`: A helper script for the autograder.
- `tiny`: A tiny web server used for testing.

## Usage:
To build the proxy and the tiny server, run the following command:
```bash
make all
```

## Running the Proxy Server
To run the proxy server, use the following command:

```bash
./proxy <port> <verbose>
```
- `<port>`: The port number on which the proxy server should listen for incoming connections.
- `<verbose>`: An optional argument that controls the verbosity level of the proxy server. Use v0 for no verbosity, v1 for low verbosity, and v2 for high verbosity.