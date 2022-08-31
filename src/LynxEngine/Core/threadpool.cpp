#include <stdio.h>
#include <chrono>
#include "logger.h"
#include "threadpool.h"

namespace Lynx {

    void ThreadPool::thread_work(Worker* worker_s)
    {
        logger.RegisterThread(std::this_thread::get_id(), "WORKER");

        ThreadPool* pool = worker_s->pool;
        Job n_job;
        {
            std::unique_lock<std::mutex> lock(pool->mutex);
            pool->alive_threads++;
        }

        for (;;) {
            log_debug("Ready");

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->job.wait(lock);

                if(pool->shouldDestroy)
                    break;

                n_job = pool->jobs.front();
                pool->jobs.pop();
                pool->working_threads++;
                log_debug("Got job %d", n_job.id);
            }

            n_job.func(n_job.args);

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->working_threads--;
                if(!pool->working_threads)
                    pool->idle.notify_one();
                log_debug("Job %d done", n_job.id);
            }
            

        }

        
        std::unique_lock<std::mutex> lock(pool->mutex);
        pool->alive_threads--;
        log_debug("Destroyed");
        
    }
    

    ThreadPool::ThreadPool(int n_threads) : n_threads(n_threads)
    {
        for(int i = 0; i < n_threads; i++) {
            log_debug("Creating thread %d", i);
            Worker* worker = new Worker();
            worker->id = i;
            worker->pool = this;
            worker->thread = std::thread(thread_work, worker);
            workers.push_back(worker);
            thread_id_map[worker->thread.get_id()] = i;
        }

        log_debug("Alive thread count : %d", alive_threads);
        while(alive_threads != n_threads) {};
        ready = true;
    }


    ThreadPool::~ThreadPool()
    {
        puts("Destroying all threads...");

        shouldDestroy = true;


        job.notify_all();

        for(Worker* worker : workers) {
            worker->thread.join();
            free(worker);
        }

        workers.clear();
        
    }


    int ThreadPool::GetWorkerID(std::thread::id tid)
    {
        if(thread_id_map.find(tid) == thread_id_map.end())
            return -1;

        return thread_id_map[tid];
    }


    void ThreadPool::PushJob(std::function<void(void*)> func, void* job_args)
    {
        std::unique_lock<std::mutex> lock(mutex);
        t_jobs++;

        log_debug("Pushing job %d\n", t_jobs);

        jobs.push(Job{func, job_args, t_jobs});
        job.notify_one();


    }


    void ThreadPool::Wait()
    {
        
        std::unique_lock<std::mutex> lock(mutex);

        log_debug("Waiting until all threads are finished...");

        while(working_threads || jobs.size())
            idle.wait(lock);
        
        
    }

    bool IsMainThread(std::thread::id thread) {
        return thread == std::this_thread::get_id();
    }

}
