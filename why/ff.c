


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define MAX_TRACE_THREADS     100
#define NOFF    __attribute__((__no_instrument_function__))
#define PRINTF(X, ...)\
    do {                                \
        if (log_level) {                \
            log_printf(__FILE__, __LINE__, __func__, X, ##__VA_ARGS__);\
        }                               \
    } while(0)
#define GETTID()    syscall(SYS_gettid)

void NOFF Trace_Out(void);
static void NOFF log_printf(const char * file, int line, const char * func, const char * fmt, ...);
static void NOFF handle_sig(int sig, siginfo_t * info, void * arg);
static void NOFF sighandler(int sig);
static int              log_level   = 0;
static int              inited      = 0;
static pthread_mutex_t  ffmutex;
static pthread_mutex_t  logmutex;

typedef struct  _DEBUGNODE
{
    struct _DEBUGNODE   *   prev;
    struct _DEBUGNODE   *   next;
    void                *   func;
    void                *   call_site;
}DEBUGNODE;

static struct _NODEHEADS
{
    pthread_t       tid;
    DEBUGNODE   *   head;
    DEBUGNODE   *   tail;
}nodes[MAX_TRACE_THREADS];

static void NOFF init(void)
{

    inited = 1;
    pthread_mutex_init(&ffmutex, NULL);
    pthread_mutex_init(&logmutex, NULL);

    char    *   env = getenv("FF_LOGLEVEL");
    if(env != NULL)
    {
        log_level = atoi(env);
    }else{
        log_level = 0;
    }
    memset(&nodes, 0, sizeof(nodes));
    PRINTF("INIT FF!\n");


    struct  sigaction   sa;
    sa.sa_flags     = SA_SIGINFO;
    sa.sa_handler   = SIG_DFL;
    sa.sa_sigaction = handle_sig;
    sigaction(SIGSEGV, &sa, 0);
    signal(SIGINT, sighandler);

}

static void NOFF log_printf(const char * file, int line, const char * func, const char * fmt, ...)
{
    if(!inited)
        init();
    pthread_mutex_lock(&logmutex);
    va_list arglist;
    if(file == NULL || func == NULL || fmt == NULL)
    {
        return;
    }
    const char *  p = strrchr(file, '\\');
    if(p == NULL)
    {
        p = file;
    }else{
        p ++;
    }

    time_t  t       = time(NULL);
    struct tm * ctm = localtime(&t);

    printf("<FF> [%02d:%02d:%02d] [%s:%05d:%s]  ", ctm->tm_hour, ctm->tm_min, 
        ctm->tm_sec, p, line, func);
    va_start(arglist, fmt);
    vfprintf(stdout, fmt, arglist);
    va_end(arglist);
    pthread_mutex_unlock(&logmutex);
}

static void NOFF add_node(int threadid, void * func, void * call_site) 
{
    int     i;
    int     index = -1;
    for(i = 0; i < MAX_TRACE_THREADS; i ++)
    {
        if(nodes[i].tid == 0 && index == -1)
        {
            index = i;
        }
        if(nodes[i].tid == threadid)
        {
            index = i;
            break;
        }
    }
    
    if(index == -1)
    {
        PRINTF("Max trace threads got. no more trace.\n");
        return;
    }

    DEBUGNODE   *   node = malloc(sizeof(DEBUGNODE));
    if(node == NULL)
    {
        PRINTF("malloc failed.\n");
        return;
    }
    memset(node, 0, sizeof(DEBUGNODE));
    node->func      = func;
    node->call_site = call_site;
    node->prev      = NULL;
    node->next      = NULL;

    nodes[index].tid = threadid;
    if(nodes[index].head == NULL)
    {
        nodes[index].head = node;
    }else{
        nodes[index].tail->next = node;
        node->prev = nodes[index].tail;
    }
    nodes[index].tail = node;

}

static void NOFF del_node(int threadid, void * func, void * call_site) 
{
    int     i;
    for(i = 0; i < MAX_TRACE_THREADS; i ++)
    {
        if(nodes[i].tid == threadid)
        {
            break;
        }
    }
    
    if(i >= MAX_TRACE_THREADS)
    {
        PRINTF("No such thread.\n");
        return;
    }

    DEBUGNODE   *   node = nodes[i].tail;
    if(node == NULL)
    {
        printf("stack is destroyed!\n");
        Trace_Out();
        return;
    }
    if(node->func != nodes[i].tail->func)
    {
        printf("stack is destroyed!\n");
        Trace_Out();
        return;
    }

    node = node->prev;
    free(nodes[i].tail);
    nodes[i].tail = node;
    if(node == NULL)
    {
        nodes[i].head   = NULL;
        nodes[i].tid    = 0;
    }else{
        node->next      = NULL;
    }

}


///////////////////////////////////////////////////////////////////////

void NOFF Trace_Out(void)
{
    int             i;
    DEBUGNODE   *   node;
    printf("------------------TRACE OUT------------------\n");
    printf("call thread: %lu\n", GETTID());
    for(i=0; i<MAX_TRACE_THREADS; i++)
    {
        if(nodes[i].tid != 0)
        {
            printf("[TRACE] thread %lu:\n", nodes[i].tid);
            node = nodes[i].head;
            while(node != NULL)
            {
                printf("[TRACE] func: %p\t\tcallsite: %p\n", node->func, node->call_site);
                node = node->next;
            }
        }
    }
    printf("------------------TRACE END------------------\n");
}

void NOFF __cyg_profile_func_enter(void * this_func, void * call_site) 
{
    if(!inited) init();
    PRINTF("func = %p, callsite = %p\n", this_func, call_site);
    pthread_mutex_lock(&ffmutex);
    int tid = GETTID();
    add_node(tid, this_func, call_site);
    pthread_mutex_unlock(&ffmutex);
}

void NOFF __cyg_profile_func_exit(void * this_func, void * call_site)
{
    if(!inited) init();
    PRINTF("func = %p, callsite = %p\n", this_func, call_site);
    int tid = GETTID();
    pthread_mutex_lock(&ffmutex);
    del_node(tid, this_func, call_site);
    pthread_mutex_unlock(&ffmutex);
}

static void NOFF handle_sig(int sig, siginfo_t * info, void * arg)
{
    printf("Get signal %d\n", sig);
    Trace_Out();
    exit(0);
}

static void NOFF sighandler(int sig)
{
    printf("Get signal %d\n", sig);
    Trace_Out();
    exit(0);
}

#if 0
void foo()
{
    char    *p = NULL;

    while(1)
    {
        ;
    }
    *p = 'a';
}

int main()
{
    struct  sigaction   sa;
    sa.sa_flags     = SA_SIGINFO;
    sa.sa_handler   = SIG_DFL;
    sa.sa_sigaction = handle_sig;
    sigaction(SIGSEGV, &sa, 0);

    signal(SIGINT, sighandler);

    foo();
    return 0;
}

#endif


