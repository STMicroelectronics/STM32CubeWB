/**
 ******************************************************************************
  * File Name          : ring_buffer.c
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

/* "SPECIFICATIONS"
 - support any type as an element (with any size): not for now, only char
 - no dynamic allocation
 - buffer size can be changed by user
 - support adding multiple elements (string)
 - support removing multiple elements (string?)

*/

/* Includes ------------------------------------------------------------------*/
#include "app_conf.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "stm_logging.h"
#include "ring_buffer.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

// WARNING: be careful to protect a buffer operation by critical section if it
// can be accessed in interruption too

// Private function to move an index one step forward
static inline void bufIncIndex(const Buffer *buf, uint32_t *index){
  *index = (*index + 1) % buf->mem_size;
}

// Initialize buffer structure, must be called first (see BUF_ALLOC)
void bufInit(Buffer *buf, uint32_t size){
  buf->idx_rd = 0;
  buf->idx_wr = 0;
  buf->mem_size = size;
  buf->data_size = 0;
}

// Test if buffer is empty
bool bufIsEmpty(const Buffer *buf){
  return (0 == buf->data_size);
}

// Test if buffer is full
bool bufIsFull(const Buffer *buf){
  return (buf->mem_size == buf->data_size);
}

// Store one byte in buffer
// Call ignored if buffer already full
void bufPutChar(Buffer *buf, char character){
  if(! bufIsFull(buf)){
    buf->memory[buf->idx_wr] = character;
    bufIncIndex(buf, &(buf->idx_wr));
    (buf->data_size)++;
  }
}

// Store a string in buffer (without '\0')
// When buffer is full, remaining bytes are dropped
void bufPutString(Buffer *buf, const char *string){
  uint32_t freeSize = buf->mem_size - buf->data_size;
  uint32_t addSize = MIN(strlen(string), freeSize);
  uint32_t toEndSize = buf->mem_size - buf->idx_wr;
  uint32_t cp1Size = addSize;
  uint32_t cp2Size = 0;
  if (addSize > toEndSize){
    cp1Size = toEndSize;
    cp2Size = addSize - toEndSize;
  }
  memcpy(&(buf->memory[buf->idx_wr]), string, cp1Size);
  if (cp2Size == 0){
    buf->idx_wr = (buf->idx_wr + cp1Size) % buf->mem_size;
  }else{
    memcpy(&(buf->memory[0]), &(string[cp1Size]), cp2Size);
    buf->idx_wr = cp2Size;
  }
  buf->data_size += addSize;
}

// Retrieve one byte from buffer
// Buffer must not be empty (check with bufIsEmpty())
char bufGetChar(Buffer *buf){
  char character;
  character = buf->memory[buf->idx_rd];
  bufIncIndex(buf, &(buf->idx_rd));
  (buf->data_size)--;
  return character;
}

// Retrieve at most size chars from buffer, return actual count copied
uint32_t bufGetMultiChar(Buffer *buf, char *dest, uint32_t size){
  uint32_t count = 0;
  while (! bufIsEmpty(buf) && (count < size)){
    *dest = bufGetChar(buf);
    dest++;
    count++;
  }
  return count;
}

#define TEST(EXP) \
  do {							    \
    if(!(EXP)) {					    \
      APP_DBG("Test failed on line %d: " #EXP, __LINE__);   \
      all_passed = false;				    \
    }							    \
  } while (0)

#define BUF_TEST_SIZE 128

// Test buffer code
void bufTest(void){
  uint32_t i;
  bool all_passed = true;
  char bufMem[sizeof(Buffer) + BUF_TEST_SIZE];
  Buffer *buf = (Buffer *)&bufMem;

  APP_DBG("entering %s()", __func__);
  bufInit(buf, BUF_TEST_SIZE);
  TEST(bufIsEmpty(buf));

  bufPutChar(buf, 'a');
  TEST(!bufIsEmpty(buf));
  TEST(!bufIsFull(buf));
  TEST('a' == bufGetChar(buf));
  TEST(bufIsEmpty(buf));

  TEST(bufIsEmpty(buf));
  const char testString[] = "ST Microelectronics";
  char checkString[64];
  i = 0;
  bufPutString(buf, testString);
  while(!bufIsEmpty(buf)){
    checkString[i] = bufGetChar(buf);
    i++;
  }
  checkString[i] = '\0';
  TEST(0 == strcmp(testString, checkString));

  for(uint32_t i = 0; i<buf->mem_size; i++){
    bufPutChar(buf, 'a');
  }
  TEST(!bufIsEmpty(buf));
  TEST(bufIsFull(buf));

  // Check that new entries are ignored when full
  bufPutChar(buf, 'b');

  i = 0;
  while(!bufIsEmpty(buf)){
    TEST('a' == bufGetChar(buf));
    i++;
  }
  TEST(buf->mem_size == i);

  // Check interleaved access
  TEST(bufIsEmpty(buf));
  bufPutChar(buf, 'a');
  bufPutChar(buf, 'b');
  TEST('a' == bufGetChar(buf));
  bufPutChar(buf, 'c');
  TEST('b' == bufGetChar(buf));
  TEST('c' == bufGetChar(buf));
  TEST(bufIsEmpty(buf));

  // Check bufPutString without rollover
  bufInit(buf, BUF_TEST_SIZE);
  const char testString2[] = "ring buffer test string";
  bufPutString(buf, testString2);
  i = 0;
  while(!bufIsEmpty(buf)){
    checkString[i] = bufGetChar(buf);
    i++;
  }
  checkString[i] = '\0';
  TEST(0 == strcmp(testString2, checkString));

// Check bufPutString with rollover
  bufInit(buf, BUF_TEST_SIZE);
  const char testString3[] = "ring buffer rollover test string";
  for (i=0; i < (BUF_TEST_SIZE - 10); i++){
    bufPutChar(buf, 'm');
  }
  for (i=0; i < 30; i++){
    bufGetChar(buf);
  }
  bufPutString(buf, testString3);
  for (i=0; i < (BUF_TEST_SIZE -10 -30); i++){
    bufGetChar(buf);
  }
  i = 0;
  while(!bufIsEmpty(buf)){
    checkString[i] = bufGetChar(buf);
    i++;
  }
  checkString[i] = '\0';
  APP_DBG(checkString);
  TEST(0 == strcmp(testString3, checkString));

  if(all_passed){
    APP_DBG("Ring buffer: all tests passed");
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
