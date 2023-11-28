#include "thread.h"

extern void *main_cpu0(void *para);
extern void *main_cpu1(void *para);
extern void *main_cpu2(void *para);
extern void *main_cpu3(void *para);

thread_info_t *m_thread_info_0, *m_thread_info_1, *m_thread_info_2, *m_thread_info_3;

int thread_init(void)
{
    m_thread_info_0 = malloc(sizeof(thread_info_t));
    m_thread_info_0->thread_id = 0;
    m_thread_info_0->sleep_nsec = THREAD_SLEEP_MSEC(500);
    pthread_create(&m_thread_info_0->thread, NULL, main_cpu0, m_thread_info_0);

    m_thread_info_1 = malloc(sizeof(thread_info_t));
    m_thread_info_1->thread_id = 1;
    m_thread_info_1->sleep_nsec = THREAD_SLEEP_MSEC(10);
    pthread_create(&m_thread_info_1->thread, NULL, main_cpu1, m_thread_info_1);

    m_thread_info_2 = malloc(sizeof(thread_info_t));
    m_thread_info_2->thread_id = 2;
    m_thread_info_2->sleep_nsec = THREAD_SLEEP_MSEC(300);
    pthread_create(&m_thread_info_2->thread, NULL, main_cpu2, m_thread_info_2);

    m_thread_info_3 = malloc(sizeof(thread_info_t));
    m_thread_info_3->thread_id = 3;
    m_thread_info_3->sleep_nsec = THREAD_SLEEP_MSEC(500);
    pthread_create(&m_thread_info_3->thread, NULL, main_cpu3, m_thread_info_3);

    pthread_join(m_thread_info_0->thread, NULL);
    pthread_join(m_thread_info_1->thread, NULL);
    pthread_join(m_thread_info_2->thread, NULL);
    pthread_join(m_thread_info_3->thread, NULL);

    return 0;
}

int thread_delete(void)
{
    free(m_thread_info_0);
    free(m_thread_info_1);
    free(m_thread_info_2);
    free(m_thread_info_3);

    return 0;
}