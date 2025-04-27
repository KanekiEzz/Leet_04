# 📚 1337 Microshell - Exam Rank 03

This project is a simple microshell implementation in C for the **Exam Rank 03** at **1337**.

---

## 📂 File: `microshell.c`

### 🛠️ Features

- Execute commands separated by `;` or `|`
- Manage pipes between commands
- Handle the `cd` builtin command
- Properly manage file descriptors
- Error handling with clear messages


### 📜 Code Overview

```c
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

// Error output
void ft_putstr_fd2(char *str, char *arg);

// Execute a command
void ft_execute(char *argv[], int i, int tmp_fd, char *env[]);

// Entry point
int main(int argc, char *argv[], char *env[]);
```

### 🔍 Functions

- **ft_putstr_fd2(str, arg)**
  - Prints an error message to `stderr`.

- **ft_execute(argv, i, tmp_fd, env)**
  - Prepares and executes a command using `execve()`.

- **main(argc, argv, env)**
  - Parses arguments and handles piping, command separation, and built-ins like `cd`.


---

## ⚙️ How It Works

1. **Parsing**: Commands are separated by `;` and `|`.
2. **Execution**:
   - If command is `cd`, handle it manually.
   - Otherwise, `fork()` a child process to `execve()`.
3. **Pipes**:
   - If a `|` is encountered, create a pipe and duplicate the file descriptors.
4. **File Descriptors**:
   - Carefully manage closing and duplicating fds to avoid leaks.
5. **Error Handling**:
   - Prints meaningful errors when commands can't be executed or `cd` fails.


---

## 🚀 Usage Example

```bash
$ ./microshell /bin/ls -l \; /bin/echo hello world \| /bin/cat -e
```

- `;` separates two commands.
- `|` pipes output of one command to another.

---

## ⚠️ Important Notes

- Only basic functionality is implemented.
- No handling of quotes, redirections (`>`, `<`), or advanced bash features.
- Every `execve` or `pipe` failure results in immediate error reporting.
- Built-in `cd` is handled without forking.

---

## 👨‍💻 Author

- **By**: [iezzam@student.42.fr](https://profile.intra.42.fr/users/iezzam)
- **Created**: 2025/04/27

---

## 📌 Additional FAQ

<details>
<summary>Why duplicate STDIN for the child process?</summary>

Because the parent must continue reading correctly after the child process. Duplication ensures safe separation of input sources.

</details>

<details>
<summary>Why close and re-duplicate fds?</summary>

To avoid descriptor leaks and to reset the STDIN back to its original source after a child finishes.

</details>

<details>
<summary>Why use WUNTRACED?</summary>

Allows the parent to detect stopped child processes as well as terminated ones, ensuring full control.

</details>

