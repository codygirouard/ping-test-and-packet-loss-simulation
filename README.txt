This project includes C and python implementations of ping tests (with simulated packet loss) over UDP.

To compile the C programs using GNU compiler on Linux:
gcc client.c -o client
gcc server.c -o server

Then start the server program first:

./server <port_num>

And then start the client program:

./client <host_name> <port_num>

To run the python programs, start the server program first and then run the client program:
python server.py <port_num>

python client.py <host_name> <port_num>