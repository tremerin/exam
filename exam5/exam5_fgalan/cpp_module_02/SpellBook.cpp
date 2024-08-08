#include "SpellBook.hpp"

SpellBook::SpellBook(SpellBook const & src)
{
	*this = src;
}

SpellBook & SpellBook::operator=(SpellBook const & src)
{
	_SpellBook = src._SpellBook;
	return (*this);
}

SpellBook::SpellBook()
{}

SpellBook::~SpellBook()
{
	std::map<std::string, ASpell*>::iterator it = _SpellBook.begin();
	for ( ; it != _SpellBook.end(); ++it) {
		delete it->second;
	}
	_SpellBook.clear();
}

void SpellBook::learnSpell(ASpell* spell)
{
	if (spell)
	{
		if (_SpellBook.find(spell->getName()) == _SpellBook.end())
			_SpellBook[spell->getName()] = spell->clone();
	}
}

void SpellBook::forgetSpell(std::string const & SpellName)
{
	if (_SpellBook.find(SpellName) != _SpellBook.end())
			_SpellBook.erase(_SpellBook.find(SpellName));
}

ASpell* SpellBook::createSpell(std::string const & SpellName)
{
	ASpell* tmp = NULL;
	if (_SpellBook.find(SpellName) != _SpellBook.end())
		tmp = _SpellBook[SpellName];
	return (tmp);
}