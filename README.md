# grep
grep - globally search for regular expression and print out.

grep searches for patterns in text-files and prints out all matched lines.

See src folder for Source code. For Compilation use Makefile. Compilation will be successful only on Linux-based systems.

### To run command:
vgrep FLAGS PATTERN file1 file2 ....

#### Flags Implemented:

-c | --count              : This prints only a count of the lines that match a pattern

-h | --no-filename        : Display the matched lines, but do not display the filenames.

-H | --with-filename      : display filenames always

-i | --ignore-case        : Ignores, case for matching

-l | --files-with-matches : Displays list of a filenames only.

-n | --line-number        : Display the matched lines and their line numbers.

-v | --invert-match       : This prints out all the lines that do not matches the pattern

-w | --word-regexp        : Match whole word

-r | --recursive          : recursively searches for the pattern in all subdirectories

-x | --line-regexp        : Match entire line

-G | --basic-regexp       : basic Regexp

-E | --extended-regexp    : extended Regexp

-F | --fixed-string       : fixed string

-L | --files-without-match: print filenames which don’t match
