/** @file Integer.h
	@brief Contains the class declarations for a class that stores integers without a size limit
	@author Aviva Prins
	@date 2/7/2017

	Positive and negative integer types are stored as objects of a class Integer so that they can be:
	1. added, subtracted, or multiplied:	a + b; a - b; a * b;
	1a.										a += b; a -= b; a *= b;
	2. incremented:							++a; a++; --a; a--;
	3. shifted:								a << increment; a >> increment;
	3a.										a <<= increment; a >>= increment;
	4. change of sign:						+a; -a; ~a;
	5. compared:							a < b; a <= b; a == b; a != b; a >= b; a > b;
	6. compared bitwise:					a & b; a | b; a ^ b;
	6a.										a &= b; a |= b; a ^= b;
	7. displayed in base 10 or base 2:		a.print_as_int(); a.print_as_bits();

	Additional useful variations of the above are also included.
*/

#ifndef INTEGER_H
#define INTEGER_H

#include <vector>

/** @class Integer
	@brief Stores an integer value using a collection of bits and a reserved signed bit.

Stores integer values as vectors of type bool, using the sign and magnitude format. (The sign is stored separately).
See file description (above) for possible manipulations of Integer type variables.

*/
class Integer {
public:
	friend std::ostream& operator<<(std::ostream& out, const Integer& i);
	friend std::istream& operator>>(std::istream& in, Integer& i);

	// Constructors
	Integer();		// Defaults to zero
	Integer(int a); // Converts int to binary

	// Mutators
	Integer& operator+=(const Integer& value); // Adds value to lhs and returns lhs
	Integer& operator-=(const Integer& value); // Subtracts value from lhs and returns lhs
	Integer& operator*=(const Integer& value); // Multiplies value to lhs and returns lhs
	//TODO: dividing Integers (for use in fraction class)

	Integer& operator++();			// Prefix increment
	Integer operator++(int unused); // Postfix increment
	Integer& operator--();			// Prefix decrement
	Integer operator--(int unused);	// Postfix decrement

	Integer operator-() const; // -a: flips the sign of a
	Integer operator+() const; // +a: returns a copy of a
	Integer abs() const;       // |a|: takes the magnitude of a

	// Accessors
	void print_as_int(std::ostream& out) const;  // Prints the lhs in base 10
	void print_as_bits(std::ostream& out) const; // Prints the lhs in base 2

	// Comparisons
	bool operator<(const Integer& rhs) const;	// True if lhs < rhs
	bool operator==(const Integer& rhs) const;  // True if lhs == rhs

	// Bitwise Operators
	Integer& operator&=(const Integer& rhs); // And: changes entry of lhs to entry(lhs&rhs)
	Integer& operator|=(const Integer& rhs); // Or:  changes entry of lhs to entry(lhs|rhs)
	Integer& operator^=(const Integer& rhs); // Xor: changes entry of lhs to entry(lhs^rhs)

	Integer& operator<<=(const unsigned int& increment); // Shift up: shifts lhs left increment number of times
	Integer& operator>>=(const unsigned int& increment); // Shift down: shifts lhs right increment number of times

	Integer operator~() const; // Negation: flips all the bits

private:
	void clean_up(); // Removes leading zeros

	std::vector<bool> bit;
	bool sign; // zero is positive; 1 indicates negative
};

Integer operator+(Integer a, const Integer& b); // Adds a + b
Integer operator-(Integer a, const Integer& b); // Subtracts a - b
Integer operator*(Integer a, const Integer& b); // Multiplies a * b

bool operator!=(const Integer& lhs, const Integer& rhs); // True if lhs != rhs
bool operator>(const Integer& lhs, const Integer& rhs);  // True if lhs > rhs
bool operator<=(const Integer& lhs, const Integer& rhs); // True if lhs <= rhs
bool operator>=(const Integer& lhs, const Integer& rhs); // True if lhs >= rhs

Integer operator&(Integer a, const Integer& b); // And: returns result of a&b (bitwise)
Integer operator|(Integer a, const Integer& b); // Or:  returns result of a|b (bitwise)
Integer operator^(Integer a, const Integer& b); // Xor: returns result of a^b (bitwise)

Integer operator<<(Integer& a, const unsigned int& increment); // Returns a shifted up increment number of times
Integer operator>>(Integer& a, const unsigned int& increment); // Returns a shifted down increment number of times


#endif //Ends the include guard