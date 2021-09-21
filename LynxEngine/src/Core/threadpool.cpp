#include <stdio.h>
#include <chrono>
#include "threadpool.h"

namespace Lynx {

    void ThreadPool::thread_work(Worker* worker_s)
    {
        ThreadPool* pool = worker_s->pool;
        Job n_job;

        {
            std::unique_lock<std::mutex> lock(pool->mutex);
            pool->alive_threads++;
        }


        for (;;) {
            printf("[%d] Waiting for jobs\n", worker_s->id);

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->job.wait(lock);

                if(pool->shouldDestroy)
                    break;

                n_job = pool->jobs.front();
                pool->jobs.pop();
                pool->working_threads++;
                printf("[%d] Got a job!\n", worker_s->id);
            }

            n_job.func(n_job.args);

            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->working_threads--;
                if(!pool->working_threads)
                    pool->idle.notify_one();
                printf("[%d] Job done!\n", worker_s->id);
            }
            

        }

        
        std::unique_lock<std::mutex> lock(pool->mutex);
        pool->alive_threads--;
        printf("[%d] Destroyed\n", worker_s->id);
        
    }
    

    ThreadPool::ThreadPool(int n_threads) : n_threads(n_threads)
    {
        for(int i = 0; i < n_threads; i++) {
            Worker* worker = new Worker();
            worker->id = i;
            worker->pool = this;
            worker->thread = std::thread(thread_work, worker);
            workers.push_back(worker);
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


    void ThreadPool::PushJob(std::function<void(void*)> func, void* job_args)
    {
        
        std::unique_lock<std::mutex> lock(mutex);

        printf("Pushing job %d\n", jobs.size());

        jobs.push(Job{func, job_args, (int)jobs.size()});
        job.notify_one();


    }


    void ThreadPool::Wait()
    {
        
        std::unique_lock<std::mutex> lock(mutex);

        puts("Waiting until all threads are finished...");

        while(working_threads || jobs.size())
            idle.wait(lock);
        
        
    }

}