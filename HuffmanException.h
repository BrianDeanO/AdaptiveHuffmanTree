/*
    Programmer: Brian dean
    Purpose: Implement the Adaptive Huffman algorithm. The constructor should
        take a string as a parameter. This string should be the alphabet that will be used
        for the encode or decode operation. Each method should take a string as a parameter and
        return a string. The driver program should accept 3 command line arguments. For encoding,
        the program should create a new file that is named with the original file name with the 
        extension .encoded appended. And with decoding, the file should be appending with .decoded.
    Course: CPTS 223
    Date: 11/8/22
*/

#include <iostream>
#include <string>
using namespace std;

// creating our HuffmanException class to handle the output of our thrown exceptions
class HuffmanException {
private: 
    // Our private member for this exception class will just be the error string
    string errorOutput;
public:
    /*
    The rest of our exception class will just be an overloaded constructor and a function
    that prints the output error on to the string.
    */

    // Constructor to set the error output message
    HuffmanException(string s) {
        errorOutput = s;
    }

    // Function that will print the error output on the screen
    void outputError() {
        cout << this->errorOutput << endl;
    };
};