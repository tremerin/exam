#include <iostream>

class Warlock {

	private :
		Warlock & operator=(Warlock const & rhs);
		Warlock(Warlock const & obj);
		Warlock();
		std::string _name;
		std::string _title;
		
	public :

		Warlock(std::string name, std::string title);
		~Warlock();
		std::string const & getName() const; 
		std::string const & getTitle() const;
		void	setTitle(std::string const & str);
		void	introduce() const;
};

/*
	>>> CONST <<<
	En una variable el const puede estar a la izquiera o a la derecha del tipo:
		Warlock & operator=(Warlock const & rhs); es lo mismo que: Warlock & operator=(Warlock const & rhs);
		std::string const & getName() const; es lo mismo que:  const std::string & getName() const;
	El const al final de un metodo lo convierte en un metodo const.
	Un objeto const solo puede llamar metedos const.
	Un metodo const es de solo lectura.
	Una variable const es de solo lectura.

	>>> PRIVATE VS PUBLIC <<<
	En el subject nos piden: (Your Warlock will also have, in addition to whatever's required by Coplien's
	form, a constructor that takes, in this order, its name and title. Your Warlock
	will not be able to be copied, instantiated by copy, or instantiated without a
	name and a title.)
	Ponemos el constructor por defecto, el de copia y el de asignación en private. 	
	El constructor con parametros y el destructor en public. 

*/