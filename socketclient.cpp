#include <iostream>
#include <winsock2.h>

using namespace std;

int main(){
    //Initialize WSA variables
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);

    // Check for initialization success
    if (wserr != 0){
        cout << "The winsock dll not found" << endl;
        return 0;
    }
    else{
        cout << "The Winsock dll found" <<endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    //Create a socket 
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if(clientSocket == INVALID_SOCKET){
        cout << "Error at socket(): " << WSAGetLastError << endl;
        WSACleanup();
        return 0;
    }
    else{
        cout << "Socket is OK!" << endl;
    }

    // Connect to the server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1"); //Replace with the server's IP address
    clientService.sin_port = htons(55555);

    //Use the connect function
    if (connect(clientSocket, (SOCKADDR*)&clientService,sizeof(clientService)) == SOCKET_ERROR){
        cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else{
        cout << "Client: Connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }

    //Sending data to the server
    char buffer[200];
    cout << "Enter the message: ";
    cin.getline(buffer,200);
    int sbyteCount = send(clientSocket, buffer, 200, 0);
    if(sbyteCount == SOCKET_ERROR){
        cout << "Client send error: " << WSAGetLastError() << endl;
        return -1;
    }
    else{
        cout << "Client: Sent" << sbyteCount << "bytes" << endl;
    }

    //Receiving data from the server
    char receiveBuffer[200];
    int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0){
        cout << "Client recv error: " << WSAGetLastError() << endl;
        return 0;
    }
    else{
        cout << "Client: Received data:" << receiveBuffer << endl;
    }

    
}