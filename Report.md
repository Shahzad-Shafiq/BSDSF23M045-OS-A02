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
ANSI escape codes are special sequences (starting with `\033[` or `\x1B[`) that tell the terminal to change text color or style until a reset code is printed.

### Feature 7 – Recursive Listing (-R)

**Q1.** In a recursive function, what is a "base case"?  
**A1.** A base case is the condition that stops recursion. In `ls -R`, recursion stops when there are no more subdirectories to process (i.e., non-directory entries or `.` and `..`).

**Q2.** Why is it essential to construct the full path before recursion?  
**A2.** Because without the full path (`parent_dir/subdir`), the recursive call would look for the subdirectory in the wrong current directory, causing errors or infinite loops.


