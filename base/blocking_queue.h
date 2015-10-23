// @author: Eric(wujiabin@baidu.com)

#ifndef LOG_REPLAYER_BLOCKING_QUEUE_H
#define LOG_REPLAYER_BLOCKING_QUEUE_H

#include <deque>
#include <string>
#include <vector>

#include "boost/circular_buffer.hpp"
#include "boost/noncopyable.hpp"

#include "mutex.h"

namespace base {

/// copy from muduo
template<typename T>
class BlockingQueue : public boost::noncopyable
{
public:
    BlockingQueue()
        : mutex_(),
          notEmpty_(mutex_),
          queue_()
    {
    }

    void put(const T& x) {
        MutexLockGuard lock(mutex_);
        queue_.push_back(x);
        notEmpty_.Notify(); // wait morphing saves us
        // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
    }

    T take() {
        MutexLockGuard lock(mutex_);
        // always use a while-loop, due to spurious wakeup
        while (queue_.empty()) {
            notEmpty_.Wait();
        }
        assert(!queue_.empty());
        T front(queue_.front());
        queue_.pop_front();
        return front;
    }

    size_t size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }

private:
    mutable MutexLock mutex_;
    Condition         notEmpty_;
    std::deque<T>     queue_;
};

/// copy from muduo
template<typename T>
class BoundedBlockingQueue : public boost::noncopyable
{
public:
    explicit BoundedBlockingQueue(int maxSize)
        : mutex_(),
          notEmpty_(mutex_),
          notFull_(mutex_),
          queue_(maxSize)
    {
    }

    void put(const T& x) {
        MutexLockGuard lock(mutex_);
        while (queue_.full()) {
            notFull_.Wait();
        }
        assert(!queue_.full());
        queue_.push_back(x);
        notEmpty_.Notify();
    }

    T take() {
        MutexLockGuard lock(mutex_);
        while (queue_.empty()) {
            notEmpty_.Wait();
        }
        assert(!queue_.empty());
        T front(queue_.front());
        queue_.pop_front();
        notFull_.Notify();
        return front;
    }

    bool empty() const {
        MutexLockGuard lock(mutex_);
        return queue_.empty();
    }

    bool full() const {
        MutexLockGuard lock(mutex_);
        return queue_.full();
    }

    size_t size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }

    size_t capacity() const {
        MutexLockGuard lock(mutex_);
        return queue_.capacity();
    }

private:
    mutable MutexLock          mutex_;
    Condition                  notEmpty_;
    Condition                  notFull_;
    boost::circular_buffer<T>  queue_;
};

} // end replayer namespace

#endif
