#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// class for representing Dictionary entries
struct entry {
    string code;
    int frequency;
    int order;
};

// global constants
const string ORIGINAL_FILEPATH = "original.txt";
const string COUT_FILEPATH = "cout.txt";
const string COMPRESSED_FILEPATH = "compressed.txt";
const string DOUT_FILEPATH = "dout.txt";

// global variables
bool mode; // 1 for compression, 2 for decompression
vector<string> original;
vector<string> compressedInstructions;
vector<int> compressionFormats;
vector<entry> dictionary;

// utility functions
string toBinaryString(int number, int minWidth)
{
    string binary = number == 0 ? "0" : "";
    while (number > 0) {
        binary = to_string(number % 2) + binary;
        number /= 2;
    }

    while (binary.length() < static_cast<size_t>(minWidth)) {
        binary = "0" + binary;
    }

    return binary;
}

int findDictionaryIndex(int i)
{
    for (int n = 0; n < dictionary.size(); n++)
    {
        if (original[i] == dictionary[n].code)
            return n;
    }
}

/*
 * Compresssion:
 *     1. create dictionary from most frequent instructions
 *     2. find most beneficial compression method for each instruction
 *     3. write compressed instruction to COUT file
 * 
 * Decompression:
 *     1. determine encoding format from first 3 bits
 *     2. determine decompressed code
 *     3. write decompressed intruction to DOUT file
 */

void constructDictionary()
{
    // parse input file into a structured list container
    ifstream file(ORIGINAL_FILEPATH);
    if (!file.is_open())
        cerr << "Failed to open file: " << ORIGINAL_FILEPATH << endl;
    
    string line;
    while (getline(file, line))
        original.push_back(line);

    file.close();

    // create dictionary from most frequent instructions
    int count = 0;
    for (const auto& line : original)
    {
        count++;

        // find if code has appeared already
        int foundEntryIndex = -1;
        for (int i = 0; i < dictionary.size(); i++)
        {
            if (dictionary[i].code == line)
            {
                foundEntryIndex = i;
                break;
            }
        }

        // add entry to dictionary if it doesn't already exist
        if (foundEntryIndex == -1)
        {
            entry e;
            e.code = line;
            e.frequency = 1;
            e.order = count;
            dictionary.push_back(e);
        }
        else // update entry if it already exists
        {
            dictionary[foundEntryIndex].frequency++;
        }
    }

    // sort elements by order then frequency so that 
    // original order is preserved for entries with the same frequency
    stable_sort(dictionary.begin(), dictionary.end(), [](const auto& a, const auto& b) {
        if (a.frequency != b.frequency)
            return a.frequency > b.frequency;
        return a.order < b.order;
    });

    // resive dictionary to maximum 16 entries
    if (dictionary.size() > 16)
        dictionary.resize(16);

    // =========================== PRINT DEBUG ======================
    for (auto entry : dictionary)
        cout << entry.code << "\n";
}

bool checkRLE(int i)
{
    // if in existing RLE occurrence
    if (i >= 1 && compressionFormats[i - 1] == 9)
        return true;

    // if not in existing RLE occurrence
    bool compresses = false;
    for (int RLE_index = 1; RLE_index < 8; RLE_index++)
    {
        if (original[i] == original[i + RLE_index])
        {
            compresses = true;
            if (i >= 1 && compressionFormats[i - 1] == 1)
                compressionFormats.push_back(1);
            compressionFormats.push_back(9);
        }
    }
    return compresses;
}


bool checkDirectMatching(int i)
{
    for (int i = 0; i < dictionary.size(); i++)
    {
        if (original[i] == dictionary[i].code)
        {
            compressionFormats.push_back(7);
            return true;
        }
    }
    return false;
}

void compressRLE(int i)
{

}

void compressDirectMatching(int i)
{
    string output;
    output.append("111 ");
    output.append(toBinaryString(findDictionaryIndex(i), 4));
    compressedInstructions.push_back(output);
}

void compress()
{
    // find best compression format
    for (int i = 0; i < original.size(); i++)
    {
        if (checkRLE(i))
            break;
        if (checkDirectMatching(i))
            break;


        int bestFormat;
    }

    // compress
    for (int i = 0; i < compressionFormats.size(); i++)
    {
        if (compressionFormats[i] == 7)
        {
            compressDirectMatching(i);
        }
        else if (compressionFormats[i] == 9)
        {
            compressRLE(i);
        }
    }

    // write compressed instruction to COUT file
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

    mode = stoi(argv[1]);
    if (mode == 1)
    {
        constructDictionary();
        compress();
    }
    else if (mode == 2)
        decompress();
    else
    {
        cerr << "Inavlid mode. Use 1 for compression, 2 for decompression.\n";
        return 1;
    }

    return 0;
}