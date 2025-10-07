## 5. Down-Then-Across vs Horizontal Complexity
The "down then across" format requires more pre-calculation because it depends on both terminal width and number of rows and columns. The horizontal format simply tracks line width dynamically, making it easier to implement.

## 6. Managing Display Modes
A mode flag variable determined by getopt() controls which display function runs: `-l` triggers long listing, `-x` triggers horizontal display, and the default uses the vertical (down-then-across) layout.
