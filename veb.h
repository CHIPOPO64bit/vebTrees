//
// Created by David Keisar on 2/11/22.
//

#ifndef _VEB_H_
#define _VEB_H_
#include <iostream>
#define _BASE_SIZE 2


/**
 * NULL: universe size
 * Complexity of building a new veb(u) = O(u)
 * 				successor, predecessor, member, delete, insert - O(lg lg(u))
 * @tparam T universe size
 */
template<class T=int>
class veb {

  /**
   *  min and max elements.
   *  min is stored implicitly in the tree which means, it is stored only in
   *  the field min, max though stored also in the clusters
   */
  T min;
  T max;

  bool empty;

  /**
   * stores the tree's clusters
   */
  veb **cluster;

  /**
   * stores the tree's summary
   */
  veb *summary;

  /**
   * the trees size
   */
  size_t universe;

  /**
   * least significant bits amount
   */
  size_t rhs_bits;
  /**
   * most significant bits amount
   */
  size_t lhs_bits;
  /**
   * upper root - 2**(lhs)
   */
  size_t high_root;
  /**
   * lower root - 2**(rhs)
   */
  size_t low_root;

  /**
   * initializes bit fields
   */
  void set_bits_amount() noexcept;

  /**
   * find x's inner index
   * @param x element in the universe
   * @return x's index inside its cluster
   */
  size_t low(T x) const noexcept {
	return (x & (this->low_root - 1));
  }
  /**
   * find x's cluster number
   * @param x element in the universe
   * @return x's cluster index
   */
  size_t high(T x) const noexcept {
	return ((x & (~(this->low_root - 1))) >> this->rhs_bits);
  }
  /**
   * return the index of x and y
   * @param x cluster index (high)
   * @param y index inside cluster (low)
   * @return index of element represneted by x and y
   */
  size_t index(T x, T y) const noexcept {
	return (this->low_root * x) + y;
  }

  /**
   * swaps between x and y
   * @param x element to swap
   * @param y element to swap
   */
  void swap(T &x, T &y) const noexcept;

  /**
   * delete the cluster field
   */
  void delete_cluster() {
	for (size_t i = 0; i < this->high_root; ++i) {
	  delete this->cluster[i];
	}
	delete[] this->cluster;
  }
  /**
   * insert x to empty tree
   * @param x element to insert
   */
  void empty_insert(T x) noexcept;

  /**
   * @return universe size
   */
  size_t get_universe_size() const noexcept {
	return this->universe;
  }

  /**
   * Iterator class
   */
  class Iterator {

