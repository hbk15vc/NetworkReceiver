/* Copyright 2018 Hobum (Vincent) Kwon. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <stack>
#include <cmath> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include "vtz_logger/common/logging.h"
#include "vtz_logger/common/macros.h"
#include "vtz_logger/common/integral_type.h"
#include "network_receiver.h"

#define VTZ_NETWORK_DOMAIN_FILE_NAME "/tmp/somefile4"
#define BUF_SIZE 10
#define PORT_NUM 50002
#define SV_SOCK_PATH "/tmp/ud_ucase"

namespace vtz {
namespace network_receiver {

 // Type: NETWORK, DOMAIN
NetworkReceiver::NetworkReceiver() {

}

void *NetworkReceiver::StartTCPUnixDomainSocket(void *) {
  sockaddr_un server_addr;
  sockaddr_un client_addr;
  int client_size;
  int server_sock;

  if ((server_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // SOCK_DGRAM, SOCK_STREAM
    exit(1);
  server_addr.sun_family = AF_UNIX;
  remove(VTZ_NETWORK_DOMAIN_FILE_NAME);
  strcpy(server_addr.sun_path, VTZ_NETWORK_DOMAIN_FILE_NAME);
  
  if (bind(server_sock, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1) {
    perror("bind error");
    exit(0);
  }
  
  if (listen(server_sock, 10) == -1) {
    perror("bind error");
    exit(0);
  }

  while (true)
  {
    LOG(INFO) << "[server] listen returned.";
    int new_fd = accept(server_sock, reinterpret_cast<sockaddr *>(&client_addr), reinterpret_cast<socklen_t*>(&client_size));
    LOG(INFO) << "[server] accept returned. " << new_fd;
    sleep(1);
    char buffer[1024];
    int char_received = recv(new_fd, buffer, 1024, 0);
    LOG(INFO) << "[server] received " << buffer; 
    strncpy(buffer, "Server", 5);
    LOG(INFO) << "[server] sent back " << buffer; 
    send(new_fd, buffer, char_received, 0);
  }
 }

}  // network_sender
}  // vtz
