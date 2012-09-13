// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_DISJOINT_SET_H
#define IMAGE_DISJOINT_SET_H


#include <list>
#include <boost/smart_ptr.hpp>


namespace image {
	
	
	/// \brief An implementation of the disjoint-set forest data structure.
	///
	/// \author Seppo Enarvi (senarvi@iki.fi)
	/// \date 2002 - 2004
	///
	/// The data structure is described in
	///
	/// \verbatim
	/// Cormen, Leiserson, Rivest
	/// "Introduction to Algorithms"
	/// The MIT Press, 1990
	/// \endverbatim
	///
	/// disjoint_set_forest stores all the nodes of all the sets in a container.
	/// Creation of nodes (sets) is only permitted through the create_set method.
	/// The nodes are destructed automatically when the disjoint_set_forest object
	/// is destructed.
	///
	template <typename NodePayload>
	class disjoint_set_forest
	{
	public:
	
		////////////////////////////////////
		// disjoint_set_forest::node class
		
		/// A node contains a single piece of data of type payload_t. A set is a
		/// collection of nodes in a tree hierarchy. The root of the tree is
		/// called the delegate node of that set.
		///
		class node
		{
		public:
			typedef NodePayload payload_type;
			friend class disjoint_set_forest<payload_type>;
			
			
		private:
			/// Create a new node (in other words, create a new set, which only
			/// contains this node as the delegate node).
			///
			node(const payload_type & payload) :
				payload_(payload),
				parent_(this),
				rank_(0) {}
	
			
		public:
			payload_type & payload()
			{ return payload_; }
	
			bool is_delegate()
			{ return parent_ == this; }
			
			node & find_set()
			{
				// If this is the root node, the parent points back to us.
				// Return a reference to this object.
				if (is_delegate()) return *this;
	
				// Recursively call this method on the parent object.
				node & root_node = parent_->find_set();
	
				// "Path compression": set the parent to point to the root
				// node before returning.
				parent_ = &root_node;
				return root_node;
			}
	
			void link(node & x)
			{
				node & x_root = x.find_set();  // The root of the other node.
				node & y_root = find_set();  // The root of this node.
	
				if (&x_root != &y_root)
				{
					// The nodes are not yet in the same tree.
					if (x_root.rank_ > y_root.rank_)
						// x_root has higher rank (that tree is higher), so
						// we make it the parent of y_root.
						y_root.parent_ = &x_root;
					else
					{
						// y_root has higher rank (or the ranks are equal), so
						// we make it the parent of x_root.
						x_root.parent_ = &y_root;
	
						if (x_root.rank_ == y_root.rank_)
							// The trees were equal in depth and we increased
							// the depth of the tree, so we'll increase the rank
							// of the new tree accordingly.
							y_root.rank_++;
					}
				}
			}
			
	
		private:
			payload_type payload_;  /// The piece of data stored in this node.
			node * parent_;  /// The parent node in the tree hierarchy.
	
			/// An upper bound on the depth of the hierarchy below the node.
			unsigned int rank_;
		};
	
		
		typedef std::list<boost::shared_ptr<node> > node_container_t;
		typedef typename node_container_t::iterator node_iterator;
		typedef typename node_container_t::const_iterator node_const_iterator;
	
		
		////////////////////////////////
		// disjoint_set_forest methods
		
		node * create_set(NodePayload root_payload = NodePayload())
		{
			node * new_node = new node(root_payload);
			all_the_nodes_.push_back(boost::shared_ptr<node>(new_node));
			return new_node;
		}
		
		node_iterator nodes_begin()
		{
			return all_the_nodes_.begin();
		}
	
		node_iterator nodes_end()
		{
			return all_the_nodes_.end();
		}
		
		
	private:
		node_container_t all_the_nodes_;
		
	};
	
	
}


#endif
