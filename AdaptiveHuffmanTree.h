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
#pragma once
#include <iostream>
#include "HuffmanException.h"
#include <cmath>
#include <bitset>
#include <cstring>
#include <string>
using namespace std;

// Creating the constant variable for the size of our alphabet array. With our process of 
//      correctly reading in the characters that start with the escape sequence, there are 
//      a empty array elements set to the NULL character, so instead of the 256 sized array,
//      we will do 300 for safe measures
const int ALPHABET_ARRAY_SIZE = 300;

// Creating a const variable for the backslash and single quote ASCII value
const int BACKSLASH_ASCII_VALUE = 92;
const int SINGLE_QUOTE_ASCII_VALUE = 39;

// Creating our Adaptive Huffman Node class
class HuffmanNode 
{
    // Declaring our private members of the HuffmanNode
    private:    
    // In each node, there will be five pointers. Pointers to the node's parent, to the node previous
    //      in the thread, to the node next in the thread, and pointers to the node's left and right child
    HuffmanNode* parent;
    HuffmanNode* prev;
    HuffmanNode* next;
    HuffmanNode* left;
    HuffmanNode* right;

    // We will create another node pointer that will only be used by the nodes within the alphabet array. 
    //      This node will be set when we add a new character to the tree, and used to jump that character
    //      node, whenever we need to obtain the path from the root to that node
    HuffmanNode* alphabet;

    // And in each node, there will be a count variable and a character data member. Nodes that are only
    //      for keeping the count will have characters of null
    int count;
    char character;

    // In our public section, we will have the node constructors and the getter and setter functions
    public:

    // Default constructor for the node, which sets every pointer to the nullptr and the count and character
    //      to one and empty, respectively.
    HuffmanNode() {
        /*
            The default constructor will be used to create the nodes that keep the counts and do not represent
            a character in the tree. 
            
            And since each new node is first intialized to one, that is why the count starts at one.
        */
        parent = nullptr;
        prev = nullptr;
        next = nullptr;
        left = nullptr;
        right = nullptr;
        alphabet = nullptr;
        count = 1;

        // Intializing the character member to be the char equivalent to NULL, so that it will
        //      never match with any other possible character in a message
        character = char(0);
    }

    // Overloaded constructor for the node
    HuffmanNode(char character) {
        parent = nullptr;
        prev = nullptr;
        next = nullptr;
        left = nullptr;
        right = nullptr;
        alphabet = nullptr;
        /*
            Since this overloaded constructor will be used when a new character is encountered, 
            the default count will be 1. 
            
            With new nodes that will be only for keeping the count, those character values passed 
            will also be the empty character.       
        */
        count = 1;
        this->character = character;
    }

    // Setter function for the character in the node
    void setCharacter(char c) {
        this->character = c;
    }

    // Getter function to obtain the character held in the node
    char getCharacter() {
        return this->character;
    }

    // Function that will set the parent of the node
    void setParentNode(HuffmanNode* parent) {
        this->parent = parent;
    }

    // Function that will return a pointer to the parent's node
    HuffmanNode* getParentNode() {
        return this->parent;
    }

    // Function that will set the node's previous node in the chain
    void setPrevNode(HuffmanNode* prev) {
        this->prev = prev;     
    }

    // Function that will get the node previous to the current node
    HuffmanNode* getPrevNode() {
        return this->prev;
    }

    // Function that will set the node's next node in the chain
    void setNextNode(HuffmanNode* next) {
        this->next = next;
    }

    // Function that will get the node next up in the chain from the current node
    HuffmanNode* getNextNode() {
        return this->next;
    }

    // Function that will set the node's left child
    void setLeftNode(HuffmanNode* left) {
        this->left = left;
    }

    // Function that will get the left child of the node
    HuffmanNode* getLeftNode() {
        return this->left;
    }

    // Function that will set the node's right child
    void setRightNode(HuffmanNode* right) {
        this->right = right;
    }

    // Function that will return the node's right child
    HuffmanNode* getRightNode() {
        return this->right;
    }

    // Function that will be used with the nodes in our alphabet array. When a new character
    //      is added to the tree, the node in the alphabet array that the character matches
    //      will have their alphabet node set to that character node in the tree
    void setAlphabetNode(HuffmanNode* alpha) {
        this->alphabet = alpha;
    }

    // This function will return the huffman node within the tree that our alphabet array node
    //      pointer is pointing to. This will allow us to jump to the character node and easily
    //      manipulate it and get the path from the root to that node for output.
    HuffmanNode* getAlphabetNode() {
        return this->alphabet;
    }

    // Function to update the count of a huffman node
    void updateCount(int var) {
        this->count += var;
    }

    // Function that returns the count of a node
    int getCount() {
        return this->count;
    }

};

// Creating the Adaptive Huffman Algorithm class to handle our encoding and decoding
class AdaptiveHuffmanTree 
{
    private:
    // Creating a node pointer to keep track of the root of our tree
    HuffmanNode* root;

    // Creating a node pointer that will keep track of the zero node of the tree
    HuffmanNode* zeroNode;

    // Creating an array to store all of the possible letters in the alphabet of characters. This array
    //      stores pointers to huffman nodes when they are placed into the tree. And we are initialzing 
    HuffmanNode alphabetArray[ALPHABET_ARRAY_SIZE];

    public:
    // Creating our overloaded constructor that takes in the alphabet string as its parameter
    AdaptiveHuffmanTree(string alphabet) {
        // First, to distribute the alphabet characters passed into the constructor, we will cast the 
        //      string into a c-string, so we can easily manipulate and place each node in the array
        const char* alphabetCString = alphabet.c_str();

        // Creating a huffman node pointer for our zero node
        zeroNode = new HuffmanNode();

        // Subtracting one from the zeroNode's count to make its count zero
        zeroNode->updateCount(-1);

        // Assigning the root node to point to our zero node for the tree
        this->root = zeroNode;

        // Now, we will run through the entire alphabet with a for loop based on the length of the string
        // In this for loop, for each letter of the alphabet, we will create a huffman node that has that 
        //      character assigned to its character data member. And for the intialization of the node, 
        //      each node will point to the nullptr.
        for(int i = 0; i < strlen(alphabetCString); i++) {
            // Making a temporary node that will be intialized with a character from the alphabet, and stored
            //      into the alphabet array
            HuffmanNode tempNode(alphabetCString[i]);

            // Getting the ascii reprensentation of the current character we are looking at
            int ascii = int(alphabetCString[i]);

            // With this if statement, we are checking to see if the current character we are reading in
            //      is a single backslash. If it is, we know that the following chracter will have to be attached
            //      to and it will create a single character with the escape sequence, not a backslash and a character
            if(ascii == BACKSLASH_ASCII_VALUE) {
                // Switch statement that will look ahead to the next character in the string after the backslash
                //      and depening on the character's ascii value, we will input a non-printable character
                //      into our alphabet array
                switch(int(alphabetCString[i+1])) {
                    // Our case for the Alert escape sequence
                    case int('a'): 
                        tempNode = '\a';
                        break;

                    // Case to handle the Backspace
                    case int('b'): 
                        tempNode = '\b';
                        break;

                    // Case to handle the Form Feed (new page)
                    case int('\f'): 
                        tempNode = '\f';
                        break;
                        
                    // Case to handle the Vertical Tab
                    case int('v'): 
                        tempNode = '\v';
                        break;
                        
                    // Case to handle the New-line
                    case int('n'):
                        tempNode = '\n';
                        break;
                        
                    // Case to handle the Horizontal Tab
                    case int('t'): 
                        tempNode = '\t';
                        break;
                        
                    // Case to handle the Carriage Return
                    case int('r'): 
                        tempNode = '\r';
                        break;
                                            
                    // Case to handle the Backslash
                    case int('\\'): 
                        tempNode = '\\';
                        break;
                        
                    // Case to handle the Single Quotation Mark. Note with this, we are just using
                    //      the ascii value itself
                    case SINGLE_QUOTE_ASCII_VALUE: 
                        tempNode = '\'';
                        break;
                        
                    // Case to handle the Double Quotation Mark
                    case int('"'): 
                        tempNode = '\"';
                        break;
                                            
                    // Case to handle the Backspace
                    case int('?'): 
                        tempNode = '\?';
                        break;
                }

                // Now placing the node into the alphabet array
                this->alphabetArray[i] = tempNode;

                // Incrementing the i value, since we already read the next character and need
                //      to jump to the one after it in the sequence
                i++;

                // Inserting the char equivalent to NULL, since with each escape character 
                //      processing, we bypass an i-value, so an array element is skipped. This 
                //      way, we fill the element with NULL, to ensure no random values do.
                this->alphabetArray[i] = char(0);
            } 

            // Else, the character is normal, so we place the node into the alphabet array
            else {
                this->alphabetArray[i] = tempNode;
            }
        }
    }

