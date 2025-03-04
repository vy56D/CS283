1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensures that all child processes complete before the shell continues accepting user input by calling waitpid() on each child process. This makes sure that the parent waits for the child process to finish before continuing. If I forgot to call waitpid() on all child processes then new processes might start before the current child process is finished. The child process may not be cleaned up properly which will lead to memory leaks and waste of resources.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because if they are not closed then there may be file descriptor leaks. If they are left open then there can be a lack a termination of a process and waste of resources. The processes will also begin to not run smoothly, without errors.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is implemented as a built-in rather than an external command because it changes the shell's environment such as the current working directory. If cd were implemented as an external process then the directory change would only be in the child as the child is the one that actually ran the process. The shell will stay in the old directory without change which means the current working directory does not change.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would modify my implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently by dynamically allocating memory. Memory will be allocated when needed and data structures will be able to increase in size if necessary. Some trade-offs to consider are the increasing complexity as memory is reallocated and any complications that may occur with memory allocation. Repeatedly reallocating memory is not very efficient and ideal in certain cases.
