1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets` preserves the entire line entered by the user (including spaces, newlines, etc.) and doesn't allow the passed buffer to be overloaded.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  If you allocated a fixed-size array, it would always need to be the absolute maximum length of a single line that could be entered the user. This would waste space with most lines entered, so it's better to allocate more space as needed.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we didn't trim these extra spaces, they would make the inputs to the commands much messier. By standardizing the number of spaces, we can keep everything consistent.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  1.) `echo "hello there" > greeting.txt` \
    2.) `./sort < db.txt > nums.txt` \
    3.) `nums.txt >> db.txt` \
    Implementing these would be challenging because we have to rewire the streams of these processes in the proper manner.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection must involve a file while pipes are used to redirect streams in commands.  There are also multiple orientations and options possible with redirection (i.e. appending or overwriting), while pipes are purely from left to right.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  For one, not separating the two would make it more difficult to differentiate between them when both are output. Having separate access to STDERR also allows for easier debugging and error messages can be handled separately.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Errors from commands should be sent to the terminal by default. It should print on separate lines from STDOUT so it's easy to differentiate between the two. We should provide a way to merge them, ideally in a way similar to an actual shell's redirection/piping. Using both STDOUT and STDERR as either the new streams of a process or the output to a file (depending on the situation) would be a way to accomplish this.