#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <cstddef>
#include <utility>

class Quote {
public:
	Quote() = default;
	Quote(const Quote&) = default;
	Quote(Quote&&) = default;
	Quote& operator=(const Quote&) = default;
	Quote& operator=(Quote&&) = default;
	Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}
	std::string isbn() const { return bookNo; }
	virtual double net_price(std::size_t n) const { return n * price; }
	virtual Quote* clone() const& { return new Quote(*this); }
	virtual Quote* clone() && { return new Quote(std::move(*this)); }
	virtual ~Quote() = default;
private:
	std::string bookNo;
protected:
	double price = 0.0;
};

class Disc_quote : public Quote {
public:
	Disc_quote() = default;
	Disc_quote(const std::string &book, double price, std::size_t qty, double disc) : Quote(book, price), quantity(qty), discount(disc) {}
	std::pair<size_t, double> discount_policy() const { return { quantity, discount }; }
	//pure virtual function
	double net_price(std::size_t) const = 0;
protected:
	std::size_t quantity = 0;
	double discount = 0.0;
};

class Bulk_quote : public Disc_quote {
public:
	Bulk_quote() = default;
	using Disc_quote::Disc_quote;
	//Bulk_quote(const std::string &book, double price, std::size_t qty, double disc) : Disc_quote(book, price, qty, disc) {}
	//must override pure virtual function
	double net_price(std::size_t) const override;
	Bulk_quote* clone() const& { return new Bulk_quote(*this); }
	Bulk_quote* clone() && {return new Bulk_quote(std::move(*this)); }
};

double print_total(std::ostream &os, const Quote &item, size_t n);