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
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>
#define THPOOL_DEBUG 0

namespace Lynx {

    class ThreadPool {
        private:
            typedef struct {
                std::function<void(void*)> func;
                void* args;
                int id;
            } Job;

            class Worker {
                public:
                    std::thread thread;
                    int id;
                    ThreadPool* pool;
            };

            static void thread_work(Worker* worker_s);
        
        public:
            ThreadPool(int n_threads = std::thread::hardware_concurrency());
            ~ThreadPool();
            void PushJob(std::function<void(void*)>, void* job_args);
            void Wait();

        private:
            std::queue<Job> jobs;
            std::vector<Worker*> workers;
            std::mutex mutex;
            std::condition_variable idle;
            std::condition_variable job;
            std::atomic<bool> shouldDestroy = false;
            std::atomic<bool> ready = false;
            int n_threads;
            int alive_threads;
            int working_threads;

    };

}


#endif