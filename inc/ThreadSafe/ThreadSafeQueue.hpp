#ifndef _THREAD_SAFE_QUEUE_HPP_
#define _THREAD_SAFE_QUEUE_HPP_

#include <queue>
#include <condition_variable>
#include <mutex>

namespace ThreadSafe{

template<typename T>
class ThreadSafeQueue{
public:
    ThreadSafeQueue() = default;

    void Push(T val){
        std::scoped_lock<std::mutex> lk(mutex_);
        data_queue_.push(std::move(val));
        data_cond_.notify_one();
    }

    // Notify that the producer thread stops, i.e. no more push operation 
    // void NotifyExit(){
    //     exit_ = true;
    //     data_cond_.notify_one();
    // }

    void WaitAndPop(T& val){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});
        val = std::move(data_queue_.front());
        data_queue_.pop();
    }

    // Add exit_ to avoid infinite waiting
    // void WaitAndPop(T& val){
    //     std::unique_lock<std::mutex> lk(mutex_);
    //     data_cond_.wait(lk, [this]{return (!data_queue_.empty()) || (exit_);});
    //     if(!exit_){
    //         val = std::move(data_queue_.front());
    //         data_queue_.pop();
    //     }
    // }

    std::shared_ptr<T> WaitAndPop(){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue_.front())));
        data_queue_.pop();
        return res;
    }

    // Add exit_ to avoid infinite waiting
    // std::shared_ptr<T> WaitAndPop(){
    //     std::unique_lock<std::mutex> lk(mutex_);
    //     data_cond_.wait(lk, [this]{return (!data_queue_.empty()) || (exit_);});
    //     if(!exit_){
    //         std::shared_ptr<T> res(
    //         std::make_shared<T>(std::move(data_queue_.front())));
    //         data_queue_.pop();
    //         return res;
    //     }else{
    //         return std::shared_ptr<T>();
    //     }
    // }

    // return true if pop is successful
    // return false if timeouts
    bool PopTimeout(T &val, const std::chrono::milliseconds &timeout){
        std::unique_lock<std::mutex> lk(mutex_);
        std::cv_status status;
        status = data_cond_.wait_for(lk, timeout);
        if(status == std::cv_status::timeout){
            return false;
        }else if(status == std::cv_status::no_timeout && !data_queue_.empty()){
            // The empty check is for spurious unblock
            val = std::move(data_queue_.front());
            data_queue_.pop();
            return true;
        }

        return false;
    }

    std::shared_ptr<T> PopTimeout(const std::chrono::milliseconds &timeout){
        std::unique_lock<std::mutex> lk(mutex_);
        std::cv_status status;
        status = data_cond_.wait_for(lk, timeout);
        if(status == std::cv_status::timeout){
            return std::shared_ptr<T>();
        }else if(status == std::cv_status::no_timeout && !data_queue_.empty()){
            // The empty check is for spurious unblock
            return std::make_shared<T>(std::move(data_queue_.front()));
        }
    }

    bool TryPop(T &val){
        std::scoped_lock<std::mutex> lk(mutex_);
        if(data_queue_.empty()){
            return false;
        }
        val = std::move(data_queue_.front());
        data_queue_.pop();
        return true;
    }

    std::shared_ptr<T> TryPop(){
        std::scoped_lock<std::mutex> lk(mutex_);
        if(data_queue_.empty()){
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue_.front())));
        data_queue_.pop();

        return res;
    }

    bool Empty() const {
        std::scoped_lock<std::mutex> lk(mutex_);
        return data_queue_.empty();
    }

    size_t Size() const {
        std::scoped_lock<std::mutex> lk(mutex_);
        return data_queue_.size();
    }
private:
    mutable std::mutex mutex_;
    std::queue<T> data_queue_;
    std::condition_variable data_cond_;
    // bool exit_ = false;
};

}

#endif