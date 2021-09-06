#include "threadpool.h"

namespace Lynx {

    void* ThreadPool::thread_work(Worker* worker_s)
    {
        ThreadPool* pool = worker_s->pool;

        while (true) {
            {
                std::unique_lock<std::mutex> lock(pool->mutex);

                pool->sem.wait();

            }

            
        }
    }

    ThreadPool::ThreadPool(int n_threads)
    {
        for(int i = 0; i < n_threads; i++) {
            Worker* worker = new Worker();
            worker->id = last_id++;
            worker->pool = this;
            worker->thread = std::thread(thread_work, worker);
            workers.push_back(worker);
        }
    }

}