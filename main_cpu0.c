#include "global.h"
#include "thread.h"
#include "main.h"

#include "shared_mem.h"
#include "sys_data_buf.h"

#include "ftl_write_cache.h"
#include "ftl_partial_write.h"
#include "ftl_fua.h"

#include "host_write.h"

static thread_info_t m_thread_info;

void main_cpu0_unit_test(void)
{
    pthread_mutex_lock(&foo.mutex);
    printf("[Thread %d]: %d\n", m_thread_info.thread_id, foo.val);
    pthread_mutex_unlock(&foo.mutex);
}

void main_init(void)
{
    shared_mem_init();
    sys_data_buf_mgr_init();

    ftl_write_cache_init();
    ftl_partial_write_init();
    ftl_fua_init();

    host_write_init();
}

void *main_cpu0(void *para)
{
    int ret;

    m_thread_info.thread = ((thread_info_t *)para)->thread;
    m_thread_info.thread_id = ((thread_info_t *)para)->thread_id;
    m_thread_info.sleep_nsec = ((thread_info_t *)para)->sleep_nsec;

    struct timespec request;
    request.tv_sec = 0;
    request.tv_nsec = m_thread_info.sleep_nsec;

    main_init();

    while (1) {
        // main_cpu0_unit_test();
        host_write_top_exec();

        if (FTL_FUA_TODO) {
            if (FTL_FUA_DONE)
                ftl_fua_cmpl_exec();
        }

        if (FTL_PARTIAL_WRITE_TODO) {
            ftl_partial_write_exec();
        }

        if (m_thread_info.sleep_nsec) {
            ret = nanosleep(&request, NULL);
            if (ret == -1)
                printf("nanosleep error, errno=%d [%s]\n",
                    request.tv_nsec, errno, strerror(errno));
        }
    }
    pthread_exit(NULL);
}