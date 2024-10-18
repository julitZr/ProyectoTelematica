#ifndef DHCP_COMMONS_H
#define DHCP_COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68
#define DHCP_CHADDR_LEN 16
#define DHCP_SNAME_LEN  64
#define DHCP_FILE_LEN   128

struct dhcp_packet {
  uint8_t  op;
  uint8_t  htype;
  uint8_t  hlen;
  uint8_t  hops;
  uint32_t xid;
  uint16_t secs;
  uint16_t flags;
  uint32_t ciaddr;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t giaddr;
  uint8_t  chaddr[DHCP_CHADDR_LEN];
  char     sname[DHCP_SNAME_LEN];
  char     file[DHCP_FILE_LEN];
  uint32_t magic_cookie;
  uint8_t options[312];
};

int get_mac_address(const char *interface, uint8_t *mac_addr);
int socket_send_broadcast(struct dhcp_packet *packet);
int socket_receive_broadcast(int sock, struct dhcp_packet *response);
void print_ip(uint32_t ip);
void parse_options(uint8_t *options, int size);
void print_dhcp_response(struct dhcp_packet *packet);

#endif // DHCP_COMMONS_H