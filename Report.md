## Feature-2

## 1. Difference between stat() and lstat()
The crucial difference is that `stat()` follows symbolic links and returns information about the target file, while `lstat()` returns information about the link itself. In the `ls` command, `lstat()` is preferred to correctly display link details without following them.

## 2. Using bitwise operators with st_mode
The `st_mode` field uses bitwise flags; by applying operators like `mode & S_IFDIR` or `mode & S_IRUSR`, we can test whether a file is a directory or if the user has read permission, respectively.

