# v2_chat
A LINUX terminal Chat program written in C. P2P with 2 users only.
I am in the process of implementing encryption.

Currently interested in this project because of my interests in specializing
Network Programming. Writing a chat program will allow me to practice threading in C
at the same time. If you see any unsafe procedures or techniques in my coding please
let me know, as I would like to stay far away from buffer overflows, & implement
proper ANSI C standards.


This program is crude but functional, I am in the process of cleaning it up and
implementing a "better look". Terminal only with no GUI. Although, in the future 
I might delve into a GUI version of v2, and apply a "chat room" functionality
instead of just 2 users.

Compiling Instructions:

gcc v2_server.c -o v2_server -Wall

gcc v2_client.c -o v2_client -Wall
