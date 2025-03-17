1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

    > The server must send a special character to the client to indicate the output is finished In our case, this is ASCII character 4, which is the End Of File (EOF) character. To ensure it receieves the entire message, the client can do `recv` calls until it reads the EOF character from the server.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

    > The message itself holds details on the length of the message. There are typcially two parts of a request: 1. The details/description of the message and 2. the actual message. The details describe aspects of the message, such as length, which can be used to traverse the proper number of bits through the message. This allows the server/client to read the entire thing and not a byte more. \
    If this is not handled correctly, the server or client could easily read memory they don't own, which would be erroneous. The message could also be interpreted incorrectly if the details of it are not properly described.

3. Describe the general differences between stateful and stateless protocols.

    > A stateful protocol is one that maintains states across the execution of commands. Things like environment variables are maintained across requests as the client may need to use them. A stateless protocol is the opposite of a stateful one in that states are not maintained. A command is executed, and then the details of that execution are lost. \
    Stateless protocol are more portable and space-efficient as they don't need to maintain nearly as much.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

    > UDP is faster and more efficient as it does not require the set-up of a connection, which is useful in our case.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

    > Sockets, the primary focus of this assignment.