    // Creating our encode method that takes in the string message that will be encoded as a parameter. This method
    //      then returns the encoded version of the original message as a string.
    string encode(string messageString) {
        // Creating a large try-catch block to handle the error we get if a character in the message is not in the 
        //     alphabet array
        try {
            // Our first task in the encoding process will be to cast the string message into a c_string so we can 
            //      easily access each character
            const char* message = messageString.c_str();

            // Creating the encoded message string that will be used to track and hold the output of our message
            //      while work through the process of encoding it
            string encodedMessage;

            // Now, we will create a for loop that will iterate through the total length of the string message
            for(int i = 0; i < strlen(message); i++) {

                // First we will create pointers to our two new nodes in the tree: the character node and the 
                //      its parent counter node
                HuffmanNode* counterNode = new HuffmanNode();

                // For the character node, we are intializing it with the character in our message that we are currently
                //      encoding
                HuffmanNode* characterNode = new HuffmanNode();

                // Creating a temporary string variable that will be used to store the eight bit representation
                //      of each character output
                string stringBits;

                // Creating a boolean variable that will be used in our conditional checking to help see if a character
                //      is within our alphabet array
                bool isFound = false;

                // Creating another boolean variable to keep track of where we are at while traversing up the chain
                bool endOfChain = false;

                // Creating an int variable to keep track of the leader count while checking the chain
                int leaderCount;

                // Creating a string that will be appended to, so we can hold the 1's and 0's as we traverse up the tree 
                //      to obtain the path from the root to a node
                string reversePath;

                // Creating another string to hold the result from the reverseString method, which will be the correct path
                //      from the root to a given node
                string correctPath;

                // Creating a traversal node that will be used to obtain the path from the root to a given node
                HuffmanNode* traversalNode = nullptr;

                // Creating a temporary node pointer to keep track of the current node we are on
                HuffmanNode* currentNode = nullptr;

                // Creating a temporary node pointer to get the parent of each node, while we traverse and update our tree
                HuffmanNode* parentNode = nullptr;

                // Creating two node pointers, one for the next node in the chain and one for the previous node in the cahin
                HuffmanNode* prevNode = nullptr;
                HuffmanNode* nextNode = nullptr;

                // Creating a node pointer called swapNode, to help keep track of the node that we swap with another
                //      leaf chracter node
                HuffmanNode* swapNode = nullptr;

                // For our swapping process, we will obtain the parent, next, and prev pointers for both the next node and
                //      the swap node, that are switching places. Below, we create all of them.
                HuffmanNode* nextNodeParent = nullptr;
                HuffmanNode* nextNodeNext = nullptr;
                HuffmanNode* nextNodePrev = nullptr;
                HuffmanNode* swapNodeParent = nullptr;
                HuffmanNode* swapNodeNext = nullptr;
                HuffmanNode* swapNodePrev = nullptr;

                // Creating a accumlator integer to also help with our check to see if a character is in our array
                int index = 0;
                
                // Our first action is to check whether or not the character that we are encoding is a character
                //      within our pre-set alphabet array. To do this, we will use a while loop and our 
                //      conditional check will be either if the character was found or we reach the end of the array
                while(!isFound && (index != ALPHABET_ARRAY_SIZE - 1)) {
                    // Using an if statement to check if a character matches
                    if(message[i] == alphabetArray[index].getCharacter()) {
                        // Entering the if statement, we will know our character is in the alphabet, so we set
                        //      isFound to be true
                        isFound = true;
                    }

                    // Else statement that will increment the index variable to go to the next array element
                    else {
                        index++;
                    }          
                }

                // Using an if else statement, we will check if the character was found in the array. If it is found,
                //       we will start the next process by checking if the corresponding node in the alphabet array
                //      is pointing a node in the tree. In other words, we will see if the character node already exists
                if(isFound) {
                    // Checking to see if the alphabet array element is pointing to a character node. If the character node
                    //       doesn't exist we will have our first case, which is that we need to add the character node 
                    //       and its parent counter node into the huffman tree
                    if(alphabetArray[index].getAlphabetNode() == nullptr) { 
                        // For the characterNode that we are adding to the tree, we will set its character member
                        //      to the new character from the message
                        characterNode->setCharacter(message[i]);

                        // Now, since the character node doesn't exist, we need to determine the path from the root node
                        //      to the zero node, so we can output it to our endoded message string. In this event, we 
                        //      have two cases: 1. The zero node is the root and 2. The zero node is not the root. 
                        // Checking to see if the zero is the root
                        if(this->root == this->zeroNode) {
                            // Within in this if statement, we will handle the case when the zero node is the root 
                            //      i.e. when this is our first character being introduced into the tree
                            // In this case, the path from the root to the zero is nothing, so we will simply 
                            //      output the bits of the character to our encoded message. To do this, we will 
                            //      utilize the bitset class to convert the character to bits.
                            bitset<8> characterBits(message[i]);

                            // Now, we will convert the eight bit representation of the character to a string variable
                            stringBits = characterBits.to_string();

                            // And now, we will append the string to our encoded message string
                            encodedMessage.append(stringBits);
                        } 

                        // Else statement that will traverse from the zero node up to the root, to determine the output
                        //      we need to append to our encode message
                        else {
                            // Now, since we are starting at the zero node, we will set our traversal node to the zero node
                            traversalNode = this->zeroNode;

                            // Next, we will use a while loop to go up the tree, logging a one if the child node is the right
                            //      child of its parent, and a zero, if the child is the left child of its parent
                            while(traversalNode->getParentNode() != nullptr) {
                                // Check to see if the traversal node is the left child of the parent
                                if(traversalNode == traversalNode->getParentNode()->getLeftNode()) {
                                    // If its the left child, we will append a zero to the reversePath path
                                    reversePath.append("0");
                                }

                                // Else, the traversal node is the right child, so we append a one
                                else {
                                    reversePath.append("1");
                                }

                                // Updating the traversal node to be its previous parent
                                traversalNode = traversalNode->getParentNode();
                            }

                            // Out of the while loop, we will have obtained the reversePath from the node to the root, so now
                            //      we will use the reverseString function and store the correct path into the correctPath string.
                            correctPath = reverseString(reversePath);

                            // Now that we have obtained the correctPath, we will output the path into our encodedMessage
                            encodedMessage.append(correctPath);

                            // The next step is to output the binary representation of the character that we are adding to the tree
                            // Getting the eight bit representation
                            bitset<8> characterBits(message[i]);

                            // Converting the eight bits of the character to a string variable
                            stringBits = characterBits.to_string();

                            // Finally, appending the string to our encoded message string
                            encodedMessage.append(stringBits);  
                        }

                        // Next, we will add the new nodes into our list, setting the correct pointer members as required
                        // First, we will set the included pointers for the parent counter node. There are two instances
                        //      that we will encounter when a new counter node is added to the tree, and those are
                        //      1. The zero node is the root and 2. The zero node is not the root.

                        // No matter the case, we always have a series of pointers to set and to add the counter node to 
                        //      the tree, these steps are:
                        // 1. The zeroNode becomes the counter node's left child
                        counterNode->setLeftNode(this->zeroNode);

                        // 2. The characterNode becomes the counter node's right child
                        counterNode->setRightNode(characterNode);

                        // 3. The characterNode becomes its next node in the chain
                        counterNode->setNextNode(characterNode);

                        // Now, we will check to see if the zero node has a parent (i.e. the zero node is not the root) 
                        if(this->zeroNode->getParentNode() != nullptr) {
                            // Setting zero node's parent node to the parentNode temporary pointer
                            parentNode = zeroNode->getParentNode();

                            // If the zeroNode has a parent, that means, for the counterNode we have to follow these steps:
                            // 1. Make the zero node's parent the parent for the counternode
                            counterNode->setParentNode(parentNode);

                            // 2. Make zeroNode's parent node's left child be the counterNode
                            parentNode->setLeftNode(counterNode);

                            // 3. Make the right child of the new parent, the previous to the counterNode
                            counterNode->setPrevNode(parentNode->getRightNode());

                            // 4. Make the right child of the new parent's next, become the counterNode
                            parentNode->getRightNode()->setNextNode(counterNode);
                        }

                        // Else, it is our first case where the zero node is the root, which means the counterNode becomes the 
                        //      new root of the tree, so we only have one more assignment to do
                        else {
                            this->root = counterNode;
                        }

                        // Now that we have finished assigning the proper pointers for the counter node, we will move on 
                        //      to placing the character node into the tree, through these steps:
                        // 1. The counter node becomes the parent of the character node
                        characterNode->setParentNode(counterNode);

                        // 2. The counter node becomes the previous node in the chain from the character node
                        characterNode->setPrevNode(counterNode);

                        // 3. The zeroNode becomes the next node in the chain after the character node
                        characterNode->setNextNode(this->zeroNode);

                        // Next, we will update the pointers for the zeroNode
                        // Making the character node the previous node in the chain
                        this->zeroNode->setPrevNode(characterNode);

                        // Setting the parent of the zero node to the counter node
                        this->zeroNode->setParentNode(counterNode);  

                        // The next step in the process will be to increment the parent node of the new counter node,
                        //      if it was assigned a parent during insertion into the tree
                        if(counterNode->getParentNode() != nullptr) {
                            counterNode->getParentNode()->updateCount(1);
                        }

                        // And finally, we will update the alphabet array huffman node by setting the character node
                        //      as its alphabet node
                        alphabetArray[index].setAlphabetNode(characterNode);

                        // To start the next process of checking the chain, we will set our previous and next nodes
                        // Checking to see if the counter node has a parent 
                        if(counterNode->getParentNode() != nullptr) {

                            // Now, checking to see if the parent of the counter node has a parent too
                            if(counterNode->getParentNode()->getParentNode() != nullptr) {

                                // If the counternode has a grandparent, we will set the previous node to 
                                //      the parent's previous and the next node to the counter nodes parent
                                prevNode = counterNode->getParentNode()->getPrevNode();
                                nextNode = counterNode->getParentNode();
                            }

                            // Else, the previous node will become the counter node's parent, and the next node
                            //      will be the counter node's previous 
                            else {
                                prevNode = counterNode->getParentNode();
                                nextNode = counterNode->getPrevNode();
                            }

                        }
                        
                        // Else statement that just sets the prevNode to the counter node, since it will be the root,
                        //      this way, our conditional checks below will operate properly
                        else {
                            prevNode = characterNode->getPrevNode();
                            nextNode = characterNode;
                        }

                    }

                    // Else statement that will run for our second case in encoding which is that the character node
                    //      exists, and we need to increment its count
                    else {
                        // Since our character node is in the tree, we will use alphabet array alphabet node ot jump 
                        //      to the character node
                        // Setting our characterNode
                        characterNode = alphabetArray[index].getAlphabetNode();

                        // Now, starting at the character node, we will traverse up to the root to obtain the path
                        //      we must take to get to the node
                        traversalNode = characterNode;

                        // Next, we will use a while loop to go up the tree, logging a one if the child node is the right
                        //      child of its parent, and a zero, if the child is the left child of its parent
                        while(traversalNode->getParentNode() != nullptr) {
                            // Check to see if the traversal node is the left child of the parent
                            if(traversalNode == traversalNode->getParentNode()->getLeftNode()) {
                                // If its the left child, we will append a zero to the reversePath path
                                reversePath.append("0");
                            }

                            // Else, the traversal node is the right child, so we append a one
                            else {
                                reversePath.append("1");
                            }

                            // Updating the traversal node to be its previous parent
                            traversalNode = traversalNode->getParentNode();
                        }

                        // Out of the while loop, we will have obtained the reversePath from the node to the root, so now
                        //      we will use the reverseString function and store the correct path into the correctPath string.
                        correctPath = reverseString(reversePath);

                        // Now that we have obtained the correctPath, we will output the path into our encodedMessage
                        encodedMessage.append(correctPath);

                        // Next, we will increment the current node's count since we saw it again in the message
                        characterNode->updateCount(1);

                        // To start the next process of checking the chain,we willset our prev node to the character 
                        //      node's prev node in the chain (i.e. its parent), and assigning the nextNode to the characterNode
                        prevNode = characterNode->getPrevNode();
                        nextNode = characterNode;
                    }

                    // Checking to see if the previous node has a previous node (i.e. if it is the root)
                    if(prevNode->getPrevNode() != nullptr) {

                        // Now, we will perform these checks through a while loop, as we need to check the chain 
                        //      up until we reach the root. 
                        while(!endOfChain) {

                            // Assigning the currentNode to the current prevNode
                            currentNode = prevNode;

                            // Since we know that we might be swapping nodes, if the leader is not the parent
                            //      of the current nextNode, we will assign next node to swapNode
                            swapNode = nextNode;

                            // Now, we need to locate the leader of the smaller count values and perform a swap if need be.
                            //      To do so, first, we will assign the leaderCount value to the count of the prevNode 
                            //      in the list, so we can compare the nodes behind it
                            leaderCount = currentNode->getCount();

                            // So, to check the order, we see if the count of the next node is less than to the 
                            //      previous node's count
                            if(currentNode->getCount() < nextNode->getCount()) {
                                    
                                // Checking to see if the current node is not the root
                                if(currentNode->getPrevNode() != nullptr) {
                                    // Now, we will use a while loop to traverse up the chain to determine where the leader of 
                                    //      this count size is at in the tree
                                    while(leaderCount == currentNode->getCount() && prevNode->getPrevNode() != nullptr) {
                                        // Setting the currentnode to its previous node, the nextnode to currentNode, and then 
                                        //      the prevNode to the new current's previous node for the next check for leader
                                        nextNode = currentNode;
                                        currentNode = currentNode->getPrevNode();
                                        prevNode = currentNode->getPrevNode();
                                    }

                                    // Now, exiting the while loop, we will have obtained the leader of the counts and it will
                                    //      be represented as the currentNode pointer. Now, we will check if the currentNode pointer
                                    //      is the parent of the swapNode

                                    // If the nextNode (leader of counts) is the parent of the swap node and there is no issue
                                    //      with the order of the chain, we will just increment the parent height 
                                    if(nextNode == swapNode->getParentNode() && nextNode->getCount() < currentNode->getCount()) {
                                        nextNode->updateCount(1);
                                    }

                                    // Else, we will perform a swap between the nextNode and the swap node
                                    else {
                                        // If the next node is the parent of the swap node, and the current node (the node previous
                                        //      to it in the chain) has a smaller count, we will assign the nextNode to be this currentNode
                                        //      since it is the leader of these smaller counts and needs to be swapped
                                        if(nextNode == swapNode->getParentNode() && currentNode->getCount() <= nextNode->getCount()) {
                                            nextNode = currentNode;
                                        }

                                        // Before we do the swap, we will get the parent, next, and prev nodes for both the next
                                        //      and swap nodes undergoing the swap in the tree
                                        nextNodeParent = nextNode->getParentNode();
                                        nextNodeNext = nextNode->getNextNode();
                                        nextNodePrev = nextNode->getPrevNode();
                                        swapNodeParent = swapNode->getParentNode();
                                        swapNodeNext = swapNode->getNextNode();
                                        swapNodePrev = swapNode->getPrevNode();

                                        // Our next steps will vary and have different cases: 1. The nextNode is the left child
                                        //      and the swapNode is the right child, 2. The nextNode and swapNode are both right children,
                                        //      3. They are both left children, and 4. The nextNode is the right child and the swapNode is the left child
                                        // Conditional that will check if the next node is the left child and if the swap node is the right 
                                        //      child, if the next and swap nodes are both right nodes, or if they are both left. These are for our first three cases
                                        if( (nextNode->getParentNode()->getLeftNode() == nextNode && swapNode->getParentNode()->getRightNode() == swapNode) ||
                                            (nextNode->getParentNode()->getRightNode() == nextNode && swapNode->getParentNode()->getRightNode() == swapNode) ||
                                            (nextNode->getParentNode()->getLeftNode() == nextNode && swapNode->getParentNode()->getLeftNode() == swapNode)) {
                                            // Now, to swap these two nodes, we will first assign the pointers to their original parents
                                            //      which means, for example, the nextNodeParent will now be the parent of the 
                                            //      swap node
                                            // Handling the parent assingment for the swap node
                                            swapNode->setParentNode(nextNodeParent);

                                            // Now, checking to see if the next node is the left or right child of its parent
                                            if(nextNode == nextNodeParent->getLeftNode()) {
                                                // Making the swap node the new left child of the current parent if the current node was its left
                                                //      child
                                                nextNodeParent->setLeftNode(swapNode);
                                            }

                                            // Else, the swap node will become its right child
                                            else {
                                                nextNodeParent->setRightNode(swapNode);
                                            }

                                            // Now, we handle the parent assignment for the current node
                                            nextNode->setParentNode(swapNodeParent);

                                            // Now, checking to see if the swap node was the left or right child of its parent
                                            if(swapNode == swapNodeParent->getLeftNode()) {
                                                // Making the current node the new left child of the swap parent if the swap node was its left
                                                //      child
                                                    swapNodeParent->setLeftNode(nextNode);
                                                }

                                            // Else, the current node will become its right child
                                            else {
                                                swapNodeParent->setRightNode(nextNode);
                                            }

                                            // Checking to see if we encounter the subcase of the swap node being the node in front of the
                                            //      nextNode in the chain, this requires special assignment.
                                            if(swapNode == nextNode->getNextNode()) {
                                                // Working with the swap node
                                                // Making the swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making the next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, the next node
                                                swapNode->setNextNode(nextNode);

                                                // Working with the next node 
                                                // Making the next node's previous node, the swap node
                                                nextNode->setPrevNode(swapNode);

                                                // Making swap node's old next node,  next node's new next node
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making swap Node next's previous, the next node
                                                swapNodeNext->setPrevNode(nextNode);
                                            }

                                            // Else, we have a normal case that can be solved with the same assignments
                                            else {
                                                // Now, we handle the assignment of the rest of the nexts and previous nodes
                                                // Working with the swap node
                                                // Making swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, next's old next
                                                swapNode->setNextNode(nextNodeNext);

                                                // Making the swap node the next node of nextNodeNext
                                                nextNodeNext->setPrevNode(swapNode);

                                                // Working with the next node
                                                // Making the next node's previous node, swap's old previous
                                                nextNode->setPrevNode(swapNodePrev);

                                                // Making swap node's previous node's next, next node
                                                swapNodePrev->setNextNode(nextNode);

                                                // Making next node's next, swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the next node the next node of swapNodeNext
                                                swapNodeNext->setPrevNode(nextNode);
                                            }
                                        }

                                        // Else if, the nextNode is the right child and the swapNode is the left child
                                        else if(nextNode->getParentNode()->getRightNode() == nextNode && swapNode->getParentNode()->getLeftNode() == swapNode){
                                            // Now, to swap these two nodes, we will first assign the pointers to their original parents
                                            //      which means, for example, the nextNodeParent will now be the parent of the 
                                            //      swap node
                                            // Handling the parent assingment for the swap node
                                            swapNode->setParentNode(nextNodeParent);

                                            // Now, checking to see if the next node is the left or right child of its parent
                                            if(nextNode == nextNodeParent->getLeftNode()) {
                                                // Making the swap node the new left child of the current parent if the current node was its left
                                                //      child
                                                nextNodeParent->setLeftNode(swapNode);
                                            }

                                            // Else, the swap node will become its right child
                                            else {
                                                nextNodeParent->setRightNode(swapNode);
                                            }

                                            // Now, we handle the parent assignment for the current node
                                            nextNode->setParentNode(swapNodeParent);

                                            // Now, checking to see if the swap node was the left or right child of its parent
                                            if(swapNode == swapNodeParent->getLeftNode()) {
                                                // Making the current node the new left child of the swap parent if the swap node was its left
                                                //      child
                                                    swapNodeParent->setLeftNode(nextNode);
                                                }

                                            // Else, the current node will become its right child
                                            else {
                                                swapNodeParent->setRightNode(nextNode);
                                            }

                                            // Now, we wil check if the swap and next nodes are siblings 
                                            if(nextNode->getNextNode() == swapNode) {
                                                // We can start to handle the assignment of the next nodes for the swap and next nodes
                                                // Making the nextNode's old next, the next node
                                                swapNode->setNextNode(nextNode);

                                                // Making swap node's new previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Now, for the next node, we will assign its next to the swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the nextNode's previous pointer, the swap node
                                                nextNode->setPrevNode(swapNode);

                                                // And assinging the swap node next as the previous next node
                                                swapNodeNext->setPrevNode(nextNode);

                                                // Assigning nextNodePrev's next node to the swapNode
                                                nextNodePrev->setNextNode(swapNode);                                     
                                            }

                                            // Else, they are not siblings
                                            else {
                                                // Now, we handle the assignment of the rest of the nexts and previous nodes
                                                // Working with the swap node
                                                // Making swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, next's old next
                                                swapNode->setNextNode(nextNodeNext);

                                                // Making the swap node the next node of nextNodeNext
                                                nextNodeNext->setPrevNode(swapNode);

                                                // Working with the next node
                                                // Making the next node's previous node, swap's old previous
                                                nextNode->setPrevNode(swapNodePrev);

                                                // Making swap node's previous node's next, next node
                                                swapNodePrev->setNextNode(nextNode);

                                                // Making next node's next, swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the next node the next node of swapNodeNext
                                                swapNodeNext->setPrevNode(nextNode);                                               
                                            }
                                        }   

                                        // Now, incrementing the height of the swap node's new parent
                                        swapNode->getParentNode()->updateCount(1);
                                    }
                                }

                                // Else, if the currentNode is the root, we will just update the current node,
                                else {
                                    currentNode->updateCount(1);
                                }
                            }

                            // Else statement that will simply update the count of the swap node's parent 
                            //      since there is nothing wrong with the chain order
                            else {
                                swapNode->getParentNode()->updateCount(1);
                            }

                            // Checking to see if the swap node's parent has a revious node of itself (i.e. if it is not the root)
                            if(swapNode->getParentNode()->getPrevNode() != nullptr) {
                                // Traversing up the chain by assigning the previous node to the current node's prevNode and the next node
                                //      to the original current node
                                nextNode = swapNode->getParentNode();
                                prevNode = swapNode->getParentNode()->getPrevNode(); 
                            }

                            // Else, the current node is the root, so we will stop the traversal process
                            else {
                                endOfChain = true;
                            }
                        }                    
                    }

                    // Else if statement that will handle the case where we are dealing with the root node directly with no iteration
                    //      through the tree. This will update the count of the root node (prevNode) if the sum of its two children
                    //      are not equal to it.
                    else if(prevNode->getCount() != (prevNode->getLeftNode()->getCount() + prevNode->getRightNode()->getCount())){
                        prevNode->updateCount(1);
                    }
                }

                // Else statement that will throw an exception since the character was not in our alphabet
                else {
                    throw HuffmanException("Invalid Character In Message. Re-Run Program To Try Again.");
                }  
            }
            
            // Finally, returning the fully encoded message
            return encodedMessage;
        }

        // Catching the error thrown if a character is not in the alphabet
        catch(HuffmanException error) {            
            error.outputError();
            return "NULL";
        }
    }

