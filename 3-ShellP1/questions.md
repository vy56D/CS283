1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: fgets() allows the number of characters to be read to be specified. Doing so will help prevent going beyond the allocated memory. fgets() also reads the whole line instead of until the first whitespace like scanf().

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  malloc() allows for the space allocated to fit that of what is actually needed. Different inputs will have different amounts of memory aloocated. malloc() also allows the size to be increased or decreased if needed using realloc().


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  It is necessary because it may interrupt command parsing. An issue that may arise when executing commands is that the command is not found. The space will throw off the search. If strcmp() is used for determining the command then the space will make it so that the command does not match. Removing the leading and trailing spaces also ensures that strchr() will work without issues.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  3 redirection examples are ls > file.txt, sort < sort.txt, cat file1>file2. The challenge implementing ls > file.txt is that it would have to open and write to the file. Issues with what would happen if the file already exists would also arise. The challenge implementing sort < sort.txt is that the file has to exist. Issues would arise if there is no handling for when the file does not exist. Issues also arise when the file is unreadable. The challenge implementing cat file1>file2 is that the data may be overwritten. There may also be issues regarding reading and writing the file.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to redirect inputs and outputs. The output of a command can be redirected into a file or the data inside a file can be redirected into a command to be run. Piping passes the output of one command into another. This allows one command to run and have its output be the input of another command without having a file in between.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: By keeping them separate, it is easier to determine what is an error output and what is a regular output. The coder will have an easier time looking at the code and fixing any potential issues without accidentally making mistakes. Keeping them separate allows separate redirecting of outputs. It is overall more organized and allows more flexibility with handling both regular and error outputs.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: Our custom shell should print an error message and have a nonzero exit code. Nonzero exit indicates that an error has occurred. We should provide a way to merge them because this allows the regular output to be with the error output in one place. Merging can be done by redirecting STDERR and STDOUT to the same destination.
