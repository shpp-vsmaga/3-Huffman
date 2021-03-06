/* File: Huffman.cpp
 * -----------------------------------------------------------------------------------------
 *
 * This is a simple archive program, that using Huffman's algorithm for coding files.
 * The main idea of this algoritm is to write most frequently used characters using less then
 * eight bits.
 * This program asks user to choose operation (coding or decoding) and enter filename for this
 * operation. Than doing selected operation and display name of result file.
 */

#include <fstream>
#include <iostream>
#include <string>

#include "pqueueshpp.h"


using namespace std;

/* Structure to save characters in binary tree*/
struct TreeNode {
    char ch;
    bool isBusy = false;
    TreeNode *left, *right;
};

/* Function prototypes*/
void archiveFile(string sourceFilename, string resultFilename);
int* getAlphabet(ifstream &infile, int &sourceFileLength);
PQueueSHPP<TreeNode*> getQueue (int *alphabet);
TreeNode* getTree(PQueueSHPP<TreeNode*> queue);
void getTable(TreeNode* tree, string way, string *table);
string getAlphabetForFile(int *alphabet);
string getCodeFromFile(ifstream &archivedFile, int sourceFileLength);
int* parseCodeString(string codeString);
void writeArchiveFile(ifstream &sourceFile, string code, string archiveName, int sourceFileLength, string *table);
void dearchiveFile(string archiveName, string resultName);
char* getBodyFromFile(ifstream &file, int delCh, int& bodySize);
void writeDeArchFile(string fileName, char* body, TreeNode *root, int sourceFileLength, int bodySize);
int getLengthFromArchive(ifstream &archivedFile);
char strToByte(string byte);
string getBitsFromChar(char ch);
void clearTree(TreeNode* tree);

const int BYTES_NUMBER = 256;


/* Main program */
int main(int argc, char* argv[]) {

    string command;
    string filename;

    if (argc == 3){
        command = argv[1];
        filename = argv[2];
    }

    if (command == "-ar"){
        try{
            cout << "Processing... " << endl << endl;
            archiveFile(filename, filename + ".huf");
            cout << "Archivation done. File: (" << filename + ".huf) " << "created." << endl;
        }
        catch(...){
            cerr << "Error while compressing file" << endl;
        }
    } else if (command == "-de"){
        try{
            if (filename.substr(filename.length() - 4) == ".huf"){
                cout << "Processing... " << endl << endl;
                dearchiveFile(filename, "ORIGINAL_"+filename.substr(0, filename.length() - 4));
                cout << "Extraction done!!! File("<< "ORIGINAL_"+filename.substr(0, filename.length() - 4) << ") created" << endl;
            } else {
                cout << "File is not Huffman archive" << endl;
            }
        }
        catch (...){
            cerr << "Error while decompressing file" << endl;
        }
    } else {
        cout << "Please enter a valid command \"-ar filename\" to archive file, \"-de filename\" to dearchive file!!!" << endl;
        return 0;
    }


    return 0;
}


//-----------------------Encoding------------------------------------------------------
/** Function: archiveFile
 * Usage: archiveFile(sourceFileName, sourceFileName + ".huf");
 * ------------------------------------------------------------------------------------
 *
 * This function implements file encoding using Huffman's algoritm.
 * At the beginning it builds an alphabet of the characters and their frequency of use in the file.
 * After that builds the priority queue of all of these characters using frequency as a priority.
 * Then build binary tree based on the queue. Using this tree our function builds new table for coding
 * characters in the source file using less bits for commonly used characters. After this it write
 * output compressed file with length of source file, coding table in string format separated with
 * double braces from other part of the file with recoded body of the source file.
 *
 * @param sourceFileName Name of the source file
 * @param resultFilename Name of the output archive file
 *
 */

