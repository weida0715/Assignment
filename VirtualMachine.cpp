#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(string folderPath, string fileName)
{
    registers = new char(7);
    for (int r = 0; r < 7; r++)
        registers[r] = 0;

    pc = 0;
    of = uf = cf = zf = false;

    for (int i = 0; i < MEMSIZE; i++)
        memory[i] = 0;

    inputPath = folderPath;
    inputFile = fileName;

    string instruction;
    ifstream asmFile(inputPath + inputFile);

    while (getline(asmFile, instruction))
    {
        if (instruction != "")
            program.push_back(instruction);
    }

    asmFile.close();
    this->program = program;

    outputFile = inputFile.substr(0, inputFile.find('.')) +
                 "_output.txt";
}

VirtualMachine::~VirtualMachine()
{
    delete[] registers;
}

void VirtualMachine::PrintProgram()
{
    for (auto instruction : program)
        cout << instruction << endl;
}

void VirtualMachine::PrintInstruction(const string &instruction)
{
    cout << instruction << endl;
}

void VirtualMachine::RunProgram()
{
    for (auto instruction : program)
    {
        cout << instruction << endl;

        string op, p1, p2;
        SplitLine(instruction, op, p1, p2);

        RunInstruction(op, p1, p2);
    }
}

char *VirtualMachine::ConvertReg(const string &r)
{
    int index = r[1] - '0';
    return (registers + index);
}

bool VirtualMachine::IsDigit(const string &str)
{
    for (auto ch : str)
    {
        if (!isdigit(ch) && ch != '-')
            return false;
    }

    return true;
}

void VirtualMachine::RunInstruction(const string &op, const string &p1, const string &p2)
{
    char *rdst, *rsrc;
    int value = 0;
    string addr = "";

    rdst = ConvertReg(p1);

    if (p2 != "")
    {
        if (p2[0] == '[')
            addr = p2.substr(1, p2.find(']') - 1);
        else if (IsDigit(p2))
            value = stoi(p2);
        else
            rsrc = ConvertReg(p2);
    }

    if (op == "IN")
        In(rdst);
    else if (op == "OUT")
        Out(rdst);
    else if (op == "MOV")
    {
        if (value != 0)
            Mov(rdst, value);
        else if (addr != "")
            Mov(rdst, addr);
        else
            Mov(rdst, rsrc);
    }
    else if (op == "ADD")
    {
        if (value != 0)
            Add(rdst, value);
        else
            Add(rdst, rsrc);
    }
    else if (op == "SUB")
    {
        if (value != 0)
            Sub(rdst, value);
        else
            Sub(rdst, rsrc);
    }
    else if (op == "MUL")
    {
        if (value != 0)
            Mul(rdst, value);
        else
            Mul(rdst, rsrc);
    }
    else if (op == "DIV")
    {
        if (value != 0)
            Div(rdst, value);
        else
            Div(rdst, rsrc);
    }
    else if (op == "INC")
    {
        if (value != 0)
            Add(rdst, value);
        else
            Add(rdst, rsrc);
    }
    else if (op == "DEC")
        Dec(rdst);
    else if (op == "ROL")
        Rol(rdst, value);
    else if (op == "ROR")
        Ror(rdst, value);
    else if (op == "SHL")
        Shl(rdst, value);
    else if (op == "SHR")
        Shr(rdst, value);
    else if (op == "LOAD")
    {
        if (value != 0)
            Load(rdst, value);
        else if (addr != "")
            Load(rdst, addr);
    }
    else if (op == "STORE")
    {
        if (value != 0)
            Store(rdst, value);
        else if (addr != "")
            Store(rdst, addr);
    }

    UpdatePC();
    ShowScreen();
}

void VirtualMachine::SplitLine(const string &instruction, string &op, string &p1, string &p2)
{
    string inst = instruction;

    op = inst.substr(0, inst.find(' '));
    inst.erase(0, inst.find(' ') + 1);

    if (inst.find(',') != string::npos)
    {
        p1 = inst.substr(0, inst.find(','));
        inst.erase(0, inst.find(',') + 2);

        p2 = inst.substr(0, sizeof(inst));
        inst.erase(0, sizeof(inst));
    }
    else
    {
        p1 = inst.substr(0, inst.find(' '));
        inst.erase(0, inst.find(' ') + 1);

        p2 = "";
    }
}

