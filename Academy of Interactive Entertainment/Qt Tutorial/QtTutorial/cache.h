////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#include <mutex>

/**
* Cache item able to be locked across threads
*/
template <typename T> class Lockable
{
public:

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
    * Locks the thread to set the data
    * @param data The data to set
    */
    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
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

private:

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