void archiveFile(string sourceFilename, string resultFilename){

    PQueueSHPP<TreeNode*> queue; // queue for building the tree
    ifstream sourceFile(sourceFilename, ifstream::binary);

    int sourceFileLength = 0;

    /* Alphabet with all characters used in the source file and their frequencies */
    int *alphabet = getAlphabet(sourceFile, sourceFileLength);
    sourceFile.close();


    /* Queue for building the tree*/
    queue = getQueue(alphabet);

    /* Huffman tree generated from the exact frequencies of the text */
    TreeNode* tree = getTree(queue);


    /* Table for coding characters saved in the array "table" */
    string way = ""; // way to the character in the binary tree in format "010100..."
    string* table = new string[BYTES_NUMBER];
    getTable(tree, way, table);


    /* Alphabet with all characters used in the source file and their frequencies
     * stored in string format and separators for subsequent writing to the archive file
     */
    string alphabetForFile = getAlphabetForFile(alphabet);


    /* Writing the result archive file with source file length, table for encoding and new body*/
    sourceFile.open(sourceFilename, ifstream::binary);
    writeArchiveFile(sourceFile, alphabetForFile, resultFilename, sourceFileLength, table);
    delete[] table;
    delete[] alphabet;
    clearTree(tree);
    sourceFile.close();
}

/** Function: getAlphabet
 * Usage: alphabet = getAlphabet(sourceFile, sourceFileLength);
 * --------------------------------------------------------------------------------------------
 *
 * This function read all characters in the recieved file stream and put their
 * frequencies to the array. Also it change the value of variable sourceFileLength, what will
 * be used in the reult file.
 *
 * @param &infile Link to the opened input file.
 * @param &sourceFileLength Link to variable for storing source file length.
 * @return array with the frequencies of the characters
 */
int* getAlphabet(ifstream &sourceFile, int &sourceFileLength){
    int *alphabet = new int[BYTES_NUMBER];
    for(int i = 0; i < BYTES_NUMBER; i++){
        alphabet[i] = 0;
    }

    /* Reading whole source file to char array*/
    sourceFile.seekg(0, sourceFile.end);
    int length = sourceFile.tellg();
    sourceFile.seekg(0, sourceFile.beg);
    sourceFileLength = length;
    char *buffer = new char[length];
    sourceFile.read(buffer, length);

    for(int i = 0; i < length; i++){
        char ch = buffer[i];
        alphabet[(unsigned char)ch]++;
    }

    return alphabet;
}

/** Function: getQueue
 * Usage: queue = getQueue(alphabet);
 * ---------------------------------------------------------------------------------
 *
 * This function build priority queue for Huffman's algoritm based on the
 * alphabet of the source file. Priority of every new element in it's queue
 * is equal to frequency in the alphabet.
 *
 * @param alphabet array with the frequencies of the characters
 * @return Ready priority queue with right priorities of every characters.
 */
PQueueSHPP<TreeNode*> getQueue(int *alphabet){

    PQueueSHPP<TreeNode*> queue;

    for(int i = 0; i < BYTES_NUMBER; i++){
        if(alphabet[i] != 0){
            unsigned char ch = i;
            TreeNode *node = new TreeNode;
            node->left = node->right = 0;
            node->ch = ch;
            node->isBusy = true;
            queue.enqueue(node, alphabet[i]);

        }
    }
    return queue;
}


/** Function: getTree
 * Usage:  BSTNode* tree = getTree(queue);
 * ---------------------------------------------------------------------
 *
 * This function builds binary tree for Huffman's algoritm based on the
 * received priority queue. It combines two elements of the queue with minimum
 * in one new element and put it in the queue with priority equals to
 * summ of priorities of this two elements.
 *
 * @param queue Priority queue with all characters of the source file.
 * @return Binary tree for the Huffman's algoritm.
 */
TreeNode* getTree(PQueueSHPP<TreeNode*> queue){

    while(queue.size() != 1){
        int newPriority = queue.peekPriority();

        TreeNode* newNode = new TreeNode;
        newNode->ch = 0;
        newNode->left = queue.dequeue();
        newPriority += queue.peekPriority();

        newNode->right = queue.dequeue();
        queue.enqueue(newNode, newPriority);

    }
    return queue.dequeue(); // last element of the queue
}

