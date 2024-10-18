#include "./dhcp-commons.h"
#include "./dhcp-request.h"

// #define DHCP_MAGIC_COOKIE 0x63825363

void process_dhcp_request(int server_fd, struct dhcp_packet *request, struct sockaddr *client_address) {
  struct dhcp_packet reply;
  printf("Procesando DHCPREQUEST\n");

  memset(&reply, 0, sizeof(struct dhcp_packet));

  // Copiar campos relevantes de la solicitud
  reply.op = 2; // Boot Reply
  reply.htype = request->htype;
  reply.hlen = request->hlen;
  reply.xid = request->xid;
  memcpy(reply.chaddr, request->chaddr, 16);
  // reply.magic_cookie = htonl(DHCP_MAGIC_COOKIE);

  // Asignar una dirección IP (ejemplo: 192.168.1.100)
  reply.yiaddr = inet_addr("192.168.1.100"); // IP ofrecida
  reply.siaddr = inet_addr("192.168.1.1"); // IP del servidor

  int offset = 0;
  uint32_t subnet_mask = inet_addr("255.255.255.0");
  uint32_t router = inet_addr("192.168.1.1");
  uint32_t dns_server = inet_addr("8.8.8.8");
  uint8_t lease_time[] = {0x00, 0x01, 0x51, 0x80}; // 86400 segundos (1 día)
  char domain[] = "ejemplo.com";
  
  // Opciones DHCP
  add_option(reply.options, &offset, 53, 1, "\x05"); // DHCPACK
  add_option(reply.options, &offset, 1, 4, &subnet_mask); // Subnet Mask
  add_option(reply.options, &offset, 3, 4, &router); // Router
  add_option(reply.options, &offset, 6, 4, &dns_server); // DNS Server
  add_option(reply.options, &offset, 51, 4, lease_time); // IP Address Lease Time
  add_option(reply.options, &offset, 15, strlen(domain), domain); // Domain Name
  add_option(reply.options, &offset, 255, 0, NULL); // End option
  
  // Enviar DHCPACK
  sendto(server_fd, &reply, sizeof(reply), 0, client_address, sizeof(*client_address));
  printf("DHCPACK enviado\n");
}