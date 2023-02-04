#include <iostream>
using namespace std;
#include "Array.cpp"

template<typename T>
ostream& operator<<(ostream &os, const jmch::Array<T> &arr) {
    for (int i = 0; i < arr.length(); i++) {
        os << arr[i] << ' ';
    }
    return os;
}

void foo(jmch::Array<int> arr) {
	for (int i = 0; i< arr.length() / 2; i++)
		arr[i] = arr[arr.length() / 2 + i];
	// X => "5 4 3 2 0 5 5 4 3 2 0 5"	
	cout << arr << endl;
}

int main(){
	jmch::Array<float> C(10);
	for (int i = 0; i < C.length(); i++) C[i] = i;
	cout << C << endl;
	// C => "0 1 2 3 4 5 6 7 8 9"
	C.pop(0);
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8 9"
	C.pop();
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8"
	C.append(100.0);
	cout << C << endl;
	// C => "1 2 3 4 5 6 7 8 100"
	C.pop(0);
	C.append(200.0);
	// C => "2 3 4 5 6 7 8 100 200"	

	C.append(300.0);
	C.append(400.0);
	// C => "2 3 4 5 6 7 8 100 200 300 400"	
	
	C.pop(); C.pop();C.pop(0);
	cout << C << endl;
	// C => "3 4 5 6 7 8 100 200"		

	jmch::Array<int> A, B;
	for(int i = 0; i < 10; i++) A.append(i);
	B = A;
	A.append(15); A.append(19);
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" 
	cout << "Select is " << A.find(5) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => 5
	cout << "Select is " << A.find(12) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => -1
	cout << "Select is " << A.find(15) << endl;
	// A => "0 1 2 3 4 5 6 7 8 9 15 19" Search => 10	
	A.prepend(10); 
	// A => "10 0 1 2 3 4 5 6 7 8 9 15 19"
	cout << "Select is " << A.find(5) << endl;
	// A => "10 0 1 2 3 4 5 6 7 8 9 15 19" Search => 6
	cout << "Select is " << A.quickSelect(3) << endl;
    // Select => 2	
	A.sort();
	// A => "0 1 2 3 4 5 6 7 8 9 10 15 19"
	A.append(11); A.prepend(1); A.prepend(2); A.prepend(3);
	cout << "capacity is " << A.capacity() << endl;
	// A => "3 2 1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"	  capacity => 32
	A.pop(0); A.pop(0);
	// A => "1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"	  capacity => 32

	foo(A);
	cout << A << endl;
	// A => "1 0 1 2 3 4 5 6 7 8 9 10 15 19 11"
	cout << B << endl;
	// B => "0 1 2 3 4 5 6 7 8 9"
}
