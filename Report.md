## 5. Down-Then-Across vs Horizontal Complexity
The "down then across" format requires more pre-calculation because it depends on both terminal width and number of rows and columns. The horizontal format simply tracks line width dynamically, making it easier to implement.

## 6. Managing Display Modes
A mode flag variable determined by getopt() controls which display function runs: `-l` triggers long listing, `-x` triggers horizontal display, and the default uses the vertical (down-then-across) layout.


### Feature 5: Alphabetical Sort

**Q1:** Why is it necessary to read all directory entries into memory before you can sort them?  
**A1:** Because sorting requires random access to all filenames, they must first be stored in memory; streaming entries directly from `readdir()` cannot be sorted without full collection.

**Q2:** Explain the purpose and signature of the comparison function required by qsort().  
**A2:** The comparison function tells `qsort()` how to order elements. It takes two `const void *` arguments so it can be used with any data type, and returns the result of comparing two strings with `strcmp()`.

### Feature 6:

## 1. How do ANSI escape codes work to produce color in a standard Linux terminal?
ANSI escape codes are special sequences (starting with `\033[` or `\x1B[`) that tell the terminal to change text color or style until a reset code is printed. For example, to print text in green:
```c
printf("\033[0;32mHello\033[0m\n");
Here, \033[0;32m sets the text color to green, and \033[0m resets it back to default.
