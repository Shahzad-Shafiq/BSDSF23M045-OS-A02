## 3. Down-Then-Across Logic
The "down then across" format prints items row by row, taking elements spaced by the number of rows (e.g., names[row + col*num_rows]). A single loop is insufficient because it prints files linearly, not in columns aligned across the screen.

## 4. Purpose of ioctl()
The `ioctl()` system call retrieves the terminal size so the program can adjust columns dynamically. Using a fixed width (like 80) would make the output misaligned on terminals with different sizes.
