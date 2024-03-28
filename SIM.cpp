#include <iostream>
#include <string>

using namespace std;

// global constants
const string ORIGINAL = "original.txt";
const string COUT = "cout.txt";
const string COMPRESSED = "compressed.txt";
const string DOUT = "dout.txt";

void compress()
{
    /*
    Compresssion:
        1. create dictionary from most frequent instructions
        2. find most beneficial compression method for each instruction
        3. write compressed instruction to COUT file

        
    */
}

void decompress()
{

}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Please supply command line argument 1 for compression, 2 for decompression.\n";
        return 1;
    }

    int mode = stoi(argv[1]);
    if (mode == 1)
        compress();
    else if (mode == 2)
        decompress();
    else
    {
        cerr << "Inavlid mode. Use 1 for compression, 2 for decompression.\n";
        return 1;
    }

    return 0;
}