#ifndef DHCP_DISCOVER_H
#define DHCP_DISCOVER_H

void process_dhcp_discover(int server_fd, struct dhcp_packet *packet, struct sockaddr *client_address);

#endif // DHCP_DISCOVER_H