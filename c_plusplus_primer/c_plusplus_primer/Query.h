#pragma once
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

class Query_Base {
	friend class Query;
protected:
	using line_no = TextQuery::line_no;
	virtual ~Query_Base() = default;
private:
	virtual QueryResult eval(const TextQuery&) const = 0;
	virtual std::string rep() const = 0;
};

class Query {
	friend Query operator~(const Query&);
	friend Query operator&(const Query&, const Query&);
	friend Query operator|(const Query&, const Query&);
public:
	Query(const std::string&);
	QueryResult eval(const TextQuery &t) const { return q->eval(t); }
	std::string rep() const { return q->rep(); }
private:
	//private constructor
	Query(std::shared_ptr<Query_Base> query) : q(query) {}
	std::shared_ptr<Query_Base> q;
};

inline std::ostream& operator<<(std::ostream &os, const Query &query)
{
	return os << query.rep();
}

class WordQuery : public Query_Base {
	friend class Query;
	WordQuery(const std::string &s) : query_word(s) {}
	//construct query result
	QueryResult eval(const TextQuery &t) const { return t.query(query_word); }
	std::string rep() const { return query_word; }
	std::string query_word;
};

inline Query::Query(const std::string &s) : q(new WordQuery(s)) {}

class NotQuery : public Query_Base {
	friend Query operator~(const Query&);
	NotQuery(const Query &q) : query(q) {}
	std::string rep() const { return "~(" + query.rep() + ")"; }
	QueryResult eval(const TextQuery&) const;
	Query query;
};

inline Query operator~(const Query &operand)
{
	//Query private construct Query is not explict, so there is con
	return std::shared_ptr<Query_Base>(new NotQuery(operand));
}

class BinaryQuery : public Query_Base {
protected:
	BinaryQuery(const Query &l, const Query &r, std::string s) : lhs(l), rhs(r), opSym(s) {}
	std::string rep() const { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }
	Query lhs, rhs;
	std::string opSym;
};

class AndQuery : public BinaryQuery {
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "&") {}
	QueryResult eval(const TextQuery&) const;
};

inline  Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_Base>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
	OrQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "|") {}
	QueryResult eval(const TextQuery&) const;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_Base>(new OrQuery(lhs, rhs));
}

TextQuery get_file(int, char**);
bool get_word(std::string&);
bool get_words(std::string&, std::string&);
