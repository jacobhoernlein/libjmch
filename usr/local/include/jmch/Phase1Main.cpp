#include <iostream>
#include <vector>
using namespace std;
#include "List.cpp"

template<typename T>
ostream& operator<<(ostream &os, const jmch::List<T> &arr) {
    for (int i = 0; i < arr.length(); i++) {
        os << arr[i] << ' ';
    }
    return os;
}

void foo(jmch::List<int> arr) {
	for (int i = 0; i< arr.length() / 2; i++)
		arr[i] = arr[arr.length() / 2 + i];
	// X => "5 4 3 2 0 5 5 4 3 2 0 5"	
	cout << arr << endl;
}

int main(){
	jmch::List<float> C(10);
	for (int i = 0; i < C.length(); i++) C[i] = i;
	cout << C << endl;
	// C => "0 1 2 3 4 5 6 7 8 9"
	C.popFront();
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8 9"
	C.popBack();
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8"
	C.pushBack(100.0);
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8 100"
	C.popFront();
	C.pushBack(200.0);
	// C => "2 3 4 5 6 7 8 100 200"	

	C.pushBack(300.0);
	C.pushBack(400.0);
	// C => "2 3 4 5 6 7 8 100 200 300 400"	
	
	C.popBack(); C.popBack();C.popFront();
	cout << C << endl;
	// C => "3 4 5 6 7 8 100 200"		

	jmch::List<int> A, B;
	for(int i = 0; i < 10; i++) A.pushBack(i);
	B = A;
	A.pushBack(15); A.pushBack(19);
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" 
	cout << "Select is " << A.find(5) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => 5
	cout << "Select is " << A.find(12) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => -1
	cout << "Select is " << A.find(15) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => 10	
	A.pushFront(10); 
	// A => "10 0 1 2 3 4 5 6 7 8 9 15 19"
	cout << "Select is " << A.find(5) << endl;
	// A => "10 0 1 2 3 4 5 6 7 8 9 15 19" Search => 6
	cout << "Select is " << A.select(3) << endl;
    // Select => 2	
	A.sort();
	// A => "0 1 2 3 4 5 6 7 8 9 10 15 19"
	A.pushBack(11); A.pushFront(1); A.pushFront(2); A.pushFront(3);
	cout << "capacity is " << A.capacity() << endl;
	// A => "3 2 1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"	  capacity => 32
	A.popFront(); A.popFront();
	// A => "1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"	  capacity => 32

	foo(A);
	cout << A << endl;
	// A => "1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"
	cout << B << endl;
	B.reverse();
	cout << B << endl;
	
	for (int a : A) {
		cout << a << ", ";
	}

	jmch::List<string> S = {"apple", "banana", "canteloupe"};
	
	// B => "0 1 2 3 4 5 6 7 8 9"
}
