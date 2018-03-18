#include "Quote.h"
#include <iostream>

using std::ostream;
using std::size_t;
using std::endl;
using std::cout;
double print_total(std::ostream &os, const Quote &item, size_t n)
{
	double ret = item.net_price(n);
	os << "ISBN: " << item.isbn() << " # sold: " << n << " total due: " << ret << endl;
	return ret;
}
void main()
{
	Quote base("0-201-82470-1", 50);
	print_total(cout, base, 10);
	Bulk_quote derived("0-201-82470-1", 50, 5, .19);
	print_total(cout, derived, 10);
	system("pause");
}