/** Function: getTable
 * Usage: getTable(tree, way, table);
 * ----------------------------------------------------------------------
 *
 * Function used for building new coding table for all characters depends on
 * it's frequensies in the source. It go through binary tree and on the way
 * to the character add's "1" or "0" to new code of character ("1" - if turned right
 * and "0" - if turned left).
 * New code for most frequently used characters consist of less number of bits.
 *
 * @param tree Pointer to the binary tree with all characters
 * @param way String variable to store new code for character
 * @param table array of new bit codes of the characters in the string format.
 */
void getTable(TreeNode *tree, string way, string *table){
    if (tree != 0){
        getTable(tree->left, way + "0", table);
        if (tree->isBusy){
            table[(int)(unsigned char)tree->ch] = way;
        }
        getTable(tree->right, way + "1", table);
    } else {
        return;
    }
}

/** Function: getAlphabetForFile
 * Usage: string alphabetForFile = getAlphabetForFile(alphabet);
 * ----------------------------------------------------------------------
 *
 * This fuction transform alphabet of the source file to the specified string format
 * for writing int the output archive file.
 *
 * @param alphabet array with the frequencies of the characters
 * @return Alphabet array transformed in the specified string.
 */
string getAlphabetForFile(int *alphabet){
    string result;
    for(int i = 0; i < BYTES_NUMBER; i++){
        if(alphabet[i] != 0){
            unsigned char ch = i;
            result += ch;
            result += to_string(alphabet[i]);
            result += ';';
        }
    }
    result += "}}"; //mark end of the coding table in archive file
    return result;
}


/** Function: writeArchiveFile
 * Usage:  writeArchiveFile(sourceFile, alphabetForFile, resultFilename, sourceFileLength, table);
 * ------------------------------------------------------------------------------------------
 *
 * This function creates and writes output archive file with specified structure.
 * At the begining of the file placed information about length of ource file (sourceFileLength),
 * after this alphabet for decoding file (alphabetForFile), then recoded source file body in
 * the binary mode.
 *
 * @param sourceFile Name of the source file.
 * @param code Alphabet for decodng in string format.
 * @param archiveName Name of the output archive file.
 * @param sourceFileLength Length of the source file.
 * @param table array of new bit codes of the characters.
 */
void writeArchiveFile(ifstream &sourceFile, string code, string archiveName, int sourceFileLength, string *table){

    ofstream outFile(archiveName, ofstream::binary);
    outFile << to_string(sourceFileLength) << "{" << code;

    /* Reading whole source file to char array*/
    sourceFile.seekg(0, sourceFile.end);
    int length = sourceFile.tellg();
    sourceFile.seekg(0, sourceFile.beg);
    char *buffer = new char[length];
    sourceFile.read(buffer, length);


    /* Go through char array, code all characters according to coding table in combination of bits
    *  and write it's binary values to string .
    */

    string bodyBitStr = "";
    for (int j = 0; j < length; j++) {
        char ch = buffer[j];
        bodyBitStr += table[(int)(unsigned char)ch];
    }

    /* Split bodyBitStr into portions of 8 bits, transorm them to real bytes and write them to the file.*/
    int count = bodyBitStr.size() / 8;
    if(bodyBitStr.size() % 8 != 0) {
        count++;
    }

    for(int i = 0; i < count; i++){
        string part = bodyBitStr.substr(i * 8, 8);
        char ch = strToByte(part);
        outFile.write((char*)&ch, sizeof(ch));
    }

    delete[] buffer;
    outFile.close();
}

/**
 * Function: strToByte
 * Usage:  char ch = strToByte(part);
 * ----------------------------------------------------------------------------
 *
 * This function transfor received string that contains 8 '0' or '1' to real byte.
 *
 * @param byte String with 8 '0' and '1'
 * @return Real char value of received string
 */
char strToByte(string byte){
    int result = 0;
    int currentBitValue = 128;

    for (int i = 0; i < 8; i++){
        if (byte[i] == '1'){
            result += currentBitValue;
        }
        currentBitValue /=2;
    }
    return (unsigned char) result;
}




//------------------------Decoding-----------------------------------------------------

