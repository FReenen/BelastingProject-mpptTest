#ifndef DEBUG_H
#define DEBUG_H

#define LOG(msg, ...) ;

#define ERROR(msg, ...) LOG("ERROR: " msg, ##__VA_ARGS__)
#define WARN(msg, ...)  LOG("WARN: " msg, ##__VA_ARGS__)
#define DEBUG(msg, ...) LOG("DEBUG: " msg, ##__VA_ARGS__)
#define INFO(msg, ...)  LOG("INFO: " msg, ##__VA_ARGS__)

#endif
