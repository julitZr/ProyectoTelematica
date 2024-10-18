#include "./dhcp-commons.h"
#include "./dhcp-discover.h"
#include "./dhcp-request.h"

#define PORT 67 // Puerto estándar DHCP

int main() {
  int server_fd;
  struct sockaddr_in address;
  struct dhcp_packet packet;

  // Crear socket
  if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
      perror("Fallo en la creación del socket");
      exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Vincular el socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("Fallo en la vinculación");
      exit(EXIT_FAILURE);
  }

  printf("Servidor DHCP escuchando en el puerto %d...\n", PORT);

  while(1) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    // Recibir solicitud
    int valread = recvfrom(server_fd, &packet, sizeof(packet), 0, 
                            (struct sockaddr *)&client_address, &client_address_len);
    printf("Solicitud DHCP recibida\n");

    // Determinar el tipo de mensaje DHCP
    uint8_t *options = packet.options;
    uint8_t msg_type = 0;
    while (*options != 255 && options < packet.options + sizeof(packet.options)) {
        if (*options == 53 && *(options + 1) == 1) {
            msg_type = *(options + 2);
            break;
        }
        options += *(options + 1) + 2;
    }

    switch(msg_type) {
      case 1: // DHCPDISCOVER
        process_dhcp_discover(server_fd, &packet, (struct sockaddr *)&client_address);
        break;
      case 3: // DHCPREQUEST
        process_dhcp_request(server_fd, &packet, (struct sockaddr *)&client_address);
        break;
      default:
        printf("Tipo de mensaje DHCP no soportado: %d\n", msg_type);
    }
  }

  return 0;
}