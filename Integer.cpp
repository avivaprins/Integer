/** @file Integer.cpp
	@author Aviva Prins
	@date 1/18/2017

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

Additional useful functions defined outside of the Integer class:
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

*/

#include <iostream>
#include <fstream>
#include "Integer.h"
#include <cmath>
#include <vector>
#include <string>

/** Output operator <<
	@param out allows std::cout, etc output types
	@param i accepts an Integer type output
	@return out so that << can be called consecutively
*/
std::ostream& operator<<(std::ostream& out, const Integer& i) {
	i.print_as_int(out);
	return out;
}

/** Input operator >>
	@param in allows std::cin, etc input types
	@param i accepts an Integer type input
	@return in so that >> can be called consecutively
*/
std::istream& operator>>(std::istream& in, Integer& i) {
	int input;
	in >> input;
	i = input;
	return in;
}


/** Default constructor of Integer class
*/
Integer::Integer() {
	bit = { 0 };
	sign = 0;
}

/** Constructor of Integer class with parameter
    @param a is an integer in base 10, which is stored in base 2
*/
Integer::Integer(int a) {
	bit = {};    // Intialize bit
	if (a < 0) { // If a is negative, sign is negative
		sign = 1;
		a = -a;  // We change the sign of a for the next step: binary conversion
	}
	else {		 // Otherwise, a is positive and the sign is positive
		sign = 0;
	}

	// Integer to binary conversion
	while (a > 1) { // If a > 1, its binary representation will be more than one digit long (digits are determined in reverse order in this method)
		if (a % 2 == 0) { // Then a is divisible by 2 and inserts 0 at first position, moving the other entries back
			bit.insert(bit.begin(), 0);
		}
		else { // a is not divisible by 2: inserts the remainder (1) at first position, moving the other entries back
			bit.insert(bit.begin(), 1);
		}
		a = a / 2; // integer division
	}

	// Using the above method, a = 1 and the first digit is one
	if (a == 1) {
		bit.insert(bit.begin(), 1);
	}

	// The only exception to the above is if the initial value is zero
	else if (a == 0) {
		bit.insert(bit.begin(), 0);
	}

	else { // We've covered all the cases, so this should never be printed (for debugging purposes only)
		std::cout << "Error in constructor class!";
	}
}

