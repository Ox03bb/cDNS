#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

void test_reauest_parsing() {
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

void test_response_parsing() {
    // Example DNS response packet (A record for www.example.com)
    char buffer[] = {
        0x12, 0x34, 0x81, 0x80, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x03, 'w', 'w', 'w', 0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e', 0x03, 'c', 'o', 'm', 0x00,
        0x00, 0x01, 0x00, 0x01,
        // Answer section
        0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x04,
        0x5d, 0xb8, 0xd8, 0x22 // 93.184.216.34
    };

    DNSResponce response;
    if (parse_dns_responce(buffer, &response) == -1) {
        fprintf(stderr, "Failed to parse DNS response\n");
        return;
    }

    // Print parsed values for verification
    printf("Parsed DNS Response:\n");
    printf("ID: %04x\n", response.header.id);
    printf("QR: %d\n", response.header.qr);
    printf("Opcode: %d\n", response.header.opcode);
    printf("AA: %d\n", response.header.aa);
    printf("TC: %d\n", response.header.tc);
    printf("RD: %d\n", response.header.rd);
    printf("RA: %d\n", response.header.ra);
    printf("Z: %d\n", response.header.z);
    printf("Rcode: %d\n", response.header.rcode);
    printf("QDCount: %d\n", response.header.qdcount);
    printf("ANCount: %d\n", response.header.ancount);
    printf("NSCount: %d\n", response.header.nscount);
    printf("ARCount: %d\n", response.header.arcount);

    for (int i = 0; i < response.header.ancount; i++) {
    printf("Answer %d:\n", i + 1);
    printf("subdomain: %s\n", response.answers[i].name->subdomain);
    printf("domain: %s\n", response.answers[i].name->domain);
    printf("tld: %s\n", response.answers[i].name->tld);
    printf("Type: %d\n", response.answers[i].type);
    printf("Class: %d\n", response.answers[i].class);
    printf("TTL: %d\n", response.answers[i].ttl);
    printf("Data length: %d\n", response.answers[i].rdlength);
    printf("Address: %d.%d.%d.%d\n",
           (unsigned char)response.answers[i].rdata[0],
           (unsigned char)response.answers[i].rdata[1],
           (unsigned char)response.answers[i].rdata[2],
           (unsigned char)response.answers[i].rdata[3]);
}
}


int main(int argc, char **argv) {
    test_response_parsing();
    return 0;
}