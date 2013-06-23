////////////////////////////////////////////////////////////////////////////////
/// @brief Declaration of 
/// @internal
/// 
/// June 22, 2013, nik: initial
////////////////////////////////////////////////////////////////////////////////

#ifndef POSTBOARD_H_
#define POSTBOARD_H_

///////////////////////////////////////////////////////////////////////////////
/// @class PostBoard PostBoard.h <posterboard/postboard.h>
/// @brief A post board for posting, reading, and modifying
/// @details  PostBoard enables the posting of posts, registering observers,
///		modifying posts, and removing posts.  This class is templated to work
///		with a single type.  That type can be polymorphic.
/// @tparam T Data type for the posts.  There are no requirements on this type.
///////////////////////////////////////////////////////////////////////////////

#include "post.h"
#include "exception.h"
#include "observer.h"

#include <string>
#include <map>
#include <stack>

namespace nik{

template <typename T, bool PassData = true, typename P = Post<T> >
class PostBoard : public Observable<P> {
public:

	typedef T PostDataType;

	typedef P PostType;

	typedef typename PostType::PostID PostID;

	typedef typename PostType::PostTraits PostTraits;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Constructor
	///////////////////////////////////////////////////////////////////////////
	PostBoard()
	{
		m_FreeIDs.push(0);
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Post a message to the board
	/// @details Posts a message to the board.  PostBoard stores its own copy
	///		of the data.
	/// @param[in] data The data to post
	/// @warning This function throws an exception if it fails to save to the
	///		post.
	/// @return The unique identifier for the post
	///////////////////////////////////////////////////////////////////////////
	PostID post(const PostDataType& data)
	{
		PostID id = generateID();
		try {
			std::pair<typename PostsTable::iterator, bool> rtn =
				m_PostsTable.insert(
						std::make_pair(
								id,
								PostType(data,id)
								)
						);

			if( !rtn.second)
			{
				RAISE_EXCEPTION("Failed to add message to board.");
			}
		} // end try
		catch(std::exception&)
		{
			freeID(id);
			throw;
		}
		if(PassData)
		{
			this->notifyAll(PostType(data, id));
		}
		else
		{
			this->notifyAll();
		}
		return id;
	} // end Post

	///////////////////////////////////////////////////////////////////////////
	/// @brief Destructor
	///////////////////////////////////////////////////////////////////////////
	virtual ~PostBoard()
	{}

protected:

	///////////////////////////////////////////////////////////////////////////
	/// @brief Generate a post identifier
	/// @return Returns an unused identifier - for this board
	///////////////////////////////////////////////////////////////////////////
	PostID generateID()
	{
		PostID id = m_FreeIDs.top();
		m_FreeIDs.pop();
		if(m_FreeIDs.empty())
		{
			m_FreeIDs.push(id+1);
		}
		return id;
	} // end generateID

	///////////////////////////////////////////////////////////////////////////
	/// @brief Free up a PostID
	/// @param[in] id ID to free
	///////////////////////////////////////////////////////////////////////////
	void freeID(PostID id)
	{
		m_FreeIDs.push(id);
	} // end freeID

private:
	// Disable copy, and assignment
	PostBoard(const PostBoard&);
	PostBoard& operator=(const PostBoard&);

	///////////////////////////////////////////////////////////////////////////
	/// @brief A stack of PostIDs
	///////////////////////////////////////////////////////////////////////////
	typedef std::stack<PostID> IDStack;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Contains the available PostIDs
	///////////////////////////////////////////////////////////////////////////
	IDStack m_FreeIDs;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Posts table type
	///////////////////////////////////////////////////////////////////////////
	typedef std::map<PostID, PostType> PostsTable;

	///////////////////////////////////////////////////////////////////////////
	/// @brief Contained posts
	///////////////////////////////////////////////////////////////////////////
	PostsTable m_PostsTable;

}; // end class PostBoard

} // end namespace nik

#endif /* POSTBOARD_H_ */