    // Creating our decode method that takes in the encoded string message and decodes it. After decoding, this method
    //      then returns decoded version of the encoded message, which should be the original message.
    string decode(string messageString) {
        // Creating a large try-catch block to handle the error we get if a character in the message is not in the 
        //     alphabet array
        try {
            // Our first task in the decoding process will be to cast the string message into a c_string so we can 
            //      easily access each character
            const char* message = messageString.c_str();

            // Creating our limitation variable for how many characters we want to iterate through our message. Due
            //      to appending an extra \n to our string message while reading it in from file, there will
            //      always be extra 

            // Creating the decoded message string that will be used to track and hold the output of our message
            //      while work through the process of decoding it
            string decodedMessage;

            // Creating a count variable that will increase with each bit we read in, and let us know when we
            //      have reached the end of the message
            int count = 0;

            // Now, we will create a while loop that will let us iterate through the entire message without 
            //      having a bounds issue
            while(count < strlen(message)) {

                // First we will create pointers to our two new nodes in the tree: the character node and the 
                //      its parent counter node
                HuffmanNode* counterNode = new HuffmanNode();

                // For the character node, we are intializing it with the character in our message that we are currently
                //      encoding
                HuffmanNode* characterNode = new HuffmanNode();

                // Creating a temporary string variable that will be used to store the eight bit representation
                //      of each character output
                string stringBits;

                // Creating a character variable to hold the character value after we decode its bits
                char character;

                // Creating a boolean variable that will be used in our conditional checking to help see if a character
                //      is within our alphabet array
                bool isFound = false;

                // Creating another boolean variable to keep track of where we are at while traversing up the chain
                bool endOfChain = false;

                // Creating an int variable to keep track of the leader count while checking the chain
                int leaderCount;

                // Creating a string that will be appended to, so we can hold the 1's and 0's as we traverse up the tree 
                //      to obtain the path from the root to a node
                string reversePath;

                // Creating another string to hold the result from the reverseString method, which will be the correct path
                //      from the root to a given node
                string correctPath;

                // Creating a traversal node that will be used to obtain the path from the root to a given node
                HuffmanNode* traversalNode = nullptr;

                // Creating a temporary node pointer to keep track of the current node we are on
                HuffmanNode* currentNode = nullptr;

                // Creating a temporary node pointer to get the parent of each node, while we traverse and update our tree
                HuffmanNode* parentNode = nullptr;

                // Creating two node pointers, one for the next node in the chain and one for the previous node in the cahin
                HuffmanNode* prevNode = nullptr;
                HuffmanNode* nextNode = nullptr;

                // Creating a node pointer called swapNode, to help keep track of the node that we swap with another
                //      leaf chracter node
                HuffmanNode* swapNode = nullptr;

                // For our swapping process, we will obtain the parent, next, and prev pointers for both the next node and
                //      the swap node, that are switching places. Below, we create all of them.
                HuffmanNode* nextNodeParent = nullptr;
                HuffmanNode* nextNodeNext = nullptr;
                HuffmanNode* nextNodePrev = nullptr;
                HuffmanNode* swapNodeParent = nullptr;
                HuffmanNode* swapNodeNext = nullptr;
                HuffmanNode* swapNodePrev = nullptr;

                // Creating a accumlator integer to also help with our check to see if a character is in our array
                int index = 0;

                // Our first action with the decoding will always be to read in the first eight bits of the message,
                //      since we know that all we have in the tree is the zero node, therfore the first bits will
                //      be a character that we will add to the tree, if it is in our alphabet
                // First, with an if statement, we check if the root is the zero node 
                if(this->zeroNode == this->root) {
                    // Using a for loop, to get the first eight bits of the message
                    for(int i = 0; i < 8; i++) {
                        // Casting the ith bit element into a string 
                        string temp(1, message[i]);

                        // Appending the temporary string into the stringBits string,  which will be later converted to 
                        //      its character value
                        stringBits.append(temp);

                        // Increasing the count by one with each bit read
                        count++;
                    }

                    // Using the convert binary function to convert the eight bits, into its character reprensentation
                    character = convertBinary(stringBits);                               
                }

                // Else statement means we need to start at the bit equal to one less than the count, and see where
                //      it leads us in the tree. We have two cases that may happen: 1. We land in the zero node
                //      and 2. We land at a character node
                else {
                    // First, we need to read in the bits until our traversal node no longer has a child
                    // Assigning the traversal node to the root
                    traversalNode = this->root;

                    // Using a while loop to traverse down the correct path given by the bits in the encoded message
                    //      Recall that a '0' is left and '1' is right.
                    while(traversalNode->getLeftNode() != nullptr && traversalNode->getRightNode() != nullptr) {
                        // Using the rule, to determine if we need to take a left or right path
                        if(message[count] == '0') {
                            traversalNode = traversalNode->getLeftNode();
                        }

                        // Else, the bit was a 1 and we take the right path
                        else {
                            traversalNode = traversalNode->getRightNode();
                        }

                        // Incrementing the count, since we looked at a bit
                        count++;
                    }

                    // Now, out of the while loop, we will either be at the zero node or a character node
                    if(traversalNode == this->zeroNode) {
                        // If the traversal node is a zero node, that means we have encountered a new character,
                        //      and we need to read in the next eight bits to determine what that character is
        
                        // Creating a temporary variable to hold the value of the current count + 8, so we 
                        //      can make sure to read all eight bits for the new character
                        int countPlusEight = count + 8;

                        // Using a for loop to read in the next eight bits, based on the current count value and note 
                        //      that using count in the for loop will also take care of the incrementing for each bit read
                        for(count; count < countPlusEight; count++) {
                            // Casting the ith bit element into a string 
                            string temp(1, message[count]);

                            // Appending the temporary string into the stringBits string,  which will be later converted to 
                            //      its character value
                            stringBits.append(temp);
                        }

                        // // Incrementing the count by one, to adjust to the next correct bit in the message
                        // count++;

                        // Using the convert binary function to convert the eight bits, into its character reprensentation
                        character = convertBinary(stringBits);
                    }

                    // Else, we are at a character node, so we just get the character from the node
                    else {
                        character = traversalNode->getCharacter();
                    }
                }

                // Next we check whether or not the character that we decoded is in our alphabet
                //      To do this, we will use a while loop and our conditional check will be either 
                //      if the character was found or we reach the end of the array
                while(!isFound && (index != ALPHABET_ARRAY_SIZE - 1)) {
                    // Using an if statement to check if a character matches
                    if(character == alphabetArray[index].getCharacter()) {
                        // Entering the if statement, we will know our character is in the alphabet, so we set
                        //      isFound to be true
                        isFound = true;
                    }

                    // Else statement that will increment the index variable to go to the next array element
                    else {
                        index++;
                    }          
                } 
                
                // Using an if else statement, we will check if the character was found in the array. If it is found,
                //       we will start the next process by checking if the corresponding node in the alphabet array
                //      is pointing a node in the tree. In other words, we will see if the character node already exists.
                if(isFound) {
                    // First, since our character is apart of the alphabet, we will append it to the decode message
                    // Converting the character back to a string variable
                    string backToString(1, character);

                    // Now appending it to the decoded message
                    decodedMessage.append(backToString);

                    // Checking to see if the alphabet array element is pointing to a character node. If the character node
                    //       doesn't exist we will have our first case, which is that we need to add the character node 
                    //       and its parent counter node into the huffman tree
                    if(alphabetArray[index].getAlphabetNode() == nullptr) { 
                        // For the characterNode that we are adding to the tree, we will set its character member
                        //      to the new character from the message
                        characterNode->setCharacter(character);

                        // Next, we will add the new nodes into our list, setting the correct pointer members as required
                        // First, we will set the included pointers for the parent counter node. There are two instances
                        //      that we will encounter when a new counter node is added to the tree, and those are
                        //      1. The zero node is the root and 2. The zero node is not the root.

                        // No matter the case, we always have a series of pointers to set and to add the counter node to 
                        //      the tree, these steps are:
                        // 1. The zeroNode becomes the counter node's left child
                        counterNode->setLeftNode(this->zeroNode);

                        // 2. The characterNode becomes the counter node's right child
                        counterNode->setRightNode(characterNode);

                        // 3. The characterNode becomes its next node in the chain
                        counterNode->setNextNode(characterNode);

                        // Now, we will check to see if the zero node has a parent (i.e. the zero node is not the root) 
                        if(this->zeroNode->getParentNode() != nullptr) {
                            // Setting zero node's parent node to the parentNode temporary pointer
                            parentNode = zeroNode->getParentNode();

                            // If the zeroNode has a parent, that means, for the counterNode we have to follow these steps:
                            // 1. Make the zero node's parent the parent for the counternode
                            counterNode->setParentNode(parentNode);

                            // 2. Make zeroNode's parent node's left child be the counterNode
                            parentNode->setLeftNode(counterNode);

                            // 3. Make the right child of the new parent, the previous to the counterNode
                            counterNode->setPrevNode(parentNode->getRightNode());

                            // 4. Make the right child of the new parent's next, become the counterNode
                            parentNode->getRightNode()->setNextNode(counterNode);
                        }

                        // Else, it is our first case where the zero node is the root, which means the counterNode becomes the 
                        //      new root of the tree, so we only have one more assignment to do
                        else {
                            this->root = counterNode;
                        }

                        // Now that we have finished assigning the proper pointers for the counter node, we will move on 
                        //      to placing the character node into the tree, through these steps:
                        // 1. The counter node becomes the parent of the character node
                        characterNode->setParentNode(counterNode);

                        // 2. The counter node becomes the previous node in the chain from the character node
                        characterNode->setPrevNode(counterNode);

                        // 3. The zeroNode becomes the next node in the chain after the character node
                        characterNode->setNextNode(this->zeroNode);

                        // Next, we will update the pointers for the zeroNode
                        // Making the character node the previous node in the chain
                        this->zeroNode->setPrevNode(characterNode);

                        // Setting the parent of the zero node to the counter node
                        this->zeroNode->setParentNode(counterNode);  

                        // The next step in the process will be to increment the parent node of the new counter node,
                        //      if it was assigned a parent during insertion into the tree
                        if(counterNode->getParentNode() != nullptr) {
                            counterNode->getParentNode()->updateCount(1);
                        }

                        // And finally, we will update the alphabet array huffman node by setting the character node
                        //      as its alphabet node
                        alphabetArray[index].setAlphabetNode(characterNode);

                        // To start the next process of checking the chain, we will set our previous and next nodes
                        // Checking to see if the counter node has a parent 
                        if(counterNode->getParentNode() != nullptr) {
                            // Now, checking to see if the parent of the counter node has a parent too
                            if(counterNode->getParentNode()->getParentNode() != nullptr) {

                                // If the counternode has a grandparent, we will set the previous node to 
                                //      the parent's previous and the next node to the counter nodes parent
                                prevNode = counterNode->getParentNode()->getPrevNode();
                                nextNode = counterNode->getParentNode();
                            }

                            // Else, the previous node will become the counter node's parent, and the next node
                            //      will be the counter node's previous 
                            else {
                                prevNode = counterNode->getParentNode();
                                nextNode = counterNode->getPrevNode();
                            }
                        }
                        
                        // Else statement that just sets the prevNode to the counter node, since it will be the root,
                        //      this way, our conditional checks below will operate properly
                        else {
                            prevNode = characterNode->getPrevNode();
                            nextNode = characterNode;
                        }

                    }

                    // Else statement that will run for our second case in decoding which is that the character node
                    //      exists, and we need to increment its count
                    else {
                        // Since our character node is in the tree, we will use alphabet array alphabet node ot jump 
                        //      to the character node
                        // Setting our characterNode
                        characterNode = alphabetArray[index].getAlphabetNode();

                        // Next, we will increment the current node's count since we saw it again in the message
                        characterNode->updateCount(1);

                        // To start the next process of checking the chain,we willset our prev node to the character 
                        //      node's prev node in the chain (i.e. its parent), and assigning the nextNode to the characterNode
                        prevNode = characterNode->getPrevNode();
                        nextNode = characterNode;
                    }

                    // Checking to see if the previous node has a previous node (i.e. if it is the root)
                    if(prevNode->getPrevNode() != nullptr) {

                        // Now, we will perform these checks through a while loop, as we need to check the chain 
                        //      up until we reach the root. 
                        while(!endOfChain) {

                            // Assigning the currentNode to the current prevNode
                            currentNode = prevNode;

                            // Since we know that we might be swapping nodes, if the leader is not the parent
                            //      of the current nextNode, we will assign next node to swapNode
                            swapNode = nextNode;

                            // Now, we need to locate the leader of the smaller count values and perform a swap if need be.
                            //      To do so, first, we will assign the leaderCount value to the count of the prevNode 
                            //      in the list, so we can compare the nodes behind it
                            leaderCount = currentNode->getCount();

                            // So, to check the order, we see if the count of the next node is less than to the 
                            //      previous node's count
                            if(currentNode->getCount() < nextNode->getCount()) {
                                    
                                // Checking to see if the current node is not the root
                                if(currentNode->getPrevNode() != nullptr) {
                                    // Now, we will use a while loop to traverse up the chain to determine where the leader of 
                                    //      this count size is at in the tree
                                    while(leaderCount == currentNode->getCount() && prevNode->getPrevNode() != nullptr) {
                                        // Setting the currentnode to its previous node, the nextnode to currentNode, and then 
                                        //      the prevNode to the new current's previous node for the next check for leader
                                        nextNode = currentNode;
                                        currentNode = currentNode->getPrevNode();
                                        prevNode = currentNode->getPrevNode();
                                    }

                                    // Now, exiting the while loop, we will have obtained the leader of the counts and it will
                                    //      be represented as the currentNode pointer. Now, we will check if the currentNode pointer
                                    //      is the parent of the swapNode

                                    // If the nextNode (leader of counts) is the parent of the swap node and there is no issue
                                    //      with the order of the chain, we will just increment the parent height 
                                    if(nextNode == swapNode->getParentNode() && nextNode->getCount() < currentNode->getCount()) {
                                        nextNode->updateCount(1);
                                    }

                                    // Else, we will perform a swap between the nextNode and the swap node
                                    else {
                                        // If the next node is the parent of the swap node, and the current node (the node previous
                                        //      to it in the chain) has a smaller count, we will assign the nextNode to be this currentNode
                                        //      since it is the leader of these smaller counts and needs to be swapped
                                        if(nextNode == swapNode->getParentNode() && currentNode->getCount() <= nextNode->getCount()) {
                                            nextNode = currentNode;
                                        }

                                        // Before we do the swap, we will get the parent, next, and prev nodes for both the next
                                        //      and swap nodes undergoing the swap in the tree
                                        nextNodeParent = nextNode->getParentNode();
                                        nextNodeNext = nextNode->getNextNode();
                                        nextNodePrev = nextNode->getPrevNode();
                                        swapNodeParent = swapNode->getParentNode();
                                        swapNodeNext = swapNode->getNextNode();
                                        swapNodePrev = swapNode->getPrevNode();

                                        // Our next steps will vary and have different cases: 1. The nextNode is the left child
                                        //      and the swapNode is the right child, 2. The nextNode and swapNode are both right children, and
                                        //      3. They are both left children, and 3. The nextNode is the right child and the swapNode is the left child
                                        // Conditional that will check if the next node is the left child and if the swap node is the right 
                                        //      child, if the next and swap nodes are both right nodes, or if they are both left. These are for our first three cases
                                        if( (nextNode->getParentNode()->getLeftNode() == nextNode && swapNode->getParentNode()->getRightNode() == swapNode) ||
                                            (nextNode->getParentNode()->getRightNode() == nextNode && swapNode->getParentNode()->getRightNode() == swapNode) ||
                                            (nextNode->getParentNode()->getLeftNode() == nextNode && swapNode->getParentNode()->getLeftNode() == swapNode)) {
                                            // Now, to swap these two nodes, we will first assign the pointers to their original parents
                                            //      which means, for example, the nextNodeParent will now be the parent of the 
                                            //      swap node
                                            // Handling the parent assingment for the swap node
                                            swapNode->setParentNode(nextNodeParent);

                                            // Now, checking to see if the next node is the left or right child of its parent
                                            if(nextNode == nextNodeParent->getLeftNode()) {
                                                // Making the swap node the new left child of the current parent if the current node was its left
                                                //      child
                                                nextNodeParent->setLeftNode(swapNode);
                                            }

                                            // Else, the swap node will become its right child
                                            else {
                                                nextNodeParent->setRightNode(swapNode);
                                            }

                                            // Now, we handle the parent assignment for the current node
                                            nextNode->setParentNode(swapNodeParent);

                                            // Now, checking to see if the swap node was the left or right child of its parent
                                            if(swapNode == swapNodeParent->getLeftNode()) {
                                                // Making the current node the new left child of the swap parent if the swap node was its left
                                                //      child
                                                    swapNodeParent->setLeftNode(nextNode);
                                                }

                                            // Else, the current node will become its right child
                                            else {
                                                swapNodeParent->setRightNode(nextNode);
                                            }

                                            // Checking to see if we encounter the subcase of the swap node being the node in front of the
                                            //      nextNode in the chain, this requires special assignment.
                                            if(swapNode == nextNode->getNextNode()) {
                                                // Working with the swap node
                                                // Making the swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making the next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, the next node
                                                swapNode->setNextNode(nextNode);

                                                // Working with the next node 
                                                // Making the next node's previous node, the swap node
                                                nextNode->setPrevNode(swapNode);

                                                // Making swap node's old next node,  next node's new next node
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making swap Node next's previous, the next node
                                                swapNodeNext->setPrevNode(nextNode);
                                            }

                                            // Else, we have a normal case that can be solved with the same assignments
                                            else {
                                                // Now, we handle the assignment of the rest of the nexts and previous nodes
                                                // Working with the swap node
                                                // Making swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, next's old next
                                                swapNode->setNextNode(nextNodeNext);

                                                // Making the swap node the next node of nextNodeNext
                                                nextNodeNext->setPrevNode(swapNode);

                                                // Working with the next node
                                                // Making the next node's previous node, swap's old previous
                                                nextNode->setPrevNode(swapNodePrev);

                                                // Making swap node's previous node's next, next node
                                                swapNodePrev->setNextNode(nextNode);

                                                // Making next node's next, swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the next node the next node of swapNodeNext
                                                swapNodeNext->setPrevNode(nextNode);
                                            }
                                        }

                                        // Else if, the nextNode is the right child and the swapNode is the left child
                                        else if(nextNode->getParentNode()->getRightNode() == nextNode && swapNode->getParentNode()->getLeftNode() == swapNode){
                                            // Now, to swap these two nodes, we will first assign the pointers to their original parents
                                            //      which means, for example, the nextNodeParent will now be the parent of the 
                                            //      swap node
                                            // Handling the parent assingment for the swap node
                                            swapNode->setParentNode(nextNodeParent);

                                            // Now, checking to see if the next node is the left or right child of its parent
                                            if(nextNode == nextNodeParent->getLeftNode()) {
                                                // Making the swap node the new left child of the current parent if the current node was its left
                                                //      child
                                                nextNodeParent->setLeftNode(swapNode);
                                            }

                                            // Else, the swap node will become its right child
                                            else {
                                                nextNodeParent->setRightNode(swapNode);
                                            }

                                            // Now, we handle the parent assignment for the current node
                                            nextNode->setParentNode(swapNodeParent);

                                            // Now, checking to see if the swap node was the left or right child of its parent
                                            if(swapNode == swapNodeParent->getLeftNode()) {
                                                // Making the current node the new left child of the swap parent if the swap node was its left
                                                //      child
                                                    swapNodeParent->setLeftNode(nextNode);
                                                }

                                            // Else, the current node will become its right child
                                            else {
                                                swapNodeParent->setRightNode(nextNode);
                                            }

                                            // Now, we wil check if the swap and next nodes are siblings 
                                            if(nextNode->getNextNode() == swapNode) {
                                                // We can start to handle the assignment of the next nodes for the swap and next nodes
                                                // Making the nextNode's old next, the next node
                                                swapNode->setNextNode(nextNode);

                                                // Making swap node's new previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Now, for the next node, we will assign its next to the swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the nextNode's previous pointer, the swap node
                                                nextNode->setPrevNode(swapNode);

                                                // And assinging the swap node next as the previous next node
                                                swapNodeNext->setPrevNode(nextNode);

                                                // Assigning nextNodePrev's next node to the swapNode
                                                nextNodePrev->setNextNode(swapNode);                                     
                                            }
                                            
                                            // Else, they are not siblings
                                            else {
                                                // Now, we handle the assignment of the rest of the nexts and previous nodes
                                                // Working with the swap node
                                                // Making swap node's previous node, next's old previous
                                                swapNode->setPrevNode(nextNodePrev);

                                                // Making next node's previous node's next, swap node
                                                nextNodePrev->setNextNode(swapNode);

                                                // Making swap node's next, next's old next
                                                swapNode->setNextNode(nextNodeNext);

                                                // Making the swap node the next node of nextNodeNext
                                                nextNodeNext->setPrevNode(swapNode);

                                                // Working with the next node
                                                // Making the next node's previous node, swap's old previous
                                                nextNode->setPrevNode(swapNodePrev);

                                                // Making swap node's previous node's next, next node
                                                swapNodePrev->setNextNode(nextNode);

                                                // Making next node's next, swap's old next
                                                nextNode->setNextNode(swapNodeNext);

                                                // Making the next node the next node of swapNodeNext
                                                swapNodeNext->setPrevNode(nextNode);                                               
                                            }                                            
                                        }

                                        // Now, incrementing the height of the swap node's new parent
                                        swapNode->getParentNode()->updateCount(1);
                                    }
                                }

                                // Else, if the currentNode is the root, we will just update the current node,
                                else {
                                    currentNode->updateCount(1);
                                }
                            }

                            // Else statement that will simply update the count of the swap node's parent 
                            //      since there is nothing wrong with the chain order
                            else {
                                swapNode->getParentNode()->updateCount(1);
                            }

                            // Checking to see if the swap node's parent has a revious node of itself (i.e. if it is not the root)
                            if(swapNode->getParentNode()->getPrevNode() != nullptr) {
                                // Traversing up the chain by assigning the previous node to the current node's prevNode and the next node
                                //      to the original current node
                                nextNode = swapNode->getParentNode();
                                prevNode = swapNode->getParentNode()->getPrevNode(); 
                            }

                            // Else, the current node is the root, so we will stop the traversal process
                            else {
                                endOfChain = true;
                            }
                        }                    
                    }

                    // Else if statement that will handle the case where we are dealing with the root node directly with no iteration
                    //      through the tree. This will update the count of the root node (prevNode) if the sum of its two children
                    //      are not equal to it.
                    else if(prevNode->getCount() != (prevNode->getLeftNode()->getCount() + prevNode->getRightNode()->getCount())){
                        prevNode->updateCount(1);
                    }
                }

                // Else statement that will throw an exception since the character was not in our alphabet
                else {
                    throw HuffmanException("Invalid Character In Message. Re-Run Program To Try Again.");
                }  

            }
            
            // Finally, returning the fully encoded message
            return decodedMessage;
        }
        
        // Catching the error thrown if a character is not in the alphabet
        catch(HuffmanException error) {            
            error.outputError();
            return "NULL";
        }
    }

