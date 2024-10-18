#include "./dhcp-commons.h"

void add_option(uint8_t *options, int *offset, uint8_t code, uint8_t len, void *data) {
    options[(*offset)++] = code;
    options[(*offset)++] = len;
    memcpy(options + *offset, data, len);
    *offset += len;
}