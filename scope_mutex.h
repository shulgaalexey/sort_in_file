/*
 * scope_mutex.h
 * Scope Mutex class
 *
 * RAII Class implementing the mutex helper on the base of libc mutex
 * which automatically locks mutex during its creation and unlocks while
 * exiting the scope
 */

#ifndef __SCOPE_MUTEX_H__
#define __SCOPE_MUTEX_H__

#include <pthread.h>

/**
 * The wrapper class over libc mutex structure
 */
class mutex_object {
	protected:
		pthread_mutex_t _m;
		pthread_mutexattr_t _attr;
	public:
		mutex_object();
		~mutex_object();
	public:
		operator pthread_mutex_t *();
};


/**
 * The class of fake mutex
 * It is useful when we don't need a synchronization of threads
 */
class no_mutex_object {
	public:
		operator pthread_mutex_t *() { return NULL; }
};

/**
 * Thescope mutex class
 */
class scope_mutex {
	protected:
		pthread_mutex_t *_m;
	public:
		scope_mutex(pthread_mutex_t *m);
		~scope_mutex();
	public:
		void halt();
};

#endif /* __SCOPE_MUTEX_H__ */
