#ifndef EXER16_61_SP_H
#define EXER16_61_SP_H
#include <cstddef>
#include <iostream>
#include <utility>
#include <functional>
#include <utility>
#include <stdexcept>
#include "Exer16_28_unique_ptr.h"
template <typename T> class shared_ptr;
template <typename T>
void swap(shared_ptr<T>&, shared_ptr<T>&);
template <typename T>
class shared_ptr {
	friend void swap<T>(shared_ptr&, shared_ptr&);
public:
	typedef typename std::function<void(T*)> Deleter;
	// constructors
	shared_ptr() : p(nullptr), ref(new std::size_t(0)), del(nullptr) {}
	// forbid implicit conversion
	explicit shared_ptr(T *q) : p(q), ref(new std::size_t(1)), del(nullptr) {}
	shared_ptr(T *q, Deleter d) : p(q), ref(new std::size_t(1)), del(d) {}
	shared_ptr(const shared_ptr &sp, Deleter d) : p(sp.p), ref(sp.ref), del(d) { ++*ref; }
	shared_ptr(unique_ptr<T>&);
	// copy control operations
	shared_ptr(const shared_ptr &sp) : p(sp.p), ref(sp.ref), del(sp.del) { ++*ref; }
	shared_ptr(shared_ptr &&sp) noexcept : p(std::move(sp.p)), ref(std::move(sp.ref)), del(std::move(sp.del)) { sp.p = nullptr; }
	shared_ptr& operator=(const shared_ptr&);
	shared_ptr& operator=(shared_ptr&&) noexcept;
	~shared_ptr();
	// overloaded operators
	T& operator*() const;
	T* operator->() const;
	explicit operator bool() const { return p; }
	// other operations
	std::size_t use_count() const { return *ref; };
	bool unique() const { return (*ref == 1);}
	T* get() const { return p; }
	void swap(shared_ptr&);
	void reset();
	void reset(T* tq);
	void reset(T*, const Deleter&);
private:
	void free() const;
	T* p; // pointer
	std::size_t *ref; // reference count
	Deleter del; // pointer that points to a callable object
};
template <typename T>
inline void shared_ptr<T>::free() const
{
	if(*ref == 0 || --*ref == 0) {
		del ? del(p) : delete p;
		delete ref;
	}
}
// copy-control members
template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr &rhs)
{
	++*rhs.ref;
	free();
	p = rhs.p;
	ref = rhs.ref;
	del = rhs.del;
	return *this;
}
template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr &&rhs) noexcept
{
	if(this != &rhs) {
		p = rhs.p;
		ref = rhs.ref;
		del = rhs.del;
		rhs.p = nullptr;
	}
	return *this;
}
template <typename T>
shared_ptr<T>::~shared_ptr()
{
	free();
}
// overloaded operators
template <typename T>
inline T& shared_ptr<T>::operator*() const
{
	if(!p)
		throw std::runtime_error("deference null pointer");
	return *p;
}
template <typename T>
inline T* shared_ptr<T>::operator->() const
{
	return &this->operator*();
}
// swap
template <typename T>
inline void swap(shared_ptr<T> &lhs, shared_ptr<T> &rhs)
{
	lhs.swap(rhs);
}
template <typename T>
inline void shared_ptr<T>::swap(shared_ptr &rhs)
{
	using std::swap;
	swap(this->p, rhs.p);
	swap(this->ref, rhs.ref);
	swap(this->del, rhs.del);
}
template <typename T>
void shared_ptr<T>::reset()
{
	free();
	p = nullptr;
	ref = new std::size_t(0); // reset reference count
}
template <typename T>
void shared_ptr<T>::reset(T* tp)
{
	free();
	p = tp;
	ref = new std::size_t(1); // reset reference count
}
template <typename T>
void shared_ptr<T>::reset(T* tp, const Deleter &d)
{
	free();
	p = tp;
	ref = new std::size_t(1); // reset reference count
	del = d;
}
// use variadic function template and forward to substitute const T&, see section 16.4, page 699
// compare this with Exer16_28_shared_ptr.h.
template <typename T, typename... Args>
inline shared_ptr<T> make_shared(Args&&... args)
{
	// use forward to preserve constness and lvalue/rvalue property
	return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
// Note: in fact we could use allocator to substitute new/delete.