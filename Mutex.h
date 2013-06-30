///////////////////////////////////////////////////////////////////////////////
/// @file Util\Mutex.h
/// @brief Contains the declaration of the Mutex class
/// @internal
///
/// 08May2010, nik: initial
/// 27May2010, nik: Converted the mutex Windows functions into a mutex class
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_MUTEX_HEADER
#define NIK_MUTEX_HEADER

namespace nik {


///////////////////////////////////////////////////////////////////////////////
/// @class Mutex Mutex.h <Util\Mutex.h>
/// @brief Class for manipulating a mutex
/// @details Mutex object that allows for the locking and unlocking of a mutex.
///     This object is created through the Create() function and should be 
///     freed with the delete operator.
/// @attention Only un-named mutexes are currently supported.  Additional 
///     functionality will be implemented as needed.
/// @note Mutex currently only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Mutex
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Mutex object
    /// @details This function creates a mutex that is ready to be used.
    /// @attention The returned pointer created with new.
    ///////////////////////////////////////////////////////////////////////////
    static Mutex* Create();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Cleans up the mutex resources
    ///////////////////////////////////////////////////////////////////////////
    ~Mutex();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Attempts to lock the mutex
    /// @details Waits the given amount of time to lock the mutex.  Use FOREVER
    ///     to disable the time out.
    /// @attention Throws nik::Error if there was an error aquiring the lock
    /// @return @arg true - The mutex has been locked
    ///         @arg false - The mutex was not aquired due to timeout
    /// @see FOREVER
    ///////////////////////////////////////////////////////////////////////////
    bool Lock(size_t timeOut = FOREVER);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Releases the mutex
    /// @details Unlocks the mutex.  The mutex should be owned by the calling
    ///     thread.
    /// @return @arg true - Lock has been released
    ///         @arg false - The mutex is not currently locked by the calling
    ///         thread.
    ///////////////////////////////////////////////////////////////////////////
    bool Unlock();

    static size_t FOREVER; ///< Flags Lock(size_t) to wait forever for the lock

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details On construction, the mutex resources are aquired.
    /// @attention Use Create() to create a mutex object.
    /// @note Construction is disallowed in client code.
    ///////////////////////////////////////////////////////////////////////////
    Mutex();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Copy construction has been disallowed.
    ///////////////////////////////////////////////////////////////////////////
    Mutex(const Mutex&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Assignment has been disallowed.
    ///////////////////////////////////////////////////////////////////////////
    Mutex& operator=(const Mutex&);

    class MutexImpl; // Implementation clas for Mutex

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The implementation object for this Mutex.
    ///////////////////////////////////////////////////////////////////////////
    MutexImpl* m_Impl;

}; // end class Mutex

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////