/** Adds value to lhs
    @param value the value being added, in base 2
*/
Integer& Integer::operator+=(const Integer& value) {
	// Temp values: a + b = c
	Integer a;         // Copy of lhs
	a.bit = bit;
	a.sign = sign;
	Integer b = value; // Amount being added to lhs (copy)
	b.sign = value.sign;
	Integer c = 0;     // Solution to addition of a and b

	// Determine sign of c
	// If the magnitude of a is less than the magnitude of b, c is the sign of b
	if (a.sign == b.sign) {
		c.sign = a.sign;
	}
	// Note: Integers are stored without leading zeros, so comparing sizes can determine inequality.
	else {
		if (a.bit.size() < b.bit.size()) { // if length lhs < length rhs ==> lhs < rhs
			c.sign = b.sign;
		}
		else if (a.bit.size() > b.bit.size()) { // if length lhs > length rhs ==> lhs > rhs
			c.sign = a.sign;
		}
		else { // length of lhs == length rhs (need to compare bitwise)
			for (size_t i = a.bit.size() - 1; i <= 0; --i) { // compares each bit, starting with the first
				if (a.bit[i] < b.bit[i]) { // entry of a < entry of b
					c.sign = b.sign; // a < b
					break;
				}
				else if (a.bit[i] > b.bit[i]) { // entry of a > entry of b
					c.sign = a.sign; // a > b
					break;
				}
				else { //lhs == rhs for all entries
					c.sign = a.sign;
				}
			}
		}
	}


	// Size Increase
	// We need to increase the sizes of a and b so that they are the same size and have two leading zeros (so there's no index issue for carrying)
	if (a.bit.size() < b.bit.size()) { // Then increase a with leading zeros to 2 + the size of b
		while (a.bit.size() < b.bit.size() + 2) { // Increases size of a until it is 2 + the size of b
			a.bit.insert(a.bit.begin(), 0);
		}
		b.bit.insert(b.bit.begin(), 0); // Increase b
		b.bit.insert(b.bit.begin(), 0);
	}
	else if (a.bit.size() > b.bit.size()) { // Then increase b with leading zeros to 2 + the size of a
		while (b.bit.size() < a.bit.size() + 2) { // Increases size of b until it is 2 + the size of a
			b.bit.insert(b.bit.begin(), 0);
		}
		a.bit.insert(a.bit.begin(), 0); // Increase a
		a.bit.insert(a.bit.begin(), 0);
	}
	else { // Then a is the same size as b. Increase both with two leading zeros.
		a.bit.insert(a.bit.begin(), 0);
		a.bit.insert(a.bit.begin(), 0);
		b.bit.insert(b.bit.begin(), 0);
		b.bit.insert(b.bit.begin(), 0);
	}

	// Two's Compelement Conversion
	// If a or b are negative, we convert them to two's compelement to solve
	if (a.sign == 1) {
		//convert to two's complement: flip bits (including signed bit), then add one 
		a = ~a;
		++a;
	}
	if (b.sign == 1) {
		//convert to two's complement: flip bits (including signed bit), then add one
		b = ~b;
		++b;
	}

	// Addition Operation
	bool carry = 0; // "carried" digit in addition method
	unsigned int temp = 0; // temporary value used in addition method

	// Adds each digit, starting with the last entry (and carrying, if necessary)
	for (size_t i = 0, n = a.bit.size(); i < n; ++i) { //credit: Prof. DeSalvo's Syle lecture
		temp = a.bit[a.bit.size() - 1 - i] + b.bit[b.bit.size() - 1 - i] + carry;
		c.bit.insert(c.bit.begin(), temp % 2); //note: last digit of c is zero from initialization. Next step is to remove the erroneous zero.
		carry = temp / 2;
	}
	// Remove trailing zero (erroneous)
	c.bit.pop_back();

	// If c is negative, convert from two's complement back to sign and magnitude (note that the sign of c has already been determined, so we don't flip the sign)
	sign = c.sign;
	if (c.sign == 1) {
		c = ~c;
		++c;
	}

	// Remove leading zeros (if there are any)
	c.clean_up();

	bit = c.bit; //reassigns c to lhs
	return (*this);
}

/** Subtracts value from lhs
    @param value the value being subtracted, in base 2
*/
Integer& Integer::operator-=(const Integer& value) {
	// We create a new variable with opposite sign of value
	Integer b = -value;

	// Now lhs - value becomes lhs + (-value) and we use the += operator
	(*this) += b;
	(*this).clean_up();
	return (*this);
}

/** Multiplies value to lhs
    @param value the value being multiplied, in base 2
*/
Integer& Integer::operator*=(const Integer& value) {
	// Temp values:  a * b = c							Example: 39*48
	// Equivalently: a * (temp[1] + 10*temp[2] + ... )  Example: 39*(8 + 40)
	Integer a;        // Copy of lhs
	a.bit = bit;
	a.sign = sign;
	Integer temp = value; // Amount being mulitiplied to lhs (copy of rhs)
	temp.sign = 0;		  // Temp value is always positive; sign of solution is determined independently
	Integer c = 0;        // Solution to multiplication of a and b

	// Multiplication Operation
	// Computed along a, starting with the last digit
	for (size_t i = 0, n = a.bit.size(); i < n; ++i) { //credit: Prof. DeSalvo's Syle lecture
		// If the digit is one, add b + trailing zero correction
		if (a.bit[a.bit.size() - 1 - i] == 1) {
			c += temp;
		}
		// Trailing zero correction: 
		temp.bit.push_back(0);
	}

	// Determine sign
	if (a.sign == value.sign) { // The signs are the same, so the solution is positive
		c.sign = 0;
	}
	else { // The signs are different, so the solution is negative
		c.sign = 1;
	}

	c.clean_up();
	bit = c.bit;   //reassigns c to lhs
	sign = c.sign;
	return (*this);
}