	T current_val;
	veb *tree_ptr;
   public:
	typedef T value_type;
	typedef T &reference;
	typedef T *pointer;
	typedef std::ptrdiff_t difference_type; // irrelevant here
	typedef std::bidirectional_iterator_tag iterator_category;
	Iterator(const veb *tree, T current) : tree_ptr(tree), current_val
		(current) {}
	Iterator &operator++() {
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->successor(current_val);
	  }
	  return *this;
	}
	Iterator operator++(int) {
	  Iterator it(tree_ptr, current_val);
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->successor(current_val);
	  }
	  return *(this);

	}
	Iterator &operator--() {
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->predecessor(current_val);
	  }
	  return *this;
	}
	Iterator operator--(int) {
	  Iterator it(tree_ptr, current_val);
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->predecessor(current_val);
	  }
	  return it;
	}

	value_type operator*() {
	  return this->current_val;
	}
	bool operator==(const Iterator &rhs) const {
	  return this->tree_ptr == rhs.tree_ptr &&
		  this->current_val == rhs.current_val;
	}
	bool operator!=(const Iterator &rhs) const {
	  return this->tree_ptr != rhs.tree_ptr ||
		  this->current_val != rhs.current_val;
	}
	bool operator<(const Iterator &rhs) const {
	  return this->current_val < rhs.current_val;
	}
	bool operator>(const Iterator &rhs) const {
	  return this->current_val > rhs.current_val;
	}
	bool operator<=(const Iterator &rhs) const {
	  return this->current_val <= rhs.current_val;
	}
	bool operator>=(const Iterator &rhs) const {
	  return this->current_val >= rhs.current_val;
	}
	pointer operator->() const {
	  return this->tree_ptr;
	}

  };

  /**
   * Const Iterator class
   */
  class ConstIterator {
	T current_val;
	const veb *tree_ptr;
   public:
	typedef T value_type;
	typedef const T &reference;
	typedef const T *pointer;
	typedef std::ptrdiff_t difference_type; // irrelevant here
	typedef std::bidirectional_iterator_tag iterator_category;

	ConstIterator(const veb *tree, T current)
		: tree_ptr(tree), current_val(current) {}
	ConstIterator &operator++() {
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->successor(current_val);
	  }
	  return *this;
	}
	ConstIterator operator++(int) {
	  ConstIterator it(tree_ptr, current_val);
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->successor(current_val);
	  }
	  return *(this);

	}
	ConstIterator &operator--() {
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->predecessor(current_val);
	  }
	  return *this;
	}
	ConstIterator operator--(int) {
	  Iterator it(tree_ptr, current_val);
	  if (current_val != tree_ptr->get_universe_size()) {
		current_val = tree_ptr->predecessor(current_val);
	  }
	  return it;
	}
	value_type operator*() const {
	  return this->current_val;
	}
	bool operator==(const ConstIterator &rhs) const {
	  return this->tree_ptr == rhs.tree_ptr &&
		  this->current_val == rhs.current_val;
	}
	bool operator!=(const ConstIterator &rhs) const {
	  return this->tree_ptr != rhs.tree_ptr ||
		  this->current_val != rhs.current_val;
	}
	bool operator<(const ConstIterator &rhs) const {
	  return this->current_val < rhs.current_val;
	}
	bool operator>(const ConstIterator &rhs) const {
	  return this->current_val > rhs.current_val;
	}
	bool operator<=(const ConstIterator &rhs) const {
	  return this->current_val <= rhs.current_val;
	}
	bool operator>=(const ConstIterator &rhs) const {
	  return this->current_val >= rhs.current_val;
	}

	pointer operator->() const {
	  return this->tree_ptr;
	}
  };

 public:
  typedef Iterator iterator;
  typedef ConstIterator const_iterator;

  /**
   * begin, cbegin, end and cend methods
   */

  iterator begin() {
	return iterator(this, this->minimum());
  }
  const_iterator begin() const {
	return const_iterator(this, this->minimum());
  }
  const_iterator cbegin() const {
	return const_iterator(this, this->minimum());
  }
  iterator end() {
	return iterator(this, this->get_universe_size());
  }
  const_iterator end() const {
	return const_iterator(this, this->get_universe_size());
  }
  const_iterator cend() const {
	return const_iterator(this, this->get_universe_size());
  }

  /**
   * base constructor
   * @param size universe size of the tree
   */
  veb(size_t size) noexcept(false);

  /**
   * Destructor
   */
  ~veb() noexcept;

  /**
   * check element's membership
   * @param x element to check for membership
   * @return bool: x in tree
   */
  bool member(T x) const noexcept;

  /**
   * When the tree is empty, returns null
   * @return minimum of the tree
   */
  size_t minimum() const noexcept {
	if (this->is_empty()){
	  return this->get_universe_size();
	}
	return min;
  }

  /**
   * When the tree is empty, returns null
   * @return maximum of the tree
   */
  size_t maximum() const noexcept {
	if (this->is_empty()){
	  return this->get_universe_size();
	}
	return max;
  }

  /**
   * tree insert
   * @param x element to insert
   */
  void insert(T x) noexcept;

  /**
   * @return bool: tree is empty
   */
  bool is_empty() const noexcept {
	return this->empty;
  }

  /**
   * tree delete
   * @param x element to delete
   */
  void delete_element(T x) noexcept;

  /**
   * @param x element to find its successor
   * @return x's successor of x if exists, else null where null is the
   * tree's universe
   */
  T successor(T x) const noexcept;

  /**
  * @param x element to find its predecessor
  * @return x's predecessor of x if exists, else null where null is the
  * tree's universe
  */
  T predecessor(T x) const noexcept;

  /**
   * comparison operator
   * @param rhs veb tree to compare
   * @return true if they have the same size, and the same elements
   */
  bool operator==(const veb& rhs) const{
	if (this->get_universe_size() != rhs.get_universe_size()){
	  return false;
	} else if (this->get_universe_size() == _BASE_SIZE){
	  if (this->is_empty() != rhs.is_empty()){
		return false;
	  } return this->minimum() == rhs.minimum() && this->maximum() == rhs
	  .maximum();
	}
	// compare all elements
	veb<T> it_lhs = this->cbegin();
	veb<T> it_rhs = rhs.cbegin();
	while (it_lhs < this->cend && it_rhs < rhs.cend()){
	  if (*it_lhs != *it_rhs){
		return false;
	  }
	  ++it_lhs;
	  ++it_rhs;
	}
	return true;
  }
