#ifndef FIFO_H
#define FIFO_H

#include "global.h"
#include "thread.h"

typedef enum _fifo_type_e
{
    FIFO_TYPE_WR = 0,
    FIFO_TYPE_RD = 1,
    FIFO_TYPE_CB,
    FIFO_TYPE_CMD0,
    FIFO_TYPE_CMD1,
    FIFO_TYPE_COM_CMD_CQ0,
    FIFO_TYPE_COM_CMD_SQ0,
    FIFO_TYPE_COM_CMD_SQ1,
    FIFO_TYPE_COM_CMD_CQ1,
    FIFO_TYPE_MAX_NUM
} fifo_type_e;

typedef enum _com_cmd_opcode_e
{
    COM_CMD_OPCODE_ECHO = 0,
    COM_CMD_OPCODE_ADD,
    COM_CMD_OPCODE_MUL,
    COM_CMD_OPCODE_MAX_NUM
} __attribute__((packed)) com_cmd_opcode_e;

typedef struct _fifo_t
{
    pthread_mutex_t mutex;
    u32 front;
    u32 rear;
    u32 size;
    u32 entry_size;
    void *ptr;
} fifo_t;

typedef struct _com_cmd_sq_format_add_t
{
    com_cmd_opcode_e opcode;
    u8 initiator_id;
    u16 param0;
    int a;
    int b;
    u32 sn;
} __attribute__((packed)) com_cmd_sq_format_add_t;

typedef struct _com_cmd_sq_entry_t
{
    com_cmd_opcode_e opcode;
    u8 callback;
    u16 param0;
    u32 param1;
    u32 param2;
    u32 param3;
} __attribute__((packed)) com_cmd_sq_entry_t;

#define FIFO_PUSH(fifo_idx)     (fifo_push(fifo_idx, 1))
#define FIFO_POP(fifo_idx)      (fifo_pop(fifo_idx, 1))
#define FIFO_PUSH_NUM(fifo_idx) (fifo_push_num(fifo_idx))
#define FIFO_POP_NUM(fifo_idx)  (fifo_pup_num(fifo_idx))

// extern fifo_t g_fifo[FIFO_MAX];
extern int fifo_lock(int fifo_idx);
extern int fifo_unlock(int fifo_idx);
extern int fifo_push(int fifo_idx, u32 add);
extern int fifo_pop(int fifo_idx, u32 add);
extern u32 fifo_push_num(int fifo_idx);
extern u32 fifo_pop_num(int fifo_idx);
extern int fifo_push_full(int fifo_idx);
extern int fifo_pop_full(int fifo_idx);
extern void *fifo_get_push_ptr(int fifo_idx);
extern void *fifo_get_pop_ptr(int fifo_idx);
extern int fifo_init(void);
extern int fifo_deinit(void);
extern void fifo_unit_test(void);

#endif // FIFO_H