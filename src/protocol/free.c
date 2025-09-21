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

void free_dns_response(DNSResponse *response) {
	if (response) {
		free_dns_header(&response->header);
		if (response->questions) {
			for (uint16_t i = 0; i < response->header.qdcount; i++) {
				free_dns_question(&response->questions[i]);
			}
			free(response->questions);
		}
		if (response->answers) {
			for (uint16_t i = 0; i < response->header.ancount; i++) {
				free_dns_answer(&response->answers[i]);
			}
			free(response->answers);
		}
		if (response->authorities) {
			for (uint16_t i = 0; i < response->header.nscount; i++) {
				free_dns_answer(&response->authorities[i]);
			}
			free(response->authorities);
		}
		if (response->additionals) {
			for (uint16_t i = 0; i < response->header.arcount; i++) {
				free_dns_answer(&response->additionals[i]);
			}
			free(response->additionals);
		}
		free(response);
	}
}
