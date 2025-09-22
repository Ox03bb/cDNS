#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"

int parse_dns_header(const char *buffer, DNSHeader *header) {
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
	return 12;
}

int parse_domain_name(const char *buffer, int offset, DomainName *dname) {
    int cur = offset;
    // Parse subdomain
    if ((char)buffer[cur] >= 63) {
        printf("Invalid subdomain length\n");
        return -1;
    }
    dname->len_subdomain = (int8_t)buffer[cur];
    if (dname->len_subdomain == 0) {
        dname->subdomain = strdup("");
    } else {
        char temp[64];
        if (dname->len_subdomain > 63) return -1;
        memcpy(temp, buffer + cur + 1, dname->len_subdomain);
        temp[dname->len_subdomain] = '\0';
        dname->subdomain = strdup(temp);
    }
    cur += 1 + dname->len_subdomain;

    // Parse domain
    if ((char)buffer[cur] >= 63) {
        printf("Invalid domain length\n");
        return -1;
    }
    dname->len_domain = (int8_t)buffer[cur];
    if (dname->len_domain == 0) {
        dname->domain = strdup("");
    } else {
        char temp[64];
        if (dname->len_domain > 63) return -1;
        memcpy(temp, buffer + cur + 1, dname->len_domain);
        temp[dname->len_domain] = '\0';
        dname->domain = strdup(temp);
    }

    cur += 1 + dname->len_domain;

    if ((char)buffer[cur] >= 63) {
        printf("Invalid tld length\n");
        return -1;
    }
    dname->len_tld = (int8_t)buffer[cur];
    if (dname->len_tld == 0) {
        dname->tld = strdup("");
    } else {
        char temp[64];
        if (dname->len_tld > 63) return -1;
        memcpy(temp, buffer + cur + 1, dname->len_tld);
        temp[dname->len_tld] = '\0';
        dname->tld = strdup(temp);
    }
    cur += 1 + dname->len_tld;

    return cur;
}

int parse_dns_question(const char *buffer,int offset, DNSQuestion *question){
	DomainName *dname = malloc(sizeof(DomainName));
	int cur = parse_domain_name(buffer,offset , dname);
	
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

int parse_dns_answer(const char *buffer, DNSAnswer *answer){
	DomainName *dname = malloc(sizeof(DomainName));
	int cur = parse_domain_name(buffer,0 , dname);
	
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

int parse_dns_request(const char *buffer, DNSRequest *request) {
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
		int res = parse_dns_question(buffer,cur, &request->questions[i]);
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
