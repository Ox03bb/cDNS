#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char *project_name;
    char *version;
    char *author;
    char *description;
} GeneralConfig;

typedef struct {
    char *host;
    int port;
    char *protocol;
    char *log_level;
    int ttl;
    int enable_cache;
} ServerConfig;

typedef struct {
    GeneralConfig general;
    ServerConfig server;
} Config;

int load_config(const char *filename, Config *cfg);

#endif // CONFIG_H