string VirtualMachine::ConvertBinary(const char &ch)
{
    string bin, revBin;
    int dec = static_cast<int>(ch);

    while (dec != 0)
    {
        revBin += ((dec % 2 == 0) ? "0" : "1");
        dec /= 2;
    }

    revBin += string(8 - revBin.length(), '0');
    reverse(revBin.begin(), revBin.end());
    bin = revBin;

    return bin;
}

string VirtualMachine::OutputScreen()
{
    stringstream output;
    output << showpoint;
    output << setw(9) << "Registers:";
    for (int i = 0; i < 7; i++)
        output << " " << setw(2) << setfill('0') << static_cast<int>(registers[i]);

    output << "#" << endl;

    output << setw(9) << left << setfill(' ') << "Flags";
    output << ": " << of << " " << uf << " "
           << cf << " " << zf << "#" << endl;

    output << setw(9) << left << setfill(' ') << "PC.";
    output << setprecision(1) << ": " << pc << endl;
    output << endl;

    output << "Memory:";
    for (int i = 0; i < MEMSIZE; i++)
    {
        if ((i) % 8 == 0)
            output << endl;
        output << setw(2) << setfill('0') << (int)memory[i] << " ";
    }

    output << endl;
    output << "#" << endl;
    output << endl;

    return output.str();
}

void VirtualMachine::ShowScreen()
{
    cout << OutputScreen();
}

void VirtualMachine::UpdatePC()
{
    pc++;
}

void VirtualMachine::UpdateFlag(const int &val1, const int &val2 = 0,
                                const string &op = "")
{
    int total = val1;
    char ch1 = static_cast<int>(val1), ch2 = static_cast<int>(val2);
    string bin1 = ConvertBinary(ch1), bin2 = ConvertBinary(ch2);

    if (op == "ADD")
        total += val2;
    else if (op == "SUB")
        total -= val2;
    else if (op == "MUL")
        total *= val2;
    else if (op == "DIV")
        total /= val2;

    if (total < -128)
        SetUF(true);
    if (total > 127)
        SetOF(true);
    if (total == 0)
        SetZF(true);
    if (val2 != 0)
    {
        for (int i = 0; i < 8; i++)
        {
            if ((bin1[i] == '1') && (bin2[i] == '1'))
                SetCF(true);
        }
    }
}

void VirtualMachine::StoreResult(const string &outputPath = "")
{
    string result = OutputScreen();
    ofstream resultFile;

    resultFile.open(outputPath + outputFile);
    resultFile << result;
    resultFile.close();
}

// Setter
void VirtualMachine::SetReg(char *rdst, const int &value)
{
    *rdst = static_cast<int>(value);
}

void VirtualMachine::SetOF(bool overflow)
{
    of = overflow;
}

void VirtualMachine::SetUF(bool underflow)
{
    uf = underflow;
}

void VirtualMachine::SetCF(bool carry)
{
    cf = carry;
}

void VirtualMachine::SetZF(bool zero)
{
    zf = zero;
}

void VirtualMachine::SetMem(const char *reg, const int &addr)
{
    memory[addr] = *reg;
}

// Getter
char VirtualMachine::GetReg(const char *rdst)
{
    return *rdst;
}

char VirtualMachine::GetMem(const int &index)
{
    return memory[index];
}

// Operator Functions
void VirtualMachine::In(char *rdst)
{
    int value = 0;
    cin >> value;
    UpdateFlag(value);
    SetReg(rdst, value);
}

void VirtualMachine::Out(const char *rsrc)
{
    int value = static_cast<int>(GetReg(rsrc));
    cout << value << endl;
}

void VirtualMachine::Mov(char *rdst, const int &value)
{
    UpdateFlag(value);
    SetReg(rdst, value);
}

