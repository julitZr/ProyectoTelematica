#include "./dhcp-commons.h"

int send_dhcp_discover(char *ip_offer) {
  // Preparar el paquete DHCPDISCOVER
  uint8_t mac_addr[6];
  struct dhcp_packet packet, response;
  memset(&packet, 0, sizeof(packet));

  // Obtener la dirección MAC
  if (get_mac_address("enp0s3", mac_addr) < 0) {
    printf("Error al obtener la dirección MAC\n");
    return -1;
  }

  packet.op = 1; // BOOTREQUEST
  packet.htype = 1; // Ethernet
  packet.hlen = 6; // Longitud de la dirección MAC
  packet.xid = htonl(random()); // ID de transacción
  packet.flags = htons(0x8000); // Set broadcast flag
  memcpy(packet.chaddr, mac_addr, 6);

  // Opciones DHCP
  uint8_t *options = packet.options;
  *options++ = 53; // Opción: Tipo de mensaje DHCP
  *options++ = 1;  // Longitud
  *options++ = 1;  // 1 = DHCPDISCOVER
  
  *options++ = 55; // Opción: Lista de parámetros solicitados
  *options++ = 4;  // Longitud
  *options++ = 1;  // Máscara de subred
  *options++ = 3;  // Router
  *options++ = 6;  // DNS
  *options++ = 15; // Nombre de dominio
  
  *options++ = 255; // Fin de opciones

  int sock = socket_send_broadcast(&packet);
  printf("DHCPDISCOVER enviado como broadcast\n");
  // Recibir solicitud
  int valread = socket_receive_broadcast(sock, &response);
  if (valread > 0) {
    print_dhcp_response(&response);
    const char *ip_str = inet_ntoa(*(struct in_addr *)&packet.yiaddr);
    strcpy(ip_offer, ip_str);
  } else {
    printf("Error al recibir DHCPOFFER\n");
  }

  close(sock);
  return 0;
}