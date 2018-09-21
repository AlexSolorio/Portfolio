#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <sys/types.h>
#include <netdb.h>
#include "TCPClient.h"
#include "P2P.h"
#include <ctime>
#include <cstdlib>
#include "openssl/sha.h"
//#include <endian.h>
//uncomment if on MacOS instead of Linux
#include "endian.h"

struct hash {
  unsigned char msg_hash[32]; // SHA-256 Hash
};

std::string get_nickname() {
  std::string nickname;
  std::cout << "Enter chat nickname: ";
  std::getline(std::cin, nickname);
  return nickname;
}

std::string get_message() {
  std::string message;
  std::getline(std::cin, message);
  return message;
}

bool addToSeenHashes(unsigned char *msg_hash, std::vector<hash> alreadySeenMsgHashes){
  
  //loop through each hash in alreadySeemMsgHashes
  for (int i = 0; i < alreadySeenMsgHashes.size(); i++){
    
    //loop through all bytes in this hash
    bool isSameMsgHash = true;
    for (int j = 0; j < 32; j++){
      if (msg_hash[j] != alreadySeenMsgHashes[i].msg_hash[j]){
        isSameMsgHash = false;
      }
    }
    
    //if each byte was the same, we have already seen this message hash in the past.
    //Do not add it to vector, and return false to let calling fct know not to handle this message.
    if (isSameMsgHash){
      return false;
    }
  }
  
  //if it makes it to here, then we make a new hash struct for it and
  //add it to the alreadySeenMsgHashes vector
  struct hash newHash;
  for (int i = 0; i < 32; i++){
    newHash.msg_hash[i] = msg_hash[i];
  }
  alreadySeenMsgHashes.push_back(newHash);
  return true;
}

/********** P2P HEADER GETTER **********/

P2PHeader getP2PHeaderFromBytes(char* scratch_buf) {
  struct P2PHeader header;
  memcpy(&header, scratch_buf, sizeof(struct P2PHeader));
  header.type = ntohs(header.type);
  header.length = ntohs(header.length);
  return header;
}

/********** CONTROL MESSAGE GETTERS **********/

ControlMessage getControlMessageFromBytes(char* scratch_buf) {
  struct ControlMessage cm;
  cm.header = getP2PHeaderFromBytes(scratch_buf);
  memcpy(&cm.control_type, &scratch_buf[sizeof(struct P2PHeader)], sizeof(uint16_t));
  cm.control_type = ntohs(cm.control_type);
  return cm;
}

ConnectMessage getConnectMessageFromBytes(char* scratch_buf) {
  struct ConnectMessage cm;
  cm.control_header = getControlMessageFromBytes(scratch_buf);
  memcpy(&cm.peer_data, &scratch_buf[sizeof(struct ControlMessage)], sizeof(struct PeerInfo));
  cm.peer_data.peer_listen_port = ntohs(cm.peer_data.peer_listen_port);
  return cm;
}

FindPeersMessage getFindPeersMessageFromBytes(char* scratch_buf) {
  struct FindPeersMessage fpm;
  fpm.control_header = getControlMessageFromBytes(scratch_buf);
  memcpy(&fpm.max_results, &scratch_buf[sizeof(struct ControlMessage)], sizeof(uint16_t));
  memcpy(&fpm.restrict_results, &scratch_buf[sizeof(struct ControlMessage) + sizeof(uint16_t)], sizeof(uint16_t));
  fpm.max_results = ntohs(fpm.max_results);
  fpm.restrict_results = ntohs(fpm.restrict_results);
  return fpm;
}

GossipPeersMessage getGossipPeersMessageFromBytes(char* scratch_buf) {
  struct GossipPeersMessage gpm;
  gpm.control_header = getControlMessageFromBytes(scratch_buf);
  memcpy(&gpm.num_results, &scratch_buf[sizeof(struct ControlMessage)], sizeof(uint16_t));
  gpm.num_results = ntohs(gpm.num_results);
  return gpm;
}

/********** DATA MESSAGE GETTERS **********/

DataMessage getDataMessageFromBytes(char* scratch_buf) {
  struct DataMessage dm;
  dm.header = getP2PHeaderFromBytes(scratch_buf);
  memcpy(&dm.data_type, &scratch_buf[sizeof(struct P2PHeader)], sizeof(uint16_t));
  dm.data_type = ntohs(dm.data_type);
  return dm;
}

SendMessage getSendMessageFromBytes(char* scratch_buf) {
  struct SendMessage sm;
  sm.data_header = getDataMessageFromBytes(scratch_buf);
  memcpy(&sm.message, &scratch_buf[sizeof(struct DataMessage)], sizeof(struct Message));
  sm.message.sender.peer_listen_port = ntohs(sm.message.sender.peer_listen_port);
  sm.message.nickname_length = ntohs(sm.message.nickname_length);
  sm.message.message_length = ntohs(sm.message.message_length);
  sm.message.send_time = be64toh(sm.message.send_time);
  return sm;
}

