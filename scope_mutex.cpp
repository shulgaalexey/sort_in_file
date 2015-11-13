/*
 * scope_mutex.cpp
 * Implementation of Scope Mutex class
 */

#include "scope_mutex.h"
#include <errno.h>
#include <iostream>
#include <sstream>
using namespace std;

static void __print_error(int error, const string &s = "")
{
	ostringstream oss;
	switch(error) {
		case 0:
			oss << "OK" << s << endl;
			break;
		case EAGAIN:
			oss << "EAGAIN [" << error << "] " << s << endl;
			break;
		case ENOMEM:
			oss << "ENOMEM [" << error << "] " << s << endl;
			break;
		case EPERM:
			oss << "EPERM [" << error << "] " << s << endl;
			break;
		case EBUSY:
			oss << "EBUSY [" << error << "] " << s << endl;
			break;
		case EINVAL:
			oss << "EINVAL [" << error << "] " << s << endl;
			break;
		case EDEADLK:
			oss << "EDEADLK [" << error << "] " << s << endl;
			break;
		default:
			oss << "UNDEFINED ERROR [" << error << "] " << s << endl;
			break;
	}
	cout << oss.str();
}

mutex_object::mutex_object()
{
	do {
		//pthread_mutex_init(&_m, &_attr);
		//pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);

		/* Create the mutex attribute */
		int error = pthread_mutexattr_init(&_attr);
		if(error) {
			__print_error(error, "pthread_mutexattr_init");
			break;
		}

		/* Set RECURSIVE type of mutex attribute */
		error = pthread_mutexattr_settype(&_attr,
				PTHREAD_MUTEX_RECURSIVE);
		if(error) {
			__print_error(error, "pthread_mutexattr_settype");
			break;
		}

		/* Init the mutex with prepared attributes */
		error = pthread_mutex_init(&_m, &_attr);
		if(error) {
			__print_error(error, "pthread_mutex_init");
			break;
		}

		/* Successfully initializes mutex */
		return;

	} while(false);
	cout << "ERROR! mutex_object constructor failed!" << endl;
}

mutex_object::~mutex_object()
{
	/*pthread_mutex_destroy(&_m);
	  pthread_mutexattr_destroy(&_attr);*/
	int error = pthread_mutex_destroy(&_m);
	if(error) {
		__print_error(error, "pthread_mutex_destroy");
	}

	error = pthread_mutexattr_destroy(&_attr);
	if(error) {
		__print_error(error, "pthread_mutexattr_destroy");
	}
}


mutex_object::operator pthread_mutex_t *()
{
	return &_m;
}

	scope_mutex::scope_mutex(pthread_mutex_t *m)
: _m(m)
{
	if(_m) {
		//pthread_mutex_lock(_m);
		const int error = pthread_mutex_lock(_m);
		if(error) {
			cout << "ERROR! pthread_mutex_lock: " << error << endl;
			__print_error(error);
		}
	}
}

scope_mutex::~scope_mutex()
{
	if(_m) {
		//pthread_mutex_unlock(_m);
		const int error = pthread_mutex_unlock(_m);
		if(error) {
			cout << "ERROR! pthread_mutex_unlock: " << error
				<< endl;
			__print_error(error);
		}
	}
	_m = NULL;
}

void scope_mutex::halt()
{
	if(_m) {
		//pthread_mutex_unlock(_m);
		const int error = pthread_mutex_unlock(_m);
		if(error) {
			cout << "ERROR! pthread_mutex_unlock: " << error
				<< endl;
			__print_error(error);
		}
	}
	_m = NULL;
}
