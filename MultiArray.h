///////////////////////////////////////////////////////////////////////////////
/// @file MultiArray.h
/// @brief Declaration of the MutliArray class
/// @internal
///
/// 03March2010, nik: initial
/// 21July2010, nik: Removed default initial value template parameter
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_MULTIARRAY_HEADER
#define NIK_MULTIARRAY_HEADER

#include <vector>

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class MultiArray MultiArray.h <Util/MultiArray.h>
/// @brief Two dimensional array
/// @tparam TYPE The type of the values stored in the array
///////////////////////////////////////////////////////////////////////////////
template<typename TYPE>
class MultiArray
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor
    ///////////////////////////////////////////////////////////////////////////
    MultiArray()
    {}

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Creates a two dimensional array given the parameters
    /// @param[in] x The X dimension of the array
    /// @param[in] y The Y dimension of the array
    ///////////////////////////////////////////////////////////////////////////
    MultiArray(int x, int y)
    :
    m_Array(x, std::vector<TYPE>(y))
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Creates a two dimensional array given the parameters and 
    ///     sets the default value for all items in the array.
    /// @param[in] x The X dimension of the array
    /// @param[in] y The Y dimension of the array
    /// @param[in] val The default value for all of the array elements
    ///////////////////////////////////////////////////////////////////////////
    MultiArray(int x, int y, TYPE val)
    :
    m_Array(x, std::vector<TYPE>(y, val))
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Get the element reference at the given index
    /// @param[in] x X dimension
    /// @param[in] y Y dimension
    /// @return Reference to the array element
    ///////////////////////////////////////////////////////////////////////////
    TYPE& operator()(int x, int y)
    {
        return m_Array[x][y];
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Get the element value at the given index
    /// @param[in] x X dimension
    /// @param[in] y Y dimension
    /// @return Element value
    ///////////////////////////////////////////////////////////////////////////
    const TYPE& operator()(int x, int y) const
    {
        return m_Array[x][y];
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Get the number of columns in the array
    /// @return The number of columns in the array
    ///////////////////////////////////////////////////////////////////////////
    size_t SizeCol() const
    {
        return m_Array.size();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Get the number of rows in the array
    /// @return The number of rows in the array
    ///////////////////////////////////////////////////////////////////////////
    size_t SizeRow() const
    {
        // Check that the column vector exists
        return (m_Array.size() > 0) ? m_Array[0].size() : 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Sets all of the array elements to the given value
    /// @param[in] value The value to set for all array elements
    ///////////////////////////////////////////////////////////////////////////
    void SetAll(const TYPE& value)
    {
        // March through the vectors and set the value
        // Iterators to the outer vector
        Array2D_t::iterator itOuter, endOuter;
        itOuter = m_Array.begin();
        endOuter = m_Array.end();
        for(; itOuter != endOuter; ++itOuter)
        {
            // Iterators to the inner vector
            Array1D_t::iterator itInner, endInner;
            itInner = itOuter->begin();
            endInner = itOuter->end();
            for(; itInner != endInner; ++itInner)
            {
                (*itInner) = value;
            }
        }
    } // end SetAll

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief One dimensional array typedef
    ///////////////////////////////////////////////////////////////////////////
    typedef std::vector<TYPE> Array1D_t;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Two dimensional array typedef
    ///////////////////////////////////////////////////////////////////////////
    typedef std::vector< Array1D_t > Array2D_t;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The underlying array object
    ///////////////////////////////////////////////////////////////////////////
    Array2D_t m_Array;

}; // end class MultiArray

} // end namespace nik

#endif

//----------------------End-File---------------------------------------------//
