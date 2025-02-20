1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use `fork/execvp` to keep the parent process running while the child process executes a new program. If `execvp` is called directly, the child can replace the parent or current program. I think`fork` provides value in creating a duplicate of the parent process which allows `execvp` to replace a duplicate and have the orginal parent program continue running.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails then -1 is returned. My implementation handles this scenario by checking if a value less than 0 is returned which would include the -1. If -1 is returned then an error message will be printed and then exited with an error code of 1.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() uses the PATH system environment variable to find the command to execute. It will look through PATH to find the command to eecute and will return an error if it is not found.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling wait() allows the parent process to wait until the child process finishes before continuing. If it is not called then resources would be wasted without proper recovery.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It provides information on the child process's exit code. It is important because it gives information on how the child process ended such as did it end sucessfully without errors or was an error encountered.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It checks each character to find exactly where the quotations start and end. This is necessary to ensure that whitespaces inside quotations and other values are not incorrectly removed or split.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I changed how the code was parsed. strtok was no longer useful as it would incorrectly parse the command given. I also had to account for quotations which I did not worry about in the previous assignment. There was unexpected challenges in removing the whitespace when refactoring my old code. I often removed whitespace in the quotations as well.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals is to notify events such as a request to stop a process or even terminate it. It differs in the way that it does not require data transfer and is only a one way communication.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL forces a process to terminate. It is used when a process does not respond to other termination signals and has to be terminated. SIGTERM is the default signal sent by kill which can only be caught by the target process. It is used to terminate a process and clean up resources. SIGINT is the signal sent when a process is interrupted. It is used to interupt a process and allow it to terminate at once.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process recieves SIGSTOP, the process's execution is stopped and paused. It cannot be caught or ignored like SIGINT because the signal is part process management system which is not able to be handled by the process itself. The process has no ways to bypass SIGSTOP.
