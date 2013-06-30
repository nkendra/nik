///////////////////////////////////////////////////////////////////////////////
/// @file Util\Thread.h
/// @brief Contains the declaration of the Thread class
/// @internal
///
/// 26May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_THREAD_HEADER
#define NIK_THREAD_HEADER

#ifdef NIK_USE_WINDOWS
#include <Windows.h>
#define NIK_API WINAPI
namespace nik {
typedef DWORD ThreadFuncReturnType_t;
typedef LPVOID ThreadFuncArgType_t;
typedef ThreadFuncReturnType_t (NIK_API *ThreadFunc_t)(ThreadFuncArgType_t);
} // end namespace nik
#else
#define NIK_API 
namespace nik {
typedef size_t ThreadFuncReturnType_t;
typedef void* ThreadFuncArgType_t;
typedef FuncReturnType_t (*ThreadFunc_t)(void*);
} // end namespace nik
#endif

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class Thread Thread.h <Util\Thread.h>
/// @brief Class for creating and manipulating a native thread
/// @details Thread objects can only be created through the Create() function.
///     The object is always created with new so remember to delete it.
/// @note Thread currently only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Thread
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new thread object
    /// @attention Returned object created with new.
    /// @attention This function returns 0 if the thread creation failed
    /// @return A pointer to the thread object
    ///////////////////////////////////////////////////////////////////////////
    static Thread* Create();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Cleans up thread resources
    ///////////////////////////////////////////////////////////////////////////
    ~Thread();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates and starts a new thread
    /// @details Given the function and argument to call, this function sets
    ///     up and new thread.  
    /// @return @arg true - New thread successfully started
    ///         @arg false - Failed to set up the new thread
    ///////////////////////////////////////////////////////////////////////////
    bool StartThread(ThreadFunc_t threadFunc, ThreadFuncArgType_t arg);
    
private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor is restricted from client code
    /// @details Use the Create() function to create a new Thread object.
    /// @attention Throws nik::Error on error during construction
    /// @note Resources are obtained on construction
    ///////////////////////////////////////////////////////////////////////////
    Thread();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent copy construction
    ///////////////////////////////////////////////////////////////////////////
    Thread(const Thread&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent assignment
    ///////////////////////////////////////////////////////////////////////////
    Thread& operator=(const Thread&);

    class ThreadImpl; // Thread implementation
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Pointer to the imlementation
    /// @details The Thread class uses an implementation class to handle the
    ///     behavior and functionality of Thread.
    ///////////////////////////////////////////////////////////////////////////
    ThreadImpl* m_Impl;

}; // end class Thread

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////

