/** @file main.cpp
	@author Aviva Prins
	@date 2/15/2017
	
Recursion using the Integer class. 
We compute tables for:
Partition:						p(k, n) = p(k, n-k) + p(k-1, n)				1 <= k <= n
Stirling numbers (1st kind):	s(k, n) = -(n-1) s(k, n-1) + s(k-1, n-1)	1 <= k <= n
Stirling numbers (2nd kind):	S(k, n) = kS(k, n-1) + S(k-1, n-1)			1 <= k <= n
Factorial:						n! = n*(n-1)!
*/

#include <iostream>
#include <fstream>
#include "Integer.h"
#include <cmath>
#include <vector>
#include <string>

// Recursive functions
Integer p(Integer k, Integer n); // Partition
Integer s(Integer k, Integer n); // Stirling #1
Integer S(Integer k, Integer n); // Stirling #2
Integer operator!(Integer n);	 // Factorial

int main() {

    // First, let's show off that the Integer class can hold more digits than an int variable
    Integer a = __INT_MAX__;
    Integer b = a + 1;
    std::cout << "Fun Fact: the largest number an int can hold is " << a << std::endl;
    std::cout << "Let's add one to that number:                   " << b << std::endl;
    // However, in this implementation the constructor with parameters accepts int variables 
    // If you wish, you can change it to be long long. 

	// Prompt user for size of table
	int m;
	std::cout << "Please input the size (mxm) of your table: ";
	std::cin >> m;

	// 1. Partition
	// Open file for writing
	std::ofstream outputData1("Partition.txt");
	std::cout << "Creating Partition.txt file ... ";

	for (size_t i = 1; i <= m; ++i) { // Generate ith entry (row)
		for (size_t j = 1; j <= m; ++j) { // Generate jth entry (column)
			Integer k = i;
			Integer n = j;
			outputData1 << p(k, n) << " ";
		}
		outputData1 << std::endl; // Go to next row (enter)
	}


	std::cout << "Done." << std::endl;
	outputData1.close();

	// 2. Stirling -- first kind
	// Open file for writing
	std::ofstream outputData2("Stirling1.txt");
	std::cout << "Creating Stirling1.txt file ... ";

	for (size_t i = 1; i <= m; ++i) { // Generate ith entry (row)
		for (size_t j = 1; j <= m; ++j) { // Generate jth entry (column)
			Integer k = i;
			Integer n = j;
			outputData2 << s(k, n) << " ";
		}
		outputData2 << std::endl; // Go to next row (enter)
	}

	std::cout << "Done." << std::endl;
	outputData2.close();

	// 3. Stirling -- second kind
	// Open file for writing
	std::ofstream outputData3("Stirling2.txt");
	std::cout << "Creating Stirling2.txt file ... ";

	for (size_t i = 1; i <= m; ++i) { // Generate ith entry (row)
		for (size_t j = 1; j <= m; ++j) { // Generate jth entry (column)
			Integer k = i;
			Integer n = j;
			outputData3 << S(k, n) << " ";
		}
		outputData3 << std::endl; // Go to next row (enter)
	}

	std::cout << "Done." << std::endl;
	outputData3.close();

	// 4. Factorial
	// Open file for writing
	std::ofstream outputData4("Factorials.txt");
	std::cout << "Creating Factorials.txt file ... ";

	Integer n = 1; // start with 1!

	while (n <= m) { // coercion: converts m to an Integer (and runs for m times)
		outputData4 << !n << std::endl;
		++n;
	}

	std::cout << "Done." << std::endl;
	outputData4.close();

	return 0;
}

/** Computes p(k,n)
	@param k first input
	@param n second input
	@return p(k,n)
*/
Integer p(Integer k, Integer n) { 

	if (k < 0 || n < 0) { // base case: any negative input returns 0
		return 0;
	}
	else if (k == 0 && n > 0) { // base case: p(0, n) = 0 for n > 0
		return 0;
	}
	else if (n == 0 && k >= 0) { // base case: p(k, 0) = 1 for k >= 0
		return 1;
	}
	else if (k > n) { // base case: p (k, n) = p(n,n) for k > n
		return p(n, n);
	}
	else { // inductive step: p(k, n) = p(k, n-k) + p(k-1, n)
		return p(k, n - k) + p(k - 1, n);
	}
	
}

/** Computes s(k,n) i.e. Stirling numbers of the first kind
	@param k first input
	@param n second input
	@return s(k,n)
*/
Integer s(Integer k, Integer n) {
	if (k < 0 || n < 0) { // base case: any negative input returns 0
		return 0;
	}
	else if (k == 0 && n > 0) { // base case: s(0, n) = 0 for n > 0
		return 0;
	}
	else if (n == 0 && k > 0) { // base case: s(k, 0) = 0 for k > 0
		return 0;
	}
	else if (k > n) { // base case: s(k, n) = 0 for k > n
		return 0;
	}
	else if (k == n) { // base case: s(n, n) = 1
		return 1;
	}
	else { // inductive step: s(k, n) = -(n - 1)*s(k, n - 1) + s(k - 1, n - 1)
		return -((n - 1)*s(k, n - 1)) + s(k - 1, n - 1);
	}
}

/** Computes S(k,n) i.e. Stirling numbers of the second kind
	@param k first input
	@param n second input
	@return S(k,n)
*/
Integer S(Integer k, Integer n) {
	if (k < 0 || n < 0) { // base case: any negative input returns 0
		return 0;
	}
	else if (k == 0 && n > 0) { // base case: S(0, n) = 0 for n > 0
		return 0;
	}
	else if (n == 0 && k > 0) { // base case: S(k, 0) = 0 for k > 0
		return 0;
	}
	else if (k > n) { // base case: S(k, n) = 0 for k > n
		return 0;
	}
	else if (k == n) { // base case: S(n, n) = 1
		return 1;
	}
	else { // inductive step: k*S(k, n - 1) + S(k - 1, n - 1)
		return k*S(k, n - 1) + S(k - 1, n - 1);
	}
}

/** Computes n! i.e. factorial of n
	@param n input
	@return n!
*/
Integer operator!(Integer n) {
	Integer result;
	if (n > 1) { // inductive step: n! = n*(n-1)!
		result = n*!(n - 1);
	}
	else { //base case: 1! = 1 and 0! = 1
		result = 1; 
	}
	return result;
}