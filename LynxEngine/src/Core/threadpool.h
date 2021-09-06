/**
 * @file threadpool.h
 * @author marlxx
 * @brief Thread pool for engine. WORK IN PROGRESS
 * @version 0.0.3
 * @date 2021-09-06
 * 
 * @copyright Copyright (c) 2021. See LICENSE for more information
 * 
 */
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <condition_variable>

namespace Lynx {

    class ThreadPool {
        private:
            class Semaphore {
                public:
                    Semaphore(int val)
                    {
                        ThreadPool::Semaphore::v = val;
                        
                    }
                    Semaphore()
                    {
                        ThreadPool::Semaphore::v = 0;
                        
                    }
                    void reset()
                    {
                        ThreadPool::Semaphore::v = 0;
                    }
                    void notify()
                    {
                        mutex.lock();
                        ThreadPool::Semaphore::v = 1;
                        cond.notify_one();
                        mutex.unlock();
                    }
                    void notify_all()
                    {
                        mutex.lock();
                        ThreadPool::Semaphore::v = 1;
                        cond.notify_all();
                        mutex.unlock();
                    }
                    void wait()
                    {
                        mutex.lock();
                        while( ThreadPool::Semaphore::v < 1 ) {
                            std::unique_lock<std::mutex> lock(mutex);
                            cond.wait(lock, []{return ThreadPool::Semaphore::v != 0;});
                        }
                        ThreadPool::Semaphore::v = 0;
                        mutex.unlock();
                    }
                private:
                    std::mutex mutex;
                    std::condition_variable cond;
                    static int v;
            };

            typedef struct {
                void (*func)(void* args);
                void* args;
            } Job;

            typedef struct {
                std::thread thread;
                unsigned int id;
                ThreadPool* pool;
            } Worker;

            static void* thread_work(Worker* worker_s);
        
        public:
            ThreadPool(int n_threads = std::thread::hardware_concurrency());
            ~ThreadPool();
            void PushJob(void (*job_func)(void*), void* job_args);
            void Wait();

        private:
            std::list<Job> jobs;
            std::vector<Worker*> workers;
            std::mutex mutex;
            std::condition_variable cond;
            Semaphore sem;
            unsigned int last_id;

    };

}


#endif