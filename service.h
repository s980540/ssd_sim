#ifndef SERVICE_H
#define SERVICE_H

#define COM_CMD_SERVICE_MAX (16)

int common_command_service(int sq_fifo_idx, int cq_fifo_idx);

#endif