/** Prefix increment: adds one to lhs
*/
Integer& Integer::operator++() {
	Integer one = 1;
	(*this) += one;	//increments the object
	return (*this);	//returns the object
}

/** Postfix increment: adds one to lhs
*/
Integer Integer::operator++(int unused) {
	Integer clone(*this);	// makes a copy
	++(*this);				// increments the object
	return clone;			// returns the clone
}

/** Prefix decrement: subtracts one from lhs
*/
Integer& Integer::operator--() {
	Integer one = 1;
	*this -= one;	//decrements the object
	return *this;	//returns the object
}

/** Postfix decrement: subtracts one from lhs
*/
Integer Integer::operator--(int unused){
	Integer clone(*this);	// makes a copy
	--(*this);				// decrements the object
	return clone;			// returns the clone
}

/** Flips the sign
    @return the implicit parameter with opposite sign
*/
Integer Integer::operator-() const {
	Integer a;
	a.bit = bit;
	a.sign = !(sign);

	return a;
}

/** Returns a copy of the implicit parameter
    @return the implicit parameter
*/
Integer Integer::operator+() const {
	Integer a;
	a.bit = bit;
	a.sign = sign;

	return a;
}

/** Returns the magnitude of the implicit parameter
    @return the magnitude of the implicit parameter
*/
Integer Integer::abs() const {
	Integer a;
	a.bit = bit;
	a.sign = 0;
	
	return a;
}

/** Prints the lhs in base 10: "####" if positive, and "-####" if negative
    @param out specifies the type of output: cout or outputData
*/
void Integer::print_as_int(std::ostream& out) const {
	std::string negative_indicator = "";
	if (sign == 1) { // If the number is negative, output is of the form -####
		negative_indicator = "-";
	}

	unsigned int numb = 0;

	// converts from binary to decimal
	for (size_t i = 0, n = bit.size(); i < n; ++i) { //credit: Prof. DeSalvo's Syle lecture
		numb += bit[i] * pow(2, (bit.size() - 1 - i));
	}

	out << negative_indicator << numb;
}

/** Prints the lhs in base 2: "(#####)_2" or "-(#####)_2"
    @param out specifies the type of output: cout or outputData
*/
void Integer::print_as_bits(std::ostream& out) const {
	Integer a;
	a.bit = bit;
	a.sign = sign;

	std::string negative_indicator = " ";
	if ((sign == 1) && (bit[0] != 0)) { // If the number is negative, output is of the form -(#####)_2 unless it is zero
		negative_indicator = "-";		// Note this assumes no leading zeros
	}

	out << negative_indicator << "(";

	//prints the number in binary
	for (size_t i = 0, n = bit.size(); i < n; ++i) { //credit: Prof. DeSalvo's Syle lecture
		out << bit[i];
	}

	out << ")_2";
}

/** Returns true if lhs < rhs (otherwise returns false)
    @param rhs
    @return true if lhs < rhs
*/
bool Integer::operator<(const Integer& rhs) const {
	if (sign == rhs.sign) { // The values have the same sign, so we cannot make a quick deduction based on that.
		if (sign == 0) {
			// Note: Integers are stored without leading zeros, so comparing sizes can determine inequality.
			if (bit.size() < rhs.bit.size()) { // if length lhs < length rhs ==> lhs < rhs
				return true;
			}
			else if (bit.size() > rhs.bit.size()) { // if length lhs > length rhs ==> lhs > rhs
				return false;
			}
			else { // length of lhs == length rhs (need to compare bitwise)
				for (size_t i = 0, n = bit.size(); i < n; ++i) { // compares each bit, starting with the first
					if (bit[i] < rhs.bit[i]) { // entry of lhs < entry of rhs
						return true; // returns true and stops loop
					}
					else if (bit[i] > rhs.bit[i]) { // entry of lhs > entry of rhs
						return false; // returns false and stops loop
					}
				}
				return false; // if loop completes, lhs == rhs for all entries
			}
		}
		else { // For negative numbers, a larger magnitude indicates less than: -20 < -2 < 1 < 20
			if (bit.size() > rhs.bit.size()) { // if length lhs > length rhs ==> lhs < rhs
				return true;
			}
			else if (bit.size() < rhs.bit.size()) { // if length lhs < length rhs ==> lhs > rhs
				return false;
			}
			else { // length of lhs == length rhs (need to compare bitwise)
				for (size_t i = 0, n = bit.size(); i < n; ++i) { // compares each bit, starting with the first
					if (bit[i] > rhs.bit[i]) { // entry of lhs > entry of rhs
						return true; // returns true and stops loop
					}
					else if (bit[i] < rhs.bit[i]) { // entry of lhs < entry of rhs
						return false; // returns false and stops loop
					}
				}
				return false; // if loop completes, lhs == rhs for all entries
			}
		}
	}
	else if (sign == 1) { // Then lhs is negative and rhs is positive ==> return true
		return true;
	}
	else { // Then lhs is positive and rhs is negative ==> return false
		return false;
	}
}

