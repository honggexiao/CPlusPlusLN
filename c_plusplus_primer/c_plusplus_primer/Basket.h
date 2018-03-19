#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include "Quote.h"

class Basket {
public:
	//copy the given object
	void add_item(const Quote& sale)
	{
		items.insert(std::shared_ptr<Quote>(sale.clone()));
	}
	//move the given object
	void add_item(Quote &&sale)
	{
		items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
	}
	void add_item(const std::shared_ptr<Quote> &sale) { items.insert(sale); }
	double total_receipt(std::ostream&) const;
private:
	static bool compare(const std::shared_ptr<Quote> &lhs, const std::shared_ptr<Quote> &rhs)
	{
		return lhs->isbn() < rhs->isbn();
	}
	//multiset ordered by isbn
	std::multiset<std::shared_ptr<Quote>, decltype(compare)*>  items{compare};
};