ForwardMessage getForwardMessageFromBytes(char* scratch_buf) {
  struct ForwardMessage fm;
  fm.data_header = getDataMessageFromBytes(scratch_buf);
  memcpy(&fm.message, &scratch_buf[sizeof(struct DataMessage)], sizeof(struct Message));
  fm.message.sender.peer_listen_port = ntohs(fm.message.sender.peer_listen_port);
  fm.message.nickname_length = ntohs(fm.message.nickname_length);
  fm.message.message_length = ntohs(fm.message.message_length);
  fm.message.send_time = be64toh(fm.message.send_time);
  return fm;
}

GetHistory getGetHistoryFromBytes(char* scratch_buf) {
  struct GetHistory gh;
  gh.data_header = getDataMessageFromBytes(scratch_buf);
  memcpy(&gh.request_id, &scratch_buf[sizeof(struct DataMessage)], sizeof(uint16_t));
  memcpy(&gh.since_time, &scratch_buf[sizeof(struct DataMessage) + sizeof(uint16_t)], sizeof(uint64_t));
  gh.request_id = ntohs(gh.request_id);
  return gh;
}

SendHistory getSendHistoryFromBytes(char* scratch_buf) {
  struct SendHistory sh;
  sh.data_header = getDataMessageFromBytes(scratch_buf);
  memcpy(&sh.request_id, &scratch_buf[sizeof(struct DataMessage)], sizeof(uint16_t));
  memcpy(&sh.num_responses, &scratch_buf[sizeof(struct DataMessage) + sizeof(uint16_t)], sizeof(uint16_t));
  sh.request_id = ntohs(sh.request_id);
  sh.num_responses = ntohs(sh.num_responses);
  return sh;
}

/********** ERROR MESSAGE GETTERS **********/

ErrorMessage getErrorMessageFromBytes(char* scratch_buf) {
  struct ErrorMessage em;
  em.header = getP2PHeaderFromBytes(scratch_buf);
  memcpy(&em.error_type, &scratch_buf[sizeof(struct P2PHeader)], sizeof(uint16_t));
  em.error_type = ntohs(em.error_type);
  return em;
}

