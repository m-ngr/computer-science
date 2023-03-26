# Tiny Web Server

Tiny is a simple, lightweight HTTP/1.0 web server designed primarily for educational purposes. The project includes three server variants (`itiny`, `ttiny`, and `dtiny`) and a CGI app called `adder`. The server is capable of handling GET, POST, and HEAD requests and serves static content as well as dynamic content through CGI.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Building the Server](#building-the-server)
- [Running the Server](#running-the-server)
- [Testing the Server](#testing-the-server)
- [Running Servers Manually](#running-servers-manually)
- [Cleaning up](#cleaning-up)

## Features

- Three server variants:
  - `itiny`: Iterative web server
  - `ttiny`: Multi-threaded web server
  - `dtiny`: Dynamic multi-threaded web server with adaptive thread management
- Simple CGI app `adder` to demonstrate server capabilities

**Note** that the Tiny web server is designed to be simple and is not intended for use in production environments. It supports only HTTP/1.0 and handles GET, POST, and HEAD requests.

## Prerequisites

To build and run the Tiny web server, you'll need:

- GCC (GNU Compiler Collection)
- GNU Make
- A Unix-based system (Linux or macOS)

## Building the server

1. Clone the repository:
   ```bash
   git clone https://github.com/m-ngr/computer-science.git
   cd computer-science/courses/cs-app/code/tiny
   ```
2. Build the project using the provided Makefile:

   ```bash
   make build
   ```

   This command will compile the Tiny web server in three different versions:

   - `itiny`: Iterative version
   - `ttiny`: Multithreaded version
   - `dtiny`: Dynamic-threaded version

   The compiled binaries will be placed in the `./bin` directory.

   Additionally, it will compile the `adder` application and place the binary in the `./dist/cgi-bin` directory.

## Running the Server

You can run any of the server variants using the provided Makefile targets:

- Iterative server (`itiny`): listens on port **2020**

  ```bash
  make itiny
  ```

- Multi-threaded server (`ttiny`): listens on port **2021**

  ```bash
  make ttiny
  ```

- Dynamic multi-threaded server (`dtiny`): listens on port **2022**

  ```bash
  make dtiny
  ```

Once the server is running, you can access it using your web browser or any HTTP client by entering the server's address and port in the URL (e.g., `http://localhost:2020`).

## Testing the Server

Two test targets are provided in the Makefile to test the multi-threaded server (`ttiny`) and the dynamic multi-threaded server (`dtiny`):

- Test the multi-threaded server (`ttiny`):

  ```bash
  make ttest
  ```

- Test the dynamic multi-threaded server (`dtiny`):
  ```bash
  make dtest
  ```

These targets will run tests to ensure the threaded servers work correctly. The tests use the `test.sh` script, which sends requests to the server and checks the number of running threads.

## Running Servers Manually

You can also run the server variants manually by executing the compiled binaries in the `bin` directory:

- Iterative server (`itiny`):

  ```bash
  ./bin/itiny [port] [verbose]
  ```

- Multi-threaded server (`ttiny`):

  ```bash
  ./bin/ttiny [port] [verbose]
  ```

- Dynamic multi-threaded server (`dtiny`):

  ```bash
  ./bin/dtiny [port] [min-threads] [max-threads] [add-threads] [verbose]
  ```

Replace the arguments in brackets with appropriate values. The `verbose` parameter is used for setting the verbosity level:

- 0: Minimal output
- 1: Request/Response lines only
- 2: Complete headers for request/response

The `min-threads`, `max-threads`, and `add-threads` parameters are used for the dtiny version to configure the thread pool settings. Set 0 to use the default values.

## Cleaning up

To remove the compiled binaries and clean up the directories, use the `clean` target:

```bash
make clean
```

This command will remove the `./bin` and `./dist/cgi-bin` directories.

**Note**: This action is irreversible, and you'll need to recompile the server and applications using `make build` if you want to use them again.
