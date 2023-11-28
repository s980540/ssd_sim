#include "service.h"
#include "fifo.h"

// Handle Command Command, FIFO_TYPE_COM_CMD_SQ0, FIFO_TYPE_COM_CMD_SQ1
int common_command_service(int sq_fifo_idx, int cq_fifo_idx)
{
     com_cmd_sq_entry_t *entry;
    int service_cnt = 0;
    if (((sq_fifo_idx != FIFO_TYPE_COM_CMD_SQ0) && (sq_fifo_idx != FIFO_TYPE_COM_CMD_SQ1)) ||
        ((cq_fifo_idx != FIFO_TYPE_COM_CMD_CQ0) && (cq_fifo_idx != FIFO_TYPE_COM_CMD_CQ1)))
        return -1;

    while (!fifo_pop_full(sq_fifo_idx)) {
        if (fifo_push_full(cq_fifo_idx))
            break;

        entry = fifo_get_pop_ptr(sq_fifo_idx);
        switch (entry->opcode) {
        case COM_CMD_OPCODE_ADD:
        {
            int a, b, sn, initiator_id;
            initiator_id = ((com_cmd_sq_format_add_t *)entry)->initiator_id;
            sn = ((com_cmd_sq_format_add_t *)entry)->sn;
            a = ((com_cmd_sq_format_add_t *)entry)->a;
            b = ((com_cmd_sq_format_add_t *)entry)->b;
            printf("[Thread 2]: initiator: %d, sn: %d, %d + %d = %d\n",
                initiator_id, sn, a, b, a + b);
            break;
        }
        }
        fifo_pop(sq_fifo_idx, 1);
        service_cnt++;
        if (service_cnt >= COM_CMD_SERVICE_MAX)
            break;
    }
}