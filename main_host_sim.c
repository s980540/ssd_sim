#include "global.h"
#include "thread.h"

#include "host_sim.h"

static thread_info_t m_thread_info;

void *main_host(void *para)
{
    struct timespec request;
    int ret;

    m_thread_info.thread = ((thread_info_t *)para)->thread;
    m_thread_info.thread_id = ((thread_info_t *)para)->thread_id;
    m_thread_info.sleep_nsec = ((thread_info_t *)para)->sleep_nsec;

    request.tv_sec = 0;
    request.tv_nsec = m_thread_info.sleep_nsec;

    host_sim_seq_write_init();

    while (1) {
        u64 lba;

        lba = host_sim_seq_write_exec();
        printf("[Thread h]: lba: %llx\n", lba);

        if (m_thread_info.sleep_nsec) {
            ret = nanosleep(&request, NULL);
            if (ret == -1)
                printf("nanosleep error, errno=%d [%s]\n",
                    request.tv_nsec, errno, strerror(errno));
        }
    }

    pthread_exit(NULL);
}