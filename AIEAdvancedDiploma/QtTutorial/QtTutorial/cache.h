////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mutex>

/**
* Dual-way Lockable data for the cache of data shared between threads
* Each thread can set or get the data. Each thread can also 'lock' the
* data after a set to ensure a get is called before the data can be set
* again. This prevents overwriting an important set call.
*/
template <typename T> class Lockable
{
public:

    /**
    * Constructor
    * @param data The value to initialise with
    */
    Lockable(const T& data) :
        m_data(data)
    {
    }

    /**
    * Constructor
    */
    Lockable() :
        m_data(T())
    {
    }

    /**
    * Destructor
    */
    virtual ~Lockable()
    {
    }

    /**
    * Locks the thread to update the data
    * @note will prevent any more setting unless GetUpdated() is called
    * @param data The data to set
    */
    void SetWithLock(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
        m_locked = true;
    }

    /**
    * Locks the thread to get the updated data
    * @note will allow setting again if SetUpdated() was previously called
    * @return a copy of the data
    */
    T GetAndUnlock()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_locked = false;
        return m_data;
    }

    /**
    * Locks the thread to set the data
    * @note will prevent setting if waiting on a GetUpdated() call
    * @param data The data to set
    */
    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_locked)
        {
            m_data = data;
        }
    }

    /**
    * Locks the thread to get the data
    * @return a copy of the data
    */
    T Get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

    /**
    * @return wether the value requires an update from the setting thread
    */
    bool RequiresUpdate() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_updated;
    }

protected:

    bool m_locked = false;        ///< Allows data to be locked after important set updates
    mutable std::mutex m_mutex;   ///< Mutex for access
    T m_data;                     ///< Internal data
};

/**
* Manages information sending between the gui and application
*/
struct Cache
{
    Lockable<bool> ApplicationRunning;
    Lockable<bool> ExampleButton;
    Lockable<float> ExampleSpinBox;
};