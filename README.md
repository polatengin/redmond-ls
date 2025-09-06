# redmond-ls

![ls CI](https://github.com/polatengin/redmond-ls/actions/workflows/ci.yml/badge.svg)

A simple implementation of the Unix `ls` command written in C from scratch.

## Features

- List files and directories
- Long listing format with file details
- Show hidden files
- Handle multiple directories and files as arguments
- Clean, readable output

## Usage

```bash
./ls [OPTION]... [FILE]...
```

### Options

- `-l` - Use long listing format (shows permissions, owner, group, size, date)
- `-a` - Show hidden files (files starting with '.')
- `-h` - Display help message

### Examples

```bash
# List current directory
./ls

# Long listing format
./ls -l

# Show hidden files
./ls -a

# Combine options
./ls -la

# List specific directory
./ls /home/user

# List multiple directories
./ls /etc /var/log
```

## Building

Compile using the provided Makefile:

```bash
make
```

Or compile manually:

```bash
gcc -Wall -Wextra -std=c99 -o ls src/main.c
```

## Installation

To install system-wide:

```bash
make install
```

To uninstall:

```bash
make uninstall
```

## Implementation Details

This implementation uses standard POSIX functions:
- `opendir()`, `readdir()`, `closedir()` for directory operations
- `stat()` for file information
- `getpwuid()`, `getgrgid()` for user/group lookup
- `localtime()`, `strftime()` for time formatting

## Limitations

This is a simplified version focusing on core functionality. It does not include:
- Color output
- Sorting options
- Detailed file type indicators
- Advanced formatting options
- All GNU ls features
