#include "fifo.h"
#include "tcm.h"
// #include "sram.h"

fifo_t g_fifo[FIFO_TYPE_MAX_NUM];

int fifo_lock(int fifo_idx)
{
    int sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    if (sts)
        printf("pthread_mutex_lock error: %d\n", sts);

    return sts;
}

int fifo_unlock(int fifo_idx)
{
    int sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    if (sts)
        printf("pthread_mutex_unlock error: %d\n", sts);

    return sts;
}

int fifo_push(int fifo_idx, u32 add)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];

    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return sts;
    // }

    fifo->front = (fifo->front + add) % fifo->size;
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return sts;
    // }

    return 0;
}

int fifo_pop(int fifo_idx, u32 add)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];

    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return sts;
    // }
    fifo->rear = (fifo->rear + add) % fifo->size;
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return sts;
    // }

    return 0;
}

u32 fifo_push_num(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    u32 push_num;

    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return U32_MASK;
    // }

    push_num = (fifo->rear > fifo->front)
                ? (fifo->rear - fifo->front)
                : (fifo->size - fifo->front + fifo->rear);

    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return U32_MASK;
    // }

    return push_num;
}

u32 fifo_pop_num(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    u32 pop_num;

    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return U32_MASK;
    // }

    if (fifo->front != fifo->rear)
        pop_num = (fifo->front > fifo->rear)
                ? (fifo->front - fifo->rear)
                : (fifo->size - fifo->rear + fifo->front);
    else
        pop_num = 0;

    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return U32_MASK;
    // }

    return pop_num;
}

int fifo_push_full(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    bool full;
    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return -1;
    // }
    full = ((fifo->front + 1) % fifo->size) == fifo->rear;
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return -1;
    // }

    return full;
}

int fifo_pop_full(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    bool full;
    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return -1;
    // }
    full = fifo->front == fifo->rear;
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return -1;
    // }

    return full;
}

void *fifo_get_push_ptr(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    void *ptr;
    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return NULL;
    // }
    ptr = (void *)((u32)(fifo->ptr) + fifo->front * fifo->entry_size);
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return NULL;
    // }
    return ptr;
}

void *fifo_get_pop_ptr(int fifo_idx)
{
    // int sts;
    fifo_t *fifo = &g_fifo[fifo_idx];
    void *ptr;
    // sts = pthread_mutex_lock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_lock error: %d\n", sts);
    //     return NULL;
    // }
    ptr = (void *)((u32)(fifo->ptr) + fifo->rear * fifo->entry_size);
    // sts = pthread_mutex_unlock(&g_fifo[fifo_idx].mutex);
    // if (sts) {
    //     printf("pthread_mutex_unlock error: %d\n", sts);
    //     return NULL;
    // }
    return ptr;
}

