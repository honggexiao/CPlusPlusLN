#include "Quote.h"

#include <algorithm>
using std::min;

#include <cstddef>
using std::size_t;

#include <iostream>
using std::ostream; using std::endl;
using std::cout;

double Bulk_quote::net_price(std::size_t cnt) const
{
	if (cnt > min_qty)
		return cnt * (1 - discount) * price;
	else
		return cnt * price;
}