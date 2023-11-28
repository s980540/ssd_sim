#ifndef WRITE_CACHE_H
#define WRITE_CACHE_H

#include "global.h"

#define WC_HOST_WR_COND (write_cache_host_write_condition())
#define WC_FLUSH_COND   (write_cache_flush_condition())

extern bool write_cache_flush_condition(void);
extern bool write_cache_host_write_condition(void);

#endif // WRITE_CACHE_H