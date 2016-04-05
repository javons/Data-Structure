/** @mainpage
 *
 * CISC 2200: Data Structures<br/>
 * Project 1: Computing Powers<br/>
 *
 * <p>Compare the performance of three different algorithms for 
 *    computing powers: </p> 
 * <p>   · Iterative </p>
 * <p>   · Naive recursive (directly based on the iterative scheme)</p>
 * <p>   · Clever (divide-and-conquer) recursive </p>
 * <p>We test these methods by computing 2<em><sup>n</sup></em> for 
 * <em>n=</em>0,1,2,..,32 with base 2 and for 2<em><sup>n</sup></em>
 * <em>n=</em>0,1,2,...64 with base 0.5. </p> 
 *
 * @author Tianmao Jiang
 * @date 4 Feburary 2016
 * @file proj1.cc
 *
 **********************************************************************/

#include <iostream>
using namespace std;

// function prototypes


template <class T>
T power1(T x, unsigned int n, unsigned int& mults);

template <class T>
T power2(T x, unsigned int n, unsigned int& mults);

template <class T>
T power3(T x, unsigned int n, unsigned int& mults);

template <class T>
void printReport(T base, unsigned int pow, 
                 T result1, T result2, T result3,
                 unsigned int mults1, unsigned int mults2, 
                 unsigned int mults3);

/**
 * The usual main function.
 * 
 * It computes the powers 2<sup><em>i</em></sup> for
 * <em>i=</em>0,1,2,..,32 via all three methods, reporting the number
 * of multiplications needed for each method.  (Note what happens for
 * 2^32.)  It then does likewise for 0.5<sup><em>i</em></sup>, but now
 * with <em>i=</em>0,1,2,...64
 *
 */
int main()
{
    unsigned int mults1, mults2, mults3;
    cout << "Test for integer base:\n";
    for (unsigned int pow = 0; pow <= 32; pow++) {
	unsigned int base = 2;
	unsigned int result1 = power1(base, pow, mults1);
	unsigned int result2 = power2(base, pow, mults2);
	unsigned int result3 = power3(base, pow, mults3);      
	printReport(base, pow, result1, result2, result3, 
		    mults1, mults2, mults3);
    }
    cout << "\nTest for floating-point base:\n";
    for (unsigned int pow = 0; pow <= 64; pow++) {
	double base = 0.5;
	double result1 = power1(base, pow, mults1);
	double result2 = power2(base, pow, mults2);
	double result3 = power3(base, pow, mults3);      
	printReport(base, pow, result1, result2, result3, 
		    mults1, mults2, mults3);
    }
}

/**
 * Iterative function to compute a power <em>x<sup>n</sup></em>.
 *
 * @param x the base
 * @param n the exponent
 * @param mults number of multiplications used, initially zero
 * @return <em>x<sup>n</sup></em>
 * @pre n >= 0
 * @post mults is the total number of multiplications used
 */
template <class T>
T power1(T x, unsigned int n, unsigned int& mults)
{
    mults = 0;
    double p = x;
    if (n == 0)
	return 1;
    else {
	for (int i = n - 1; i > 0; i--) {
	    p = p * x;
	    mults += 1;
	} 
	return p;
    }
}

/**
 * Naive recursive function to compute a power <em>x<sup>n</sup></em>.
 * Computes <em>x<sup>n</sup></em> as 
 * <em>x*x<sup>n</sup></em><sup>-1</sup>.
 *
 * @param x the base
 * @param n the exponent
 * @param mults total of multiplications used, which is assumed to
 * have been correctly set by any previous recursive call
 * @return <em>x<sup>n</sup></em>
 * @pre n >= 0
 * @post mults is the new number of multiplications used
 */
template <class T>
T power2(T x, unsigned int n, unsigned int& mults)
{
    mults = 0;
    static int count = 0;
    if (n == 0) 
	return 1;
    else if (n == 1) {
	mults = count;
	count = 0;
	return x;
    }
    else {
	count += 1;
	return x * power2(x, n - 1, mults);
    }
}
/**
 * Divide-and-conquer recursive function to compute a power 
 * <em>x<sup>n</sup></em>.
 * If the exponent is even, the result is simply the square of
 * <em>x<sup>n</sup></em><sup>/2</sup>; if the exponent is even,
 * then the result is <em>x</em>, multiplied by the square of 
 * <em>x</em><sup>(<em>n</em>-1)/2</sup>. 
 *
 * @param x the base
 * @param n the exponent
 * @param mults total of multiplications used, which is assumed to
 * have been correctly set by any previous recursive call
 * @return <em>x<sup>n</sup></em>
 * @pre n >= 0
 * @post mults is the new number of multiplications used
 */
template <class T>
T power3(T x, unsigned int n, unsigned int& mults)
{
    mults = 0;
    static int count = 0;
    if (n == 0) {
	return 1;
    }
    else if (n == 1) {
	mults = count;
	count = 0;
	return x;
    }
    else if ( (n >= 2) & (n % 2 == 0) ) {
	count += 1;
	double q = power3(x, n / 2, mults);
	return  q * q;
    }
    else {
	count += 2;
	double p = power3(x, (n - 1) / 2, mults);
	return x * p * p;
    }
}

/**
 * Report the results of three different exponentiation calculations.
 * If the three results differ, then all three are printed out, with
 * a warning message. If all three results are the same, the result is
 * printed, along with the number of multiplications used by each of 
 * the three algorithms.
 * 
 * @param base the number whose power we seek
 * @param pow the power 
 * @param result1 the result of using power1 to compute 
 * base<sup>power</sup>
 * @param result2 the result of using power2 to compute 
 * base<sup>power</sup>
 * @param result3 the result of using power3 to compute 
 * base<sup>power</sup>
 * @param mults1 the number of multiplications used by power1
 * @param mults2 the number of multiplications used by power2
 * @param mults3 the number of multiplications used by power3
 * @pre pow>=0
 * @pre result1, result2, result3 are as described in the parameter
 * section
 * @post none
 */

template <class T>
void printReport(T base, unsigned int pow, 
		 T result1, T result2, T result3,
		 unsigned int mults1, unsigned int mults2, 
		 unsigned int mults3)
{
    cout << base << "^" << pow << " = ";
    // print out the result if they are the same
    if (result1 == result2 && result2 == result3)
	cout << result1 << ": ";
    // print out the results if they varies
    else
	cout << "(" << result1 << ", " << result2 << ", " << result3 
	     << ") [ERROR!]: ";
    cout << "mults1 = " << mults1 << ", mults2 = " << mults2
	 << ", mults3 = " << mults3 << endl;
}
