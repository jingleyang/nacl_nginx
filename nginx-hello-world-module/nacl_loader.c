#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nacl_loader.h"

#include "native_client/src/shared/platform/nacl_check.h"
#include "native_client/src/shared/platform/nacl_log.h"
#include "native_client/src/trusted/service_runtime/include/bits/nacl_syscalls.h"
#include "native_client/src/trusted/service_runtime/load_file.h"
#include "native_client/src/trusted/service_runtime/nacl_all_modules.h"
#include "native_client/src/trusted/service_runtime/nacl_app.h"
#include "native_client/src/trusted/service_runtime/nacl_app_thread.h"
#include "native_client/src/trusted/service_runtime/nacl_copy.h"
#include "native_client/src/trusted/service_runtime/nacl_syscall_handlers.h"
#include "native_client/src/trusted/service_runtime/sel_ldr.h"
#include "native_client/tests/minnacl/minimal_test_syscalls.h"

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

static int g_para = 17;
static int g_ans = 0;
static pthread_t g_tid = 0;

static int32_t MySyscallGetInt(struct NaClAppThread *natp){
    NaClCopyDropLock(natp->nap);
    printf("nexe thread:%x requests parameter: %d\n",(int)natp->host_thread.tid,g_para);
    g_tid = natp->host_thread.tid;
    fflush(stdout);
    return g_para;
}

static int32_t MySyscallReportTrue(struct NaClAppThread *natp){
    NaClCopyDropLock(natp->nap);
    printf("nexe reports answer: true\n");
    fflush(stdout);
    g_ans=1;
    return 0;
}

static int32_t MySyscallReportFalse(struct NaClAppThread *natp){
    NaClCopyDropLock(natp->nap);
    printf("nexe reports answer: false\n");
    fflush(stdout);
    g_ans=0;
    return 0;
}

static int32_t NotImplementedDecoder(struct NaClAppThread *natp) {
  NaClCopyDropLock(natp->nap);
  printf("Error: entered an unexpected syscall!\n");
  fflush(stdout);
  _exit(1);
}

static int32_t MySyscallInvoke(struct NaClAppThread *natp) {
  NaClCopyDropLock(natp->nap);
  printf("Inside custom test 'invoke' syscall\n");
  fflush(stdout);
  /* Return a value that the test guest program checks for. */
  return 123;
}

static int32_t MySyscallExit(struct NaClAppThread *natp) {
  NaClCopyDropLock(natp->nap);
  printf("Inside custom test 'exit' syscall\n");
  fflush(stdout);
  NaClAppThreadTeardown(natp);
  return 0;
}


int get_para(char* para_buf){
    int ret=0;
    for (int i=0;para_buf[i];i++){
        if ('=' == para_buf[i] && 0 != para_buf[i+1]){
            ret = atoi(para_buf+i+1); 
        }
    }
    return ret;
}

int nacl_loader(const char* fname,char*para_buf,char* text_buf){
    char cwd[1024];
    struct NaClApp app;
    //struct NaClApp *nap = &app;
    struct NaClSyscallTableEntry syscall_table[NACL_MAX_SYSCALLS] = {{0}};
    int index;

    g_para = get_para(para_buf);
    g_ans=0;

    for (index = 0; index < NACL_MAX_SYSCALLS; index++) {
    syscall_table[index].handler = NotImplementedDecoder;
    }
    syscall_table[TEST_SYSCALL_INVOKE].handler = MySyscallInvoke;
    syscall_table[TEST_SYSCALL_EXIT].handler = MySyscallExit;
    syscall_table[TEST_SYSCALL_GET_INT].handler = MySyscallGetInt;
    syscall_table[TEST_SYSCALL_REP_TRUE].handler = MySyscallReportTrue;
    syscall_table[TEST_SYSCALL_REP_FALSE].handler = MySyscallReportFalse;

    NaClAllModulesInit();

    CHECK(NaClAppWithSyscallTableCtor(&app, syscall_table));
    CHECK(NaClAppLoadFileFromFilename(&app, fname) == LOAD_OK);
    printf("nexe will run\n");
    CHECK(NaClCreateMainThread(&app, 0, NULL, NULL));
  //ret=NaClWaitForMainThreadToExit(&app);  will lock
    sleep(1);
    sprintf(text_buf,"<html><body><h1>NaCl %s is loaded with parameter %d in path %s</h1>\n<h1>Whether number %d is prime? ans=%d</h1></body></html>",fname,g_para,getcwd(cwd,sizeof(cwd)),g_para,g_ans); 
    return 0;
}
