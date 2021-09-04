# *Professor Office Hour Simulation*

## Description
**_Simulation of Professor's Office Hour Schedule in C_** <br/>
ADD DESCRIPTION HERE...

###### MAYBE ADD SCREENSHOT
SCREENSHOT WOULD GO HERE.
<p>&nbsp;</p>

**Resources for Project:**
- C (Programming language)
- Atom (Text editor)

# Running / Viewing the Project
## Command Line
1. First, make sure that you have your terminal open. Information about your terminal can be found here:<br/>
   -Mac: https://support.apple.com/guide/terminal/open-or-quit-terminal-apd5265185d-f365-44cb-8b09-71a064a42125/mac<br/>
   -Windows: https://docs.microsoft.com/en-us/windows/terminal/
2. Next, check if you have the C compiler already installed in your system. If you are using the UNIX / Linux operating systems in your terminal, then you likely have a C compiler called GCC in your system. To determine if you already have GCC installed in your system, type `cc --version` or `gcc --version` in your terminal. Information regarding the version of your C Compiler will show on the terminal after you enter a "--version" command. The C programming language is already built into your system (regardless of what computer / operating system you have). For more information on C and C compilers, click the link below. <br/>
   -C Compiler Information: https://www.w3schools.in/c-tutorial/install/
3. On this repository page, go to the "Code" button and click the "Download Zip" option.
4. Go to your "Downloads" section on your computer and place the zip file in a folder/location other than "Downloads".
5. Double click on the zip file to open or "unzip" it so you can access the file contents.
6. In your opened terminal, navigate to the location where you opened the downloaded zip file and make sure you are inside of the "Office-Hour-Simulation-master" folder. (If you are unsure of command line commands, see the resources provided bellow.)<br/>
   -Linux and Max: http://mally.stanford.edu/~sr/computing/basic-unix.html<br/>
   -Windows: https://blogs.umass.edu/Techbytes/2014/11/14/file-navigation-with-windows-command-prompt/
7. Inside of the "Office-Hour-Simulation-master" folder on your terminal, navigate into the "office-simulation" folder.
8. Inside of the "office-simulation" folder, type the following command: `make`. We are using the C compiler along with a Makefile that was provided in this project to compile the code. An executable file is created called "simulator".
9. After typing the command above, type this command: `./simulator -f infile1.txt`. Here we are running the compiled version of the program (the executable file), which we named "simulation". The argument flag listed after the executable file tells the program what kind of algorithm to run in the program. We also pass a text file, included in this project, into the program to be read and analyzed.
10. After running the command in the previous step, output should be printed out on your terminal window. Information about other command arguments you can use is provided below.
    -To indicate that FIFO is being used, a "-f" flag is used as the first command-line argument. 
    -To indicate that advising appointments are being prioritized, a "-a" flag is used as the first command-line argument. 
    -To indicate that project demos are prioritized, a "-d" flag is used as the first command-line argument. 
    -To indicate that project questions are prioritized, a "-q" flag is used as the first command-line argument.
    -Example input file are provided in the forms of infile1.txt, infile2.txt, and infile3.txt. Each of these files can be passed through the program when it is run as the second command line argument.
11. Try using the other argument flags, "-a", "-d", and "-q", when you run the office hour simulation program. Also, try running the program using the other files, infile2.txt and infile3.txt. To view the text files, you can open them using a text editor. You can also type the `cat` command and then the name of the text file to view the contents of the text file on your terminal (Example: `cat infile1.txt` lets you see the file contents of infile1.txt).

<p>&nbsp;</p>

**_@Author: Andrew Krause_** <br/>
*LinkedIn:* https://www.linkedin.com/in/andrew-krause-b6aa21179/ <br/>
*Instagram:* https://www.instagram.com/aek.krause/ <br/>
*Facebook:* https://www.facebook.com/andrew.krause.35325