    // Creating a reverse method that will allow us to properly track the path it takes from the root down to the 
    //      desired character.
    string reverseString(string path) {

        // Creating a c_string from the string parameter passed into the function
        const char* cPath = path.c_str();

        // First, we will make a temporary char array to hold our characters. To set the correct length, we will use
        //      the length of the string passed in to the function.
        char tempPathArray[path.length()];

        // Creating a string to hold our corrected path
        string correctedPath;

        // Getting the length of the path and subtracting one from it, so we can use it to properly access
        //      the correct elements of the passed in array in our for loop
        int reversedIndex = path.length() - 1;

        // Creating a count variable, so we can correctly assign elements to the temporary array
        int correctingIndex = 0;

        // To perform the reverse of the path, we will use a for loop to traverse the loop starting from the 
        //      the last element of the char array passed to this function
        for(int i = reversedIndex; i >= 0; i--) {
            // Casting the path element into a string 
            string temp(1, cPath[i]);

            // Appending the last element in the reversed path to our correct path, which is the reversal process
            correctedPath.append(temp);

            // Incrementing the index variable
            correctingIndex++;
        }

        // Finally, returning the correct version of the path
        return correctedPath;
    } 

    // Creating a function that takes in the binary representation of from the encoded message, and converts it into the corresponding character
    char convertBinary(string binaryString) {
        // ASCII Value accumulator variable to keep track of the total while we traverse through the eight bits
        int asciiValue = 0;

        // This powerOfTwo variable will allow us to calculate the correct ASCII value. For each iteration of the for loop,
        //      this variable will be decremented as the power of two's decrease by one as you go from the left to right. And
        //      as we are starting on the left side of the eight bits, the first power of two is 7, so we decrease from there.
        int powerOfTwo = 7;

        // // Converting the binaryString passed into the function into a cstring, so we can access each bit one at a time
        // const char* binaryCString = binaryString.c_str();

        // Creating a for loop to help us iterate through the bit array and convert into our ASCII value
        for(int i = 0; i < binaryString.length(); i++) {
            // For the bits, we will check if it is a '1', if it is, we will add the correct power of two to the ASCII value,
            //      if not, we will do nothing since the bit was a zero not a one
            if(binaryString[i] == '1') {
                asciiValue += pow(2, powerOfTwo);
            } 

            // And decrementing powerOfTwo since we are going to move to the bit to the right
            powerOfTwo--;
        }

        // Finally, casting the ASCII value to generate the corresponding character and returning it from the function        
        return char(asciiValue);
    }
};