#include "./dhcp-commons.h"

int get_mac_address(const char *interface, uint8_t *mac_addr) {
  struct ifreq ifr;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
      perror("socket");
      return -1;
  }

  strcpy(ifr.ifr_name, interface);
  if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
      perror("ioctl");
      close(sock);
      return -1;
  }

  memcpy(mac_addr, ifr.ifr_hwaddr.sa_data, 6);
  close(sock);
  return 0;
}

int socket_send_broadcast(struct dhcp_packet *packet) {
  int sock;
  struct sockaddr_in server_addr, client_addr;

  // Crear socket
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error en la creación del socket");
    return -1;
  }

  // Configurar socket para broadcast
  int broadcast = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    perror("Error al configurar socket para broadcast");
    close(sock);
    return -1;
  }

  // Configurar dirección del cliente
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  // client_addr.sin_port = htons(DHCP_CLIENT_PORT);
  client_addr.sin_addr.s_addr = INADDR_ANY;

  // Vincular socket
  if (bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    perror("Error al vincular socket");
    close(sock);
    return -1;
  }

  // Configurar dirección del servidor (broadcast)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(DHCP_SERVER_PORT);
  server_addr.sin_addr.s_addr = INADDR_BROADCAST;

  // Enviar DHCPDISCOVER
  if (sendto(sock, packet, sizeof(*packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Error al enviar DHCPDISCOVER");
    close(sock);
    return -1;
  }

  return sock;
}

int socket_receive_broadcast(int sock, struct dhcp_packet *response) {
  struct sockaddr_in server_address;
  socklen_t server_address_len = sizeof(server_address);
  // Recibir solicitud
  int valread = recvfrom(sock, response, sizeof(*response), 0, (struct sockaddr *)&server_address, &server_address_len);
  return valread;
}

void print_ip(uint32_t ip) {
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

void parse_options(uint8_t *options, int size) {
  int i = 0;
  while (i < size) {
    uint8_t code = options[i++];
    uint8_t len = options[i++];
    
    switch(code) {
      case 1: // Subnet Mask
        printf("Máscara de subred: ");
        print_ip(*(uint32_t*)(options + i));
        printf("\n");
        break;
      case 3: // Router
        printf("Router: ");
        print_ip(*(uint32_t*)(options + i));
        printf("\n");
        break;
      case 6: // DNS Server
        printf("Servidor DNS: ");
        print_ip(*(uint32_t*)(options + i));
        printf("\n");
        break;
      case 15: // Domain Name
        printf("Nombre de dominio: %.*s\n", len, options + i);
        break;
      case 51: // IP Address Lease Time
        printf("Tiempo de concesión: %u segundos\n", ntohl(*(uint32_t*)(options + i)));
        break;
      case 255: // End
        return;
    }
    i += len;
  }
}

void print_dhcp_response(struct dhcp_packet *packet) {
  printf("RESPUESTA recibida:\n");
  printf("IP ofrecida: %s\n", inet_ntoa(*(struct in_addr *)&packet->yiaddr));
  printf("IP del servidor: %s\n", inet_ntoa(*(struct in_addr *)&packet->siaddr));
  
  // Imprimir opciones DHCP
  if (packet->options[0] == 53 && packet->options[2] == 2) {
    printf("Tipo de mensaje: DHCPOFFER\n");
  }
  if (packet->options[0] == 53 && packet->options[2] == 5) {
    printf("Tipo de mensaje: DHCPACK\n");
  }
  
  parse_options(packet->options, sizeof(packet->options));
}