///////////////////////////////////////////////////////////////////////////////
/// @file CallBack.h
/// @brief Contains the declaration of the generic callback classes
/// @internal
///
/// 15June2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_CALL_BACK_HEADER
#define NIK_CALL_BACK_HEADER

namespace nik 
{

// Forward declarations
class GenericFunctor;

///////////////////////////////////////////////////////////////////////////////
/// @class CallBack CallBack.h <Util/CallBack.h>
/// @brief Generic callback class
/// @details This class is used to generically call a function for callback.
///////////////////////////////////////////////////////////////////////////////
class CallBack
{
public:
    
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @param[in] func The functor to call on callback
    ///////////////////////////////////////////////////////////////////////////
    CallBack(GenericFunctor* func);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    ///////////////////////////////////////////////////////////////////////////
    ~CallBack();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Copy Constructor
    /// @param[in] orig The object to copy from
    ///////////////////////////////////////////////////////////////////////////
    CallBack(const CallBack& orig);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Assignment operator
    /// @param[in] rhs The object to copy from
    /// @return A reference to this object
    ///////////////////////////////////////////////////////////////////////////
    CallBack& operator=(const CallBack& rhs);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Swaps the contents of two objects
    /// @param[in] rhs The object to swap contents with
    ///////////////////////////////////////////////////////////////////////////
    void Swap(CallBack& rhs);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Calls the callback
    ///////////////////////////////////////////////////////////////////////////
    void Call();

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The functor to call on callback
    /// @note This object is created with new
    ///////////////////////////////////////////////////////////////////////////
    GenericFunctor* m_Func;

}; // end class CallBack

///////////////////////////////////////////////////////////////////////////////
/// @class GenericFunctor CallBack.h <Util/CallBack.h>
/// @brief Interface class used for a generic function call
///////////////////////////////////////////////////////////////////////////////
class GenericFunctor
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    ///////////////////////////////////////////////////////////////////////////
    GenericFunctor()
    {}

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    ///////////////////////////////////////////////////////////////////////////
    virtual ~GenericFunctor()
    {}

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The function to call
    ///////////////////////////////////////////////////////////////////////////
    virtual void CallFunc() = 0;

}; // end class GenericFunctor

} // end namespace nik
#endif 

////////////////////////End-of-File////////////////////////////////////////////
