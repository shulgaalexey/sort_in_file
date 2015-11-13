/**
 * job_manager.cpp
 */

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include "job_manager.h"
#include "bucket.h"


/*----------------------------------------------------------------------------*/

job::job()
{
}

job::~job()
{
}

/*----------------------------------------------------------------------------*/

sort_bucket_job::sort_bucket_job(class bucket *b)
	: _b(b)
{
}

sort_bucket_job::~sort_bucket_job()
{
}

void sort_bucket_job::run()
{
	/* Sort the bucket */
	if(_b)
		_b->sort();
}

/*----------------------------------------------------------------------------*/

pthread_mutex_t __wrx_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile bool __working = true; /* TODO: it must be a member variable */

pthread_cond_t __job_cond  = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t __job_mutex = PTHREAD_MUTEX_INITIALIZER;

worker_manager::worker_manager()
	: _ref_cnt(0)
	  , _worker_number(3)
	  , _wait_for_job_ms(100)
{
	/* Use it if we want to have workers awailable always */
	start(); /* I beleive it is a good idea, but tests show opposite */
}

worker_manager::~worker_manager()
{
	stop();
}

worker_manager *worker_manager::get_manager()
{
	static worker_manager mgr;
	return &mgr;
}

void worker_manager::start()
{
	pthread_mutex_lock(&__wrx_mutex);
	const int cur_ref_cnt = _ref_cnt;
	_ref_cnt ++;
	pthread_mutex_unlock(&__wrx_mutex);

	/* Start threads, if needed */
	if(cur_ref_cnt == 0) {

		/* Notify all threads, that it is time to start working */
		pthread_mutex_lock(&__wrx_mutex);
		__working = true;
		pthread_mutex_unlock(&__wrx_mutex);

		for(size_t i = 0; i < _worker_number; i ++) {
			pthread_t _thread = 0;
			int ret = pthread_create(&_thread,
						 NULL,
						 _worker_thread,
						 this);
			if(ret) {
				std::cout << "ERROR! pthread_create() return code: "
					<<  ret << std::endl;
				break;
			}
		}
	}
}

void worker_manager::stop()
{
	pthread_mutex_lock(&__wrx_mutex);
	_ref_cnt --;
	const int cur_ref_cnt = _ref_cnt;
	pthread_mutex_unlock(&__wrx_mutex);

	/* Release threads, if needed */
	if(cur_ref_cnt == 0) {
		/* Notify all threads, that it is time to stop */
		pthread_mutex_lock(&__wrx_mutex);
		__working = false;
		pthread_mutex_unlock(&__wrx_mutex);

		/* Wait for threads finish */
		for(std::map<pthread_t, int>::iterator it = _workers.begin();
		    it != _workers.end(); ++it)
			pthread_join(it->first, NULL);
	}

	/* Correcting ref count in case of wrong multiple decrement */
	pthread_mutex_lock(&__wrx_mutex);
	if(_ref_cnt < 0)
		_ref_cnt = 0;
	pthread_mutex_unlock(&__wrx_mutex);
}

void worker_manager::register_job_manager(class job_manager *mgr)
{
	_job_managers[mgr] = 1;
}

void worker_manager::unregister_job_manager(class job_manager *mgr)
{
	_job_managers[mgr] = 0;
}

void *worker_manager::_worker_thread(void *arg)
{
	worker_manager *wm = (worker_manager *)arg;
	if(!arg)
		return NULL;

	const pthread_t my_id = pthread_self();
	wm->on_worker_free(my_id);

	while(true) {
		/* Check if we should work now */
		pthread_mutex_lock(&__wrx_mutex);
		const bool working = __working;
		pthread_mutex_unlock(&__wrx_mutex);

		if(!working)
			break;

#if 0
		/* Wait for a job */
		bool must_work = false;
		pthread_mutex_lock(&__job_mutex);

		struct timeval tv;
		struct timespec ts;

		gettimeofday(&tv, NULL);
		ts.tv_sec = time(NULL) + wm->_wait_for_job_ms / 1000;
		ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000
			* (wm->_wait_for_job_ms % 1000);
		ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
		ts.tv_nsec %= (1000 * 1000 * 1000);

		/* Attempt to realise whether must work */
		int rt = pthread_cond_timedwait(&__job_cond,
						&__job_mutex,
						&ts);
		if(rt == 0)
			must_work = true;
		else if(rt == ETIMEDOUT) {
			//std::cout << "keep waiting for __job_cond" << std::endl;
		} else
			std::cout << "ERROR of timedwait" <<std::endl;

		pthread_mutex_unlock(&__job_mutex);
#endif

		bool must_work = true;
		if(must_work) {
			/* Let the manager know that I am busy */
			wm->on_worker_busy(my_id);


			/* Extract the current job */
			job *j = NULL;
			job_manager *mgr = NULL;
			for(std::map<job_manager *, int>::iterator it =
			    wm->_job_managers.begin();
			    it != wm->_job_managers.end(); ++it) {
				if(it->second != 1)
					continue;

				mgr = it->first;
				if(!mgr)
					continue;

				j = mgr->pop_job();
				break;
			}


			/* Perform the job */
			if(j && mgr) {
				j->run();
				mgr->finish_job(j);
				delete j;
			} else {
				/* We are done */

				/* Let the manager know that I am free */
				wm->on_worker_free(my_id);
			}

			/* Let the manager know that I am free */
			wm->on_worker_free(my_id);
		}
	}

	return NULL;
}

