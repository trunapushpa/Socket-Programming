# Computer Networks Assignment 1 Socket Programming

## Structure

* The directory 'Non-Persistent' contains basic Non-Persistent codes for Server and Client.
* The directory 'Persistent' contains Persistent codes for Server and Client.

## How to run

The following are the steps to run the code (applicable for both Q1 and Q2)
1. Open a terminal and go to directory `Server`
2. Compile the server with the following command `g++ server.cpp -o server`
3. Run the server `./server`
4. Enter directory name as `Data`
5. Open a terminal and go to directory `Client`
6. Compile the client with the following command `g++ client.cpp -o client`
7. Run the client `./client`

## Comparision between Persistent and Non Persistent connections

The following test case on both persistent and non persistent codes
`
download t1 t2 t3 t4
e
`

Time taken by persistent connection
`
real    0m0.167s
user    0m0.004s
sys     0m0.000s
`

Time taken by non-persistent connection
`
real    0m0.044s
user    0m0.004s
sys     0m0.004s
`