//TODO: finish operators: =, copy constructor 

//  /**
//   * copy assignment operator
//   * @param rhs veb tree to assign
//   * @return the new veb
//   */
//  veb &operator=(const veb& rhs) {
//	if (this == &rhs){
//	  return *this;
//	}
//
//
//	// init primitives
//	this->universe = rhs.get_universe_size();
//	this->high_root = rhs.high_root;
//	this->low_root = rhs.low_root;
//	this->min = rhs.min;
//	this->max = rhs.max;
//	this->empty = rhs.empty;
//	this->rhs_bits = rhs.rhs_bits;
//	this->lhs_bits = rhs.lhs_bits;
//
//	// init dynamic
//	*(this->summary) = *(rhs.summary);
//	this->delete_cluster();
//	this->cluster =
//
//
//  }

  /**
   * operator <<
   * @param os stream to print the tree in ascending order
   * @param tree veb tree
   * @return os
   */
  friend std::ostream& operator<<(std::ostream& os, const veb<T> & tree) {
	for(auto item : tree) os << item << std::endl;
	return os;
  }




};
template<class T>
veb<T>::veb(size_t size) noexcept(false): empty(true), universe(size) {
  // initialize inner fields
  this->set_bits_amount();

  // base case, empty base sized tree
  if (size == _BASE_SIZE) {
	this->cluster = nullptr;
	this->summary = nullptr;
	this->min = 0;
	this->max = 0;
  } else {
	// init summary tree
	this->summary = new veb(this->high_root);
	// init each cluster tree
	this->cluster = new veb *[this->high_root];
	for (size_t i = 0; i < this->high_root; ++i) {
	  this->cluster[i] = new veb(this->low_root);
	}
  }
}
template<class T>
void veb<T>::set_bits_amount() noexcept {
  size_t amount = 0, current = 1;
  // compute log2 of universe
  while (current < this->universe) {
	++amount;
	current = current << 1;
  }
  // floor(lg(u)/2)
  this->rhs_bits = amount / 2;
  // ceil(lg(u)/2)
  this->lhs_bits = amount - this->rhs_bits;
  // least significant bits of universe
  this->high_root = 1 << this->lhs_bits;
  // most significant bits of universe
  this->low_root = 1 << this->rhs_bits;
}

template<class T>
veb<T>::~veb() noexcept {
  // base case
  if (this->universe == _BASE_SIZE) {
	return;
  }
  delete this->summary;
  // delete all clusters
  this->delete_cluster();
}
template<class T>
bool veb<T>::member(T x) const noexcept {
  // out of bounds
  if (this->empty || x >= this->universe) {
	return false;
  } else if (x == this->min || x == this->max) {
	return true;
  } else if (this->universe == _BASE_SIZE) {
	return false;
  }
  // search in cluster
  return this->cluster[this->high(x)]->member(this->low(x));
}
template<class T>
void veb<T>::empty_insert(T x) noexcept {
  this->min = x;
  this->max = x;
  this->empty = false;
}
template<class T>
void veb<T>::swap(T &x, T &y) const noexcept {
  // xor(a, xor(a,b))= b
  x = x ^ y;
  y = y ^ x;
  x = x ^ y;
}
template<class T>
void veb<T>::insert(T x) noexcept {
  // out of bounds
  if (x >= this->universe) {
	return;
  }
  if (this->empty) {
	this->empty_insert(x);
	return;
  } // make sure x is ready to be inserted
  else if (x < this->min) {
	// swap because min should not be in the tree
	this->swap(this->min, x);
  }
  if (this->universe > _BASE_SIZE) {
	// check if summary should be updated
	if (this->cluster[this->high(x)]->is_empty()) {
	  this->summary->insert(this->high(x));
	  this->cluster[this->high(x)]->empty_insert(this->low(x));
	} else {
	  this->cluster[this->high(x)]->insert(this->low(x));
	}
  }
  // update max if needed
  if (this->max < x) {
	this->max = x;
  }
}

