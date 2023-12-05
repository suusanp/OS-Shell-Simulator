# OS Shell Simulator in C

Welcome to my Simulated OS Shell! This project is a part of the Operating Systems COMP 310 – ECSE 427 course at McGill University. Throughout the semester, I worked on implementing a multi-process scheduling system with various scheduling policies to create a simulated operating system environment.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)


## Description

This simulated OS shell is written entirely in C. It support concurrent processes and introduces different scheduling policies, including First-Come-First-Serve (FCFS), Shortest Job First (SJF), Round Robin (RR), and Aging (AGING). It simulates a basic operating system shell where users can interact with the system, manage variables, execute commands from scripts, and work with directories and files.



## Features

- **Interactive and Batch Modes**: The shell supports interactive mode for entering commands one by one and batch mode for executing commands from a text file.

- **Basic Commands**: It includes fundamental commands:
  - `help`: Displays available commands.
  - `quit`: Exits the shell.
  - `set VAR STRING`: Assigns a value to shell memory.
  - `print VAR`: Displays the value assigned to VAR.
  - `run SCRIPT.TXT`: Executes commands from a text file.

- **Variable Management**: Create and manage variables using the `set` and `print` commands. The shell handles variable assignment and retrieval.

- **Error Handling**: The shell provides clear error messages, including for unsupported commands and assigning values with too many tokens.

- **`echo` Command**: It includes an `echo` command for displaying strings. The string can be alphanumeric or a variable name preceded by `$`.

- **Batch Mode** It returns to interactive mode. The `$` symbol is displayed only in interactive mode.

- **Directory Commands**: Additional commands include:
  - `my_ls`: Lists files and directories in the current directory.
  - `my_mkdir dirname`: Creates a new directory, supporting both alphanumeric and variable-based directory names.
  - `my_touch filename`: Creates an empty file with an alphanumeric name.
  - `my_cd dirname`: Changes the current directory.
  - `exec SCRIPT1.TXT SCRIPT2.TXT SCRIPT3.TXT POLICY`: Executes script according to input policy. Policy includes: FCFS, SJF, RR, AGING

- **Chaining Commands**: Chain multiple instructions together using semicolons, enabling sequential execution of commands.



## Getting Started

### Prerequisites

- A Unix-like operating system (Linux, macOS).
- C compiler (e.g., GCC).

### Installation

Clone the repository to your local machine.

For interactive use, simply run ./mysh.