void worker_manager::on_worker_busy(pthread_t worker_id)
{
	_workers[worker_id] = 2;
}

void worker_manager::on_worker_free(pthread_t worker_id)
{
	_workers[worker_id] = 1;
}

void worker_manager::on_new_job()
{
#if 0
	/* Mark that new Job is available */
	pthread_mutex_lock(&__job_mutex);
	pthread_cond_signal(&__job_cond);
	pthread_mutex_unlock(&__job_mutex);
#endif
}

/*----------------------------------------------------------------------------*/

job_manager::job_manager()
{
	worker_manager::get_manager()->register_job_manager(this);
}

job_manager::~job_manager()
{
	worker_manager::get_manager()->unregister_job_manager(this);

	{
		scope_mutex sm(_jobs_queue_mutex);
		while(!_jobs.empty()) {
			job *j = _jobs.front();
			_jobs.pop();
			if(j)
				delete j;
		}

		while(!_paused_jobs.empty()) {
			job *j = _paused_jobs.front();
			_paused_jobs.pop();
			if(j)
				delete j;
		}
	}
}

void job_manager::add(job *j)
{
	if(!j)
		return;

	{
		scope_mutex sm(_jobs_queue_mutex);
		/* Add the Job to the Queue */
		_jobs.push(j);
	}

	/* Notify the Worker Manager, that the new Job is available */
	worker_manager::get_manager()->on_new_job();
}

void job_manager::start()
{
	worker_manager::get_manager()->start();

	usleep(0 * 1000);
}

void job_manager::stop()
{
	usleep(0 * 1000);

	while(true) {
		const size_t jobs_number = jobs_waiting();
		if(jobs_number <= 0) {
			break;
		}
		usleep(100 * 1000);
	}

	usleep(0 * 1000);

	/* Release all jobs from the queue */
	worker_manager::get_manager()->stop();
}

void job_manager::pause()
{
	scope_mutex sm(_jobs_queue_mutex);
	/* Save all jobs in the pause-queue */
	while(!_jobs.empty()) {
		job *j = _jobs.front();
		_jobs.pop();
		if(j)
			_paused_jobs.push(j);
	}
}

void job_manager::resume()
{
	scope_mutex sm(_jobs_queue_mutex);
	/* Resume all paused jobs */
	while(!_paused_jobs.empty()) {
		job *j = _paused_jobs.front();
		_paused_jobs.pop();
		if(j)
			_jobs.push(j);
	}
}

job *job_manager::pop_job()
{
	scope_mutex sm(_jobs_queue_mutex);

	if(_jobs.empty()) {
		return NULL;
	}

	/* Return the first job in the queue */
	job *j = _jobs.front();
	_jobs.pop();

	start_job(j);

	/* Check if we finished the whole work */
	//const bool finished = _jobs.empty();

	return j;
}

void job_manager::start_job(job *j)
{
	if(!j)
		return;

	scope_mutex sm(_running_jobs_mutex);
	_running_jobs[j] = 1;
}

void job_manager::finish_job(job *j)
{
	if(!j)
		return;

	scope_mutex sm(_running_jobs_mutex);
	_running_jobs.erase(j);
}

size_t job_manager::jobs_waiting()
{
	scope_mutex sm(_running_jobs_mutex);
	const size_t number = _running_jobs.size();
	return number;
}


