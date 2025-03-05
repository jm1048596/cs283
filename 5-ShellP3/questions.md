1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

> My implementation makes the parent wait for all of its children before proceeding by calling `waitpid()` on each of the child process id's. \
If this was not done, the program would continue before the children have finished their execution, which could lead to weird output in the terminal. This would also cause the children to become zombie processes after their parent process ends, making them continue to take up OS resources.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

> If the pipes were left open, the OS's file descriptor table would have spots taken up that are no longer actuallt being used, similar to a memory leak. This may not affect much on a small scale, but as more pipes and file descriptors are used, forgetting to close them may lead to this table being filled.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

> If `cd` was an external process, it would create a child process. This would be problematic as called `chdir` on the child would change the child's directory, not the parent's. This is obviously not the anticipated behavior, which is why the parent itself must build in `cd`.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

> To keep this memory efficient, you could only create pipes as you go instead of creating all of them prior to all of the rewiring. This would also imply closing the pipes as you go, allowing for the reuse of those file descriptors. This would essentially eliminate the possibility of exceeding the limit of the OS's file descriptor table.