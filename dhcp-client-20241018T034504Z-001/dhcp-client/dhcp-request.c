#include "./dhcp-commons.h"

int send_dhcp_request(char* ip_offer) {
  uint8_t mac_addr[6];
  struct dhcp_packet packet, response;
  memset(&packet, 0, sizeof(packet));

  // Obtener la dirección MAC
  if (get_mac_address("enp0s3", mac_addr) < 0) {
    printf("Error al obtener la dirección MAC\n");
    return -1;
  }

  // Preparar el paquete DHCPREQUEST
  memset(&packet, 0, sizeof(packet));
  packet.op = 1; // BOOTREQUEST
  packet.htype = 1; // Ethernet
  packet.hlen = 6; // Longitud de la dirección MAC
  packet.xid = htonl(random()); // ID de transacción
  packet.flags = htons(0x8000); // Set broadcast flag
  memcpy(packet.chaddr, mac_addr, 6);

  // Añadir opciones DHCP
  uint8_t *option_ptr = packet.options;

  // Opción: DHCP Message Type (53)
  *option_ptr++ = 53;
  *option_ptr++ = 1;
  *option_ptr++ = 3; // 3 = DHCPREQUEST

  // Opción: Client Identifier (61)
  *option_ptr++ = 61;
  *option_ptr++ = 7; // longitud: tipo (1) + MAC (6)
  *option_ptr++ = 1; // tipo de hardware: Ethernet
  memcpy(option_ptr, mac_addr, 6); // MAC address (ejemplo)
  option_ptr += 6;

  // Opción: Requested IP Address (50)
  printf("IP solicitada: %s\n", ip_offer);
  *option_ptr++ = 50;
  *option_ptr++ = 4;
  *(uint32_t*)option_ptr = inet_addr(ip_offer); // IP solicitada
  option_ptr += 4;

  // Opción: End (255)
  *option_ptr++ = 255;

  int sock = socket_send_broadcast(&packet);
  printf("DHCPREQUEST enviado como broadcast\n");
  // Recibir solicitud
  int valread = socket_receive_broadcast(sock, &response);
  if (valread > 0) {
    print_dhcp_response(&response);
  } else {
    printf("Error al recibir DHCPOFFER\n");
  }

  close(sock);
  return 0;
}