int fifo_init(void)
{
    int sts;
    sts = pthread_mutex_init(&g_fifo[FIFO_TYPE_COM_CMD_SQ0].mutex, NULL);
    if (sts)
        printf("pthread_mutex_init error: %d\n", sts);
    g_fifo[FIFO_TYPE_COM_CMD_SQ0].front = 0;
    g_fifo[FIFO_TYPE_COM_CMD_SQ0].rear = 0;
    g_fifo[FIFO_TYPE_COM_CMD_SQ0].size = COM_CMD_SQ_FIFO_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_SQ0].entry_size = COM_CMD_SQ_FIFO_ENTRY_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_SQ0].ptr = tcm_get_ptr(SYS_MEM_TYPE_B1TCM, COM_CMD_SQ0_FIFO_IDX);
    printf("g_fifo[FIFO_TYPE_COM_CMD_SQ0].ptr: %8x\n", (u32)g_fifo[FIFO_TYPE_COM_CMD_SQ0].ptr);

    sts = pthread_mutex_init(&g_fifo[FIFO_TYPE_COM_CMD_SQ1].mutex, NULL);
    if (sts)
        printf("pthread_mutex_init error: %d\n", sts);
    g_fifo[FIFO_TYPE_COM_CMD_SQ1].front = 0;
    g_fifo[FIFO_TYPE_COM_CMD_SQ1].rear = 0;
    g_fifo[FIFO_TYPE_COM_CMD_SQ1].size = COM_CMD_SQ_FIFO_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_SQ1].entry_size = COM_CMD_SQ_FIFO_ENTRY_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_SQ1].ptr = tcm_get_ptr(SYS_MEM_TYPE_B1TCM, COM_CMD_SQ1_FIFO_IDX);
    printf("g_fifo[FIFO_TYPE_COM_CMD_SQ1].ptr: %8x\n", (u32)g_fifo[FIFO_TYPE_COM_CMD_SQ1].ptr);

    sts = pthread_mutex_init(&g_fifo[FIFO_TYPE_COM_CMD_CQ0].mutex, NULL);
    if (sts)
        printf("pthread_mutex_init error: %d\n", sts);
    g_fifo[FIFO_TYPE_COM_CMD_CQ0].front = 0;
    g_fifo[FIFO_TYPE_COM_CMD_CQ0].rear = 0;
    g_fifo[FIFO_TYPE_COM_CMD_CQ0].size = COM_CMD_CQ_FIFO_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_CQ0].entry_size = COM_CMD_CQ_FIFO_ENTRY_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_CQ0].ptr = tcm_get_ptr(SYS_MEM_TYPE_B1TCM, COM_CMD_CQ0_FIFO_IDX);
    printf("g_fifo[FIFO_TYPE_COM_CMD_CQ0].ptr: %8x\n", (u32)g_fifo[FIFO_TYPE_COM_CMD_CQ0].ptr);

    sts = pthread_mutex_init(&g_fifo[FIFO_TYPE_COM_CMD_CQ1].mutex, NULL);
    if (sts)
        printf("pthread_mutex_init error: %d\n", sts);
    g_fifo[FIFO_TYPE_COM_CMD_CQ1].front = 0;
    g_fifo[FIFO_TYPE_COM_CMD_CQ1].rear = 0;
    g_fifo[FIFO_TYPE_COM_CMD_CQ1].size = COM_CMD_CQ_FIFO_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_CQ1].entry_size = COM_CMD_CQ_FIFO_ENTRY_SIZE;
    g_fifo[FIFO_TYPE_COM_CMD_CQ1].ptr = tcm_get_ptr(SYS_MEM_TYPE_B1TCM, COM_CMD_CQ1_FIFO_IDX);
    printf("g_fifo[FIFO_TYPE_COM_CMD_CQ1].ptr: %8x\n", (u32)g_fifo[FIFO_TYPE_COM_CMD_CQ1].ptr);

    return 0;
}

int fifo_delete(void)
{
    return 0;
}

void fifo_unit_test(void)
{
    int i = 0;
#if 0
    printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
        fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
    while (!fifo_push_full(FIFO_TYPE_COM_CMD_SQ0)) {
        fifo_push(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
        fifo_pop(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
        fifo_push(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
        if (fifo_push_full(FIFO_TYPE_COM_CMD_SQ0)) {
            printf("FIFO_TYPE_COM_CMD_SQ0 full\n");
        }
    }

    printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
        fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
    while (!fifo_pop_full(FIFO_TYPE_COM_CMD_SQ0)) {
        fifo_pop(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %d %d\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0));
    }
#endif
    printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %3d %3d %8x %8x\n",
        fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0),
        (u32)fifo_get_push_ptr(FIFO_TYPE_COM_CMD_SQ0), (u32)fifo_get_pop_ptr(FIFO_TYPE_COM_CMD_SQ0));
    while (!fifo_push_full(FIFO_TYPE_COM_CMD_SQ0)) {
        fifo_push(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %3d %3d %8x %8x\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0),
            fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0),
            (u32)fifo_get_push_ptr(FIFO_TYPE_COM_CMD_SQ0), (u32)fifo_get_pop_ptr(FIFO_TYPE_COM_CMD_SQ0));
        if (fifo_push_full(FIFO_TYPE_COM_CMD_SQ0)) {
            printf("FIFO_TYPE_COM_CMD_SQ0 full\n");
        }
    }

    printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %3d %3d %8x %8x\n",
        fifo_push_num(FIFO_TYPE_COM_CMD_SQ0), fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0),
        (u32)fifo_get_push_ptr(FIFO_TYPE_COM_CMD_SQ0), (u32)fifo_get_pop_ptr(FIFO_TYPE_COM_CMD_SQ0));
    while (!fifo_pop_full(FIFO_TYPE_COM_CMD_SQ0)) {
        fifo_pop(FIFO_TYPE_COM_CMD_SQ0, 1);
        printf("fifo(FIFO_TYPE_COM_CMD_SQ0): %3d %3d %8x %8x\n",
            fifo_push_num(FIFO_TYPE_COM_CMD_SQ0),
            fifo_pop_num(FIFO_TYPE_COM_CMD_SQ0),
            (u32)fifo_get_push_ptr(FIFO_TYPE_COM_CMD_SQ0), (u32)fifo_get_pop_ptr(FIFO_TYPE_COM_CMD_SQ0));
    }
}