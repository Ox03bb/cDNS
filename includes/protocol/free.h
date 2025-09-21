#ifndef FREE_H
#define FREE_H

#include "parser.h"

void free_dns_header(DNSHeader *header);
void free_domain_name(DomainName *dname);
void free_dns_question(DNSQuestion *question);
void free_dns_answer(DNSAnswer *answer);
void free_dns_request(DNSRequest *request);
void free_dns_response(DNSResponse *response);

#endif // FREE_H