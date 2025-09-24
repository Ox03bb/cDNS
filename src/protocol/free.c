#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"


void free_dns_header(DNSHeader *header) {
	if (header) {
		free(header);
	}
}

void free_domain_name(DomainName *dname) {
	if (dname) {
		free(dname);
	}
}

void free_dns_question(DNSQuestion *question) {
	if (question) {
		if (question->qname) {
			free_domain_name(question->qname);
		}
		free(question);
	}
}

void free_dns_answer(DNSAnswer *answer) {
	if (answer) {
		if (answer->name) {
			free_domain_name(answer->name);
		}
		if (answer->rdata) {
			free(answer->rdata);
		}
		free(answer);
	}
}

void free_dns_request(DNSRequest *request) {
	if (request) {
		free_dns_header(&request->header);
		if (request->questions) {
			for (uint16_t i = 0; i < request->header.qdcount; i++) {
				free_dns_question(&request->questions[i]);
			}
			free(request->questions);
		}
		free(request);
	}
}

void free_dns_responce(DNSResponce *responce) {
	if (responce) {
		free_dns_header(&responce->header);
		if (responce->questions) {
			for (uint16_t i = 0; i < responce->header.qdcount; i++) {
				free_dns_question(&responce->questions[i]);
			}
			free(responce->questions);
		}
		if (responce->answers) {
			for (uint16_t i = 0; i < responce->header.ancount; i++) {
				free_dns_answer(&responce->answers[i]);
			}
			free(responce->answers);
		}
		if (responce->authorities) {
			for (uint16_t i = 0; i < responce->header.nscount; i++) {
				free_dns_answer(&responce->authorities[i]);
			}
			free(responce->authorities);
		}
		if (responce->additionals) {
			for (uint16_t i = 0; i < responce->header.arcount; i++) {
				free_dns_answer(&responce->additionals[i]);
			}
			free(responce->additionals);
		}
		free(responce);
	}
}
