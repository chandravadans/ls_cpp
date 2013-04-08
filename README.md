 Name        : Chandravadan S

Readme
 An attempt to mimic the behavior of the unix ls command with the options
 -l : long listing
 -t : in reverse time sort order
 -S : size
 -a : all
 -d : directory
 -r : Recursive

-> In -l, bytes in HUMAN READABLE format! ^_^
-> If redirected to a file, ls puts each entry on a seperate line
-> In case of -t and -S being given, sorts on whichever occurs last

Upcoming Features
-> Colors

-> segfaults on files with extended SELinux permissions (-rwxr--r--.) -TO FIX

-> Use ncurses to adjust printing o/p




