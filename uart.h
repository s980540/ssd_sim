#ifndef UART_H
#define UART_H

#include "global.h"

#if (OPT_USE_UART_INT)
#if (defined(SD_BICS5) || defined(SD_BICS5_128GB)) && (EN_TCG_OPAL)
#define CONFIG_UART_TX_BUF_SIZE     (956)
#else
#define CONFIG_UART_TX_BUF_SIZE     (1024)
#endif
#define CONFIG_UART_TX_FIFO_SIZE    (16)

#define TX_NEXT_IDX(cur_idx)        ((cur_idx + 1) % CONFIG_UART_TX_BUF_SIZE)
#define TX_ADVANCE_IDX(p)           do {p = TX_NEXT_IDX(p);} while (0)
#define TX_ADVANCE_HEAD_IDX()       TX_ADVANCE_IDX(TX_BUF_HEAD)
#define TX_ADVANCE_TAIL_IDX()       TX_ADVANCE_IDX(TX_BUF_TAIL)
#define TX_IS_EMPTY()               (TX_BUF_HEAD == TX_BUF_TAIL)
#define TX_IS_FULL()                (TX_NEXT_IDX(TX_BUF_TAIL+1) == TX_BUF_HEAD)
#define TX_PUT_CHAR(c)              do {g_uart_tx_buf[TX_BUF_TAIL] = c;} while (0)
#define TX_GET_CHAR(c)              do {c = g_uart_tx_buf[TX_BUF_HEAD];} while (0)
#endif

#endif // UART_H