template<class T>
void veb<T>::delete_element(T x) noexcept {
  // base case
  if ((!this->empty) && (this->min == this->max)) {
	this->empty = true;
	return;
  } else if (this->universe == _BASE_SIZE) {
	// delete x from tree with min < max
	if (x == 0) {
	  this->min = 1;
	} else {
	  this->max = this->min;
	}
	return;
	// make sure x is ready to be deleted, since min is not in the clusters,
	// we need to delete next min and make it the new minimum
  } else if (x == this->min) {
	// find next minimum and make it the new minimum
	// minimum cluster
	T first_cluster = this->summary->minimum();
	// minimum value in minimum cluster converted back to current cluster
	x = this->index(first_cluster, this->cluster[first_cluster]->minimum
		());
	this->min = x;
  }
  // delete x
  this->cluster[this->high(x)]->delete_element(this->low(x));
  // check if summary should be updated
  if (this->cluster[this->high(x)]->is_empty()) {
	// delete cluster from summary
	this->summary->delete_element(this->high(x));
	// update maximum which was deleted
	if (x == this->max) {
	  // the only element is min
	  if (this->summary->is_empty()) {
		this->max = this->min;
	  } else {
		// there is element smaller than last max, find it
		// by finding the maximum summary, and its maximum element
		T summary_max = this->summary->maximum();
		T offset = this->cluster[summary_max]->maximum();
		// convert maximum to current cluster
		this->max = this->index(summary_max, offset);
	  }
	}
	// update max if needed - maximum cluster and its maximum
  } else if (x == this->max) {
	T offset = this->cluster[this->high(x)]->maximum();
	this->max = this->index(this->high(x), offset);
  }
}
template<class T>
T veb<T>::successor(const T x) const noexcept {
  // return null in case of empty tree
  if (this->is_empty()) {
	return this->get_universe_size();
  }
  // base case
  if (this->universe == _BASE_SIZE) {
	if (x == 0 && this->max == 1) {
	  return 1;
	}
	return this->get_universe_size();
  } else if ((!this->empty) && x < this->min) {
	return this->min;
  } else {
	// check if successor in current (x's) cluster
	T max_low = this->cluster[this->high(x)]->maximum();
	// successor in current cluster
	if ((!this->cluster[this->high(x)]->is_empty())
		&& this->low(x) < max_low) {
	  T offset = this->cluster[this->high(x)]->successor(this->low(x));
	  return this->index(this->high(x), offset);
	} else {
	  // find successor of current cluster
	  T succ_cluster = this->summary->successor(this->high(x));
	  // return null if does not exist
	  if (succ_cluster == this->summary->get_universe_size()) {
		return this->get_universe_size();
	  }
	  // find x's successor
	  T offset = this->cluster[succ_cluster]->minimum();
	  return this->index(succ_cluster, offset);
	}
  }
}
template<class T>
T veb<T>::predecessor(T x) const noexcept {
  // return null in case of empty tree
  if (this->is_empty()) {
	return this->get_universe_size();
  }
  // base case
  if (this->universe == _BASE_SIZE) {
	if (x == 1 && this->min == 0) {
	  return 0;
	}
	return this->get_universe_size();
  } else if ((!this->empty) && x > this->max) {
	return this->max;
  } else {
	// check if predecessor in current (x's) cluster
	T min_low = this->cluster[this->high(x)]->minimum();
	// predecessor in current cluster
	if ((!this->cluster[this->high(x)]->is_empty()) && (this->low(x) >
		min_low)) {
	  T offset = this->cluster[this->high(x)]->predecessor(this->low(x));
	  return this->index(this->high(x), offset);
	} else {
	  // find predecessor of current cluster
	  T pred_cluster = this->summary->predecessor(this->high(x));
	  // return null if does not exist
	  if (pred_cluster == this->summary->get_universe_size()) {
		if (x > this->min) {
		  return this->min;
		}
		return this->get_universe_size();
	  }
	  // find x's predecessor
	  T offset = this->cluster[pred_cluster]->maximum();
	  return this->index(pred_cluster, offset);
	}
  }
}

#endif //_VEB_H_
