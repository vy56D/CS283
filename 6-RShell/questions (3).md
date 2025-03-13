1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received from the server when it recieves a signal to signify the end of the transmission. Some techniques that can be used are sending the length of the message to indicate the size in bytes and by using delimiters to signify the end.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol can define and detect the beginning and end of a command sent over a TCP connection by using the length of the protocol. Termination signals and delimitors can also be used.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols keep track of the clients state while stateless protocols do not store any kind of nfomation. Stateful protocols allow for more detailed interaction than stateless protocols.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used because it has not that much latency. UDP is also easy to implement and does not have the worries of error handling. It provides good control despite its unreliability.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides a socket interface to enable applications to use network communications. Connections are established and data will be sent through send() and recv().