///////////////////////////////////////////////////////////////////////////////
/// @file Util\CmdLine.h
/// @brief Declaration of the CmdLine class
/// @internal
///
/// 13March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_CMD_LINE_HEADER
#define NIK_CMD_LINE_HEADER

#include <map>
#include "Utility.h"

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class CmdLine CmdLine.h <Util\Control.h>
/// @brief Parses command line arguments
///////////////////////////////////////////////////////////////////////////////
class CmdLine
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default Constructor
    /// @details Constructs the object without any command line parameters.
    ///////////////////////////////////////////////////////////////////////////
    CmdLine();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Parses the command line arguments and stores the parameters
    ///     inside the object.
    /// @param[in] argc Number of command line arguments
    /// @param[in] argv Command line arguments
    ///////////////////////////////////////////////////////////////////////////
    CmdLine(int argc, char** argv);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Parses the command line arguments
    /// @details Parses the command line arguments and stores the parameters
    ///     inside the object.
    /// @param[in] argc Number of command line arguments
    /// @param[in] argv Command line arguments
    ///////////////////////////////////////////////////////////////////////////
    void ParseCmdArg(int argc, char** argv);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Returns the field corresponding to a flag
    /// @details This function returns the data that is associated with a flag.
    ///     If no data was parsed for the flag, the pointer returned will be
    ///     the empty string.  If the flag was not parsed at all, the pointer
    ///     returned will be equal to zero.
    /// @param[in] flag The flag to check
    /// @return The data associated with the flag
    ///////////////////////////////////////////////////////////////////////////
    const char* GetArg(char flag) const ;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Returns the field corresponding to a flag
    /// @details This function returns the data that is associated with a flag.
    ///     If no data was parsed for the flag, the pointer returned will be
    ///     the empty string.  If the flag was not parsed at all, the pointer
    ///     returned will be equal to zero.
    /// @param[in] flag The flag to check 
    /// @return The data associated with the flag
    ///////////////////////////////////////////////////////////////////////////
    const char* GetArg(const char* const flag) const;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Determines if the flag is set
    /// @return @arg true The flag was set in the command line arguments
    ///         @arg false The flag was not found
    ///////////////////////////////////////////////////////////////////////////
    bool IsFlagSet(char flag) const;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Determines if the flag is set
    /// @return @arg true The flag was set in the command line arguments
    ///         @arg false The flag was not found
    ///////////////////////////////////////////////////////////////////////////
    bool IsFlagSet(const char* const flag) const;

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Maps flags to data fields
    ///////////////////////////////////////////////////////////////////////////
    typedef std::map<std::string, std::string, nik::LessThanStr> ArgMap_t;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Maps flags to data fields
    /// @details For flags that do not have any associated data, the data field
    ///     will be the empty string.
    ///////////////////////////////////////////////////////////////////////////
    ArgMap_t m_Args;

}; // end class CmdLine

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////
