/**
 ******************************************************************************
  * File Name          : ring_buffer.h
  * Description        : utility to provide ring buffer component
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t idx_rd;      // index of the next element to read
  uint32_t idx_wr;      // index of the next element to write
  uint32_t data_size;   // current number of elements
                        // (needed to be able to store mem_size elements)
  uint32_t mem_size;    // size of memory[]
  char memory[];        // user data starts at this point
}Buffer;

/* Buffer memory should be allocated with:
BUF_ALLOC(memoryVariable, BUFFER_SIZE);
Buffer *userBufferPointer = (Buffer *)&memoryVariable;
*/

// Allocate memory for buffer, with alignment guarantee
#define BUF_ALLOC(variable, size) \
  static void *(variable)[(sizeof(Buffer) + (size)) / (sizeof(void *)) + 1]

void bufInit(Buffer *buf, uint32_t size);

bool bufIsEmpty(const Buffer *buf);
bool bufIsFull(const Buffer *buf);

void bufPutChar(Buffer *buf, char character);
void bufPutString(Buffer *buf, const char *string);

char bufGetChar(Buffer *buf);
uint32_t bufGetMultiChar(Buffer *buf, char *dest, uint32_t size);

void bufTest(void);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
