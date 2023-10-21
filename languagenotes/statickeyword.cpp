// In most OOP languages accessing instance variables
// arent allowed in static methods; check staticCounter()
// method; We can allow static methods to access instance
// variables only by passing it as a reference.
#include <iostream>

class Foo 
{
	public: //methods
		Foo(): 
			non_static_counter(0) {}
		~Foo() = default;

		static void staticCounter();
		static void accessNonStaticVar(Foo* ref);

	public: // attributes
		static int counter;
		int non_static_counter;
};


int Foo::counter = 0;
void Foo::staticCounter()
{
	counter++;
}

void Foo::accessNonStaticVar(Foo* ref)
{
	ref->non_static_counter++;
}


int main()
{
	Foo* foo = new Foo();

	Foo::staticCounter();

	std::cout << "counter " << Foo::counter << std::endl;

	Foo::accessNonStaticVar(foo);

	std::cout << "counter " << foo->non_static_counter << std::endl;

	delete foo;
}