/** Function: dearchiveFile
 * Usage: dearchiveFile(archiveFileName, "ORIGINAL_"+archiveFileName.substr(0, archiveFileName.length() - 4));
 * ------------------------------------------------------------------------------------
 *
 * This function open archive file with received name and decode them. At the begining it read length of the
 * source file, than it read and parse decoding table and after this read other part of the
 * archive file, decode it and write the output file with received name.
 *
 * @param archiveName Name of the input archive file
 * @param resultName Name of the output result file.
 */
void dearchiveFile(string archiveName, string resultName){
    ifstream archivedFile(archiveName, ifstream::binary);

    /* Reading length of the source file from archive */
    int sourceFileLength = getLengthFromArchive(archivedFile);

    /* Reading string with encoding table*/
    string codeString = getCodeFromFile(archivedFile, sourceFileLength);

    /* Writing string with encoding table to array*/
    int* alphFromFile = parseCodeString(codeString);

    /* Reading body of the source file from archive*/
    int bodySize;
    char* inputFileBody = getBodyFromFile(archivedFile, codeString.length() + 3 + to_string(sourceFileLength).length(), bodySize);
    archivedFile.close();

    /* Queue for building the tree generated from encoding table*/
    PQueueSHPP<TreeNode*> queue = getQueue(alphFromFile);

    /* Huffman tree generated from the encoding table */
    TreeNode * root = getTree(queue);

    /* Writing encoded file*/
    writeDeArchFile(resultName, inputFileBody, root, sourceFileLength, bodySize);
    delete[] alphFromFile;
    clearTree(root);
}

/**
 * Function: getLengthFromArchive
 * Usage: int sourceFileLength = getLengthFromArchive(archivedFile);
 * --------------------------------------------------------------------------------
 *
 * This function read length of the source file writed in the archve file.
 * It read some first characters of the input file stream while not
 * meeted "{" symbol. Then convert it to integer.
 *
 * @param archivedFile Input file stream with opened archive file.
 * @return Integer length of the source file.
 */
int getLengthFromArchive(ifstream &archivedFile){
    string result = "";
    unsigned char currentCh = archivedFile.get();
    while ((int)currentCh != 123) {
        result += currentCh;
        currentCh = archivedFile.get();
    }
    return stoi(result);
}

/**
 * Function: getCodeFromFile
 * Usage: string codeString = getCodeFromFile(archivedFile);
 * -----------------------------------------------------------------------------
 * This function read coding table from the input file stream. It read some
 * first characters of the input file stream while not meeted "}}" symbols.
 *
 * @param archivedFile Input file stream with opened archive file.
 * @return String with coding table.
 */
string getCodeFromFile(ifstream &archivedFile, int sourceFileLength){
    string result = "";

    archivedFile.seekg(0, archivedFile.end);
    int length = archivedFile.tellg();
    archivedFile.seekg(0, archivedFile.beg);

    int skip = to_string(sourceFileLength).size() + 1;
    for (int i = 0; i < skip; i++){
        archivedFile.get();
    }

    unsigned char currentCh = archivedFile.get();
    unsigned char nextCh = archivedFile.get();
    for(int i = 0; i < length - 2; i++){
        if ((int)currentCh == 125 && (int)nextCh == 125){
            break;
        }
        result += currentCh;
        currentCh = nextCh;
        nextCh = archivedFile.get();
    }

    return result;
}

/**
 * Function: parseCodeString
 * Usage:int *alphFromFile = parseCodeString(codeString);
 * -----------------------------------------------------------------------------------
 *
 * This function parsing coding table and save it to array of the frequensies of the characters
 * in the source file.
 * @param codeString Coding table in the string format.
 * @return array with the frequencies of the characters.
 */
int* parseCodeString(string codeString){
    int* result = new int[BYTES_NUMBER];
    for(int i = 0; i < BYTES_NUMBER; i++){
        result[i] = 0;
    }

    bool keyTrig = true; // trigger to separate character
    bool valueTrig = false; // trigger to separate frequensy
    char key;

    string valueString;
    for (int i = 0; i < codeString.length(); i++){
        if (keyTrig){ //write key
            key = codeString[i];
            keyTrig = false; // switch to reading value
            valueTrig = true;
            continue;
        }

        if(valueTrig && codeString[i] != ';'){ //read frequensy until met with the separator ";"
            valueString += codeString[i];
            continue;
        }

        if(!keyTrig && codeString[i] == ';'){

            result[(int)(unsigned char)key] = stoi(valueString); // write frequensy to array
            keyTrig = true; // reset triggers to start again readint next key
            valueTrig = false;
            valueString.clear();
            continue;
        }
    }
    return result;
}

