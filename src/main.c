#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

void test_parsing() {
    char buffer[] = {0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
                     0x01, 0x03, 'w',  'w',  'w',  0x07, 'e',  'x',  'a',  'm',  'p',
                     'l',  'e',  0x03, 'c',  'o',  'm',  0x00, 0x00, 0x01, 0x00, 0x01};

    DNSRequest request;
    if (parse_dns_request(buffer, &request) == -1) {
        fprintf(stderr, "Failed to parse DNS request\n");
        return;
    }

    // Print parsed values for verification
    printf("Parsed DNS Request:\n");
    printf("ID: %04x\n", request.header.id);
    printf("QR: %d\n", request.header.qr);
    printf("Opcode: %d\n", request.header.opcode);
    printf("AA: %d\n", request.header.aa);
    printf("TC: %d\n", request.header.tc);
    printf("RD: %d\n", request.header.rd);
    printf("RA: %d\n", request.header.ra);
    printf("Z: %d\n", request.header.z);
    printf("Rcode: %d\n", request.header.rcode);
    printf("QDCount: %d\n", request.header.qdcount);
    printf("ANCount: %d\n", request.header.ancount);
    printf("NSCount: %d\n", request.header.nscount);
    printf("ARCount: %d\n", request.header.arcount);

    for (int i = 0; i < request.header.qdcount; i++) {
        printf("subdomain: %s\n", request.questions[i].qname->subdomain);
        printf("domain: %s\n", request.questions[i].qname->domain);
        printf("tld: %s\n", request.questions[i].qname->tld);
        printf("Type: %d\n", request.questions[i].qtype);
        printf("  Class: %d\n", request.questions[i].qclass);
    }
}

int main(int argc, char **argv) {
    test_parsing();
    return 0;
}