/** Returns true if lhs == rhs (otherwise returns false)
    @param rhs
    @return true if lhs == rhs
*/
bool Integer::operator==(const Integer& rhs) const {
	if (sign == rhs.sign) { // The values have the same sign
		// Note: Integers are stored without leading zeros, so comparing sizes can determine equality.
		if (bit.size() != rhs.bit.size()) { // if length lhs != length rhs ==> lhs != rhs
			return false;
		}
		else { // length of lhs == length rhs (need to compare bitwise)
			for (size_t i = 0, n = bit.size(); i < n; ++i) { // compares each bit, starting with the first
				if (bit[i] != rhs.bit[i]) { // entry of lhs != entry of rhs
					return false; // returns false and stops loop
				}
			}
			return true; // if loop completes, lhs == rhs for all entries
		}
	}
	else { // The values have different sign, so they are not equal
		return false;
	}
}

/** And equals: changes lhs to the bit-wise comparison of lhs&rhs
    @param rhs value that lhs is being compared to
    @return the bit-wise comparison lhs&rhs
*/
Integer& Integer::operator&=(const Integer& rhs) {
	Integer a = rhs; // copy of rhs
	a.sign = rhs.sign;

	while (a.bit.size() < bit.size()) { // If a is shorter than lhs, increase a to the same length with leading zeros
		a.bit.insert(a.bit.begin(), 0);
	}

	while (bit.size() < a.bit.size()) { // If lhs is shorter than a, increase lhs to the same length with leading zeros
		bit.insert(bit.begin(), 0);
	}

	for (size_t i = 0, n = bit.size(); i < n; ++i) { // Compares each entry and changes the lhs accordingly
		bit[i] = bit[i] & a.bit[i];
	}

	(*this).clean_up(); //remove leading zeros
	return (*this);
}

/** Or equals: changes lhs to the bit-wise comparison of lhs|rhs
    @param rhs value that lhs is being compared to
    @return the bit-wise comparison lhs|rhs
*/
Integer& Integer::operator|=(const Integer& rhs) {
	Integer a = rhs; // copy of rhs
	a.sign = rhs.sign;

	while (a.bit.size() < bit.size()) { // If a is shorter than lhs, increase a to the same length with leading zeros
		a.bit.insert(a.bit.begin(), 0);
	}

	while (bit.size() < a.bit.size()) { // If lhs is shorter than a, increase lhs to the same length with leading zeros
		bit.insert(bit.begin(), 0);
	}

	for (size_t i = 0, n = bit.size(); i < n; ++i) { // Compares each entry and changes the lhs accordingly
		bit[i] = bit[i] | a.bit[i];
	}

	(*this).clean_up(); //remove leading zeros
	return (*this);
}

/** Xor equals: changes lhs to the bit-wise comparison of lhs^rhs
    @param rhs value that lhs is being compared to
    @return the bit-wise comparison lhs^rhs
*/
Integer& Integer::operator^=(const Integer& rhs) {
	Integer a = rhs; // copy of rhs
	a.sign = rhs.sign;

	while (a.bit.size() < bit.size()) { // If a is shorter than lhs, increase a to the same length with leading zeros
		a.bit.insert(a.bit.begin(), 0);
	}

	while (bit.size() < a.bit.size()) { // If lhs is shorter than a, increase lhs to the same length with leading zeros
		bit.insert(bit.begin(), 0);
	}

	for (size_t i = 0, n = bit.size(); i < n; ++i) { // Compares each entry and changes the lhs accordingly
		bit[i] = bit[i] ^ a.bit[i];
	}

	(*this).clean_up(); //remove leading zeros
	return (*this);
}

