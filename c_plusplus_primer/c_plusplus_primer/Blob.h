#pragma once
#include <iterator>
#include <string>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <initializer_list>

template <typename> class BlobPtr;
template <typename> class Blob;
template <typename T>
bool operator==(const Blob<T>&, const Blob<T>&);

template<typename T>
class Blob {
	friend class BlobPtr<T>;
	friend bool operator==(const Blob<T>&, const Blob<T>&);
public:
	typedef T val_type;
	typedef typename std::vector<T>::size_type size_type;
	Blob();
	Blob(std::initializer_list<T> il);
	//iterator construct function template
	template<typename It> Blob(It b, It e);
	Blob(T*, std::size_t);
	
	BlobPtr<T> begin() { return BlobPtr<T>(*this); }
	BlobPtr<T> end() { auto ret = BlobPtr<T>(*this, data->size()); }

 	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }
	void push_back(const T &t) { data->push_back(t); }
	void push_back(T &&t) { data->push_back(std::move(t)); }
	void pop_back();

	T& front();
	T& back();
	T& at(size_type);
	//designed to const Blob<T> object
	const T& back() const;
	const T& front() const;
	const T& at(size_type) const;
	T& operator[](size_type i);
	const T& operator[](size_type i) const;

	void swap(Blob &b) { data.swap(b.data); }
private:
	std::shared_ptr<std::vector<T>> data;
	void check(size_type i, const std::string &msg) const;
};

//constructor
template<typename T>
Blob<T>::Blob(T *p, std::size_t n) : data(std::make_shared<std::vector<T>>(p, p + n)) { }

template <typename T>
Blob<T>::Blob() : data(std::make_shared<std::vector<T>>()) { }

template<typename T>
template<typename It>
Blob<T>::Blob(It b, It e) : data(std::make_shared<std::vector<T>>(b, e)) {}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) : data(std::make_shared<std::vector<T>>(il)) {}

template<typename T>
void Blob<T>::check(size_type i, const std::string &msg) const
{
	if (i > data->size())
		throw std::out_of_range(msg);
}

template<typename T>
T& Blob<T>::front()
{
	check(0, "front on empty Blob");
	return data->front();
}

template<typename T>
T& Blob<T>::back()
{
	check(0, "back on empty Blob");
	return data->back();
}

template<typename T>
T& Blob<T>::operator[](size_type i)
{
	check(i, "subscirpt out of range");
	return (*data)[i];
}

template<typename T>
void Blob<T>::pop_back()
{
	check(0, "pop_back on empty Blob");
	data->pop_back();
}

template<typename T>
const T& Blob<T>::front() const
{
	check(0, "front on empty Blob");
	return data->front();
}

template <typename T>
const T& Blob<T>::back() const
{
	check(0, "back on empty Blob");
	return data->back();
}

template <typename T>
T& Blob<T>::at(size_type i)
{
	// if i is too big, check will throw, preventing access to a nonexistent element
	check(i, "subscript out of range");
	return (*data)[i];  // (*data) is the vector to which this object points
}

template <typename T>
const T&
Blob<T>::at(size_type i) const
{
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T>
const T&
Blob<T>::operator[](size_type i) const
{
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T>
std::ostream&
operator<<(std::ostream &os, const Blob<T> a)
{
	os << "< ";
	for (size_t i = 0; i < a.size(); ++i)
		os << a[i] << " ";
	os << " >";
	return os;
}

template <typename T>
bool
operator==(const Blob<T> lhs, const Blob<T> rhs)
{
	if (rhs.size() != lhs.size())
		return false;
	for (size_t i = 0; i < lhs.size(); ++i) {
		if (lhs[i] != rhs[i])
			return false;
	}
	return true;
}


template <typename T>
bool operator==(const BlobPtr<T>&, const BlobPtr<T>&);

template<typename T> class BlobPtr {
	friend bool operator==<T>(const BlobPtr<T>&, const BlobPtr<T>&);
public:
	BlobPtr() : curr(0) {}
	BlobPtr(Blob<T> &a, size_t sz = 0) : weak_ptr(a.data), curr(sz) {}
	T& operator*() const 
	{
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}
	T* operator->() const
	{ // delegate the real work to the dereference operator
		return &this->operator*();
	}
	//prefix operators
	BlobPtr& operator++();
	BlobPtr& operator--();
	//postfix operators
	BlobPtr operator++(int);
	BlobPtr operator--(int);
private:
	std::shared_ptr<std::vector<T>> check(std::size_t, const std::string&) const;
	std::weak_ptr<std::vector<T>> wptr;
	std::size_t curr;
};

template <typename T>
bool operator==(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs)
{
	return lhs.wptr.lock().get() == rhs.wptr.lock().get() &&
		lhs.curr == rhs.curr;
}

template <typename T>
bool operator!=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs)
{
	return !(lhs == rhs);
}

template <typename T>
std::shared_ptr<std::vector<T>>
BlobPtr<T>::check(std::size_t i, const std::string &msg) const
{
	auto ret = wptr.lock();   // is the vector still around?
	if (!ret)
		throw std::runtime_error("unbound BlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret; // otherwise, return a shared_ptr to the vector
}
// prefix: return a reference to the incremented/decremented object
template <typename T>
BlobPtr<T>& BlobPtr<T>::operator++()
{
	// if curr already points past the end of the container, can't increment it
	check(curr, "increment past end of BlobPtr");
	++curr;       // advance the current state
	return *this;
}

template <typename T>
BlobPtr<T>& BlobPtr<T>::operator--()
{
	// if curr is zero, decrementing it will yield an invalid subscript
	--curr;       // move the current state back one element
	check(-1, "decrement past begin of BlobPtr");
	return *this;
}
// member operators
// postfix: increment/decrement the object but return the unchanged value
template <typename T>
BlobPtr<T> BlobPtr<T>::operator++(int)
{
	// no check needed here; the call to prefix increment will do the check
	BlobPtr ret = *this;   // save the current value
	++*this;     // advance one element; prefix ++ checks the increment
	return ret;  // return the saved state
}

template <typename T>
BlobPtr<T> BlobPtr<T>::operator--(int)
{
	// no check needed here; the call to prefix decrement will do the check
	BlobPtr ret = *this;  // save the current value
	--*this;     // move backward one element; prefix -- checks the decrement
	return ret;  // return the saved state
}