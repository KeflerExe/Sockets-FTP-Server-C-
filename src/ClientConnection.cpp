//****************************************************************************
//                         REDES Y SISTEMAS DISTRIBUIDOS
//                      
//                     2º de grado de Ingeniería Informática
//                       
//              This class processes an FTP transactions.
// 
//****************************************************************************



#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cerrno>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h> 
#include <iostream>
#include <dirent.h>

#include "common.h"

#include "ClientConnection.h"



ClientConnection::ClientConnection(int s) {
    int sock = (int)(s);
  
    char buffer[MAX_BUFF];

    control_socket = s;
    // Check the Linux man pages to know what fdopen does.
    fd = fdopen(s, "a+");
    if (fd == NULL){
	std::cout << "Connection closed" << std::endl;

	fclose(fd);
	close(control_socket);
	ok = false;
	return ;
    }
    
    ok = true;
    data_socket = -1;
    parar = false;
   
  
  
};


ClientConnection::~ClientConnection() {
 	fclose(fd);
	close(control_socket); 
  
}


int connect_TCP( uint32_t address,  uint16_t  port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(address);
    serv_addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        close(sock);
        return -1;
    }
    
    return sock;
}






void ClientConnection::stop() {
    close(data_socket);
    close(control_socket);
    parar = true;
  
}





    
#define COMMAND(cmd) strcmp(command, cmd)==0

// This method processes the requests.
// Here you should implement the actions related to the FTP commands.
// See the example for the USER command.
// If you think that you have to add other commands feel free to do so. You 
// are allowed to add auxiliary methods if necessary.

