#include <bits/stdc++.h>
#include "utility.hpp"
#define sz(x) int(x.size())
using namespace std;

ifstream fin;
ofstream fout, fout2;
string label, opcode, operand, line, current_block = "DEFAULT",error="";
int line_number, LOCCTR, save_LOCCTR, current_block_number, current_number_of_blocks = 1, program_length;
vector<pair<int, string>> block_mapping;

int EvaluateExpression(string s, bool chk = 0)
{
    if (s == "*")
        return LOCCTR;

    vector<string> s1;
    vector<char> s2;

    if (sz(s) <= 1)
        return -1;

    if (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
    {
        s2.push_back(s[0]);
        s = s.substr(1);
    }

    int n = sz(s);
    string operand = "";
    for (int i = 0; i < n; i++)
    {
        if (!(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'))
        {
            operand += s[i];
        }
        else
        {
            s1.push_back(operand);
            operand = "";

            s2.push_back(s[i]);
        }
    }
    if (int(operand.size()))
        s1.push_back(operand);
    int value;
    if (sz(s1) == sz(s2))
    {
        int plus = 0, minus = 0;
        if (SYMTAB.find(s1[0]) != SYMTAB.end() && SYMTAB[s1[0]].relative == 1)
            if (s2[0] == '+')
                plus++;
            else if (s2[0] == '-')
                minus++;
            else
                return -1;
        for (int i = 1; i < sz(s2); i++)
        {
            if (s2[i] == '+')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1)
                    plus++;
            }
            else if (s2[i] == '-')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1)
                    minus++;
            }
            else if (s2[i] == '*' || s2[i] == '/')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1 || SYMTAB.find(s1[i - 1]) != SYMTAB.end() && SYMTAB[s1[i - 1]].relative == 1)
                {
                    return -1;
                }
            }
        }
        if (plus - minus >= 2 || plus - minus < 0)
            return -1;
        if (plus - minus == 0)
            SYMTAB[label].relative = 0;
        vector<string> x;
        for (int i = 0; i < sz(s1); i++)
        {
            string ans;
            if (SYMTAB.find(s1[i]) != SYMTAB.end())
            {
                int temp = SYMTAB[s1[i]].address;
                if (chk == 1)
                    temp += BLOCKTAB[block_mapping[SYMTAB[s1[i]].block].second].start;
                ans = to_string(temp);
            }
            else
                ans = s1[i];
            x.push_back(ans);
        }
        string f = "0";
        for (int i = 0; i < sz(x); i++)
            f += s2[i] + x[i];
        value = GetValue(f);
    }
    else
    {
        int plus = 0, minus = 0;
        if (SYMTAB.find(s1[0]) != SYMTAB.end() && SYMTAB[s1[0]].relative == 1)
            plus++;
        for (int i = 0; i < sz(s2); i++)
        {
            if (s2[i] == '+')
            {
                if (SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                    plus++;
            }

            else if (s2[i] == '-')
            {
                if (SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                    minus++;
            }

            else if (s2[i] == '*' || s2[i] == '/')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1 || SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                {
                    return -1;
                }
            }
        }

        if (plus - minus >= 2 || plus - minus < 0)
            return -1;

        if (plus == minus)
            SYMTAB[label].relative = 0;

        vector<string> x;
        for (int i = 0; i < sz(s1); i++)
        {
            string ans;
            if (SYMTAB.find(s1[i]) != SYMTAB.end())
            {
                int temp = SYMTAB[s1[i]].address;
                if (chk == 1)
                    temp += BLOCKTAB[block_mapping[SYMTAB[s1[i]].block].second].start;
                ans = to_string(temp);
            }
            else
                ans = s1[i];
            x.push_back(ans);
        }

        string f = "";
        for (int i = 0; i < sz(s2); i++)
            f += x[i] + s2[i];
        f += x.back();
        value = GetValue(f);
    }
    return value;
}
int EvalByteSequence(string &s)
{
    if (s == "*")
        return LOCCTR;
    char c = s[0];
    int value = 0;
    int len = sz(s);
    if (c == 'C')
    {
        int x = 1;
        for (int i = len - 2; i >= 2; i--)
        {
            value += int(s[i]) * x;
            x *= 256;
        }
    }
    else if (c == 'X')
    {
        int x = 1;
        for (int i = len - 2; i >= 2; i--)
        {
            int current = (s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : s[i] - 'A';
            value += current * x;
            x *= 16;
        }
    }
    return value;
}
string Modified(string s, char c = ' ', int x = 6)
{
    int len = sz(s);
    string temp = s + string(x - len, c);
    return temp;
}
void PrintLine(int l1 = LOCCTR, int l2 = current_block_number, string l3 = label, string l4 = opcode, string l5 = operand)
{

    if (l3 == "")
        l3 = "-";

    if (l5 == "")
        l5 = "-";

    if (l1 != -1)
        fout<<Modified(IntToHexString(l1)) << " " << l2 << " " << Modified(l3) << " " << Modified(l4) << " " << l5 << endl;
    else
        fout<<Modified("-") << " " << l2 << " " << Modified(l3) << " " << Modified(l4) << " " << l5 << endl;
}
void InitializeSegments(string &s)
{
    string temp = "";
    operand = "", label = "", opcode = "";
    int len = sz(s), i = 0;
    vector<string> x;
    while (i < len)
    {
        if (s[i] != ' ')
            temp += s[i];
        else if (sz(temp))
        {
            x.push_back(temp);
            temp = "";
        }
        i++;
    }
    if (temp != "")
        x.push_back(temp);
    for (auto &sx : x)
        Capitalize(sx);

    if (OPTAB.find(x[0]) != OPTAB.end() || ASMDIR.find(x[0]) != ASMDIR.end())
    {
        opcode = x[0];
        string p = "";
        for (int i = 1; i < sz(x); i++)
            p += x[i] + " ";
        if (p != "")
        {
            p.pop_back();
            operand = p;
        }
        else
            operand = "";
        label = "";
    }
    else
    {
        label = x[0];
        if (OPTAB.find(x[1]) != OPTAB.end() || ASMDIR.find(x[1]) != ASMDIR.end())
        {
            opcode = x[1];
            string p = "";
            for (int i = 2; i < sz(x); i++)
                p += x[i] + " ";
            p.pop_back();
            operand = p;
        } 
        else
        {
            error+="Line Number "+to_string(line_number)+": Invalid Instruction\n";
        }
    }
}
bool CheckLiteralFormat(string &s)
{
    if(s=="=*")
        return 1;

    char c1 = s[0], c2 = s[1], c3 = s[2], c4 = s.back();

    if (c1 == '=' && (c2 == 'C' || c2 == 'X') && c3 == '\'' && c4 == '\'')
        return 1;
    return 0;
}
void HandleLTORG()
{
    for (auto &literal : LITTAB)
    {
        if (literal.second.unassigned == 0)
            continue;

        literal.second.address.insert(LOCCTR);
        literal.second.blocknumber = current_block_number;
        literal.second.unassigned = 0;

        PrintLine(LOCCTR, current_block_number, "", "*", literal.first);
        LOCCTR += literal.second.length;
        line_number++;
    }
}
void ModifyBLKTAB()
{
    for (auto block : BLOCKTAB)
        block_mapping.push_back({block.second.id, block.first});
    sort(block_mapping.begin(), block_mapping.end());

    for (auto &x : block_mapping)
    {
        if (x.first == 0)
            continue;

        BLOCKTAB[x.second].start = BLOCKTAB[block_mapping[x.first - 1].second].start + BLOCKTAB[block_mapping[x.first - 1].second].length;
    }

    program_length = BLOCKTAB[(block_mapping.back()).second].start + BLOCKTAB[(block_mapping.back()).second].length;
}
int GetLengthForLOCCTR()
{
    string x = IntToHexString(LOCCTR);
    reverse(x.begin(),x.end());
    int len = sz(x);
    int i;
    for (i = 0; i < len; i++)
    {
        if (x[i] == '0')
            break;
    }
    return (i + 2) / 2;
}
class TEXT_RECORD
{
    vector<string> v;
    int curr_len, curr_start;
    string curr_string;

public:
    TEXT_RECORD()
    {
        curr_len = 0;
        curr_start = -1;
        curr_string = "";
    }

    void Insert(string s, int address)
    {
        if (curr_len + int(s.size()) <= 60)
        {
            curr_len += int(s.size());
            curr_string += s + "^";

            if (curr_start == -1)
                curr_start = address;
        }
        else
        {
            v.push_back("T^" + IntToHexString(curr_start, 2) + "^" + IntToHexString(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
            curr_len = int(s.size());
            curr_string = s + "^";
            curr_start = address;
        }
    }
    void BreakTextRecords()
    {
        if (curr_len == 0)
            return;

        if (int(curr_string.size()))
        {
            v.push_back("T^" + IntToHexString(curr_start, 2) + "^" + IntToHexString(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
            curr_len = 0;
            curr_start = -1;
            curr_string = "";
        }
    }
    void PrintTextRecords(ofstream &fout)
    {
        if (curr_len)
        {
            v.push_back("T^" + IntToHexString(curr_start, 2) + "^" + IntToHexString(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
        }

        for (auto &s : v)
            fout << s << endl;
    }
};

/*PASS1*/
void PASS1(string ifile, string ofile)
{

    fin.open(ifile);
    if (!fin.is_open())
    {
        cout << "Error opening the file:" << ifile << endl;
        return;
    }

    fout.open(ofile);
    if (!fout.is_open())
    {
        cout << "Error opening the file:" << ofile << endl;
        return;
    }

    line_number=1;
    getline(fin, line);
    while (IsCommentLine(line))
        getline(fin, line);

    error+="PASS1 Errors:\n";
    InitializeSegments(line);
    if (opcode == "START")
    {
        int start_address = StringHexToInt(operand);
        LOCCTR = start_address;
    }
    PrintLine();

    line_number++;
    while (opcode != "END")
    {
        getline(fin, line);
        if (IsCommentLine(line))
            continue;

        InitializeSegments(line);
        if (sz(label) != 0)
        {
            if (SYMTAB.find(label) != SYMTAB.end())
                SYMTAB[label].error = 1;
            else
                SYMTAB[label].error = 0;
            SYMTAB[label].block = current_block_number;
            SYMTAB[label].address = LOCCTR;
        }

        if (opcode == "LTORG" || opcode == "ORG" || opcode == "EQU" || opcode == "USE" || opcode == "BASE" || opcode == "NOBASE")
            PrintLine(-1);
        else
            PrintLine();

        if (CheckLiteralFormat(operand))
        {
            string name = operand.substr(1);
            int len;
            if (!(IsValidByteSequence(name)||name=="*"))
                error+="Line Number "+to_string(line_number)+": Invalid Byte-Sequence\n";
            else if (name[0] == 'X')
                len = (sz(name) - 3) / 2;
            else if (name[0] == 'C')
                len = sz(name) - 3;
            else if (name == "*")
            {
                len = GetLengthForLOCCTR();
                name += IntToHexString(LOCCTR);
            }
            if (LITTAB.find(name) != LITTAB.end() && LITTAB[name].blocknumber == current_block_number)
                LITTAB[name].error = 1;
            else
                LITTAB[name].error = 0;
            LITTAB[name].length = len;
            LITTAB[name].unassigned = 1;
            LITTAB[name].operand_value = EvalByteSequence(name);
            LITTAB[name].blocknumber = -1;
        }

        if (opcode == "RESB")
            LOCCTR += stoi(operand);
        else if (opcode == "RESW")
            LOCCTR += 3 * stoi(operand);
        else if (opcode == "WORD")
            LOCCTR += 3;
        else if (opcode == "BYTE")
        {
            char c = operand[0];
            if (!IsValidByteSequence(operand))
                error+="Line Number "+to_string(line_number)+": Invalid Byte-Sequence\n";
            else if (c == 'X')
                LOCCTR += (sz(operand) - 3) / 2;
            else if (c == 'C')
                LOCCTR += sz(operand) - 3;
        }

        else if (OPTAB.find(opcode) != OPTAB.end())
            LOCCTR += OPTAB[opcode].format;

        else if (opcode == "LTORG")
            HandleLTORG();
        else if (opcode == "EQU")
        {
            int val = EvaluateExpression(operand, 0);
            if (val == -1)
                error+="Line Number "+to_string(line_number)+": EQU is used, but all required labels were not found.\n";
            
            else
                SYMTAB[label].address = val;
        }
        else if (opcode == "ORG")
        {
            if (sz(operand) != 0)
            {
                int val = EvaluateExpression(operand);
                if (val == -1)
                    error+="Line Number "+to_string(line_number)+": EQU is used, but all required labels were not found.\n";
                else
                {
                    save_LOCCTR = LOCCTR;
                    LOCCTR = val;
                }
            }
            else
                LOCCTR = save_LOCCTR;
        }
        else if (opcode == "USE")
        {
            BLOCKTAB[current_block].length = LOCCTR;
            if (sz(operand) == 0)
            {
                current_block = "DEFAULT";
                current_block_number = BLOCKTAB["DEFAULT"].id;
                LOCCTR = BLOCKTAB["DEFAULT"].length;
            }
            else
            {
                current_block = operand;
                if (BLOCKTAB.find(operand) == BLOCKTAB.end())
                {
                    current_block_number = current_number_of_blocks;
                    BLOCKTAB[operand].id = current_number_of_blocks++;
                    LOCCTR = 0;
                }
                else
                {
                    current_block_number = BLOCKTAB[operand].id;
                    LOCCTR = BLOCKTAB[operand].length;
                }
            }
        }
        line_number++;
    }
    HandleLTORG();
    BLOCKTAB[current_block].length = LOCCTR;
    ModifyBLKTAB();
    fin.close();
    fout.close();
}