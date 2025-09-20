#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct{
	char subdomain[64];
	char domain[64];
	char tld[64]; // top level domain name

	uint16_t len_subdomain;
	uint16_t len_domain;
	uint16_t len_tld;
} DomainName;
typedef struct {
	uint16_t id;
	uint16_t qr :1;
	uint16_t opcode :4;
	uint16_t aa :1;
	uint16_t tc :1;
	uint16_t rd :1;
	uint16_t ra :1;
	uint16_t z :3;
	uint16_t rcode :4;
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


void parse_dns_header(const unsigned char *buffer, DNSHeader *header) {
	header->id = (buffer[0] << 8) | buffer[1];
	header->qr = (buffer[2] >> 7) & 0x01;
	header->opcode = (buffer[2] >> 3) & 0x0F;
	header->aa = (buffer[2] >> 2) & 0x01;
	header->tc = (buffer[2] >> 1) & 0x01;
	header->rd = buffer[2] & 0x01;
	header->ra = (buffer[3] >> 7) & 0x01;
	header->z = (buffer[3] >> 4) & 0x07;
	header->rcode = buffer[3] & 0x0F;
	header->qdcount = (buffer[4] << 8) | buffer[5];
	header->ancount = (buffer[6] << 8) | buffer[7];
	header->nscount = (buffer[8] << 8) | buffer[9];
	header->arcount = (buffer[10] << 8) | buffer[11];
}

void parse_dns_question(const unsigned char *buffer, DNSQuestion *question){
	DomainName *dname = malloc(sizeof(DomainName));

	uint16_t cur = 0;

	if (buffer[cur] <= 63){
		return;
	}

	dname->len_subdomain = buffer[0];
	memcpy(dname->subdomain, &buffer[1], dname->len_subdomain);

	cur += 1 + dname->len_subdomain;
	
	if (! buffer[cur] <= 63){
		return;
	}

	dname->len_domain = buffer[cur];
	memcpy(dname->domain, buffer[cur], dname->len_domain);


	cur += 1 + dname->len_domain;
	
	if (! buffer[cur] <= 63){
		return;
	}

	dname->len_tld = buffer[cur];
	memcpy(dname->domain, buffer[cur], dname->len_tld);
	
	cur += 1 + dname->len_tld;


	question->qclass = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	question->qtype = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	question->qname = dname;
}	



