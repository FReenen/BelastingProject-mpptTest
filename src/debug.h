#ifndef DEBUG_H
#define DEBUG_H

#define LOG(msg) ;

#define ERROR(msg) LOG("ERROR: " msg)
#define WARN(msg)  LOG("WARN: " msg)
#define DEBUG(msg) LOG("DEBUG: " msg)
#define INFO(msg)  LOG("INFO: " msg)

#endif
