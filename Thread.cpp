///////////////////////////////////////////////////////////////////////////////
/// @file Util\Thread.cpp
/// @brief Contains the implementation of the Thread class
/// @internal
///
/// 26May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#include "Thread.h"
#include "Utility.h"
#include <Windows.h>
#include <cassert>

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class Thread::ThreadImpl Thread.cpp <Util\Thread.cpp>
/// @brief The implementation of the Thread class
/// @details Defines the behavior of the Thread class.
/// @note Thread currently only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Thread::ThreadImpl
{
public:
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor
    /// @details Initializes the member variables but does not allocate 
    ///     resources.
    ///////////////////////////////////////////////////////////////////////////
    ThreadImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates and starts the native thread
    /// @return @arg true - Success
    ///         @arg false - Resources could not be obtained to create the 
    ///             thread
    ///////////////////////////////////////////////////////////////////////////
    bool StartThread(ThreadFunc_t threadFunc, ThreadFuncArgType_t arg);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Frees the thread resource
    ///////////////////////////////////////////////////////////////////////////
    ~ThreadImpl();

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The thread resource
    /// @note Currently only support windows
    ///////////////////////////////////////////////////////////////////////////
    HANDLE m_Thread;

}; // end class Thread::ThreadImpl

//----------------------Thread-Implementation--------------------------------//
//----------------------Static-Members---------------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Thread* Thread::Create()
{
    return new Thread;
} // end Thread::Create

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{
    delete m_Impl;
} // end Thread::~Thread

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Thread::StartThread
    (
    ThreadFunc_t threadFunc,
    ThreadFuncArgType_t arg
    )
{
    return m_Impl->StartThread(threadFunc, arg);
} // end Thread::StartThread
//----------------------Private-Implementation-------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Thread::Thread()
{
    m_Impl = new ThreadImpl;
} // end Thread::Thread

//----------------------Thread::ThreadImpl-Implementation--------------------//
//----------------------Static-Members---------------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Thread::ThreadImpl::ThreadImpl()
:
m_Thread(0)
{}

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Thread::ThreadImpl::StartThread
    (
    ThreadFunc_t threadFunc,
    ThreadFuncArgType_t arg
    )
{
    if(m_Thread)
    {
        // THread already created
        return true;
    }
    DWORD threadID; /// @todo What do i do with threadID?
    m_Thread = ::CreateThread
                        ( 
                        0,               // default security attributes
                        0,               // use default stack size  
                        threadFunc,      // thread function name
                        arg,            // argument to thread function 
                        0,               // use default creation flags 
                        &threadID);      // returns the thread identifier 

    assert(m_Thread);
    return m_Thread != 0;
} // end Thread::ThreadImpl::StartThread

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Thread::ThreadImpl::~ThreadImpl()
{
    if(m_Thread)
    {
        CloseHandle(m_Thread);
    }
} // end Thread::ThreadImpl::~ThreadImpl

} // end namespace nik

////////////////////////End-of-File////////////////////////////////////////////
