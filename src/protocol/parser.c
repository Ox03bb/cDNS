#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"

uint8_t parse_dns_header(const unsigned char *buffer, DNSHeader *header) {
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
	return 0;
}

uint8_t parse_domain_name(const unsigned char *buffer, DomainName *dname) {
	uint16_t cur = 0;

	if (buffer[cur] <= 63){
		return -1;
	}

	dname->len_subdomain = buffer[0];
	memcpy(dname->subdomain, &buffer[1], dname->len_subdomain);

	cur += 1 + dname->len_subdomain;
	
	if (! buffer[cur] <= 63){
		return -1;
	}

	dname->len_domain = buffer[cur];
	memcpy(dname->domain, &buffer[cur], dname->len_domain);

	cur += 1 + dname->len_domain;
	
	if (! buffer[cur] <= 63){
		return -1;
	}
	
	dname->len_tld = buffer[cur];
	memcpy(dname->tld, &buffer[cur], dname->len_tld);

	cur += 1 + dname->len_tld;

	return cur;
}

uint8_t parse_dns_question(const unsigned char *buffer, DNSQuestion *question){
	DomainName *dname = malloc(sizeof(DomainName));
	int cur = parse_domain_name(buffer, dname);
	
	if (cur == -1) {
		free(dname);
		return -1;
	}

	question->qclass = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	question->qtype = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	question->qname = dname;

	return cur;
}	

uint8_t parse_dns_answer(const unsigned char *buffer, DNSAnswer *answer){
	DomainName *dname = malloc(sizeof(DomainName));
	int cur = parse_domain_name(buffer, dname);
	
	if (cur == -1) {
		free(dname);
		return -1;
	}

	answer->class = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	answer->type = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	answer->ttl = (buffer[cur] << 24) | (buffer[cur + 1] << 16) | (buffer[cur + 2] << 8) | buffer[cur + 3];
	cur += 4;

	answer->rdlength = (buffer[cur] << 8) | buffer[cur + 1];
	cur += 2;

	answer->rdata = malloc(answer->rdlength);
	memcpy(answer->rdata, &buffer[cur], answer->rdlength);
	cur += answer->rdlength;

	answer->name = dname;

	return cur;
}

uint8_t parse_dns_request(const unsigned char *buffer, DNSRequest *request) {
	int cur = 0;
	
	cur += parse_dns_header(buffer, &request->header);
	if (cur == -1) {
		return -1;
	}

	request->questions = malloc(request->header.qdcount * sizeof(DNSQuestion));
	if (request->questions == NULL) {
		return -1;
	}

	for (int i = 0; i < request->header.qdcount; i++) {
		int res = parse_dns_question(&buffer[cur], &request->questions[i]);
		if (res == -1) {
			for (int j = 0; j < i; j++) {
				free(request->questions[j].qname);
			}
			free(request->questions);
			return -1;
		}
		cur += res;
	}

	return cur;
}
