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

    void WaitAndPop(T& val){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});
        val = std::move(data_queue_.front());
        data_queue_.pop();
    }

    // Add if_wait to avoid infinite waiting
    void WaitAndPop(T& val, bool if_wait){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return (!data_queue_.empty()) || (!if_wait);});
        if(if_wait){
            val = std::move(data_queue_.front());
            data_queue_.pop();
        }
    }

    std::shared_ptr<T> WaitAndPop(){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue_.front())));
        data_queue_.pop();
        return res;
    }

    // Add if_wait to avoid infinite waiting
    std::shared_ptr<T> WaitAndPop(bool if_wait){
        std::unique_lock<std::mutex> lk(mutex_);
        data_cond_.wait(lk, [this]{return (!data_queue_.empty()) || (!if_wait);});
        if(if_wait){
            std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue_.front())));
            data_queue_.pop();
            return res;
        }else{
            return std::shared_ptr<T>();
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
};

}

#endif