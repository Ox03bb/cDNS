#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

typedef struct {
    char* subdomain;
    char* domain;
    char* tld; // top level domain name

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
    char *rdata;
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
} DNSResponce;

int parse_dns_header(const char *buffer, DNSHeader *header);
int parse_dns_answer(const char *buffer, int offset, DNSAnswer *answer);
int parse_dns_request(const char *buffer, DNSRequest *request);
int parse_dns_responce(const char *buffer, DNSResponce *responce);

#endif // PARSER_H