# Adaptive Huffman Tree

## Purpose
Written in C++, this program allows the user to encode and decode messages using the Adaptive Huffman algorithm. The user can provide their own alphabet that encompasses all the possible characters represented in their original message. And within the command line, the user can first encode their original message, which must also be a text file. After the encoding process, the user will receive a text file with a "decoded" extension attached to the end of their original file name. Using the decoded command, the original alphabet, and the decoded file, the program will use the Huffman algorithm once more to decode the encoded, binary version of their message. After decoding, a second file will be produced, with an "encoded" extension attached to the end of their original file name. An example of this process is found below. 

## About the Huffman Algorithm
The Huffman Algorithm is a data compression algorithm that will encode and decode messages using a pre-determined alphabet. The algorithm uses a binary tree that sorts the characters based on the frequency of appearance. Therefore, the characters from the original message that are seen more often are sorted to the top of the tree. During encoding, for each new character encountered, that character's ASCII code is determined and saved in an eight-bit binary representation. Subsequent occurrences of the character will be represented in the encoded message as the path it takes to traverse from the root of the Huffman tree to that character's node. And while walking down the path, each left branch taken is denoted as a 0, and each right branch is represented as a 1. Once the complete message is encoded, the process is reversed to decode back to the original form of the message.

## Test Example
#### Now, we will run through how to encode and decode a message, using this Adaptive Huffman Algorithm program. 


#### These are the contents of the alphabet.txt file. Note that among the common characters, there are several esacpe sequences that enable the processing of a newline and tab within the original message and help to keep the original format of the message.
![image](https://user-images.githubusercontent.com/54780901/213881660-3ed62550-6b11-4c36-8c52-b7e5ff38794a.png)


#### This is the original message file before encoding.
![image](https://user-images.githubusercontent.com/54780901/213881796-2fcf676f-59b2-46ed-b336-5f2d2a85aee6.png)

#### Now, this is the correct command line entry to encode the message with the given alphabet.
![image](https://user-images.githubusercontent.com/54780901/213881988-310e5a19-78de-4d0d-922c-8bd24a20d8bd.png)


#### After encoding, this message will appear in the terminal, along with this file in the current directory.
![image](https://user-images.githubusercontent.com/54780901/213882028-db9c5fa8-617d-4983-b7e3-db2775225c90.png)
![image](https://user-images.githubusercontent.com/54780901/213882041-0fa39c02-5c3b-4c0d-a5d0-fa90fca55d1c.png)


#### While the complete binary representation of the decoded message cannot all fit on the screen, this is a portion of the encoded message.
![image](https://user-images.githubusercontent.com/54780901/213882160-8daef74a-4436-41b1-9fdd-94f344af3989.png)


#### To then decode this message using the reverse process of the Adaptive Huffman encoding method, the user provides the decode command and the encoded message file.
![image](https://user-images.githubusercontent.com/54780901/213882267-77046a8e-8aab-4c3f-8b92-63e2c398c711.png)


#### The user then receives this command line message and their decoded file.
![image](https://user-images.githubusercontent.com/54780901/213882296-dc1ed7e1-ef43-474f-8a28-485f4790e972.png)
![image](https://user-images.githubusercontent.com/54780901/213882310-ee338162-7a50-4122-ba6e-3eab440742c3.png)


#### And finally, within the message.txt.decoded file, is the user's original message, formatted and printed in the same way!
![image](https://user-images.githubusercontent.com/54780901/213882342-0ad2baaa-d350-4754-bc58-409f98c95a27.png)
