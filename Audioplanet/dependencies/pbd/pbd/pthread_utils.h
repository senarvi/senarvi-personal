#ifndef __pbd_pthread_utils__
#define __pbd_pthread_utils__

#include <pthread.h>
#include <string>

int  pthread_create_and_store (std::string name, pthread_t  *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void * arg);
void pthread_cancel_one (pthread_t thread);
void pthread_kill_all (int signum);
void pthread_cancel_all ();

#endif /* __pbd_pthread_utils__ */
