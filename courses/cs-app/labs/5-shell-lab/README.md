# Shell
The shell program provided is a simple command-line interface that allows users to execute commands. It supports both built-in commands and external commands.

## Building The Shell
To build the shell program run the following command:
```bash
gcc -o tsh tsh.c
```

## Usage
Just run it from your terminal.

## Command Syntax
The shell program supports both built-in commands and external commands. Built-in commands are executed directly by the shell, while external commands are executed as separate processes.

### Built-in Commands
The following built-in commands are supported:

 - `quit`: Terminates the shell program.
 - `jobs`: Lists all the currently running or stopped jobs.
 - `fg <job>`: Brings a job to the foreground. `<job>` can be either a job ID (e.g., %1) or a process ID (e.g., 1234).
 - `bg <job>`: Resumes a stopped job in the background. `<job>` can be either a job ID or a process ID.

### External Commands
The shell program can execute external commands available in the system. Simply enter the command and its arguments at the shell prompt. For example:

```bash
tsh> ls -l
```
The shell will execute the `ls -l` command and display the output.