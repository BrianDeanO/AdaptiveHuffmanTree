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
#include "AdaptiveHuffmanTree.h"
#include <bitset>
#include <fstream>
using namespace std;

const int VALID_COMMAND_LINE_ARGUMENTS = 4;
int main(int argc, const char *argv[]) {

    // Now, we will embed all of our operations in the main, within a try catch block so that we can 
    //      throw error exceptions when necessary
    try {
        // Our first task is to check if the user has entered the correct amount of arguments into the command line.
        if(argc != VALID_COMMAND_LINE_ARGUMENTS) {
            throw HuffmanException("Invalid Number Of Command Line Arguments. Re-Run Program To Try Again.");
        }

        else {
            // Looking at the second command line argument (the argv[1] element) and turning it
            //      into a string variable, so we can use it to confirm which operation the user
            //      wants to execute
            string command = argv[1];
            
            // Creating temporary string variables to hold the strings that we read in from our files
            string alphabetString;
            string messageString;

            // Another set of string variables that will be used to store the encoded and decoded messages
            //      created via the encode and decode methods
            string encodedMessage;
            string decodedMessage;

            // Converting the alphabet text file argument to a string
            string alphabetFileName = argv[2];

            // Using the message file name and converting it to a string
            string messageFileName = argv[3];

            // Finding the location of the end of the original file name, using the .txt delimiter
            int dotTextLocation = messageFileName.find(".txt");

            // Getting the original file substring and making the start of the encoded and decoded file
            //      names with it
            string encodedFileName = messageFileName.substr(0, dotTextLocation);
            string decodedFileName = messageFileName.substr(0, dotTextLocation);

            // For our encoding and decoding processes, they will use the original message file name, append
            //      an extension based on the process, and create a completely new file with those two things.
            encodedFileName.append(".txt.encoded");
            decodedFileName.append(".txt.decoded");

            // Next, since the user entered the correct number of commmand line arguments, we will proceed in reading 
            //      in the third argument in the command line (the argv[2] element), which should contain a string of 
            //      the alphabet being used for the encoding and decoding processes
            // Creating an ifstream file object to allow us to read in our alphabet            
            ifstream alphabetFile(alphabetFileName);

            // Using an if else statement to check that it opened properly to ensure we can work with it
            if(alphabetFile) {
                // Per the assignment instructions, the characters in the alphabet should all be on one line, so to get this 
                //      single string of characters, we will use the getline function
                getline(alphabetFile, alphabetString);
            }

            // Else statement that will thrown an error exception if the file could not open 
            else {
                throw HuffmanException("Error When Opening Alphabet File. Re-Run Program To Try Again.");
            }

            // Closing the alphabetFile
            alphabetFile.close();

            // And creating our AdaptiveHuffmanTree object with our alphabet passed as its parameter so we can set
            //      the alphabet for our encoding and decoding methods
            AdaptiveHuffmanTree huffmanTree(alphabetString);
            
            // Our next task is to access to the second file (the argv[3] element) that holds the message that will be 
            //      either encoded or decoded
            // Creating the ifstream file object for our message file
            ifstream messageFile(messageFileName);

            // Using an if statement to check that it was opened correctly
            if(messageFile) {
                // Now, for the message file, the message itself could be more than one line. So, to read it in 
                //      properly, we will use a while loop and getline to read in all of the lines until the end of the
                //      file is reached

                // Creating a temporary variable to store the line read in from the line. We will append this to the 
                //      messageString variable, which will be the string that we pass into the encode or decode method
                string tempMessageString;

                // Reading in the first line of the string message
                getline(messageFile, tempMessageString);

                // From our first line of the file, we append the string, first, to our string message variable 
                messageString.append(tempMessageString);

                int count = 0;

                // Now we will enter our while loop to read in and create our message string until the end of the file is met.
                //      We will now append the newline and the actual line read in, in a specific order, so that we ensure
                //      the newline character is only appended to lines that actually have it. This way, the last line
                //      of the file is not stuck with a newline at the end of it.
                while(!messageFile.eof()) {
                    
                    // Appending the newline character since our getline ignores it when reading the message file
                    messageString.append("\n");

                    // Reading in the next line of the string message
                    getline(messageFile, tempMessageString);

                    // Appending the newly read line to our actual message string variable
                    messageString.append(tempMessageString);
                }
            }

            // Else statement that will throw an exception error if the file failed to open 
            else {
                throw HuffmanException("Error When Opening Message File. Re-Run Program To Try Again.");
            }

            // Next up, we will use the command string variable which is the same as the second command line argument check to see if it is 
            //      either encode or decode. If it is one of those commands, we will continue on with it, if not, an exception will be thrown
            if(command == "encode") {

                // If the user entered the encode command, we will use make AdaptiveHuffmanTree object to call the encode method,
                //      with the message read in from the string as its argument. Within this method, the message will be encoded,
                //      returned, and assinged to the encodedMessage variable
                encodedMessage = huffmanTree.encode(messageString);

                // Our encode method returns NULL if a character encountered in the message is not a part of the alphabet array,
                //      so we will use an if else statement to control our operations.
                // If the message returned from the encode function is NULL, we will exit the program.
                if(encodedMessage == "NULL") {
                    return 0;
                }

                // Else statement that will run and operate if the encodedMessage was good and fully encoded
                else {
                    // Now that our encoding operation has happened, we will use the new file name created earlier that has the
                    //      .encoded extension on it.
                    ofstream encodedFile(encodedFileName);

                    // Now, we will check if the file opens correctly or not
                    if(encodedFile) {
                        // If the file is created and opens properly, we will now write the encoded message into it
                        encodedFile << encodedMessage;
                    }

                    // Else statement that will thrown an error exception if the file was not created or did not open correctly 
                    else {
                        throw HuffmanException("Error When Creating/Opening Encoded Message File. Re-Run Program To Try Again.");
                    }

                    // Closing the encodedFile
                    encodedFile.close();            
                }

                // Outputting message to the screen letting the user know the message has been encoded
                cout << "Message Encoded. Check Folder For .encoded File For Encrypted Message." << endl;
            }

            // Else if statement that will check for and handle the decode operation
            else if(command == "decode") {
                // If the user entered the decode command, we will use make AdaptiveHuffmanTree object to call the decode method,
                //      with the message read in from the string as its argument. Within this method, the message will be decoded,
                //      returned, and assinged to the decodedMessage variable
                decodedMessage = huffmanTree.decode(messageString);

                // As with the encode method, if a character encountered within the message file is not within the alphabet array,
                //      there will be an exception thrown and the decodedMessage will be set to NULL
                // If the message returned from the decode function is NULL, we will exit the program.
                if(decodedMessage == "NULL") {
                    return 0;
                }

                // Else statement that will run and operate if the decodedMessage had no issues and was properly decoded
                else {
                    // Now that our decoding operation has happened, we will use the new file name created earlier that has the
                    //      .decoded extension on it.
                    ofstream decodedFile(decodedFileName);

                    // Now, we will check if the file opens correctly or not
                    if(decodedFile) {
                        // If the file is created and opens properly, we will now write the encoded message into it
                        decodedFile << decodedMessage;
                    }

                    // Else statement that will thrown an error exception if the file was not created or did not open correctly 
                    else {
                        throw HuffmanException("Error When Creating/Opening Decoded Message File. Re-Run Program To Try Again.");
                    }

                    // Closing the decodedFile
                    decodedFile.close();      
                }

                // Outputting message to the screen letting the user know the message has been decoded
                cout << "Message Decoded. Check Folder For .decoded File For Decrypted Message." << endl;
            }

            // Else statement that will throw an exception error if the command is incorrect
            else {
                throw HuffmanException("Incorrect Command Format. Re-Run Program To Try Again.");
            }
        }
    } 

    // Catching any errors thrown
    catch(HuffmanException error) {            
        error.outputError();
    }

    return 0;
}