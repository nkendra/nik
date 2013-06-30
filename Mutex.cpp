///////////////////////////////////////////////////////////////////////////////
/// @file Util\Mutex.cpp
/// @brief Implements the mutex functions defined in Util\Mutex.h
/// @internal
///
/// 08May2010, nik: initial
/// 27May2010, nik: Convert mutex functions into the Mutex class
///////////////////////////////////////////////////////////////////////////////

#include "Mutex.h"
#include "Utility.h"

#include <sstream>
#include <cassert>

#include <Windows.h>

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class Mutex::MutexImpl Mutex.h <Util\Mutex.h>
/// @brief Implementation class for Mutex
/// @details Implements the Mutex functionality.
/// @note Only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Mutex::MutexImpl 
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Mutex resources are aquired on construction
    /// @attention Throws a nik::Error if the mutex resources could not be
    ///     aquired.
    ///////////////////////////////////////////////////////////////////////////
    MutexImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Mutex resources are released on destruction
    ///////////////////////////////////////////////////////////////////////////
    ~MutexImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Locks the mutex
    /// @copydetails Mutex::Lock(size_t)
    ///////////////////////////////////////////////////////////////////////////
    bool Lock(size_t timeOut);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Unlocks the mutex
    /// @copydetails Mutex::Unlock()
    ///////////////////////////////////////////////////////////////////////////
    bool Unlock();

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The mutex resource
    ///////////////////////////////////////////////////////////////////////////
    HANDLE m_Mutex;

}; // end Mutex::MutexImpl

//----------------------Mutex-Implementation---------------------------------//
//----------------------Static-Members---------------------------------------//
size_t Mutex::FOREVER = INFINITE;
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Mutex* Mutex::Create()
{
    return new Mutex();
} // end Mutex::Create

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex()
{
    delete m_Impl;
} // end Mutex::~Mutex

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Mutex::Lock
    (
    size_t timeOut
    )
{
    return m_Impl->Lock(timeOut);
} // end Mutex::Lock

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Mutex::Unlock()
{
    return m_Impl->Unlock();
} // end Mutex::Unlock

//----------------------Private-Implementation-------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Mutex::Mutex()
{
    m_Impl = new MutexImpl;
} // end Mutex::Mutex


//----------------------Mutex::MutexImpl-Implementation----------------------//
//----------------------Static-Members---------------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Mutex::MutexImpl::MutexImpl()
:
m_Mutex(0)
{
    m_Mutex = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             // unnamed mutex
    if(!m_Mutex)
    {
        assert(0);
        DWORD errCode = GetLastError();
        std::ostringstream msg;
        msg << "Mutex::Mutex - Create mutex failed with code: ";
        msg << errCode << std::endl;
        throw Error(msg.str());
    }
}

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Mutex::MutexImpl::~MutexImpl()
{
    CloseHandle(m_Mutex);
}

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Mutex::MutexImpl::Lock
    (
    size_t timeOut
    )
{
    DWORD result = WaitForSingleObject(m_Mutex, timeOut);  
    // Check for timeout
    if(result == WAIT_TIMEOUT)
    {
        // couldn't get the mutex, return without writing
        return false;
    }
    if(result != WAIT_OBJECT_0)
    {
        assert(false);
        DWORD errCode = GetLastError();
        std::ostringstream msg;
        msg << "Mutex::Lock - Wait failed with code: ";
        msg << errCode;
        throw nik::Error(msg.str());
    }
    return true;
} // end Mutex::MutexImpl::Lock

///////////////////////////////////////////////////////////////////////////////
// 27May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Mutex::MutexImpl::Unlock()
{
    // ReleaseMutex returns 0 on failure
    return ReleaseMutex(m_Mutex) != 0;
} // end Mutex::MutexImpl::Unlock

///////////////////////////////////////////////////////////////////////////////
// 08May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool GetMutexLock(HANDLE mutex, DWORD timeout)
{
    DWORD result = WaitForSingleObject(mutex, timeout);  
    // Check for timeout
    if(result == WAIT_TIMEOUT)
    {
        // couldn't get the mutex, return without writing
        return false;
    }
    if(result != WAIT_OBJECT_0)
    {
        DWORD errCode = GetLastError();
        std::ostringstream msg;
        msg << "Error: ScopeLock->Wait failed with code: ";
        msg << errCode << std::endl;
        throw nik::Error(msg.str());
    }
    return true;
} // end GetMutex

} // end namespace nik

////////////////////////End-of-File////////////////////////////////////////////
