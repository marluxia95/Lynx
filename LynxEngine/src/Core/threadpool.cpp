#include <stdio.h>
#include <chrono>
#include "logger.h"
#include "threadpool.h"

namespace Lynx {

    void ThreadPool::thread_work(Worker* worker_s)
    {
        ThreadPool* pool = worker_s->pool;
        Job n_job;
        {
            std::unique_lock<std::mutex> lock(pool->mutex);
            log_debug("T%d : Active", worker_s->id);
            pool->alive_threads++;
        }


        for (;;) {
            log_debug("T%d : Waiting for jobs", worker_s->id);

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->job.wait(lock);

                if(pool->shouldDestroy)
                    break;

                n_job = pool->jobs.front();
                pool->jobs.pop();
                pool->working_threads++;
                log_debug("T%d : Got job %d", worker_s->id, n_job.id);
            }

            n_job.func(n_job.args);

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->working_threads--;
                if(!pool->working_threads)
                    pool->idle.notify_one();
                log_debug("T%d : Job %d done", worker_s->id, n_job.id);
            }
            

        }

        
        std::unique_lock<std::mutex> lock(pool->mutex);
        pool->alive_threads--;
        log_debug("T%d : Destroyed", worker_s->id);
        
    }
    

    ThreadPool::ThreadPool(int n_threads) : n_threads(n_threads)
    {
        for(int i = 0; i < n_threads; i++) {
            Worker* worker = new Worker();
            worker->id = i;
            worker->pool = this;
            worker->thread = std::thread(thread_work, worker);
            workers.push_back(worker);
            thread_id_map[worker->thread.get_id()] = i;
            printf("Created thread %d\n", i);
        }

        printf("Alive thread count : %d\n", alive_threads);
        while(alive_threads != n_threads) {};
        puts("All threads are ready!");
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

        printf("Pushing job %d\n", t_jobs);

        jobs.push(Job{func, job_args, t_jobs});
        job.notify_one();


    }


    void ThreadPool::Wait()
    {
        
        std::unique_lock<std::mutex> lock(mutex);

        puts("Waiting until all threads are finished...");

        while(working_threads || jobs.size())
            idle.wait(lock);
        
        
    }

    bool IsMainThread(std::thread::id thread) {
        return thread == std::this_thread::get_id();
    }

}