/**
 * Function: getBodyFromFile
 * Usage: string inputFileBody = getBodyFromFile(archivedFile, codeString.length() + 3 + integerToString(sourceFileLength).length());
 * --------------------------------------------------------------------------------------
 *
 * This function read body of the source file from the input archive file. It read whole file
 * and then delete the part of file from the begining to the end of coding table.
 * @param file Link to the input archive file stream.
 * @param delCh Number of characters to delete from the begining.
 * @return Body of the source file saved it the string.
 */
char* getBodyFromFile(ifstream &file, int delCh, int& bodySize){

    /* Determining the size of the file */
    file.seekg(0, file.end); // set cursor position to the end of file
    int length = file.tellg(); // tell it's position
    file.seekg(0, file.beg); // set cursor position to the begining of file

    /* Read entire file to char array*/
    char *buffer = new char[length];
    file.read(buffer, length);

    bodySize = length - delCh;

    int j = 0;
    char *result = new char[length - delCh];
    for (int i = delCh; i < length; i++){
        result[j] = buffer[i];
        j++;
    }
    return result;
}

/**
 * Function: writeDeArchFile
 * Usage: writeDeArchFile(resultName, inputFileBody, root, sourceFileLength);
 * -------------------------------------------------------------------------------------
 *
 * This function decoding the archive file and write the output result file. It receive
 * link to the coded body, binary tree for decoding and length of the source file. In the decoding
 * process it write output file with received name. The main idea of decoding is to go though
 * coded body in binary mode. When meeted "1", programm turns right in the binary tree, and left
 * if "0". Proceed this operation until not meeted character in binary tree. After this programm
 * writes it character to the result file and return to the root of the tree. It stop this
 * operation when length of the output file equals length of the source file.
 *
 * @param fileName Name of the output result file.
 * @param body Body of the source file in string format.
 * @param root Binary tree with characters for decoding.
 * @param sourceFileLength Length of the source file.
 */
void writeDeArchFile(string fileName, char* body, TreeNode *root, int sourceFileLength, int bodySize){
    string bitStr = "";

    for (int i = 0; i < bodySize; i++){
        bitStr += getBitsFromChar(body[i]);
    }

    ofstream result(fileName, ios::out | ios::binary);

    int chCounter = 0;
    TreeNode *node = root;

    for (unsigned int i = 0; i < bitStr.size(); i++) {
        if (bitStr[i] == '1') {
            node = node->right; // turn right if 1
        } else if (bitStr[i] == '0') {
            node = node->left; // turn left if 0
        }

        /* if character found add it to the result file and back to start of the tree*/
        if (node->isBusy){
            char byte = node->ch;
            result.write((char*)&byte, sizeof(byte));
            chCounter++;
            node = root;
            if(chCounter == sourceFileLength) break; //stop writing the result file if achieved length of the source file
        }


    }
    result.close();
}

/**
 * Function: getBitsFromChar
 * Usage: bitStr += getBitsFromChar(body[i]);
 * -----------------------------------------------
 * This function convert received byte in string
 * of '0' and '1'
 *
 * @param ch Received byte of char type
 * @return Byte in string representaion
 */
string getBitsFromChar(char ch){
    string result;
    for(int i = 7; i >= 0; i--){
        int bit = (ch >> i) & 1;
        result += to_string(bit);
    }
    return result;
}


/**
 * Function: clearTree
 * Usage: clearTree(tree);
 *
 * -------------------------------------------------
 * Frees memory allocated for Huffmans tree
 *
 * @param tree Pointer to tree node
 */
void clearTree(TreeNode *tree){
    if (tree->left != 0){
        clearTree(tree->left);
    }
    if (tree->right != 0){
        clearTree(tree->right);
    }
    delete tree;
}
