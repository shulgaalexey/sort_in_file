/**
 * job_manager.h
 */

#ifndef _JOB_MANAGER_H_INCLUDED_
#define _JOB_MANAGER_H_INCLUDED_

#include <queue>
#include <map>
#include <stdlib.h>
#include "scope_mutex.h"

/*----------------------------------------------------------------------------*/

class job {
public:
	job();
	virtual ~job();
public:
	virtual void run() = 0;
};

/*----------------------------------------------------------------------------*/

class sort_bucket_job : public job {
private:
	class bucket *_b;
public:
	sort_bucket_job(class bucket *b);
	virtual ~sort_bucket_job();
public:
	virtual void run();
};

/*----------------------------------------------------------------------------*/

class worker_manager {
	std::map<class job_manager *, int> _job_managers;
	std::map<pthread_t, int> _workers;
	volatile int _ref_cnt;
	const size_t _worker_number;
	const unsigned int _wait_for_job_ms;
public:
	worker_manager();
	virtual ~worker_manager();
public:
	static worker_manager *get_manager();
public:
	void start();
	void stop();
	void on_new_job();
public:
	void register_job_manager(class job_manager *mgr);
	void unregister_job_manager(class job_manager *mgr);
protected:
	void on_worker_free(pthread_t id);
	void on_worker_busy(pthread_t id);
protected:
	static void *_worker_thread(void *arg);
};

/*----------------------------------------------------------------------------*/

class job_manager {
	std::queue<job *> _jobs;
	std::queue<job *> _paused_jobs;
	std::map<job *, int> _running_jobs;

	mutex_object _jobs_queue_mutex;
	mutex_object _running_jobs_mutex;
public:
	job_manager();
	virtual ~job_manager();
public:
	void add(job *j);
	void start();
	void stop();
	void pause();
	void resume();
public:
	job *pop_job();
public:
	void start_job(job *j);
	void finish_job(job *j);
	size_t jobs_waiting();
};

#endif /* _JOB_MANAGER_H_INCLUDED_ */
