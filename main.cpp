#include <iostream>
#include "VirtualMachine.h"
using namespace std;

// Function Prototypes
void startProgram(const string &, const string &);
void endProgram();

// Main Function
int main()
{
    string folderPath = "test/";

    // Test Programs
    string testFiles[] = {
        "test_io.asm",
        "test_mov.asm",
        "test_arithmetic.asm",
        "test_inc_dec.asm",
        "test_rotate_shift.asm",
        "test_load_store.asm",
        "test.asm",
        "big_test.asm"};

    // Start Program
    for (auto test : testFiles)
    {
        startProgram(folderPath, test);
        endProgram();
    }

    // End Program

    return 0;
}

// Function Definitions
void startProgram(const string &folderPath, const string &filename)
{
    VirtualMachine env(folderPath, filename);
    cout << "----- " << filename << " start -----" << endl;
    env.RunProgram();
    cout << "----- " << filename << " result -----" << endl;
    env.ShowScreen();
    env.StoreResult("output/");
    cout << "----- " << filename << " end -----" << endl;
    cout << endl;
}

void endProgram()
{
    system("pause");
}
