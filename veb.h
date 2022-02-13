//
// Created by David Keisar on 2/11/22.
//

#ifndef _VEB_H_
#define _VEB_H_
#include <iostream>
#define _BASE_SIZE 2
// layer one - methods
// layer two - errors

template<class T=int>
class veb {

  T min;
  T max;
  bool empty;
  veb **cluster;
  veb *summary;
  size_t universe;
  size_t bits;
  size_t rhs_bits;
  size_t lhs_bits;
  size_t high_root;
  size_t low_root;

  void set_bits_amount() noexcept;

  size_t low(T x) const noexcept {
	return (x & (this->low_root - 1));
  }
  size_t high(T x) const noexcept {
//	std::cout << "high: element: " << x << " high x: "
//			  << ((x & (~(this->low_root -
//				  1))) >> this->rhs_bits) << " division: "
//			  << x / this->low_root << std::endl;
	return  ((x & (~(this->low_root - 1))) >> this->rhs_bits);
  }
  size_t index(T x, T y) const noexcept {
	return (this->low_root * x) + y;
  }

  void swap(T &x, T &y) const noexcept;

  void delete_cluster() {
	for (size_t i = 0; i < this->high_root; ++i) {
	  delete this->cluster[i];
	}
	delete[] this->cluster;
  }
  void empty_insert(T x) noexcept;

 public:
  veb(size_t size) noexcept(false);
  ~veb() noexcept;
  bool member(T x) const noexcept;
  size_t minimum() const noexcept {
	return min;
  }
  size_t maximum() const noexcept {
	return max;
  }
  void insert(T x) noexcept;
  bool is_empty() const noexcept {
	return this->empty;
  }
  void delete_element(T x) noexcept;

};
template<class T>
veb<T>::veb(size_t size) noexcept(false): empty(true), universe(size) {
  this->set_bits_amount();
  if (size == _BASE_SIZE) {
	this->cluster = nullptr;
	this->summary = nullptr;
	this->min = 0;
	this->max = 0;
  } else {
	this->summary = new veb(this->high_root);
	this->cluster = new veb *[this->high_root];
	for (size_t i = 0; i < this->high_root; ++i) {
	  this->cluster[i] = new veb(this->low_root);
	}
  }
}
template<class T>
void veb<T>::set_bits_amount() noexcept {
  size_t amount = 0, current = 1;
  while (current < this->universe) {
	++amount;
	current = current << 1;
  }

  this->bits = amount;
  this->rhs_bits = amount / 2;
  this->lhs_bits = amount - this->rhs_bits;
  this->high_root = 1 << this->lhs_bits;
  this->low_root = 1 << this->rhs_bits;
}

template<class T>
veb<T>::~veb() noexcept {
  if (this->universe == _BASE_SIZE) {
	return;
  }
  delete this->summary;
  this->delete_cluster();
}
template<class T>
bool veb<T>::member(T x) const noexcept {
  if (this->empty || x >= this->universe) {
	return false;
  } else if (x == this->min || x == this->max) {
//	std::cout << "in member second if: " << this->min << " universe: "
//														 "" << this->universe
//			  << std::endl;
	return true;
  } else if (this->universe == _BASE_SIZE) {
	return false;
  }
//  std::cout << "third if , member universe: "
//			   "" << this->universe << " high x: " << this->high(x)
//			<< std::endl;
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
  x = x ^ y;
  y = y ^ x;
  x = x ^ y;
}
template<class T>
void veb<T>::insert(T x) noexcept {
  // need to handle invalid arguments such as out of bounds insertions
  if (x >= this->universe) {
	return; // exception
  }
  if (this->empty) {
	this->empty_insert(x);
  } else if (x < this->min) {
	std::cout << this->min<< " before: "<<std::endl;
	this->swap(this->min, x);
	std::cout << this->min<< " after: "<<std::endl;
  }
  if (this->universe > _BASE_SIZE) {
//	std::cout<< "seg fault universe: " <<this->universe << " x: "<<x<<
//	" size of cluster: "<<this->low_root<< " high x: "<< this->high(x)<<
//	std::endl;
//	std::cout << this->cluster[this->high(x)]->is_empty() << std::endl;

	if (this->cluster[this->high(x)]->is_empty()) {
	  this->summary->insert(this->high(x));
	  this->cluster[this->high(x)]->empty_insert(this->low(x));
	} else {
	  this->cluster[this->high(x)]->insert(this->low(x));
	}
  }
  if (this->max < x) {
	this->max = x;
  }
}

template<class T>
void veb<T>::delete_element(T x) noexcept {
//  std::cout<< "low root: "<<this->low_root<<std::endl;

  if ((!this->empty) && (this->min == this->max)) {
	this->empty = true;
	return;
  } else if (this->universe == _BASE_SIZE) {
	if (x == 0) {
	  this->min = 1;
	} else {
	  this->max = this->min;
	}
//	std::cout << " min: " << this->min << std::endl;
	return;
  } else if (x == this->min) {

	T first_cluster = this->summary->minimum();

	x = this->index(first_cluster, this->cluster[first_cluster]->minimum
		());
	std::cout<< x << " x after change"<< " universe size: "<<
	this->universe <<std::endl;
	this->min = x;
//	std::cout << x << std::endl;
  }
//  std::cout<< "high x: "<< this->high(x)<< " low x: "<< this->low(x)
//  <<std::endl;
  this->cluster[this->high(x)]->delete_element(this->low(x));
  if (this->cluster[this->high(x)]->is_empty()) {
	this->summary->delete_element(this->high(x));
	if (x == this->max) {
	  std::cout << "in here" << std::endl;
	  if (this->summary->is_empty()) {
		this->max = this->min;
	  } else {
		T summary_max = this->summary->maximum();
//		std::cout<< "summary max: "<<summary_max<<std::endl;
		T offset = this->cluster[summary_max]->maximum();
		this->max = this->index(summary_max, offset);
	  }
	}
  } else if (x == this->max) {
//	std::cout << "here max 2" << std::endl;
	T offset = this->cluster[this->high(x)]->maximum();
	this->max = this->index(this->high(x), offset);
  }
}

#endif //_VEB_H_
