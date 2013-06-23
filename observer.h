////////////////////////////////////////////////////////////////////////////////
/// @brief Declaration of 
/// @internal
/// 
/// Jun 22, 2013, nik: initial
////////////////////////////////////////////////////////////////////////////////

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "exception.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>

namespace nik {

template <typename T>
class Observable;


///////////////////////////////////////////////////////////////////////////////
/// @class Observer Observer.h <Observer.h>
/// @brief Observer pattern
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class Observer {

	///////////////////////////////////////////////////////////////////////////
	/// @brief Observable needs internal access
	///////////////////////////////////////////////////////////////////////////
	friend class Observable<T>;

public:

	///////////////////////////////////////////////////////////////////////////
	/// @brief Notify data type
	///////////////////////////////////////////////////////////////////////////
	typedef T NotifyDataType;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Default constructor
	///////////////////////////////////////////////////////////////////////////
	Observer()
	: m_ID(InvalidID), m_Observable(0)
	{}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Destructor
	///////////////////////////////////////////////////////////////////////////
	virtual ~Observer();
//	{
//		if(m_Observable)
//		{
//			m_Observable->unregisterObs(this);
//			m_Observable = 0;
//		}
//	} // end ~Observer

private:

	///////////////////////////////////////////////////////////////////////////
	/// @brief Notify function
	/// @details This function is called by Observable to notify the Observer
	///		of an update.
	/// @note No implementation by default.  Clients should override this
	///		method.
	///////////////////////////////////////////////////////////////////////////
	virtual void signal() {}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Notify function with data
	/// @details This function is called by Observable to notify the Observer
	/// 	of an update, passing along information through the data parameter.
	/// @param[in] data Some data
	/// @note No implementation by default.  Clients should override this
	///		method.
	///////////////////////////////////////////////////////////////////////////
	virtual void notify(const NotifyDataType& data) {}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Observer ID type
	///////////////////////////////////////////////////////////////////////////
	typedef unsigned int ObserverID;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Invalid Observer ID
	///////////////////////////////////////////////////////////////////////////
	static const ObserverID InvalidID = 0;

	///////////////////////////////////////////////////////////////////////////
	/// @brief The ID assigned to this Observer
	///////////////////////////////////////////////////////////////////////////
	ObserverID m_ID;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Observable type
	///////////////////////////////////////////////////////////////////////////
	typedef Observable<NotifyDataType> ObservableType;

	///////////////////////////////////////////////////////////////////////////
	/// @brief The Observable that this Observer observes
	/// @details This enables the Observer to unregister itself upon
	///		destruction.
	/// @note This will always point to 0 if it is not register to an
	///		Observable; otherwise, it will be non-zero.
	///////////////////////////////////////////////////////////////////////////
	ObservableType* m_Observable;


	///////////////////////////////////////////////////////////////////////////
	/// @class EqualPred EqualPred.h
	/// @brief Equality predicate for Observer
	/// @details  Useful for STL algorithms, this predicate determines if an
	///		Observer is equal to some target observer.
	/// @note Private declaration for class Observable to use.
	///////////////////////////////////////////////////////////////////////////
	class EqualPred {
		///////////////////////////////////////////////////////////////////////
		/// @brief ID of the Observer to determine equality.
		///////////////////////////////////////////////////////////////////////
		ObserverID m_TargetID;
	public:
		///////////////////////////////////////////////////////////////////////
		/// @brief Constructor
		/// @param[in] obs The target Observer to check for equality.
		///////////////////////////////////////////////////////////////////////
		EqualPred(Observer* obs)
		: m_TargetID(obs->m_ID)
		{}

		///////////////////////////////////////////////////////////////////////
		/// @brief Check for equality
		/// @param[in] Check if the given Observer is equal to the target
		///		Observer.
		///	@return true - The given Observer matches the target
		//			false - Not a match
		///////////////////////////////////////////////////////////////////////
		bool operator()(Observer* obs)
		{
			return m_TargetID == obs->m_ID;
		}
	}; // end class EqualPred

	///////////////////////////////////////////////////////////////////////////
	/// @class Notifier observer.h
	/// @brief Functor to execute Observer<T>::notify method
	///	@details Useful for STL algorithms
	/// @note Private declaration Observable to use
	///////////////////////////////////////////////////////////////////////////
	class Notifier {
		const NotifyDataType& m_Data;
	public:
		///////////////////////////////////////////////////////////////////////
		/// @brief Constructor
		/// @param[in] data The notification data to pass to Observers
		///////////////////////////////////////////////////////////////////////
		Notifier(const NotifyDataType& data)
		: m_Data(data)
		{}

		///////////////////////////////////////////////////////////////////////
		/// @brief Notify the Observer, passing the notify data
		/// @param[in] obs Notify this observer
		///////////////////////////////////////////////////////////////////////
		void operator()(Observer* obs)
		{
			obs->notify(m_Data);
		}
	}; // end class Notifier


}; // end class Observer

///////////////////////////////////////////////////////////////////////////////
/// @class Observable Observable.h <Observable.h>
/// @brief Observable object
/// @details This class is a part of the Observer pattern.  Observers can
///		register with an Observable object to receive notifications.
/// @tparam T The data type that is passed for notifications
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class Observable {
public:

	///////////////////////////////////////////////////////////////////////////
	/// @brief Observer type
	///////////////////////////////////////////////////////////////////////////
	typedef Observer<T> ObserverType;
	typedef typename ObserverType::NotifyDataType NotifyDataType;
	typedef typename ObserverType::ObserverID ObserverID;
	static const ObserverID InvalidID = ObserverType::InvalidID;

	Observable()
	{
		m_FreeIDs.push_back(InvalidID + 1);
	}

	virtual ~Observable()
	{
		typename ObserverList::reverse_iterator end(m_ObserverList.begin());
		for(typename ObserverList::reverse_iterator it(m_ObserverList.end()); it != end; ++it)
		{
			unregisterObs(*it);
		}
	} // end ~Observable

	void registerObs(ObserverType* obs)
	{
		// If the observer has been assigned an ID, check to see
		// if it's already been registered.
		if(obs->m_ID != ObserverType::InvalidID)
		{
			typename ObserverList::const_iterator it =
			std::find_if(m_ObserverList.begin(), m_ObserverList.end(), typename ObserverType::EqualPred(obs));
			if(it == m_ObserverList.end())
			{
				return; // it has already been registered
			}
		} // end if(observer ID already set
		obs->m_ID = generateID();
		m_ObserverList.push_back(obs);
	} // end registerObs

	void unregisterObs(ObserverType* obs)
	{
		typename ObserverList::iterator it =
				std::find_if(m_ObserverList.begin(),
							m_ObserverList.end(),
							typename ObserverType::EqualPred(obs)
							);
		if(it == m_ObserverList.end())
		{
			RAISE_EXCEPTION("Observer has not been registered with this Observable.");
		}

		(*it)->m_ID = InvalidID;
		m_ObserverList.erase(it);
	} // end unregisterObs

protected:

	void notifyAll()
	{
		std::for_each(
				m_ObserverList.begin(),
				m_ObserverList.end(),
				std::mem_fun(&ObserverType::signal)
		);

	}
	void notifyAll(const NotifyDataType& data)
	{
		std::for_each(
				m_ObserverList.begin(),
				m_ObserverList.end(),
				typename ObserverType::Notifier(data)
				);
	}
	ObserverID generateID()
	{
		assert(!m_FreeIDs.empty());
		ObserverID id = m_FreeIDs.back();
		m_FreeIDs.pop_back();
		if(m_FreeIDs.empty())
		{
			m_FreeIDs.push_back(id+1);
		}
		return id;
	} // end generateID
	void freeID(ObserverID id)
	{
		m_FreeIDs.push_back(id);
	}
private:

	typedef std::vector<ObserverType*> ObserverList;
	typedef std::vector<ObserverID> ObserverIDList;
	ObserverIDList m_FreeIDs;

	ObserverList m_ObserverList;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief Destructor
/// @details The destructor unregisters itself from the Observable, IFF it
///		is registered with an Observable.
///////////////////////////////////////////////////////////////////////////////
template<typename T>
Observer<T>::~Observer()
{
	if(m_Observable)
	{
		m_Observable->unregisterObs(this);
		m_Observable = 0;
	}
} // end ~Observer

} /* namespace nik */
#endif /* OBSERVER_H_ */
