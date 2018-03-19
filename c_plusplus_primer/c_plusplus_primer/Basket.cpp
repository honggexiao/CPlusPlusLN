#include "Quote.h"
#include "Basket.h"

#include <cstddef>
using std::size_t;

#include <set>
using std::multiset;

#include <string>
using std::string;

#include <iostream>
using std::ostream; using std::endl;
using std::cout;

double Basket::total_receipt(ostream &os) const
{
	double sum = 0.0;
	for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter))
	{
		sum += print_total(os, **iter, items.count(*iter));
	}
	os << "Total Sale: " << sum << endl;
	return sum;
}