void VirtualMachine::Mov(char *rdst, const char *rsrc)
{
    int value = static_cast<int>(*rsrc);
    UpdateFlag(value);
    SetReg(rdst, value);
}

void VirtualMachine::Mov(char *rdst, const string &index)
{
    int value = registers[stoi(index)];
    UpdateFlag(value);
    SetReg(rdst, value);
}

void VirtualMachine::Add(char *rdst, const int &value)
{
    int total = static_cast<int>(*rdst) + value;

    UpdateFlag(*rdst, value, "ADD");
    SetReg(rdst, total);
}

void VirtualMachine::Add(char *rdst, const char *rsrc)
{
    int total = static_cast<int>((*rdst) + (*rsrc));

    UpdateFlag(*rdst, *rsrc, "ADD");
    SetReg(rdst, total);
}

void VirtualMachine::Sub(char *rdst, const int &value)
{
    int total = static_cast<int>(*rdst) - value;

    UpdateFlag(*rdst, value, "SUB");
    SetReg(rdst, total);
}

void VirtualMachine::Sub(char *rdst, const char *rsrc)
{
    int total = static_cast<int>((*rdst) - (*rsrc));

    UpdateFlag(*rdst, *rsrc, "SUB");
    SetReg(rdst, total);
}

void VirtualMachine::Mul(char *rdst, const int &value)
{
    int total = static_cast<int>(*rdst) * value;

    UpdateFlag(*rdst, value, "MUL");
    SetReg(rdst, total);
}

void VirtualMachine::Mul(char *rdst, const char *rsrc)
{
    int total = static_cast<int>((*rdst) * (*rsrc));

    UpdateFlag(*rdst, *rsrc, "MUL");
    SetReg(rdst, total);
}

void VirtualMachine::Div(char *rdst, const int &value)
{
    int total = static_cast<int>(*rdst) / value;

    UpdateFlag(*rdst, value, "DIV");
    SetReg(rdst, total);
}

void VirtualMachine::Div(char *rdst, const char *rsrc)
{
    int total = static_cast<int>((*rdst) / (*rsrc));

    UpdateFlag(*rdst, *rsrc, "DIV");
    SetReg(rdst, total);
}

void VirtualMachine::Inc(char *rdst)
{
    int total = *rdst + 1;
    UpdateFlag(total);
    (*rdst)++;
}

void VirtualMachine::Dec(char *rdst)
{
    int total = *rdst - 1;
    UpdateFlag(total);
    (*rdst)--;
}

void VirtualMachine::Rol(char *rdst, const int &count)
{
    int pos1 = (*rdst) << count;
    int pos2 = (*rdst) >> (8 - count);
    int pos = pos1 | pos2;
    UpdateFlag(pos);
    SetReg(rdst, pos);
}

void VirtualMachine::Ror(char *rdst, const int &count)
{
    int pos1 = (*rdst) >> count;
    int pos2 = (*rdst) << (8 - count);
    int pos = pos1 | pos2;
    UpdateFlag(pos);
    SetReg(rdst, pos);
}

void VirtualMachine::Shl(char *rdst, const int &count)
{
    int pos = (*rdst) << count;
    UpdateFlag(pos);
    SetReg(rdst, pos);
}

void VirtualMachine::Shr(char *rdst, const int &count)
{
    int pos = (*rdst) >> count;
    UpdateFlag(pos);
    SetReg(rdst, pos);
}

void VirtualMachine::Load(char *rdst, const int &addr)
{
    char value = GetMem(addr);
    SetReg(rdst, value);
}

void VirtualMachine::Load(char *rdst, const string &index)
{
    int value = registers[stoi(index)];
    SetReg(rdst, value);
}

void VirtualMachine::Store(char *rsrc, const int &addr)
{
    SetMem(rsrc, addr);
}

void VirtualMachine::Store(char *rsrc, const string &index)
{
    int value = GetReg(rsrc);
    char *rdst = registers + stoi(index);
    SetReg(rdst, value);
}
