////////////////////////////////////////////////////////////////////////////////
/// @brief Definition for class todo
/// @internal 
/// Jun 22, 2013, nik: initial
////////////////////////////////////////////////////////////////////////////////

#ifndef GENERIC_UTILITY_H_
#define GENERIC_UTILITY_H_

namespace nik{

class NullType{

};

template <bool result, typename T1, typename T2>
class Select {
public:
	typedef T1 Result;
};
template <typename T1, typename T2>
class Select<true, T1, T2> {
public:
	typedef T2 Result;
};

template <typename D>
class DestructPolicy {
public:
	static void destroy(D& obj)
	{
		// do nothing
	}
};

template <typename D>
class DestructPolicy<D*> {
public:
	static void destroy(D*& obj)
	{
		delete obj;
		obj = 0;
		delete obj;
	}
};

} // end namespace nik

#endif

//-----------------------------------End-of-File--------------------------------
