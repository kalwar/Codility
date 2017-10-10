// WinterLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/*
https://codility.com/programmers/task/winter_lights/
https://codility.com/programmers/challenges/vanadium2016/
Also
https://codility.com/programmers/task/count_palindromic_slices/   !!
https://codility.com/programmers/challenges/gamma2011/

Winter is coming and Victor is going to buy some new lights. In the store, lights are available in 10 colors, numbered from 0 to 9.
They are connected together in a huge chain. Victor can choose any single segment of the chain and buy it.
This task would be easy if it weren't for Victor's ambition. He wants to outdo his neighbors with some truly beautiful lights,
so the chain has to look the same from both its left and right sides (so that both neighbors see the same effect).
Victor is a mechanic, so after buying a segment of the chain, he can rearrange its lights in any order he wants.
However, now he has to buy a chain segment that will satisfy above condition when its lights are reordered. Can you compute how many possible segments he can choose from?
Write a function:
int solution(string &S);
that, given a description of the chain of lights, returns the number of segments that Victor can buy modulo 1,000,000,007. The chain is represented by a string of digits (characters from '0' to '9') of length N. The digits represent the colors of the lights. Victor can only buy a segment of the chain in which he can reorder the lights such that the chain will look identical from both the left and right sides (i.e. when reversed).
For example, given:
S = "02002"
the function should return 11. Victor can buy the following segments of the chain:
"0", "2", "0", "0", "2", "00", "020", "200", "002", "2002", "02002"
Note that a segment comprising a single "0" is counted three times: first it describes the subchain consisting of only the first light, then the subchain consisting of the third light and finally the subchain consisting of the fourth light. Also note that Victor can buy the whole chain ("02002"), as, after swapping the first two lights, it would become "20002", which is the same when seen from both from left and right.
Assume that:
string S consists only of digits (0-9);
the length of S is within the range [1..200,000].
Complexity:
expected worst-case time complexity is O(N);
expected worst-case space complexity is O(1) (not counting the storage required for input arguments).

IDEA:
Count and keep for any interval [0, i]  amount of any digits
Also keep an array of 1024, any index under 1024 translates into 10 of 1 and 0 meaning that a digit appeared odd or even times.
Any value in the array means how many times the combination appears.
One every movement from left to right the array is updated.
The current [0, i] has amounts of digits, we must subtract from the array in order to set all dgits even or all but oen, it is a palindrome.
If current is all eve: amoutn of all evens and 10 one odd goes.
If current has k odds, that one can diminish till k-1 or k, or diminish to all odd and than add all evesn and one odd.

https://codility.com/demo/results/trainingNQPSUQ-NNT/     Had bad answers
better
https://codility.com/demo/results/trainingMV5NG2-2E7/     Only timeouts
https://codility.com/demo/results/trainingTZPPTS-NAS/     Essentially the same
https://codility.com/demo/results/trainingTVKVAC-Y3B/     Essentially the same
Correctness: 100%, performance 50%, overall 60%
Detected time complexity:
O(N) or O(N**2)
https://codility.com/demo/results/trainingFRD2E5-3AF/
Correctness	Performance	Task score
100%        62%	        70%

/*
 https://codility.com/programmers/task/winter_lights/
 https://codility.com/programmers/challenges/vanadium2016/
 https://codility.com/demo/results/trainingTEQW4T-RGF/
 
 100% 100%   
 Troubles
 1. Slow modular.
 2. Very difficult to summarize after the btable is ready
 3. At the end divisin by 2 was faulty, modular division must be done via inverse
 4. Never ever be stuck and never ever write something you do not understand. Will pay much more!

*/


////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const unsigned int allDigitsOdd = 0x3ff;
const unsigned int allDigitsEve = 0;
vector<unsigned long long> bigAccummulator(allDigitsOdd + 1, 0);
vector<bool> smaAccummulator(10, false); //true - odd, false - even
const long long MODULO = 1000000007;

