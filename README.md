# SluShell 

## Overview

SluShell is a unique shell program implemented in C. What sets it apart is its "backward" syntax for pipes. It also includes basic shell features such as directory navigation and automated error handling.

## Features

- **Automated Error Handling**: Automatic handling for common issues like syntax errors and invalid directory changes.
- **Backward Syntax for Piping**: Allows backward syntax for commands that generally use pipes. For example, the command `more ( sort ( ps aux` would be equivalent to `ps aux | sort | more` in a standard shell.
- **Built-in Commands**: Supports basic shell commands such as `cd` for directory navigation.
- **Signal Handling**: SluShell catches `SIGINT` signals, preventing accidental termination of the shell.

## Compilation

Compile `slushell.c` using:

```bash
gcc -o slushell slushell.c
```

## Running the Program

After compilation, you can run SluShell using:

\`\`\`bash
./slushell
\`\`\`

You'll then be presented with the `INPUT:` prompt, where you can start entering commands.

## Usage

### Basic Commands

At the `INPUT:` prompt, enter standard shell commands:

\`\`\`bash
INPUT: ls
\`\`\`

### Directory Navigation

You can use `cd` to change directories:

\`\`\`bash
INPUT: cd Documents
\`\`\`

### Using the Backward Syntax for Piping

SluShell allows backward syntax for piped commands:

\`\`\`bash
INPUT: more ( sort ( ps aux
\`\`\`

## Known Limitations and Issues

- You should not use consecutive parentheses `((` as it results in syntax errors.
- Commands should not end with an open parenthesis.
