//
//  sucms_delete.cpp
//  Project1
//
//  Created by Alex Solorio on 4/29/18.
//  Copyright © 2018 Alex Solorio. All rights reserved.
//

#include "SUCMS.h"
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <netdb.h>

std::string get_username() {
  std::string username;
  std::cout << "Enter username: ";
  std::getline(std::cin, username);
  return username;
}

std::string get_password() {
  std::string password;
  std::cout << "Enter password: ";
  std::getline(std::cin, password);
  return password;
}

std::string get_filename() {
  std::string filename;
  std::cout << "Enter filename to delete: ";
  std::getline(std::cin, filename);
  return filename;
}

/**
 * UDP client for sending/receiving "delete" commands from server
 *
 * @param argc count of arguments on the command line
 * @param argv array of command line arguments
 * @return 0 on success, non-zero if an error occurred
 */

int main(int argc, char *argv[]) {
  
  char *ip_string;
  char *port_string;
  
  // Port to send UDP data to. Need to convert from command line string to a number
  unsigned int port;
  
  // The socket used to send UDP data on
  int udp_socket;
  
  // Variable used to check return codes from various functions
  int ret;
  
  // Length of the client address of client
  socklen_t client_address_length = sizeof(struct sockaddr_in);
  
  // Invalid number of args
  if (argc < 3) {
    std::cerr << "Please specify IP PORT as first two arguments." << std::endl;
    return 1;
  }
  
  ip_string = argv[1];
  port_string = argv[2];
  
  // Create the UDP socket.
  // AF_INET is the address family used for IPv4 addresses
  // SOCK_DGRAM indicates creation of a UDP socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  
  // Make sure socket was created successfully
  if (udp_socket == -1) {
    std::cerr << "Failed to create udp socket!" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    return 1;
  }
  
  struct addrinfo hints;
  struct addrinfo *results;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_addr = NULL;
  hints.ai_canonname = NULL;
  //restricts us to getting back results that are IPv4
  hints.ai_family = AF_INET;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_DGRAM;
  
  ret = getaddrinfo(ip_string, port_string, &hints, &results);
  
  // Check whether the specified IP was parsed properly. If not, exit.
  if (ret == -1) {
    std::cerr << "Failed to getaddrinfo!" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    close(udp_socket);
    return 1;
  }
  
  struct sockaddr* dest_addr = results->ai_addr;
  
  
  
  
  
  
  //////////////////////////////////////////////////////
  //////////////// Send connect message ////////////////
  //////////////////////////////////////////////////////
  
  char sendBuffer[2048];
  std::string username = get_username();
  std::string password = get_password();
  std::string filename = get_filename();
  
  //Create SUCMSHeader
  SUCMSHeader connectHeader;
  connectHeader.sucms_msg_type = htons(MSG_COMMAND);
  connectHeader.sucms_msg_length = htons(sizeof(CommandMessage) + username.length() + sizeof(SUCMSClientFileRWRequest) + filename.length());
  
  //Create CommandMessage
  CommandMessage cmdMsg;
  cmdMsg.command = htons(COMMAND_DELETE);
  cmdMsg.username_len = htons(username.length());
  MD5((unsigned char*)password.c_str(), password.length(), cmdMsg.password_hash);
  
  //Create SUCMSClientFileRWRequest
  SUCMSClientFileRWRequest rw;
  rw.filename_length = htons(filename.length());
  
  const char* usernameCString = username.c_str();
  const char* filenameCString = filename.c_str();
  
  //Fill in sendBuffer
  memcpy(sendBuffer, &connectHeader, sizeof(connectHeader));
  memcpy(&sendBuffer[sizeof(connectHeader)], &cmdMsg, sizeof(cmdMsg));
  memcpy(&sendBuffer[sizeof(connectHeader) + sizeof(cmdMsg)], usernameCString, username.length());
  memcpy(&sendBuffer[sizeof(connectHeader) + sizeof(cmdMsg) + username.length()], &rw, sizeof(rw));
  memcpy(&sendBuffer[sizeof(connectHeader) + sizeof(cmdMsg) + username.length() + sizeof(rw)], filenameCString, filename.length());
  
  int bytesToSend = sizeof(connectHeader) + sizeof(cmdMsg) + username.length() + sizeof(rw) + filename.length();
  
  // Send the data to the destination.
  ret = sendto(udp_socket, &sendBuffer, bytesToSend, 0,
               dest_addr, client_address_length);
  
  if (ret == -1) {
    std::cerr << "Failed to send data!" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    close(udp_socket);
    return 1;
  }
  
  
  
  
  
  
  //////////////////////////////////////////////////////
  //////////////// Receive from server /////////////////
  //////////////////////////////////////////////////////
  
  // Buffer to hold data received from network
  unsigned char recv_buf[2048];
  
  // Receive data using recvfrom. client_address will hold the client
  // address that sent the data.
  ret = recvfrom(udp_socket, recv_buf, 2047, 0, dest_addr, &client_address_length);
  
  if (ret == -1) {
    std::cerr << "Failed to recv!" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    close(udp_socket);
    return 1;
  }
  
  SUCMSHeader sHeader;
  memcpy(&sHeader, &recv_buf[0], sizeof(sHeader));
  //Convert to Host Byte Order
  sHeader.sucms_msg_length = ntohs(sHeader.sucms_msg_length);
  sHeader.sucms_msg_type = ntohs(sHeader.sucms_msg_type);
  
  CommandResponse sResponse;
  memcpy(&sResponse, &recv_buf[sizeof(sHeader)], sizeof(sResponse));
  //Convert to Host Byte Order
  sResponse.command_response_code = ntohs(sResponse.command_response_code);
  sResponse.result_id = ntohs(sResponse.result_id);
  sResponse.message_data_size = ntohl(sResponse.message_data_size);
  sResponse.message_count = ntohs(sResponse.message_count);
  
  //Test if server sent us any errors
  if (sResponse.command_response_code == AUTH_FAILED){
    std::cout << "Received AUTH_FAILED from server.\n";
    return 1;
  }
  if (sResponse.command_response_code == ACCESS_DENIED){
    std::cout << "Received ACCESS_DENIED from server.\n";
    return 1;
  }
  if (sResponse.command_response_code == NO_SUCH_FILE){
    std::cout << "Received NO_SUCH_FILE from server.\n";
    return 1;
  }
  if (sResponse.command_response_code == INVALID_RESULT_ID){
    std::cout << "Received INVALID_RESULT_ID from server.\n";
    return 1;
  }
  if (sResponse.command_response_code == INTERNAL_SERVER_ERROR){
    std::cout << "Received INTERNAL_SERVER_ERROR from server.\n";
    return 1;
  }
  if (sResponse.command_response_code == INVALID_CLIENT_MESSAGE){
    std::cout << "Received INVALID_CLIENT_MESSAGE from server.\n";
    return 1;
  }
  
  close(udp_socket);
  return 0;
}

