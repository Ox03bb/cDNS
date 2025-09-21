#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

typedef struct {
    char subdomain[64];
    char domain[64];
    char tld[64]; // top level domain name

    uint16_t len_subdomain;
    uint16_t len_domain;
    uint16_t len_tld;
} DomainName;

typedef struct {
    uint16_t id;
    uint16_t qr : 1;
    uint16_t opcode : 4;
    uint16_t aa : 1;
    uint16_t tc : 1;
    uint16_t rd : 1;
    uint16_t ra : 1;
    uint16_t z : 3;
    uint16_t rcode : 4;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} DNSHeader;

typedef struct {
    DomainName *qname;
    uint16_t qtype;
    uint16_t qclass;
} DNSQuestion;

typedef struct {
    DomainName *name;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t rdlength;
    unsigned char *rdata;
} DNSAnswer;

typedef struct {
    DNSHeader header;
    DNSQuestion *questions;
} DNSRequest;

typedef struct {
    DNSHeader header;
    DNSQuestion *questions;
    DNSAnswer *answers;
    DNSAnswer *authorities;
    DNSAnswer *additionals;
} DNSResponse;

uint8_t parse_dns_header(const unsigned char *buffer, DNSHeader *header);
uint8_t parse_dns_answer(const unsigned char *buffer, DNSAnswer *answer);
uint8_t parse_dns_request(const unsigned char *buffer, DNSRequest *request);

#endif // PARSER_H