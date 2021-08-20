//
//  threadPool.hpp
//  Assignment_3
//
//  Created by Filippo Camoli on 13/05/21.
//

#ifndef threadPool_hpp
#define threadPool_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

namespace pipeline3D {

    class ThreadPool{
    public:
        
        ThreadPool(int nThreads = std::thread::hardware_concurrency()){
            
            jobs_loaded = false;
            
            if(!nThreads)
                nThreads = 1;
            
            for(int i = 0; i < nThreads; ++i){
                threads.push_back(std::thread ([this](){
                    while(true){
                        std::unique_ptr<std::function<void()>> scheduledJob { nullptr };
                        {
                            std::unique_lock<std::mutex> lock(jobs_queue_mutex);
                            jobs_condition.wait(lock, [this]() {
                                if(!jobs_queue.empty() || (jobs_loaded && jobs_queue.empty()))
                                    return true;
                                else
                                    return false;
                            });
                            if(jobs_loaded && jobs_queue.empty())
                                break;
                            scheduledJob = std::move(jobs_queue.front());
                            jobs_queue.pop();
                        }
                        jobs_condition.notify_one();
                        (*scheduledJob)();
                    }
                }));
            }
        }
        
        ~ThreadPool(){}
        
        void joinAll(){
            for(std::thread &t : threads)
                t.join();
        }
        
        void jobsLoaded(){
            {
                std::unique_lock<std::mutex> lock(jobs_queue_mutex);
                jobs_loaded = true;
            }
            jobs_condition.notify_all();
        }
        
        void addJob(std::function<void()> j){
            
            std::unique_ptr<std::function<void()>> jobReference = std::make_unique<std::function<void()>>(std::move(j));
            {
                std::unique_lock<std::mutex> addJobLock(jobs_queue_mutex);
                jobs_queue.push(std::move(jobReference));
            }
            jobs_condition.notify_one();
        }
        
    private:
        bool jobs_loaded;
        std::queue<std::unique_ptr<std::function<void()>>> jobs_queue;
        std::mutex jobs_queue_mutex;
        std::condition_variable jobs_condition;
        std::vector<std::thread> threads;
    };
    
}

#endif /* threadPool_hpp */
