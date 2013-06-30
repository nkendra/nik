///////////////////////////////////////////////////////////////////////////////
/// @file Util\GenericFactory.h
/// @brief Declaration of GenericFactory template
/// @internal
///
/// 20March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_GENERIC_FACTORY_HEADER
#define NIK_GENERIC_FACTORY_HEADER

#include <map>

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @brief A generic object factory
/// @details This class is an implementation of the Object Factory pattern.
///     This class uses registration to set up the types of objects this
///     factory will create.  Product classes register themselves with the
///     factory by supplying a type-ID and a creation function.
/// @tparam Product The type the factory will create
/// @tparam IDType The type for the type-ID fields.
/// @tparam ProductCreate The prototype for the creation functions that will 
///     be registered for each type.
/// @tparam IDTypeCompare The functor to use to compare the type-IDs for 
///     equality
///////////////////////////////////////////////////////////////////////////////
template
< 
    class Product,
    class IDType,
    class ProductCreator = Product* (*)(),
    class IDTypeCompare = std::less<IDType>
>
class GenericFactory 
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Registers a creation function and type ID
    /// @details This function attempts to register a creation function mapped to 
    ///     a type ID.  If the type ID has already been taken, this function
    ///     returns false;
    /// @param[in] id The ID to map to the creation function
    /// @param[in] creator The function to call to create the product
    /// @return @arg true Registration succeeded
    ///         @arg false Registration failed
    ///////////////////////////////////////////////////////////////////////////
    bool Register(const IDType& id, ProductCreator creator)
    {
        // Check if the type ID has already been taken
        Creator_Map_t::const_iterator it = m_Creator.find(id);
        if(it != m_Creator.end())
        {
            return false;
        }
    
        m_Creator.insert(std::make_pair(id, creator));
        return true;
    } // end Register

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates the object type corresponding the the type ID
    /// @param[in] id The ID of the type to create
    /// @return Returns a pointer to the newly created object.  Will return 0 if
    ///     the ID did not match any known type.
    ///////////////////////////////////////////////////////////////////////////
    Product* Create(const IDType& id)
    {
        Creator_Map_t::iterator it = m_Creator.find(id);
        if(it == m_Creator.end())
        {
            return 0;
        }
        return it->second();
    } // end Create


private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Maps type ID's to creation functions
    ///////////////////////////////////////////////////////////////////////////
    typedef std::map<IDType, ProductCreator, IDTypeCompare> Creator_Map_t;
    
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Stores the type ID, creation function pairs
    ///////////////////////////////////////////////////////////////////////////
    Creator_Map_t m_Creator;

}; // end class GenericFactory

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////
