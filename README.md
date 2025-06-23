# Linux Tree Command Implementation in C

A professional implementation of the `tree` command in C, displaying directory structures with file information including permissions, ownership, and file sizes. This is a professional, memory-efficient version of a basic tree command implementation.

## Features

- **File Information Display**: Shows file permissions, owner, group, and size
- **Tree Structure**: Visual tree representation with proper connectors
- **Hidden File Filtering**: Automatically skips hidden files and directories
- **Multiple Paths**: Support for processing multiple directories
- **Memory Efficient**: Professional memory management without global variables
- **Error Handling**: Robust error handling with proper system call checks

## Installation

### Prerequisites
- GCC compiler
- Linux/Unix system with POSIX support

### Build and Install

```bash
# Clone the repository
git clone https://github.com/VictoKu1/LinuxTree_Command.git
cd LinuxTree_Command

# Build the program
make

# Install system-wide (optional)
sudo make install

# Or run directly
./stree
```

## Usage

### Basic Usage
```bash
# Display current directory tree
./stree

# Display specific directory tree
./stree /path/to/directory

# Display multiple directories
./stree /path1 /path2 /path3
```

### Output Format
The program displays each file/directory with the following information:
- File type and permissions (e.g., `drwxr-xr-x`)
- Owner and group names
- File size in bytes
- File/directory name

Example output:
```
.
├── [drwxr-xr-x user     group        4096]  directory1
│   ├── [-rw-r--r-- user     group        1024]  file1.txt
│   └── [-rw-r--r-- user     group        2048]  file2.txt
└── [drwxr-xr-x user     group        4096]  directory2
    └── [-rw-r--r-- user     group        512]  file3.txt

2 directories, 3 files
```

## Build Options

The makefile provides several useful targets:

```bash
make           # Build the program
make install   # Install to /usr/local/bin
make clean     # Remove object files
make distclean # Remove all build files
make help      # Show available targets
```

## Technical Details

### Key Improvements Over Basic Implementation

1. **Memory Efficiency**: 
   - Removed global variables
   - Fixed memory allocation issues (no more `int arr[INT_MAX]`)
   - Proper stack usage with reasonable limits (`MAX_DEPTH = 64`)

2. **Code Quality**:
   - Comprehensive error handling
   - Proper function documentation
   - Consistent coding style
   - Modular design with separate functions

3. **Professional Standards**:
   - Proper compiler warnings enabled
   - Standard C99 compliance
   - Installation support
   - Comprehensive documentation

### Architecture

The program uses the `nftw()` (file tree walk) function for efficient directory traversal and maintains a context structure to pass information between callback functions without global variables. The tree display logic matches the original implementation exactly while being more memory-efficient.

### Compatibility

This implementation produces **identical output** to the original basic implementation while being much more professional and maintainable. It maintains the same command-line interface and behavior.

## License

This project is open source. Feel free to use, modify, and distribute according to your needs.

## Contributing

Contributions are welcome! Please ensure your code follows the existing style and includes proper documentation.







