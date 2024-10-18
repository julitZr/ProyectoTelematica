#ifndef DHCP_REQUEST_H
#define DHCP_REQUEST_H

void process_dhcp_request(int server_fd, struct dhcp_packet *packet, struct sockaddr *client_address);

#endif // DHCP_REQUEST_H