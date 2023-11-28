#include "global.h"
#include "thread.h"
#include "main.h"

#include "fifo.h"

static thread_info_t m_thread_info;

void main_cpu1_unit_test(void)
{
    pthread_mutex_lock(&foo.mutex);
    foo.val = foo.val + m_thread_info.thread_id;
    printf("[Thread %d]: %d\n", m_thread_info.thread_id, foo.val);
    pthread_mutex_unlock(&foo.mutex);
}

void com_cmd_add(int fifo_idx, int a, int b)
{
    com_cmd_sq_format_add_t *entry;
    if (fifo_push_full(fifo_idx)) {
        printf("fifo_push_full\n");
        return;
    }
    entry = fifo_get_push_ptr(fifo_idx);
    entry->opcode = COM_CMD_OPCODE_ADD;
    entry->initiator_id = m_thread_info.thread_id;
    entry->a = a;
    entry->b = b;
    entry->sn = rand();
    printf("[Thread 1]: initiator: %d, sn: %d, %d + %d = %d\n",
        entry->initiator_id, entry->sn, a, b, a + b);
    fifo_push(fifo_idx, 1);
}

void *main_cpu1(void *para)
{
    // thread_info_t *m_thread_info = (thread_info_t *)para;
    struct timespec request;
    int ret;

    m_thread_info.thread = ((thread_info_t *)para)->thread;
    m_thread_info.thread_id = ((thread_info_t *)para)->thread_id;
    m_thread_info.sleep_nsec = ((thread_info_t *)para)->sleep_nsec;

    printf("%d %d %d\n",
        m_thread_info.thread.p,
        m_thread_info.thread_id,
        m_thread_info.sleep_nsec);

    request.tv_sec = 0;
    request.tv_nsec = m_thread_info.sleep_nsec;

    // srand(time(NULL));

    while (1) {
        int a, b;
        a = rand() % 100;
        b = rand() % 100;
        com_cmd_add(FIFO_TYPE_COM_CMD_SQ0, a, b);
        // main_cpu1_unit_test();
        if (m_thread_info.sleep_nsec) {
            ret = nanosleep(&request, NULL);
            if (ret == -1)
                printf("nanosleep error, errno=%d [%s]\n",
                    request.tv_nsec, errno, strerror(errno));
        }
    }
    pthread_exit(NULL);
}