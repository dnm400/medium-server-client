#include <iostream>
#include <winsock2.h>

using namespace std;
int main(){
    //Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    //Check for initialization success
    if (wsaerr != 0){
        cout << "The Winsock dll not found!" << endl;
        return 0;
    }
    else {
        cout << "The Winsock dll found" << endl;
        cout << "The status: " << wsaData.szSystemStatus <<endl;
    }

    //Create a socket 
    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Check for socket creation success
    if (serverSocket == INVALID_SOCKET){
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }    
    else {
        cout << "Socket is OK!" << endl;
    }

    // Bind the socket to an IP address and port number
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1"); //Replace with your desired Ip address
    service.sin_port = htons(55555); //choose a port number

    //Use the bind function
    if(bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR){
        cout <<  "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is OK!" << endl;
    }

    //Listen for incoming connections
    if(listen(serverSocket, 1) == SOCKET_ERROR){
        cout << "listen(): Error listening on socket: " << WSAGetLastError() << endl;
    }
    else{
        cout << "listen() is OK! I'm waiting for new connections..." << endl;
    }

    //Accept incoming connections
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    //Check for successful connection
    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket); // Close the server socket before cleaning up
        WSACleanup();
        return -1;
    }

    else {
        cout << "accept() is OK!" << endl;
    }

    // Receive data from the client
    char receiveBuffer[200]= {0};
    int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        cout << "Server recv error: " << WSAGetLastError() << endl;
        closesocket(acceptSocket); // Close the accept socket before cleaning up
        closesocket(serverSocket); // Close the server socket before cleaning up
        WSACleanup();
        return 0;
    }
    else{
        cout << "Received data: " << receiveBuffer << endl;
    }

    //Send a response to the client
    char buffer[200];
    cout << "Enter the message: ";
    cin.getline(buffer, 200);
    int sbyteCount = send(acceptSocket, buffer, 200, 0);
    if(sbyteCount == SOCKET_ERROR){
        cout << "Server send error: " << WSAGetLastError() << endl;
        closesocket(acceptSocket); // Close the accept socket before cleaning up
        closesocket(serverSocket); // Close the server socket before cleaning up
        WSACleanup();
        return -1;
    }
    else {
        cout << "Server: Sent" << sbyteCount << "bytes" << endl;
    }

    // Clean up
    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();

    	
    return 0;
}
