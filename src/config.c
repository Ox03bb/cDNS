#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <toml.h>

#include "config.h"

int load_config(const char *filename, Config *cfg) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    char errbuf[200];
    toml_table_t *conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!conf) {
        fprintf(stderr, "TOML parse error: %s\n", errbuf);
        return -1;
    }

    // [general]
    toml_table_t *general = toml_table_in(conf, "general");
    if (general) {
        toml_datum_t pn = toml_string_in(general, "project_name");
        if (pn.ok)
            cfg->general.project_name = strdup(pn.u.s);

        toml_datum_t ver = toml_string_in(general, "version");
        if (ver.ok)
            cfg->general.version = strdup(ver.u.s);

        toml_datum_t auth = toml_string_in(general, "author");
        if (auth.ok)
            cfg->general.author = strdup(auth.u.s);

        toml_datum_t desc = toml_string_in(general, "description");
        if (desc.ok)
            cfg->general.description = strdup(desc.u.s);
    }

    // [server]
    toml_table_t *server = toml_table_in(conf, "server");
    if (server) {
        toml_datum_t host = toml_string_in(server, "host");
        if (host.ok)
            cfg->server.host = strdup(host.u.s);

        toml_datum_t port = toml_int_in(server, "port");
        if (port.ok)
            cfg->server.port = (int)port.u.i;

        toml_datum_t proto = toml_string_in(server, "protocol");
        if (proto.ok)
            cfg->server.protocol = strdup(proto.u.s);

        toml_datum_t log = toml_string_in(server, "log_level");
        if (log.ok)
            cfg->server.log_level = strdup(log.u.s);

        toml_datum_t ttl = toml_int_in(server, "ttl");
        if (ttl.ok)
            cfg->server.ttl = (int)ttl.u.i;

        toml_datum_t cache = toml_bool_in(server, "enable_cache");
        if (cache.ok)
            cfg->server.enable_cache = cache.u.b;
    }

    toml_free(conf);
    return 0;
}