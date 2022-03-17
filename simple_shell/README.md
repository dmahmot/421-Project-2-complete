
# Project 2 - Simple Shell - Dustin Mahmot

## CMSC 421 Spr22 - Dr. Larson


### Part 1 Requirements

1. Used a while loop to prompt user with '$' after each entered command.

2. Used argc in main (before while loop) to check number of command line arguments. 

3. Used getline() to read input from user.

4. Used character array manipulation and a series of string functions (and utils.c functions) to parse user input.

5. Used isdigit() and some char array checks to try to parse exit command arguments as an integer.

6. Used a clean up function (and constant valgrind checks along the way) to ensure no memory leakage.

### Part 2 Requirements

- Used char array manipulation to take user input and put it into filepath form.

- Used fopen() to open specified file and a while loop to read from the file and print it to stdout.

### Misc

- Used fprintf() to print to stderr for all errors.