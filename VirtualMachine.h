#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int MEMSIZE = 64;

class VirtualMachine
{
private:
    char *registers;
    int pc;
    bool of, uf, cf, zf;
    char memory[MEMSIZE];
    string inputPath, inputFile;
    vector<string> program;
    string outputFile;

public:
    // Constructor
    // VirtualMachine();
    VirtualMachine(string, string);
    ~VirtualMachine();

    void PrintProgram();

    void PrintInstruction(const string &);
    void SplitLine(const string &, string &, string &, string &);
    void RunProgram();
    char *ConvertReg(const string &);
    bool IsDigit(const string &);
    void RunInstruction(const string &, const string &, const string &);
    string ConvertBinary(const char &ch);

    string OutputScreen();
    void ShowScreen();
    void UpdatePC();
    void UpdateFlag(const int &, const int &, const string &);
    void StoreResult(const string &);

    // Setter
    void SetReg(char *, const int &);
    void SetOF(bool);
    void SetUF(bool);
    void SetCF(bool);
    void SetZF(bool);
    void SetMem(const char *, const int &);

    // Getter
    char GetReg(const char *);
    char GetMem(const int &);

    // Operator functions
    void In(char *);
    void Out(const char *);
    void Mov(char *, const int &);
    void Mov(char *, const char *);
    void Mov(char *, const string &);
    void Add(char *, const int &);
    void Add(char *, const char *);
    void Sub(char *, const int &);
    void Sub(char *, const char *);
    void Mul(char *, const int &);
    void Mul(char *, const char *);
    void Div(char *, const int &);
    void Div(char *, const char *);
    void Inc(char *);
    void Dec(char *);
    void Rol(char *, const int &);
    void Ror(char *, const int &);
    void Shl(char *, const int &);
    void Shr(char *, const int &);
    void Load(char *, const int &);
    void Load(char *, const string &);
    void Store(char *, const int &);
    void Store(char *, const string &);
};