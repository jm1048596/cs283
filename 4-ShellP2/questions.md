1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  `fork` is required to ensure we keep the original process in tact. If we simply called `execvp` without `fork`'ing first, the orinigal process would be transformed into the command called by `execvp`, thus losing the ability to use it further.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  if `fork` fails, it returns a negative value. My code handles this by reporting the error with `perror` and `exit`'ing thereafter.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The first argument provided to `execvp` is the name of the executable. It searches the `PATH` environment variable for this exe.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  `wait`, as the name implies, waits for the child process to be finished before proceeding. If this wasn't called, the child would never have it's exit status collected and would continue to take up space.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  `WEXITSTATUS()` is used to extract the child's specific exit status. It's important because the variable being assigned into in `wait` contains more than just the exit status, so `WEXITSTATUS()` is necessary to extract only the exit code.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I kept a boolean variable that tracks when I am inside quotes. If I am inside quotes, ignore spaces; if I'm not in quotes, turn spaces into null bytes. This is necessary as quotes are used in the shell for this exact purpose, to "ignore" spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I had to change quite a bit in this assignment from the previous. In this one, I went character by character in the buffer as opposed to using `strtok` in the previous assignemnt. I felt this was necessary to handle quoted arguments as `strtok` would always replace a space with a null byte.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are primarily used as notifications in Linux to tell a process when something specific is happening, has happened, or should happen. They are different from other forms of IPC in that they are asynchronous, meaning they don't occur one after another.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  1.) SIGTERM asks a process to end and allows it to do what it needs to do to end (i.e. cleanup). \
    2.) SIGKILL immediately ends a process without allowing it to do any cleanup first. \
    3.) SIGSEGV is the signal that an invalid memory address had been attempted to be accessed.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP is similar to SIGKILL in that it immediately stops/pauses a process. It cannot be ignored like SIGINT as it is kernel-enforced and does not allow for handling of it.