//this define is good for non-negative numbers
#define FIX(v) do { if (v >= MODULO) v = v % MODULO; } while (v >= MODULO);
void increment(unsigned long long &i) {
	i++;
	FIX(i);
}
unsigned long long summ(unsigned long long i, unsigned long long j) {
	FIX(i);
	FIX(j);
	unsigned long long res = i + j;
	FIX(res);
	return res;
}
unsigned long long multt(unsigned long long i, unsigned long long j) {
	FIX(i);
	FIX(j);
	unsigned long long res = i * j;
	FIX(res);
	return res;
}
//extended recursive Euclid algorithm, used to calculate modular inverse
// a,b - input, x,y - output
//from https://stackoverflow.com/questions/35226781/finding-binomial-coefficient-for-large-n-and-k-modulo-m
long long  xGCD(long long a, long long b, long long &x, long long &y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	long long  x1, y1, gcd = xGCD(b, a % b, x1, y1);
	x = y1;
	y = x1 - (long long)(a / b) * y1;
	return gcd;
}
//calculates modular inverse that is from an integer a produces b:  a*b = 1 modulo mod
// used to produce a modular ratio of two integers, the denominator is first must be inversed and then
//used as a factor instead of division
//from https://stackoverflow.com/questions/35226781/finding-binomial-coefficient-for-large-n-and-k-modulo-m
long long inverse(long long a) {
	long long  x, y;
	xGCD(a, MODULO, x, y);
	return (x + MODULO) % MODULO;
}
int char2digit(char c) {
	return c - '0';
}
unsigned int oneDigitOdd(unsigned int bitNum) {
	if (bitNum > 9) return 0;
	return 1 << bitNum;
}
unsigned int setBit(unsigned int &i, int bitNum) {
	if (bitNum > 9) return 0;
	i = i | oneDigitOdd(bitNum);
	return i;
}
int smallAccummulatorToNumber() {
	unsigned int res = allDigitsEve;
	for (int i = 0; i < 10; i++) {
		if (smaAccummulator.at(i)) {
			setBit(res, i);
		}
	}
	return res;
}
int digitSetToNumber(vector<unsigned int> v) {
	unsigned int res = allDigitsEve;
	for (unsigned int i = 0; i < v.size(); i++) {
		setBit(res, v.at(i));
	}
	return res;
}
/*
This solution is O(N) but it still too slow!
Reason: a lot of calculations per cycle
Need to collect data and then count, which is more difficult
*/
int solution2(string &S) {

	bigAccummulator = vector<unsigned long long>(allDigitsOdd + 1, 0);
	smaAccummulator = vector<bool>(10, false);
	unsigned long long total = 0;
	unsigned int N = S.length();
	if (1 == N)
		return 1;
	if (0 == N)
		return 0;
	for (unsigned i = 0; i < N; i++) {
		char c = S.at(i);
		int digit = char2digit(c);
		smaAccummulator.at(digit) = !smaAccummulator.at(digit);
		vector<unsigned int> oddDigits, evenDigits;
		for (int j = 0; j < 10; j++) {
			if (smaAccummulator.at(j))
				oddDigits.push_back(j);
			else
				evenDigits.push_back(j);
		} //we found how many digits till now even and how many odd, palindrom can have <= 1 digits with odd number

		  //the current can be palindromon too
		if (1 >= oddDigits.size())
			increment(total);

		//suppose we have n even and m odd digits. we can arrive to 1 odd and 0 odd

		// all even is easy: ind in big accummulator how many can decrease these odds to even and others leave even
		//ALL EVEN CASE
		if (evenDigits.size() > 0 && oddDigits.empty()) {
			total = summ(total, bigAccummulator.at(0));  //leave all even even
			for (unsigned int k = 0; k < evenDigits.size(); k++) { //make one odd in circular motion
				vector<unsigned int> oneOdd(1, evenDigits.at(k));
				unsigned int soughtIndex0 = digitSetToNumber(oneOdd);
				total = summ(total, bigAccummulator.at(soughtIndex0));
			}
		}
		//ONE ODD CASE
		if (1 == oddDigits.size()) {
			total = summ(total, bigAccummulator.at(0)); //leave one odd odd, all even even
			unsigned int soughtIndex = digitSetToNumber(oddDigits);  //single odd into even, others left even
			total = summ(total, bigAccummulator.at(soughtIndex));

			for (unsigned int k = 0; k < evenDigits.size(); k++) { //make the single odd into even and another even into odd
				vector<unsigned int> twoToFlip;
				twoToFlip.push_back(oddDigits.at(0));
				twoToFlip.push_back(evenDigits.at(k));
				unsigned int soughtIndex5 = digitSetToNumber(twoToFlip);
				total = summ(total, bigAccummulator.at(soughtIndex5));
			}
		}
		//SEVERAL ODDs case
		if (oddDigits.size() > 1)
		{ //split into three subcases
			{ //sub 1: make all odds into even
				unsigned int soughtIndex = digitSetToNumber(oddDigits);
				total = summ(total, bigAccummulator.at(soughtIndex));
			}
			//sub 2: make all but one odds into even
			for (unsigned int k = 0; k < oddDigits.size(); k++) {
				vector<unsigned int> oddDigitsWithoutOne = oddDigits;
				oddDigitsWithoutOne.erase(oddDigitsWithoutOne.begin() + k);
				unsigned int soughtIndex6 = digitSetToNumber(oddDigitsWithoutOne);
				total = summ(total, bigAccummulator.at(soughtIndex6));
			}
			//sub 3: make all odds into even and then one even into odd
			for (unsigned int k = 0; k < evenDigits.size(); k++) {
				vector<unsigned int> flipv = oddDigits;
				flipv.push_back(evenDigits.at(k));
				unsigned int soughtIndex8 = digitSetToNumber(flipv);
				total = summ(total, bigAccummulator.at(soughtIndex8));
			}
		}
		unsigned int currentConfiguration = smallAccummulatorToNumber();
		increment(bigAccummulator.at(currentConfiguration));
	}
	return (int)total;
}
void decode(int i, vector<unsigned int> &oddDigits, vector<unsigned int> &evenDigits) {
	oddDigits.resize(0);
	evenDigits.resize(0);
	for (int j = 0; j < 10; j++) {
		unsigned int bitMask = 1 << j;	
		bool odd = ((i & bitMask) != 0);
		if (odd)
			oddDigits.push_back(j);
		else
			evenDigits.push_back(j);
	}
}
/*
The main idea: 
keep info per digit has it met even amount of times or odd
A combination like 0 - odd, 1 - even, 2 - odd is encoded in binary: 101b = 5d, LSB is for 0, next for 1 etc
There is also a decodig function that turns 0 into all digits even, 1 - only 0 is odd etc
An array smaAccummulator keeps info on every digit while passing from left to right of input S
One every state of smaAccummulator it is encoded
Array bigAccummulator of 1024 positions keeps hoe many every configurations of smaAccummulator is met
bigAccummulator in essence counts configurations of all fragments with the beginning of S till some position
After all S is done we take bigAccummulator and compute from it all 
Every fragment with odd amount of digits that is <= 1 is potentially palindromic (PP)
Every fragment of some cfg can be subtracted from another fragment anbd will get all even - also PP
And some other combinations
The main problem is that some possibilites are counted twice. So we count all of them twice, all non-twice *2
At the end we need to divide by 2 modular, in order to do it use inverse of 2 MODULO
*/
int solution(string &S) {

	bigAccummulator = vector<unsigned long long>(allDigitsOdd + 1, 0);
	smaAccummulator = vector<bool>(10, false);
	unsigned int N = S.length();
	if (1 == N)
		return 1;
	if (0 == N)
		return 0;
	for (unsigned i = 0; i < N; i++) {
		char c = S.at(i);
		int digit = char2digit(c);
		smaAccummulator.at(digit) = !smaAccummulator.at(digit);
		unsigned int currentConfiguration = smallAccummulatorToNumber();
		increment(bigAccummulator.at(currentConfiguration));
	}
	unsigned long long total = 0;
	for (int i = 0; i <= allDigitsOdd; i++) { //the loop through all possiblilites like 1,3,4 appears odd times, others 0 or even 
		unsigned long long howMany = bigAccummulator.at(i); //how many fragments from the left boundary to some position has this cfg
		if (0 == howMany)
			continue; //this cfg does not contribute at all
		unsigned long long additionPerCase = 0;
		unsigned long long additionPerCycle = 0;
		vector<unsigned int> oddDigits, evenDigits;
		decode(i, oddDigits, evenDigits);  //which digits appear odd times, which - even, 

		//CASE Count myself
		if (oddDigits.size() <= 1) {
			total = summ(total, multt(2,howMany)); //all these fragments are potential palindroms
			//cout << "Initial total set to " << total << " by how many: " << howMany << endl;
		}

		//restrict the current type of fragments to all even
		//remove fragment with three odds from another fragment with three odds
		//CASE restriction to all even
		total = summ(total, multt(howMany, howMany - 1) );  //you will get an all even. But we count every one twice, so /2
		// cout << "Total grew by restriction to all even " << total << " addition: " << multt(howMany, howMany - 1) / 2 << endl;

		//CASE all evens case to one odd
		if (0 == oddDigits.size()) {
			for (unsigned int j = 0; j < evenDigits.size(); j++) {
				vector<unsigned int> evenToFlip;
				evenToFlip.push_back(evenDigits.at(j));
				unsigned int soughtIndex = digitSetToNumber(evenToFlip);
				unsigned long long howManyNewOdds = bigAccummulator.at(soughtIndex); //how many we have with less odds by 1		
				additionPerCase = multt(2,multt(howMany, howManyNewOdds)) ; ////divide by 2 or not?
				additionPerCycle = summ(additionPerCycle, additionPerCase);//divide by 2 or not?
				// cout << "Themselves gave: " << additionPerCase << endl;
			}
		}

		//CASE  restriction to all even but one existing odd
		//restrict the current type of fragments to one odd only from existing
		if(1 < oddDigits.size())
		{
			for (unsigned int j = 0; j < oddDigits.size(); j++) {
				vector<unsigned int> oddDigitsWithoutOne = oddDigits;
				oddDigitsWithoutOne.erase(oddDigitsWithoutOne.begin() + j);  //from set of odds remove one
				unsigned int soughtIndex = digitSetToNumber(oddDigitsWithoutOne);
				unsigned long long howManyLessOdds = bigAccummulator.at(soughtIndex); //how many we have with less odds by 1
				//divide by 2 or not?
				additionPerCase = multt(howMany, howManyLessOdds)  ; ////divide by 2 or not?
				additionPerCycle = summ(additionPerCycle, additionPerCase);
				// cout << "Added by restriction to one existing odd: " << additionPerCase << endl;
			}
		}

		//CASE  restriction to all even with an additionPerCase of new odd
		if(0 < oddDigits.size())
		{
			for (unsigned int j = 0; j < evenDigits.size(); j++) {
				vector<unsigned int> flipDigits = oddDigits;
				flipDigits.push_back(evenDigits.at(j));  //flip set is all odds and one even
				unsigned int soughtIndex = digitSetToNumber(flipDigits);
				unsigned long long howManyWithNewOdd = bigAccummulator.at(soughtIndex);
				additionPerCase = multt(howMany, howManyWithNewOdd)  ; ////divide by 2 or not?
				additionPerCycle = summ(additionPerCycle, additionPerCase); 
				// cout << "Added by restriction to even and adding one new odd: " << additionPerCase << endl;
			}
		}
		total = summ(total, additionPerCycle);
		additionPerCase = 0;
		additionPerCycle = 0; 
	} // for (int i = 0; i <= allDigitsOdd; i++)
	return (int)multt(total, 500000004); // must divide by 2, but with modular must do inversion modulary and multiply
	//500000004 is 2 inverted with MODULO
}
////////////////////////////////////////////////////////////////////////////////////////////////
int solution2a(const char *S) { return solution2(std::string(S)); }
int solution1a(const char *S) { return solution (std::string(S)); }
void check(const char *test, int expected = -1) {
	int len = strlen(test);
	int res2 = solution2a(test);
	int res1 = solution1a(test);
	string blanks(max(20 - len, 1), ' ');
	cout << test << blanks.c_str() << " " << " Res2: " << res2;
	if (expected > 0) {
		if (res2 != expected)
			cout << " WRONG! ";
	}
	cout << endl;
	cout << test << blanks.c_str() << " " << " Res1: " << res1;
	cout << endl;
}
int main()
{
	cout << "WinterLight" << endl;
	cout << "inverse 2: " << inverse(2) << endl;
	check("1", 1);
	check("01", 2);
	check("11", 3);
	check("12", 2);
	check("011", 5);
	check("010", 4);
	check("000", 6);
	check("012", 3);
	check("1012", 5);
	check("1012", 5);
	check("39893", 7);
	check("11", 3);
	check("02002", 11);
	check("111", 6);
	check("1111", 10);
	check("11111", 15);
	check("123", 3);
	check("1232", 5);
	check("12321", 7);
	check("123216", 8);
	check("1232162", 9);
	check("12321623", 10);
	check("123216232", 13);
	check("1232162323", 17);
	check("12321623234", 19);
	check("123216232348", 20);
	return 0;
}
