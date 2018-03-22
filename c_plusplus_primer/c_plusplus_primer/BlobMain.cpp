#include <iostream>
#include "Blob.h"
#include <functional>
#include <type_traits>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::less;
using std::remove_reference;
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
	system("pause");
}