/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "native_client/src/trusted/service_runtime/nacl_config.h"

/* This test syscall prints a test message and returns. */
#define TEST_SYSCALL_INVOKE 2
/* This test syscall prints a test message and exits. */
#define TEST_SYSCALL_EXIT 3

/* This test syscall return a integer parameter */
#define TEST_SYSCALL_GET_INT 4
/* This test syscall report true */
#define TEST_SYSCALL_REP_TRUE 5
/* This test syscall report false */
#define TEST_SYSCALL_REP_FALSE 6


#define UNTYPED_SYSCALL(s) ((int (*)()) NACL_SYSCALL_ADDR(s))


static void SimpleAbort(void) {
  while (1) {
    /* Exit by causing a crash. */
    *(volatile int *) 0 = 0;
  }
}

int isPrime(int num){
    if (num<=1){    // 0,1 are not prime numbers
        return 0; 
    }
    for (int i=2; i*i < num; i++){
        if (0==num%i){ // found a factor
            return 0;
        }
    }
    return 1;
}
void _start(void) {
  int retval = UNTYPED_SYSCALL(TEST_SYSCALL_INVOKE)();
  if (retval != 123) {
    /* This sandbox is so simple that we have no way of printing a
       failure message. */
    SimpleAbort();
  }
  int para = UNTYPED_SYSCALL(TEST_SYSCALL_GET_INT)();
  int ans = isPrime(para);
  // report ans 
  if (ans){
    UNTYPED_SYSCALL(TEST_SYSCALL_REP_TRUE)();
  }else{
    UNTYPED_SYSCALL(TEST_SYSCALL_REP_FALSE)();  
  }
  /* now it will be terminated*/
  UNTYPED_SYSCALL(TEST_SYSCALL_EXIT)();
  /* Should not reach here. */
  SimpleAbort();
}

