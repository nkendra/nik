///////////////////////////////////////////////////////////////////////////////
/// @file Utility.h
/// @brief Contains small, helpful types and functions
/// @internal
///
/// 13March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_UTILITY_HEADER
#define NIK_UTILITY_HEADER

#include <string>

namespace nik
{

///////////////////////////////////////////////////////////////////////////////
/// @class LessThanStr Utility.h <Util\Utility.h>
/// @brief Functor for comparing std::string
///////////////////////////////////////////////////////////////////////////////
class LessThanStr
{
public:
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Less than operator for two strings
    /// @param[in] lhs The left-hand-side string
    /// @param[in] rhs The right-hand-side string
    /// @return @arg true The left-hand-side is less than the right-hand-side
    ///         @arg false Otherwise
    ///////////////////////////////////////////////////////////////////////////
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return strcmp(lhs.c_str(), rhs.c_str()) < 0;
    }
}; // end class LessThanStr

///////////////////////////////////////////////////////////////////////////////
/// @class Error <Util\Utility.h>
/// @brief Exception class
/// @note Inherits from std::runtime_error
///////////////////////////////////////////////////////////////////////////////
class Error : public std::runtime_error
{
public:
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    ///////////////////////////////////////////////////////////////////////////
    Error(const std::string& msg)
        :
    std::runtime_error(msg)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    ///////////////////////////////////////////////////////////////////////////
    virtual ~Error(){}

}; // end class Error

} // end namespace nik

#endif 
////////////////////////End-of-File////////////////////////////////////////////
