#include <bits/stdc++.h>
#include <fstream>
#include "pass2.hpp"
#define sz(x) int(x.size())
using namespace std;

void Compile(string inputFile)
{
    load_BLOCKS();
    load_OPTAB();
    PASS1(inputFile, "intermediate_file.txt");
    PASS2("intermediate_file.txt", "obj_program.txt", "assembly_listing.txt");
}

int main()
{
    string filename;
    cout << "Enter the input file name: ";
    cin >> filename;

    ifstream file(filename);

    if (!file)
    {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return 1;
    }

    Compile(filename);
    
    return 0;
}