int main(int argc, char *argv[]) {
  struct sockaddr_storage incoming_client;
  socklen_t incoming_client_len;
  std::vector<TCPClient *> client_list;
  TCPClient *temp_client;
  char recv_buf[DEFAULT_BUFFER_SIZE];
  char send_buf[DEFAULT_BUFFER_SIZE];
  char scratch_buf[DEFAULT_BUFFER_SIZE];
  struct timeval timeout;
  
  struct addrinfo hints;
  struct addrinfo *results;
  struct addrinfo *results_it;
  struct addrinfo serverAddrInfo;
  struct addrinfo seedAddrInfo;
  
  char *listen_hostname = NULL;
  char *listen_port = NULL;
  int server_listen_port_int;
  char *seed_hostname = NULL;
  char *seed_port = NULL;
  
  int server_socket;
  int temp_fd;
  bool listenOnlyServer = false;
  
  int ret;
  bool stop = false;
  
  fd_set read_set;
  fd_set write_set;
  int max_fd;
  
  std::vector<hash> alreadySeenMsgHashes;
  
  if ((argc != 5) && (argc != 3)) {
    std::cerr << "Specify SERVER_HOST SERVER_PORT SEED_HOST SEED_PORT as four args, or just SERVER_HOST SERVER_PORT for listen-only peer." << std::endl;
    return 1;
  }
  
  if (argc == 3){
    listenOnlyServer = true;
  }
  
  //get command line args
  listen_hostname = argv[1];
  listen_port = argv[2];
  server_listen_port_int = atoi(argv[2]);
  if (!listenOnlyServer){
    seed_hostname = argv[3];
    seed_port = argv[4];
  }
  
  // Create the TCP socket.
  // AF_INET is the address family used for IPv4 addresses
  // SOCK_STREAM indicates creation of a TCP socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  
  // Make sure socket was created successfully, or exit.
  if (server_socket == -1) {
    std::cerr << "Failed to create tcp socket!" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    return 1;
  }
  
  //Create addrinfo struct for server
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_addr = NULL;
  hints.ai_canonname = NULL;
  hints.ai_family = AF_INET;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  ret = getaddrinfo(listen_hostname, listen_port, &hints, &results);
  if (ret != 0) {
    std::cerr << "Getaddrinfo failed with error " << ret << std::endl;
    perror("getaddrinfo");
    return 1;
  }
  results_it = results;
  ret = -1;
  
  while (results_it != NULL) {
    ret = bind(server_socket, results_it->ai_addr,
               results_it->ai_addrlen);
    if (ret == 0) {   //this is the address we actually used
      
      //save the server address info for future code
      memcpy(&serverAddrInfo, results_it, sizeof(struct addrinfo));
      break;
    }
    perror("bind");
    results_it = results_it->ai_next;
  }
  
  if (ret != 0) {
    std::cerr << "Failed to bind to any addresses. Be sure to specify a local address/hostname, and an unused port?"
    << std::endl;
    return 1;
  }
  
  in_addr_t serverAddr = ((struct sockaddr_in *)serverAddrInfo.ai_addr)->sin_addr.s_addr;
  
  // Always free the result of calling getaddrinfo
  freeaddrinfo(results);
  
  if (!listenOnlyServer){
    //Connect to seed peer
    ret = getaddrinfo(seed_hostname, seed_port, &hints, &results);
    if (ret != 0) {
      std::cerr << "Getaddrinfo failed with error " << ret << std::endl;
      perror("getaddrinfo");
      return 1;
    }
    results_it = results;
    ret = -1;
    
    while (results_it != NULL) {
      int seed_socket = socket(AF_INET, SOCK_STREAM, 0);
      ret = connect(seed_socket, results_it->ai_addr,
                    results_it->ai_addrlen);
      
      if (ret == 0) {   //successful connection
        
        //save the seed address info for future code
        memcpy(&seedAddrInfo, results_it, sizeof(struct addrinfo));
        
        // Create a new TCPClient from the connection
        temp_client = new TCPClient(seed_socket, (sockaddr_storage *)results_it->ai_addr, results_it->ai_addrlen);
        
        // Add the new client to the list of clients we have
        client_list.push_back(temp_client);
        
        /************** SEND CONNECT MESSAGE **************/
        
        // Structure to fill in with connect message info
        struct ConnectMessage connect_message;
        //zero and fill out the struct
        memset(&connect_message, 0, sizeof(struct ConnectMessage));
        connect_message.control_header.control_type = htons(CONNECT);
        connect_message.control_header.header.type = htons(CONTROL_MSG);
        connect_message.control_header.header.length = htons(sizeof(struct ConnectMessage));
        memcpy(&connect_message.peer_data.ipv4_address, &((struct sockaddr_in *)serverAddrInfo.ai_addr)->sin_addr.s_addr, sizeof(struct in_addr));
        connect_message.peer_data.peer_listen_port = htons(server_listen_port_int);
        
        // Create hash after filling in rest of message.
        unsigned char hashCharArray[sizeof(struct ConnectMessage)];
        memcpy(hashCharArray, &connect_message, sizeof(connect_message));
        size_t shaSize = sizeof(struct ConnectMessage) - sizeof(struct P2PHeader);
        SHA256(&hashCharArray[sizeof(struct P2PHeader)], shaSize, connect_message.control_header.header.msg_hash);
        
        //send it
        if (temp_client->add_send_data((char *) &connect_message, sizeof(struct ConnectMessage)) != true) {
          std::cerr << "Failed to add send data to client!" << std::endl;
        }
        break;
      }
      perror("connect");
      results_it = results_it->ai_next;
    }
    
    // Always free the result of calling getaddrinfo
    freeaddrinfo(results);
  }
  
  // Listen on the server socket with a max of 50 outstanding connections.
  ret = listen(server_socket, 50);
  if (ret != 0) {
    perror("listen");
    close(server_socket);
    return 1;
  }
  
  max_fd = 0;
  std::string nickname = get_nickname();
  
  //store the last time that we send out a FindPeer request
  const int SECONDS_BETWEEN_FP_REQUESTS = 6;
  time_t lastFPRequestTime;
  time(&lastFPRequestTime);
  
  
  /********** MAIN SELECT LOOP **********/
  while (stop == false) {
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    
    // Mark the server_socket in the read set
    // If this is then set, it means we need to accept a new connection.
    FD_SET(server_socket, &read_set);
    
    //Mark the stdin in the read set
    FD_SET(0, &read_set);
    
    if (server_socket > max_fd)
      max_fd = server_socket + 1;
    
    // For each client, set the appropriate descriptors in the select sets
    for (int i = 0; i < client_list.size(); i++) {
      
      if (client_list[i] == NULL) {
        continue;
      }
      
      // Check if the client has sent us data
      FD_SET(client_list[i]->get_fd(), &read_set);
      
      // Check if client has data to send. If so, add it to the write_set
      if (client_list[i]->bytes_ready_to_send() > 0) {
        FD_SET(client_list[i]->get_fd(), &write_set);
      }
      
      // Update max_fd if needed
      if (client_list[i]->get_fd() > max_fd)
        max_fd = client_list[i]->get_fd() + 1;
    }
    
    // If select hasn't returned after 5 seconds, return anyways so other events can be triggered
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
    ret = select(max_fd + 1, &read_set, &write_set, NULL, &timeout);
    
    if (ret == -1) {
      perror("select");
      continue;
    }
    
    // Check if server_socket is in the read set. If so, a new client has connected to us!
    if (FD_ISSET(server_socket, &read_set)) {
      temp_fd = accept(server_socket, (struct sockaddr *) &incoming_client, &incoming_client_len);
      if (temp_fd == -1) {
        perror("accept");
        continue;
      }
      // Create a new TCPClient from the connection
      temp_client = new TCPClient(temp_fd, &incoming_client, incoming_client_len);
      // Add the new client to the list of clients we have
      client_list.push_back(temp_client);
    }
    
    //Check if the server user entered anything on stdin
    if (FD_ISSET(0, &read_set)){
      
      std::string message = get_message();
      
      //loop through all clients and send them the message
      struct SendMessage sendMsg;
      //zero and fill out the struct
      memset(&sendMsg, 0, sizeof(struct SendMessage));
      sendMsg.data_header.data_type = htons(SEND_MESSAGE);
      sendMsg.data_header.header.type = htons(DATA_MSG);
      sendMsg.data_header.header.length = htons(sizeof(struct SendMessage) + nickname.length() + message.length());
      time_t currentTime;
      time(&currentTime);
      sendMsg.message.send_time = currentTime;
      sendMsg.message.send_time = htobe64(sendMsg.message.send_time);
      sendMsg.message.message_length = htons(message.length());
      sendMsg.message.nickname_length = htons(nickname.length());
      sendMsg.message.sender.ipv4_address = serverAddr;
      sendMsg.message.sender.peer_listen_port = htons(server_socket);
      
      //copy sendMsg, nickname, and message into a new buffer for computing the hash
      unsigned char hash_buf[sizeof(struct SendMessage) + nickname.length() + message.length()];
      memcpy(hash_buf, &sendMsg, sizeof(sendMsg));
      memcpy(&hash_buf[sizeof(sendMsg)], nickname.c_str(), nickname.length());
      memcpy(&hash_buf[sizeof(sendMsg) + nickname.length()], message.c_str(), message.length());
      size_t shaSize = sizeof(struct SendMessage) + nickname.length() + message.length() - sizeof(struct P2PHeader);
      SHA256(&hash_buf[sizeof(struct P2PHeader)], shaSize, sendMsg.data_header.header.msg_hash);
      
      //copy it all back into the buffer now that the hash is computed correctly
      memcpy(scratch_buf, &sendMsg, sizeof(sendMsg));
      memcpy(&scratch_buf[sizeof(sendMsg)], nickname.c_str(), nickname.length());
      memcpy(&scratch_buf[sizeof(sendMsg) + nickname.length()], message.c_str(), message.length());
      unsigned int bytes_to_send = sizeof(sendMsg) + nickname.length() + message.length();
      
      //add the buffer data into each client's send data
      for (int i = 0; i < client_list.size(); i++){
        client_list[i]->add_send_data(scratch_buf, bytes_to_send);
      }
    }
    
    //if it has been more than SECONDS_BETWEEN_FP_REQUESTS seconds
    //since last FindPeers request, send another one to a random peer
    time_t currentTime;
    time(&currentTime);
    if (difftime(currentTime, lastFPRequestTime) > SECONDS_BETWEEN_FP_REQUESTS){
      
      //send CONNECT_OK back to client
      struct FindPeersMessage outFindPeersMessage;
      //zero and fill out the struct
      memset(&outFindPeersMessage, 0, sizeof(struct FindPeersMessage));
      outFindPeersMessage.control_header.control_type = htons(FIND_PEERS);
      outFindPeersMessage.control_header.header.type = htons(CONTROL_MSG);
      outFindPeersMessage.control_header.header.length = htons(sizeof(struct FindPeersMessage));
      outFindPeersMessage.max_results = htons(1);
      outFindPeersMessage.restrict_results = htons(AF_INET);
      
      //copy struct into a new buffer for computing the hash
      unsigned char hash_buf[sizeof(struct FindPeersMessage)];
      memcpy(hash_buf, &outFindPeersMessage, sizeof(struct FindPeersMessage));
      size_t shaSize = sizeof(struct FindPeersMessage) - sizeof(struct P2PHeader);
      SHA256(&hash_buf[sizeof(struct P2PHeader)], shaSize, outFindPeersMessage.control_header.header.msg_hash);
      
      //copy struct into the scratch buffer now that the hash is stored
      memcpy(scratch_buf, &outFindPeersMessage, sizeof(struct FindPeersMessage));
      
      int indexOfPeerToRequest = rand() % client_list.size();
      
      //send it
      if (client_list[indexOfPeerToRequest]->add_send_data(scratch_buf, sizeof(struct FindPeersMessage)) != true) {
        std::cerr << "Failed to add send data to client!" << std::endl;
      }
      
      //reset lastFPRequestTime
      time(&lastFPRequestTime);
    }
    
    for (int i = 0; i < client_list.size(); i++) {
      
      if (client_list[i] == NULL) {
        continue;
      }
      
      // Check if this client has sent us data (read_set)
      if (FD_ISSET(client_list[i]->get_fd(), &read_set)) {
        ret = recv(client_list[i]->get_fd(), recv_buf, DEFAULT_BUFFER_SIZE, 0);
        if (ret == -1) {
          perror("recv");
          // On error, something bad has happened to this client. Remove.
          close(client_list[i]->get_fd());
          client_list.erase(client_list.begin() + i);
          break;
        } else if (ret == 0) {
          // On 0 return, client has initiated connection shutdown.
          close(client_list[i]->get_fd());
          client_list.erase(client_list.begin() + i);
          break;
        } else {
          // Add the newly received data to the client buffer
          client_list[i]->add_recv_data(recv_buf, ret);
        }
      }
      
      // Check if we have data to send to this client (write_set)
      if ((client_list[i]->bytes_ready_to_send() > 0) && (FD_ISSET(client_list[i]->get_fd(), &write_set))) {
        // Store how many bytes we are ready to send to the client
        int bytes_to_send = client_list[i]->bytes_ready_to_send();
        // Copy send bytes into our local send buffer
        client_list[i]->get_send_data(send_buf, DEFAULT_BUFFER_SIZE);
        // Finally, send the data to the client.
        ret = send(client_list[i]->get_fd(), send_buf, bytes_to_send, 0);
        if (ret == -1) {
          perror("send");
          // On error, something bad has happened to this client. Remove.
          close(client_list[i]->get_fd());
          client_list.erase(client_list.begin() + i);
          break;
        }
      }
      
      // Process any incoming client data
      if (client_list[i]->bytes_ready_to_recv() > 0) {
        
        // Store how many bytes are ready to be handled
        int bytes_to_process = client_list[i]->bytes_ready_to_recv();
        
        // Read the data into a temporary buffer (scratch_buf)
        client_list[i]->get_recv_data(scratch_buf, DEFAULT_BUFFER_SIZE);
        
        struct P2PHeader inHeader = getP2PHeaderFromBytes(scratch_buf);
        
        //Master switch statement for handling input from connected peers
        switch (inHeader.type) {
          case CONTROL_MSG:
          {
            struct ControlMessage inControlMessage = getControlMessageFromBytes(scratch_buf);
            
            //Switch statement for control types
            switch (inControlMessage.control_type) {
              case CONNECT:
              {
                struct ConnectMessage inConnectMessage = getConnectMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inConnectMessage.control_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                //store client listen address
                struct sockaddr_in clientListenAddress;
                //zero and fill out the struct
                memset(&clientListenAddress, 0, sizeof(struct sockaddr_in));
                clientListenAddress.sin_addr.s_addr = inConnectMessage.peer_data.ipv4_address;
                clientListenAddress.sin_family = AF_INET;
                clientListenAddress.sin_port = inConnectMessage.peer_data.peer_listen_port;
                client_list[i]->add_client_listen_address((sockaddr_storage *)&clientListenAddress, incoming_client_len);
                
                //send CONNECT_OK back to client
                struct ConnectMessage connectOK;
                //zero and fill out the struct
                memset(&connectOK, 0, sizeof(struct ConnectMessage));
                connectOK.control_header.control_type = htons(CONNECT_OK);
                connectOK.control_header.header.type = htons(CONTROL_MSG);
                connectOK.control_header.header.length = htons(sizeof(struct ConnectMessage));
                connectOK.peer_data.ipv4_address = serverAddr;
                connectOK.peer_data.peer_listen_port = htons(server_socket);
                
                //copy struct into a new buffer for computing the hash
                unsigned char hash_buf[sizeof(struct ConnectMessage)];
                memcpy(hash_buf, &connectOK, sizeof(struct ConnectMessage));
                size_t shaSize = sizeof(struct ConnectMessage) - sizeof(struct P2PHeader);
                SHA256(&hash_buf[sizeof(struct P2PHeader)], shaSize, connectOK.control_header.header.msg_hash);
                
                //copy struct into the scratch buffer now that the hash is stored
                memcpy(scratch_buf, &connectOK, sizeof(struct ConnectMessage));
                
                //send it
                if (client_list[i]->add_send_data(scratch_buf, sizeof(struct ConnectMessage)) != true) {
                  std::cerr << "Failed to add send data to client!" << std::endl;
                }
                
                break;
              }
              case CONNECT_OK:
              {
                struct ConnectMessage inConnectOKMessage = getConnectMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inConnectOKMessage.control_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                struct sockaddr_in clientListenAddress;
                //zero and fill out the struct
                memset(&clientListenAddress, 0, sizeof(struct sockaddr_in));
                clientListenAddress.sin_addr.s_addr = inConnectOKMessage.peer_data.ipv4_address;
                clientListenAddress.sin_family = AF_INET;
                clientListenAddress.sin_port = inConnectOKMessage.peer_data.peer_listen_port;
                client_list[i]->add_client_listen_address((sockaddr_storage *)&clientListenAddress, incoming_client_len);
                
                break;
              }
              case DISCONNECT:
              {
                //close socket for this client and erase client from client_list
                close(client_list[i]->get_fd());
                client_list.erase(client_list.begin() + i);
                
                break;
              }
              case FIND_PEERS:
              {
                struct FindPeersMessage inFindPeersMessage = getFindPeersMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inFindPeersMessage.control_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                //create GossipPeersMessage
                struct GossipPeersMessage outGossipPeersMessage;
                //zero and fill out the struct
                memset(&outGossipPeersMessage, 0, sizeof(struct GossipPeersMessage));
                outGossipPeersMessage.control_header.control_type = htons(GOSSIP_PEERS);
                outGossipPeersMessage.control_header.header.type = htons(CONTROL_MSG);
                outGossipPeersMessage.control_header.header.length = htons(sizeof(struct GossipPeersMessage) + sizeof(struct PeerInfo));
                outGossipPeersMessage.num_results = htons(1);
                
                //find random peer to send peer info to
                int indexOfPeerToGossip = rand() % client_list.size();
                
                //ensure we don't send the client back it's own information
                //unless it's the only client we're connected to
                while (client_list.size() > 1 && indexOfPeerToGossip == i){
                  indexOfPeerToGossip = rand() % client_list.size();
                }
                
                //Get peer listen info
                struct PeerInfo outPeerInfo;
                //zero and fill out the struct
                memset(&outPeerInfo, 0, sizeof(struct PeerInfo));
                struct sockaddr_in *clientAddr = (sockaddr_in *)client_list[indexOfPeerToGossip]->get_client_listen_address();
                outPeerInfo.peer_listen_port = htons(clientAddr->sin_port);
                outPeerInfo.ipv4_address = clientAddr->sin_addr.s_addr;
                
                //copy everything into a new buffer for computing the hash
                unsigned char hash_buf[sizeof(struct GossipPeersMessage) + sizeof(struct PeerInfo)];
                memcpy(hash_buf, &outGossipPeersMessage, sizeof(struct GossipPeersMessage));
                memcpy(&hash_buf[sizeof(struct GossipPeersMessage)], &outPeerInfo, sizeof(struct PeerInfo));
                size_t shaSize = sizeof(struct GossipPeersMessage) + sizeof(struct PeerInfo) - sizeof(struct P2PHeader);
                SHA256(&hash_buf[sizeof(struct P2PHeader)], shaSize, outGossipPeersMessage.control_header.header.msg_hash);
                
                //copy it all into the scratch buffer now that the hash is computed correctly
                memcpy(scratch_buf, &outGossipPeersMessage, sizeof(struct GossipPeersMessage));
                memcpy(&scratch_buf[sizeof(struct GossipPeersMessage)], &outPeerInfo, sizeof(struct PeerInfo));
                unsigned int bytes_to_send = sizeof(struct GossipPeersMessage) + sizeof(struct PeerInfo);
                
                //send it
                if (client_list[i]->add_send_data(scratch_buf, bytes_to_send) != true) {
                  std::cerr << "Failed to add send data to client!" << std::endl;
                }
                
                break;
              }
              case GOSSIP_PEERS:
              {
                struct GossipPeersMessage inGossipPeersMessage = getGossipPeersMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inGossipPeersMessage.control_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                //read in the PeerInfo struct from input buffer
                struct PeerInfo inPeerInfo;
                //zero and fill out the struct
                memset(&inPeerInfo, 0, sizeof(struct PeerInfo));
                memcpy(&inPeerInfo, &scratch_buf[sizeof(struct GossipPeersMessage)], sizeof(struct PeerInfo));
                inPeerInfo.peer_listen_port = ntohs(inPeerInfo.peer_listen_port);
                
                //check if I'm already connected to this peer, if so, break
                bool alreadyConnected = false;
                for (int j = 0; j < client_list.size(); j++){
                  in_addr_t clientAddr = ((struct sockaddr_in *)client_list[j]->get_client_address())->sin_addr.s_addr;
                  //in_port_t clientListenPort = ntohs(((struct sockaddr_in *)client_list[j]->get_client_listen_address())->sin_port);
                  if (inPeerInfo.ipv4_address == clientAddr){//} && inPeerInfo.peer_listen_port == clientListenPort){
                    alreadyConnected = true;
                    //std::cout << inet_ntoa(((struct sockaddr_in *)client_list[j]->get_client_address())->sin_addr) << std::endl;
                    break;
                  }
                }
                if (alreadyConnected){
                  //std::cout << "Tried to connect to peer I'm already connected to. \n";
                  break;
                }
                
                struct sockaddr_in newPeerSockAddr;
                //zero and fill out the struct
                memset(&newPeerSockAddr, 0, sizeof(struct sockaddr_in));
                newPeerSockAddr.sin_addr.s_addr = inPeerInfo.ipv4_address;
                newPeerSockAddr.sin_family = AF_INET;
                newPeerSockAddr.sin_port = htons(inPeerInfo.peer_listen_port);
                
                //create socket and connect to new peer
                int newPeerSocket = socket(AF_INET, SOCK_STREAM, 0);
                ret = connect(newPeerSocket, (struct sockaddr *)&newPeerSockAddr,
                              sizeof(newPeerSockAddr));
                
                if (ret != 0) {   //unsuccessful connection
                  std::cout << "Error trying to connect to gossipped peer.\n";
                  perror("connect");
                  break;
                }
                
                // Create a new TCPClient from the connection
                temp_client = new TCPClient(newPeerSocket, (sockaddr_storage *)&newPeerSockAddr, sizeof(newPeerSockAddr));
                
                // Add the new client to the list of clients we have
                client_list.push_back(temp_client);
                
                /************** SEND CONNECT MESSAGE TO NEW PEER **************/
                
                // Structure to fill in with connect message info
                struct ConnectMessage outConnectMessage;
                // Zero and fill out structure
                memset(&outConnectMessage, 0, sizeof(struct ConnectMessage));
                outConnectMessage.control_header.control_type = htons(CONNECT);
                outConnectMessage.control_header.header.type = htons(CONTROL_MSG);
                outConnectMessage.control_header.header.length = htons(sizeof(struct ConnectMessage));
                memcpy(&outConnectMessage.peer_data.ipv4_address, &(newPeerSockAddr.sin_addr.s_addr), sizeof(struct in_addr));
                outConnectMessage.peer_data.peer_listen_port = htons(newPeerSockAddr.sin_port);
                
                // Create hash after filling in rest of message.
                unsigned char hashCharArray[sizeof(struct ConnectMessage)];
                memcpy(hashCharArray, &outConnectMessage, sizeof(struct ConnectMessage));
                size_t shaSize = sizeof(struct ConnectMessage) - sizeof(struct P2PHeader);
                SHA256(&hashCharArray[sizeof(struct P2PHeader)], shaSize, outConnectMessage.control_header.header.msg_hash);
                
                //send it
                if (temp_client->add_send_data((char *) &outConnectMessage, sizeof(struct ConnectMessage)) != true) {
                  std::cerr << "Failed to add send data to client!" << std::endl;
                }
                
                break;
              }
              default:
                std::cout << "Unknown control type in control message. \n";
                break;
            }
            break;
          }
            
          case DATA_MSG:
          {
            struct DataMessage inDataMessage = getDataMessageFromBytes(scratch_buf);
            
            //Switch statement for data types
            switch (inDataMessage.data_type) {
              case SEND_MESSAGE:
              {
                struct SendMessage inSendMessage = getSendMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inSendMessage.data_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                //print out the nickname and message that we received
                int nameLength = inSendMessage.message.nickname_length;
                int messageLength = inSendMessage.message.message_length;
                char nameAndMessageBuf[nameLength + messageLength];
                
                //printing and buffering nickname
                for (int j = 0; j < nameLength; j++){
                  std::cout << scratch_buf[sizeof(struct SendMessage) + j];
                  nameAndMessageBuf[j] = scratch_buf[sizeof(struct SendMessage) + j];
                }
                std::cout << " said: ";
                
                //printing and buffering message
                for (int j = 0; j < messageLength; j++){
                  std::cout << scratch_buf[sizeof(struct SendMessage) + nameLength + j];
                  nameAndMessageBuf[nameLength + j] = scratch_buf[sizeof(struct SendMessage) + nameLength + j];
                }
                std::cout << "\n";
                
                //make forward message struct
                struct ForwardMessage outForwardMessage;
                //zero and fill out the struct
                memset(&outForwardMessage, 0, sizeof(struct ForwardMessage));
                outForwardMessage.data_header.data_type = htons(FORWARD_MESSAGE);
                outForwardMessage.data_header.header.type = htons(DATA_MSG);
                outForwardMessage.data_header.header.length = htons(sizeof(struct SendMessage) + nameLength + messageLength);
                outForwardMessage.message.send_time = htobe64(inSendMessage.message.send_time);
                outForwardMessage.message.message_length = htons(messageLength);
                outForwardMessage.message.nickname_length = htons(nameLength);
                outForwardMessage.message.sender.peer_listen_port = htons(inSendMessage.message.sender.peer_listen_port);
                outForwardMessage.message.sender.ipv4_address = inSendMessage.message.sender.ipv4_address;
                
                //copy data into a new buffer for computing the hash
                unsigned char hash_buf[sizeof(struct ForwardMessage) + nameLength + messageLength];
                memcpy(hash_buf, &outForwardMessage, sizeof(struct ForwardMessage));
                memcpy(&hash_buf[sizeof(struct ForwardMessage)], nameAndMessageBuf, nameLength + messageLength);
                size_t shaSize = sizeof(struct ForwardMessage) + nameLength + messageLength - sizeof(struct P2PHeader);
                SHA256(&hash_buf[sizeof(struct P2PHeader)], shaSize, outForwardMessage.data_header.header.msg_hash);
                
                //copy it all into the scratch buffer now that the hash is computed
                memcpy(scratch_buf, &outForwardMessage, sizeof(struct ForwardMessage));
                memcpy(&scratch_buf[sizeof(struct ForwardMessage)], nameAndMessageBuf, nameLength + messageLength);
                unsigned int bytes_to_send = sizeof(struct ForwardMessage) + nameLength + messageLength;
                
                //Add the forward message to seen hashes so that if we are forwarded this same message
                //in the future, we do not print it again
                addToSeenHashes(outForwardMessage.data_header.header.msg_hash, alreadySeenMsgHashes);
                
                //forward the message to all peers except the one that sent to us
                for (int j = 0; j < client_list.size(); j++){
                  
                  //if client j is not the client that sent us the data...
                  if (j != i){
                    client_list[j]->add_send_data(scratch_buf, bytes_to_send);
                  }
                }
                
                break;
              }
              case FORWARD_MESSAGE:
              {
                struct ForwardMessage inForwardMessage = getForwardMessageFromBytes(scratch_buf);
                if (!addToSeenHashes(inForwardMessage.data_header.header.msg_hash, alreadySeenMsgHashes)){
                  //we have already seen this message hash, do nothing
                  break;
                }
                
                //when we get a forward message, we print the data within
                //and then simply forward the exact same bytes onward
                
                //print out the nickname and message that we received
                int nameLength = inForwardMessage.message.nickname_length;
                int messageLength = inForwardMessage.message.message_length;
                
                //printing nickname
                for (int j = 0; j < nameLength; j++){
                  std::cout << scratch_buf[sizeof(struct SendMessage) + j];
                }
                std::cout << " said: ";
                
                //printing message
                for (int j = 0; j < messageLength; j++){
                  std::cout << scratch_buf[sizeof(struct SendMessage) + nameLength + j];
                }
                std::cout << "\n";
                
                unsigned int bytes_to_send = sizeof(inForwardMessage) + nameLength + messageLength;
                
                //forward the message to all peers except the one that sent to us
                for (int j = 0; j < client_list.size(); j++){
                  
                  //if client j is not the client that sent us the data...
                  if (j != i){
                    client_list[j]->add_send_data(scratch_buf, bytes_to_send);
                  }
                }
                
                break;
              }
              case GET_MESSAGE_HISTORY:
              {
                struct GetHistory inGetHistory = getGetHistoryFromBytes(scratch_buf);
                //Not yet implemented
                
                break;
              }
              case SEND_MESSAGE_HISTORY:
              {
                struct SendHistory inSendHistory = getSendHistoryFromBytes(scratch_buf);
                //Not yet implemented
                
                break;
              }
              default:
                std::cout << "Unknown data type in data message. \n";
                break;
            }
            break;
          }
            
          case ERROR_MSG:
          {
            struct ErrorMessage inErrorMessage = getErrorMessageFromBytes(scratch_buf);
            
            std::cout << "We got an error message from client #" << i << ": ";
            
            //Switch statement for error types
            switch (inErrorMessage.error_type) {
              case INCORRECT_MESSAGE_TYPE:
              {
                std::cout << "Incorrect Message Type.\n";
                break;
              }
              case INCORRECT_MESSAGE_SIZE:
              {
                std::cout << "Incorrect Message Size.\n";
                break;
              }
              case INCORRECT_MESSAGE_DIGEST:
              {
                std::cout << "Incorrect Message Digest.\n";
                break;
              }
              default:
                std::cout << "Unknown error type in error message.\n";
                break;
            }
            break;
          }
            
          default:
            break;
        }
      }
    }
  }
}
