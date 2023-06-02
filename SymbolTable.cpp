#include <algorithm>
#include "SymbolTable.h"
#include "Exception.h"

void SymbolTable::Serialize(Serializer& out) const
{
	out << dictionary_.size();
	for (auto it = dictionary_.begin(); it != dictionary_.end(); ++it)
		out << it->first << it->second;
	out << curId_;
}

void SymbolTable::DeSerialize(DeSerializer& in)
{
	dictionary_.clear();
	// unsigned int size;	// bug!!!
	unsigned long size;
	in >> size;
	for (unsigned int i=0; i<size; ++i)
	{
		std::string str;
		unsigned int id;
		in >> str >> id;
		dictionary_[str] = id;
	}
	in >> curId_;

}

unsigned int SymbolTable::Add(const std::string&  str)
{
	dictionary_[str] = curId_;
	return curId_++;
}

unsigned int SymbolTable::Find(const std::string& str) const
{
	auto it = dictionary_.find(str);
	if (it != dictionary_.end())
		return it->second;

	return IDNOTFOUND;
}

void SymbolTable::Clear()
{
	dictionary_.clear();
	curId_ = 0;
}


std::string SymbolTable::GetSymbolName(unsigned int id) const
{
	auto it = find_if(dictionary_.begin(), dictionary_.end(), 
					[id](const auto& elem)
					{
						return elem.second == id;
					});

	if (it == dictionary_.end())
	{
		throw Exception("Internal error: missing entry in symbol table.");
	}

	return it->first;
}
