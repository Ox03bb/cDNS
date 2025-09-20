#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
	__uint16_t id;
	unsigned qr :1;
	unsigned opcode :4;
	unsigned aa :1;
	unsigned tc :1;
	unsigned rd :1;
	unsigned ra :1;
	unsigned z :3;
	unsigned rcode :4;
	__uint16_t qdcount;
	__uint16_t ancount;
	__uint16_t nscount;
	__uint16_t arcount;
} DNSHeader;

typedef struct {
	char *qname;
	__uint16_t qtype;
	__uint16_t qclass;
} DNSQuestion;

typedef struct {
	char *name;
	__uint16_t type;
	__uint16_t class;
	__uint32_t ttl;
	__uint16_t rdlength;
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