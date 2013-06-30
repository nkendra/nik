///////////////////////////////////////////////////////////////////////////////
/// @file Util\ScopeLock.h
/// @brief Contains the declaration of the ScopeLock class
/// @internal
///
/// ??????2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_SCOPE_LOCK_HEADER
#define NIK_SCOPE_LOCK_HEADER

#include <Util\Mutex.h>
#include <sstream>
#include <Util\Utility.h>
#include <cassert>

namespace nik {
///////////////////////////////////////////////////////////////////////////////
/// @class ScopeLock ScopeLock.h <Util/ScopeLock.h>
/// @brief Scope lock
/// @details Used in conjunction with Mutex to lock a scope.  The mutex is
///     locked upon creation of the ScopeLock and released on the deletion of
///     the ScopeLock object.
/// @see nik::Mutex
///////////////////////////////////////////////////////////////////////////////
class ScopeLock
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Gets the lock on construction of the object.  This function
    ///     will wait without timing out for the lock.
    /// @param[in] mutex The mutex to lock
    ///////////////////////////////////////////////////////////////////////////
    ScopeLock(Mutex* mutex)
        :
    m_Mutex(mutex)
    {
        #ifdef NIK_DEBUG
        bool result = m_Mutex->Lock(500);
        
        // Crappy express compiler won't catch assertions
        if(!result)
        {
            assert(result);
        }
        #else
        m_Mutex->Lock();
        #endif 
    } // end ScopeLock

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Releases the mutex
    ///////////////////////////////////////////////////////////////////////////
    ~ScopeLock()
    {
        m_Mutex->Unlock();
    }

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The mutex corresponding to this scope lock
    ///////////////////////////////////////////////////////////////////////////
    Mutex* m_Mutex;

}; // end class ScopeLock

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////
