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
	return x / this->low_root;
  }
  size_t high(T x) const noexcept {
	return (x & (this->low_root - 1));
  }

  void swap(T &x, T &y) const noexcept;

  void delete_cluster() {
	for (size_t i = 0; i < this->low_root; ++i) {
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

};
template<class T>
veb<T>::veb(size_t size) noexcept(false): empty(true), universe(size) {

  if (size == _BASE_SIZE) {
	this->cluster = nullptr;
	this->summary = nullptr;
  } else {
	this->set_bits_amount();
	this->summary = new veb(this->high_root);
	this->cluster = new veb *[this->low_root];
	for (size_t i = 0; i < this->low_root; ++i) {
	  this->cluster[i] = new veb(this->high_root);
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
  if (this->empty) {
	return false;
  } else if (x == this->min || x == this->max){
	return true;
  }
  else if (this->universe == _BASE_SIZE) {
	return false;
  }
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
  if (this->empty) {
	this->empty_insert(x);
  } else if (x < this->min) {
	this->swap(this->min, x);
  }
  if (this->universe > _BASE_SIZE) {
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

#endif //_VEB_H_
