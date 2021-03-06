#ifndef HASPTR_H
#define HASPTR_H
#include <iostream>
#include <string>
#include <algorithm>
class HasPtr{
	// swap on page 517
	friend void swap(HasPtr&, HasPtr&);
	// relational operator required by exercise 13.31
	friend bool operator<(const HasPtr&, const HasPtr&);
public:
	HasPtr(const std::string &s = std::string()) : ps(new std::string(s)), i(0) { i = ps->size(); }
	// here, we can use private members directly! Because this is a member 
	// function of class HasPtr. They can access any member of the class. Review
	// member functions, access control and class scope for more details.
	HasPtr(const HasPtr &hp) : ps(new std::string(*(hp.ps))), i(hp.i) { } 
	// move constructor from page 540
	HasPtr(HasPtr &&p) noexcept : ps(p.ps), i(p.i) { p.ps=0; }
	// . has higher precedence, so the parenthesize could be ignored here
	// copy-assignment operator required by exercise 13.8
	HasPtr& operator=(const HasPtr&);
	HasPtr& operator=(HasPtr&&) noexcept;
	std::ostream& print(std::ostream&) const;
	// destructor required by exercise 13.11
	~HasPtr() { delete ps; } 
private:
	std::string *ps;
	int i;
};
HasPtr& HasPtr::operator=(const HasPtr &hp)
{
	// WARNING: ensure self assignment gets the correct result!!!
	// Remember to free resources before deleting!!!
	auto newp = new std::string(*hp.ps);
	delete ps;
	ps = newp;
	i = hp.i;
	return *this; // return this object
}
HasPtr& HasPtr::operator=(HasPtr &&rhs) noexcept
{
	if(this != &rhs)
	{
		ps = rhs.ps;
		i = rhs.i;
		rhs.ps = 0;
	}
	return *this;
}
bool operator<(const HasPtr &lhs, const HasPtr &rhs)
{
	return *lhs.ps < *rhs.ps;
}
std::ostream& HasPtr::print(std::ostream &os) const
{
	if(ps)
		os << *ps;
	return os;
}
inline void swap(HasPtr &lhs, HasPtr &rhs)
{
	using std::swap;
	swap(lhs.ps, rhs.ps);
	swap(lhs.i, rhs.i);
	std::cout << "two HasPtr objects are swapped" << std::endl; // required by exercise 13.30
}
#endif
// value-like copy and assign required by exercise 13.22
// Note: both copy and copy-assign member allocate new memory, so they make the 
// class behave like a value. 