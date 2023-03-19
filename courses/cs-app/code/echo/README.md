# Echo Client-Server Project

This project is an implementation of an echo client-server system, featuring multiple server types to demonstrate various concurrency models in C. The project includes iterative, process-based, thread-based, pre-threaded, and dynamic threaded servers. This project was developed to gain practical exposure to socket programming and concurrency concepts in the context of learning from the Computer Systems: A Programmer's Perspective (CS:APP) book.

## Table of Contents

- [Requirements](#requirements)
- [Usage](#usage)
- [Directory Structure](#directory-structure)
- [Server Types](#server-types)
- [Makefile](#makefile)
- [Running the Applications Manually](#running-the-applications-manually)

## Requirements

- GCC (GNU Compiler Collection)
- GNU Make

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/m-ngr/computer-science.git
   ```

2. Change to the echo project directory:

   ```bash
   cd computer-science/courses/cs-app/code/echo
   ```

3. Build the project:

   ```bash
   make build
   ```

4. Run the desired server type in the background:

   ```bash
   make [server-type] &
   ```

Replace `[server-type]` with one of the available server types: `iserver`, `pserver`, `tserver`, `ptserver`, `dtserver`.

5. Run the client:

   ```bash
   make [client-type]
   ```

Replace `[client-type]` with one of the available client types: `iclient`, `pclient`, `tclient`, `ptclient`, `dtclient`.

## Directory Structure

- `./bin` - Compiled binaries for the client and server applications.
- `./lib` - Helper libraries and utility functions.
- `client.c` - Source code for the echo client.
- `iserver.c` - Source code for the iterative echo server.
- `pserver.c` - Source code for the process-based echo server.
- `tserver.c` - Source code for the thread-based echo server.
- `ptserver.c` - Source code for the pre-threaded echo server.
- `dtserver.c` - Source code for the dynamic threaded echo server.
- `Makefile` - Makefile for building and running the project.

## Server Types

1. Iterative Server (`iserver`): A single-threaded server that processes one client connection at a time.
2. Process-Based Server (`pserver`): A server that forks a new process for each client connection.
3. Thread-Based Server (`tserver`): A server that creates a new thread for each client connection.
4. Pre-Threaded Server (`ptserver`): A server that pre-allocates a fixed number of worker threads to handle client connections.
5. Dynamic Threaded Server (`dtserver`): A server that maintains a dynamic pool of worker threads to handle client connections, adjusting the pool size based on demand.

## Makefile

The Makefile contains various targets to build and run the client and server applications. The main targets are:

- `build`: Compiles the client and server applications.
- `clean`: Removes the generated binaries and output directories.
- `[client-type]`: Runs the client application with the specified server type.
- `[server-type]`: Runs the server application with the specified server type.

## Running the Applications Manually

To run the Echo Client-Server Project executables from the `bin` directory without using the Makefile, follow these steps:

1.  Change to the `bin` directory:

    ```bash
    cd bin
    ```

2.  Run the desired server type in a separate terminal:

    ```bash
    ./echo-[server-type] [port]
    ```

    Replace `[server-type]` with one of the available server types: `iserver`, `pserver`, `tserver`, `ptserver`, `dtserver`. Replace `[port]` with the desired port number for the server to listen on.

    For example, to run the iterative server on port 1111:

    ```bash
    ./echo-iserver 1111
    ```

    For the `ptserver`, you will also need to provide the number of worker threads:

    ```bash
    ./echo-ptserver [port] [num_threads]
    ```

    For the `dtserver`, you will need to provide the minimum, maximum, and step values for the number of worker threads:

    ```bash
    ./echo-dtserver [port] [min_threads] [max_threads] [step]
    ```

3.  Run the client in a separate terminal:

    ```bash
    ./echo-client [server-ip] [server-port]
    ```

    Replace `[server-ip]` with the IP address of the server (use `127.0.0.1` for localhost) and `[server-port]` with the port number on which the server is listening.

    For example, to connect to an iterative server running on localhost and port 1111:

    ```bash
    ./echo-client 127.0.0.1 1111
    ```

After running the client and server applications, you can interact with the echo server by typing messages into the client terminal. The server will echo the messages back to the client.
