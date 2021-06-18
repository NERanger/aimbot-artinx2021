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
    ThreadSafeQueue(size_t capacity) : capacity_(capacity){};

    void Push(T& new_val){
        std::lock_guard<std::mutex> lock(this->mut_);
        while (this->data_queue_.size() >= this->capacity_){
            this->data_queue_.pop();
        }
        this->data_queue_.push(new_val);
        this->data_cond_.notify_one();
    }

    void WaitAndPop(T& val){
        std::unique_lock<std::mutex> lock(this->mut_);
        data_cond_.wait(lock, [this]{
            return !this->data_queue_.empty();
        });
        val = this->data_queue_.front();
        this->data_queue_.pop();
    }

private:
    mutable std::mutex mut_;

    size_t capacity_ = 2;

    std::queue<T> data_queue_;
    std::condition_variable data_cond_;
};

}

#endif