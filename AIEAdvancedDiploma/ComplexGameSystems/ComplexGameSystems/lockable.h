////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lockable.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mutex>

/**
* Synchronizes data between threads
*/
template <typename T> class Lockable
{
public:

    Lockable(const T& data) :
        m_data(data)
    {
    }

    Lockable() :
        m_data(T())
    {
    }

    T Swap(T data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::swap(data, m_data);
        return data;
    }

    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
    }

    T Get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

private:

    /**
    * Prevent copying
    */
    Lockable(const Lockable&) = delete;
    Lockable& operator=(const Lockable&) = delete;

    T m_data;                  
    mutable std::mutex m_mutex;  
};

/**
* Synchronizes data between threads
*/
template <typename T> class BlockingLockable
{
public:

    BlockingLockable(const T& data) :
        m_data(data)
    {
    }

    BlockingLockable() :
        m_data(T())
    {
    }

    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
        m_hasBeenSet = true;
        m_cv.notify_one();
    }

    T Get()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this]()
        {
            return m_hasBeenSet;
        });

        T data = m_data;
        lock.unlock();
        m_cv.notify_one();
        return data;
    }

private:

    /**
    * Prevent copying
    */
    BlockingLockable(const BlockingLockable&) = delete;
    BlockingLockable& operator=(const BlockingLockable&) = delete;

    T m_data;
    std::condition_variable m_cv;
    mutable std::mutex m_mutex;
    bool m_hasBeenSet = false;
};