/** Shifts lhs left increment number of times
    @param increment the number of times to shift lhs left
*/
Integer& Integer::operator<<=(const unsigned int& increment) {
	for (size_t i = 0; i < increment; ++i) {
		bit.push_back(0);
	}
	(*this).clean_up();
	return (*this);
}

/** Shifts lhs right increment number of times
    @param increment the number of times to shift lhs right
*/
Integer& Integer::operator>>=(const unsigned int& increment) {
	for (size_t i = 0; i < increment; ++i) {
		bit.pop_back();
	}
	(*this).clean_up();
	return (*this);
}

Integer Integer::operator~() const {
	Integer a; // Copy the implicit parameter
	a.bit = bit;
	a.sign = !(sign);

	a.bit.flip();
	a.clean_up();
	return a;
}

/** Removes leading zeros, if the number is not 0
*/
void Integer::clean_up() {
	for (size_t i = 0, n = bit.size(); i < n; ++i) {
		if (bit[0] == 0 && bit.size() != 1) { //check if lhs has a leading zero (and isn't zero)
			bit.erase(bit.begin()); //remove the zero
		}
	}
}

/** Adds a and b; returns the result
    @param a of type Integer
    @param b of type Integer
    @return a + b
*/
Integer operator+(Integer a, const Integer& b) {
	return a += b;
}

/** Subtracts a and b; returns the result
    @param a of type Integer
    @param b of type Integer
    @return a - b
*/
Integer operator-(Integer a, const Integer& b) {
	return a -= b;
}

/** Multiplies a and b; returns the result
    @param a
    @param b
    @return a * b
*/
Integer operator*(Integer a, const Integer& b) {
	return a *= b;
}

/** Returns true if lhs != rhs (otherwise, false)
    @param lhs
    @param rhs
    @return true if lhs != rhs
*/
bool operator!=(const Integer& lhs, const Integer& rhs) {
	return !(lhs == rhs);
}

/** Returns true if lhs > rhs (otherwise, false)
    @param lhs
    @param rhs
    @return true if lhs > rhs
*/
bool operator>(const Integer& lhs, const Integer& rhs) {
	return rhs < lhs;
}

/** Returns true if lhs <= rhs (otherwise, false)
    @param lhs
    @param rhs
    @return true if lhs <= rhs
*/
bool operator<=(const Integer& lhs, const Integer& rhs) {
	return !(lhs > rhs);
}

/** Returns true if lhs >= rhs (otherwise, false)
    @param lhs
    @param rhs
    @return true if lhs >= rhs
*/
bool operator>=(const Integer& lhs, const Integer& rhs) {
	return !(lhs < rhs);
}

/** Returns the bit-wise result of a&b
    @param a
    @param b
    @return a&b (bit-wise comparison)
*/
Integer operator&(Integer a, const Integer& b){
	return a &= b;
}

/** Returns the bit-wise result of a|b
    @param a
    @param b
    @return a|b (bit-wise comparison)
*/
Integer operator|(Integer a, const Integer& b) {
	return a |= b;
}

/** Returns the bit-wise result of a^b
    @param a
    @param b
    @return a^b (bit-wise comparison)
*/
Integer operator^(Integer a, const Integer& b) {
	return a ^= b;
}

/** Returns a shifted left increment number of times
    @param a
    @param increment
    @return a shifted left increment number of times
*/
Integer operator<<(Integer& a, const unsigned int& increment) {
	return a <<= increment;
}
/** Returns a shifted right increment number of times
    @param a
    @param increment
    @return a shifted right increment number of times
*/
Integer operator>>(Integer& a, const unsigned int& increment) {
	return (a >>= increment);
}