#include <iostream>
#include "Blob.h"
#include <functional>
#include <type_traits>
#include <sstream>
#include <string.h>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::less;
using std::remove_reference;
using std::ostringstream;
using std::ostream;
template<typename T>
int compare(const T &t1, const T &t2)
{
	if (less<T>()(t1, t2))
		return -1;
	if (less<T>()(t2, t1))
		return 1;
	return 0;
}
template<typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
	return *beg;
}

template<typename It>
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type
{
	return *beg;
}
void f(int v1, int &v2)
{
	cout << v1 << " " << ++v2 << endl;
}
template<typename F, typename T1, typename T2>
void flip1(F f, T1 t1, T2 t2)
{
	f(t2, t1);
}
template<typename F, typename T1, typename T2>
void flip2(F f, T1&& t1, T2&& t2)
{
	f(t2, t1);
}
void g(int&& i, int& j)
{
	cout << i << " " << j << endl;
}
template<typename F, typename T1, typename T2>
void flip(F f, T1&& t1, T2&& t2)
{
	f(std::forward<T2>(t2), std::forward<T1>(t1));
}
//template overload
template<typename T>
string debug_rep(const T& t)
{
	ostringstream ret;
	ret << t;
	return ret.str();
}

template<typename T>
string debug_rep(T *p)
{
	ostringstream ret;
	ret << "pointer: " << p;
	if (p)
		ret << " " << debug_rep(*p);
	else
		ret << "null pointer";
	return ret.str();
}
string debug_rep(const string &s)
{
	return '"' + s + '"';
}

//mutabale function parameter
template<typename T>
ostream &print(ostream &os, const T &t)
{
	return os << t;
}

template<typename T, typename... Args>
ostream &print(ostream &os, const T &t, const Args&... rest)
{
	os << t << ", ";
	return print(os, rest...);
}
//template specialization
template<>
int compare(const char* const &p1, const char* const &p2)
{
	return strcmp(p1, p2);
}
void main()
{
	/*Blob<string> articles = { "a", "an", "the" };
	cout << articles.back()<<endl;
	articles.push_back("honglin");
	cout << articles.back()<<endl;
	Blob<int> squares = { 0, 1, 3, 4, 5, 6, 7, 8, 9 };
	for (size_t i = 0; i != squares.size(); i++)
		squares[i] = i * i;
	cout << squares.back() << endl;*/
	long lng;
	compare<long>(lng, 1024);
	compare<int>(lng, 1024);
	vector<int> vi = { 1, 2, 3, 4, 5 };
	vector<string> ca = { "hi", "bye" };
	cout << fcn(vi.begin(), vi.end()) << endl;
	cout << fcn(ca.begin(), ca.end()) << endl;
	cout << fcn2(vi.begin(), vi.end()) << endl;
	cout << fcn2(ca.begin(), ca.end()) << endl;
	int j = 9;
	flip1(f, j, 42);
	cout << j << endl;
	flip2(f, j, 42);
	cout << j << endl;
	flip(g, j, 42);
	cout << j << endl;
	string s("hi");
	cout << debug_rep(s)<<endl;
	cout << debug_rep(&s) << endl;
	const string *sp = &s;
	cout << debug_rep(sp) << endl;
	cout << debug_rep(s) << endl;
	cout << debug_rep("hi world") << endl;
	int i = 90;
	string u("am");
	print(cout, i, u, 42);
	system("pause");
}