void ClientConnection::WaitForRequests() {
    if (!ok) {
	 return;
    }
    
    fprintf(fd, "220 Service ready\n");
  
    while(!parar) {

      fscanf(fd, "%s", command);
      if (COMMAND("USER")) {
	    fscanf(fd, "%s", arg);
	    fprintf(fd, "331 User name ok, need password\n");
      }
      else if (COMMAND("PWD")) {
	   
      }
      else if (COMMAND("PASS")) {
        fscanf(fd, "%s", arg);
        if(strcmp(arg,"1234") == 0){
            fprintf(fd, "230 User logged in\n");
        }
        else{
            fprintf(fd, "530 Not logged in.\n");
            parar = true;
        }
	   
      }
      else if (COMMAND("PORT")) {
        int ip1, ip2, ip3, ip4, port1, port2;
        fscanf(fd, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
        uint32_t ip_address = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
        uint16_t data_port = (port1 << 8) | port2;
        data_socket = connect_TCP(ip_address, data_port);
        fprintf(fd, "200 Connection established.\n");
      }
      else if (COMMAND("PASV")) {
        int data_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in data_sockaddr;
        memset(&data_sockaddr, 0, sizeof(data_sockaddr));
        data_sockaddr.sin_family = AF_INET;
        data_sockaddr.sin_addr.s_addr = INADDR_ANY;
        data_sockaddr.sin_port = htons(0); // Let the system choose a free port
        if (bind(data_socket, (struct sockaddr*)&data_sockaddr, sizeof(data_sockaddr)) < 0) {
            perror("Error binding data socket");
            return;
        }
        if (listen(data_socket, 1) < 0) {
            perror("Error listening on data socket");
            return;
        }
        // Get the port number chosen by the system
        struct sockaddr_in data_sockaddr_res;
        socklen_t len = sizeof(data_sockaddr_res);
        if (getsockname(data_socket, (struct sockaddr*)&data_sockaddr_res, &len) < 0) {
            perror("Error getting data socket name");
            return;
        }
        uint16_t port = ntohs(data_sockaddr_res.sin_port);
        // Send the address and port to the client
        char addr_str[32];
        inet_ntop(AF_INET, &data_sockaddr_res.sin_addr, addr_str, sizeof(addr_str));
        char* tok = strtok(addr_str, ".");
        int ip1 = atoi(tok);
        tok = strtok(NULL, ".");
        int ip2 = atoi(tok);
        tok = strtok(NULL, ".");
        int ip3 = atoi(tok);
        tok = strtok(NULL, ".");
        int ip4 = atoi(tok);
        fprintf(fd, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\n", ip1, ip2, ip3, ip4, port / 256, port % 256);
        // Store the data socket for future use
        data_socket = accept(data_socket, NULL, NULL);
        if (data_socket < 0) {
            perror("Error accepting data connection");
            return;
        }
      }
      else if (COMMAND("STOR") ) {
        fscanf(fd, "%s", arg);
        fprintf(fd, "150 File status okay; about to open data connection.\n");
        fflush(fd);

        // Establish data connection with the client
        struct sockaddr_in data_addr;
        socklen_t data_addr_len = sizeof(data_addr);

        int data_socket = accept(data_socket, (struct sockaddr*)&data_addr, &data_addr_len);

        if (data_socket < 0) {
            fprintf(fd, "425 Can't open data connection.\n");
            fflush(fd);
            return;
        }

        // Open file to store data received from client
        FILE* file = fopen(arg, "wb");

        if (!file) {
            fprintf(fd, "450 Requested file action not taken.\n");
            fflush(fd);
            return;
        }

        // Read data from client and write it to file
        char buffer[MAX_BUFF];
        int bytes_read = 0;

        while ((bytes_read = read(data_socket, buffer, MAX_BUFF)) > 0) {
            fwrite(buffer, sizeof(char), bytes_read, file);
        }

        // Close file and data connection
        fclose(file);
        close(data_socket);

        fprintf(fd, "226 Closing data connection. Requested file action successful.\n");
        fflush(fd);
      }
      else if (COMMAND("RETR")) {
        fscanf(fd, "%s", arg);
        FILE* file = fopen(arg, "r");
        if (file == NULL) {
            fprintf(fd, "550 File not found.\n");
        } else {
            fprintf(fd, "150 Opening data connection.\n");

            // Open the data connection to send the file
            struct sockaddr_in data_addr;
            socklen_t data_addr_len = sizeof(data_addr);
            if (getsockname(data_socket, (struct sockaddr*) &data_addr, &data_addr_len) < 0) {
                perror("Error getting socket name");
                fclose(file);
                return;
            }

            // Send file
            char buffer[MAX_BUFF];
            int num_bytes_read;
            while ((num_bytes_read = fread(buffer, 1, MAX_BUFF, file)) > 0) {
                if (send(data_socket, buffer, num_bytes_read, 0) < 0) {
                    perror("Error sending file");
                    fclose(file);
                    return;
                }
            }

            fclose(file);

            fprintf(fd, "226 File transfer successful.\n");
            close(data_socket);
        }
      }
      else if (COMMAND("LIST")) {
        char data[MAX_BUFF];
        int len;
        std::string file_list = "";
        DIR *dirp;
        struct dirent *dp;
        dirp = opendir(".");
        if (dirp == NULL) {
            fprintf(fd, "550 Failed to open directory.\n");
            continue;
        }
        while ((dp = readdir(dirp)) != NULL) {
            file_list += std::string(dp->d_name) + "\n";
        }
        closedir(dirp);
        fprintf(fd, "150 Here comes the directory listing.\n");
        data_socket = connect_TCP(data_socket, data_port);
        if (data_socket < 0) {
            fprintf(fd, "425 Can't open data connection.\n");
            continue;
        }
        len = file_list.length();
        strncpy(data, file_list.c_str(), sizeof(data));
        data[sizeof(data) - 1] = 0;
        send(data_socket, data, len, 0);
        close(data_socket);
        fprintf(fd, "226 Directory send OK.\n");
      }
      else if (COMMAND("SYST")) {
           fprintf(fd, "215 UNIX Type: L8.\n");   
      }

      else if (COMMAND("TYPE")) {
	  fscanf(fd, "%s", arg);
	  fprintf(fd, "200 OK\n");   
      }
     
      else if (COMMAND("QUIT")) {
        fprintf(fd, "221 Service closing control connection. Logged out if appropriate.\n");
        close(data_socket);	
        parar=true;
        break;
      }
  
      else  {
	    fprintf(fd, "502 Command not implemented.\n"); fflush(fd);
	    printf("Comando : %s %s\n", command, arg);
	    printf("Error interno del servidor\n");
	
      }
      
    }
    
    fclose(fd);

    
    return;
  
};
