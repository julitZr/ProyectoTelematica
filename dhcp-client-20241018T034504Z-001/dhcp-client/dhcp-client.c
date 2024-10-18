#include "./dhcp-discover.h"
#include "./dhcp-request.h"

int main() {
  char ip_offer[30];
  send_dhcp_discover(ip_offer);
  send_dhcp_request(ip_offer);
  return 0;
}