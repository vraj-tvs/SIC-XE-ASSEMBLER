#include "pass1.hpp"
#include <bits/stdc++.h>
#define sz(x) int(x.size())

using namespace std;

void PrintLineV2(int l1 = LOCCTR, int l2 = current_block_number, string l3 = label, string l4 = opcode, string l5 = operand, string l6 = "")
{

    if (l3 == "-")
        l3 = "";

    if (l5 == "-")
        l5 = "";

    if (l1 != -1)
        fout2 << Modified(IntToHexString(l1)) << "\t" << l2 << "\t" << Modified(l3) << "\t" << Modified(l4) << "\t" << Modified(l5, ' ', 40) << "\t" << l6 << endl;
    else
        fout2 << Modified("") << "\t" << l2 << "\t" << Modified(l3) << "\t" << Modified(l4) << "\t" << Modified(l5, ' ', 40) << l6 << endl;
}
void InitializeSegmentsV2(string &s)
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

    bool f = 1;
    for (auto temp : x[0])
        if (!(temp >= '0' && temp <= '9' || temp >= 'A' && temp <= 'F'))
            f = 0;

    if (f == 1)
        LOCCTR = StringHexToInt(x[0]);
    else
        LOCCTR = -1;
    current_block_number = stoi(x[1]);
    label = x[2];
    opcode = x[3];
    operand = "";
    for (i = 4; i < sz(x); i++)
        operand += x[i];
}
pair<int, int> AreValidRegisters(string s)
{
    string r1 = "", r2 = "";

    r1 += s[0];
    if (REGTAB.find(r1) == REGTAB.end())
        return {-1, -1};

    if (sz(s) >= 3)
    {
        r2 += s[2];
        if (REGTAB.find(r2) == REGTAB.end())
            return {-1, -1};
    }
    else
        r2 = "A";

    return {REGTAB[r1], REGTAB[r2]};
}
string ValidateMemReferences(string s)
{
    char c = s[0];
    // debug(c);
    if (c == '#' || c == '@')
    {
        s = s.substr(1);
        // debug(s);
        if (SYMTAB.find(s) != SYMTAB.end())
        {
            if (c == '#')
                return s + "I";
            return s + "D";
        }
        return "-1";
    }
    else
    {
        int comma = 0;
        int len = sz(s);
        for (; comma < len; comma++)
            if (s[comma] == ',')
                break;
        if (comma == len)
            if (SYMTAB.find(s) != SYMTAB.end())
                return s + "S";
            else
                return "-1";

        string s1 = s.substr(0, comma);
        string s2 = s.substr(comma + 1);
        if (SYMTAB.find(s1) != SYMTAB.end())
            if (s2 == "X")
                return s1 + "X";
        return "-1";
    }
}
string ValidateLiteralReferences(string s)
{
    char c = s[0];
    if (c != '=')
        return "-1";

    if(sz(s)<=1)
        return "-1";

    if (s[0] == '=' && s[1] == '*' && sz(s)==2)
        return "*";

    s = s.substr(1);
    c = s[0];
    if (sz(s) <= 3)
        return "-1";
    if (!(s[1] == '\'' && s.back() == '\''))
        return "-1";
    if (!(s[0] == 'X' || s[0] == 'C'))
        return "-1";

    if (c == 'X')
    {
        for (int i = 2; i < sz(s) - 1; i++)
            if (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F'))
                return "-1";
    }

    return s;
}
string ValidateImmUsage(string s)
{
    char c = s[0];
    if (c == '#')
    {
        bool f = 1;
        for (int i = 1; i < sz(s); i++)
            if (!(s[i] >= '0' && s[i] <= '9'))
                f = 0;

        if (f)
            return s.substr(1);
    }
    return "-1";
}


/*PASS2*/
void PASS2(string inter, string out, string out2)
{
    string h, e;
    TEXT_RECORD t;
    vector<string> m;

    fin.open(inter);
    if (!fin.is_open())
    {
        cout << "Error opening the file:" << inter << endl;
        return;
    }

    fout.open(out);
    if (!fout.is_open())
    {
        cout << "Error opening the file:" << out << endl;
        return;
    }

    fout2.open(out2);
    if (!fout2.is_open())
    {
        cout << "Error opening the file:" << out2 << endl;
        return;
    }

    line_number=1;
    error+="\nPASS2 Errors:\n";
    getline(fin, line);
    InitializeSegmentsV2(line);
    int start = LOCCTR;
    if (opcode == "START")
        h = "H^" + Modified(label, '_') + "^" + IntToHexString(StringHexToInt(operand), 2) + "^" + IntToHexString(program_length - start, 2);
    PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, "");
    line_number++;start=-1;
    while (getline(fin, line))
    {
        InitializeSegmentsV2(line);

        if(start==-1&&OPTAB.find(opcode)!=OPTAB.end())
            start=LOCCTR;
        
        if (OPTAB.find(opcode) != OPTAB.end())
        {
            string temp = "";
            if (OPTAB[opcode].format == 1)
                temp = OPTAB[opcode].opcode;
            else if (OPTAB[opcode].format == 2)
            {
                temp = OPTAB[opcode].opcode;
                auto check = AreValidRegisters(operand);
                if (check == make_pair(-1, -1))
                    error+="Line Number "+to_string(line_number)+": Invalid Registers\n";
                string s1 = to_string(check.first);
                string s2 = to_string(check.second);
                temp += s1 + s2;
            }
            else if (OPTAB[opcode].format == 3)
            {
                if (opcode == "RSUB")
                    temp = "4F0000";

                else
                {
                    auto check1 = ValidateMemReferences(operand);
                    auto check2 = ValidateImmUsage(operand);
                    auto check3 = ValidateLiteralReferences(operand);

                    if (check1 != "-1")
                    {
                        char c = check1.back();
                        int len = sz(check1);
                        int n, i, x = 0, e = 0, p, b;
                        if (c == 'S')
                            n = 1, i = 1;
                        else if (c == 'I')
                            n = 0, i = 1;
                        else if (c == 'D')
                            n = 1, i = 0;
                        else
                            n = 1, i = 1, x = 1;

                        string f = check1.substr(0, len - 1);
                        int location = SYMTAB[f].address + BLOCKTAB[block_mapping[SYMTAB[f].block].second].start;
                        int disp = location - (LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + OPTAB[opcode].format);
                        if (disp >= -2048 && disp <= 2047)
                            p = 1, b = 0;
                        else
                        {
                            if (!used_BASE)
                                error+="Line Number "+to_string(line_number)+": BASE is not used when required\n";
                            else
                            {
                                disp = disp + LOCCTR - base + OPTAB[opcode].format;
                                if (disp >= 0 && disp <= 4095)
                                    b = 1, p = 0;
                                else
                                    error+="Line Number "+to_string(line_number)+": Invalid Displacement\n";
                            }
                        }
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(disp, 5);
                    }
                    else if (check2 != "-1")
                    {
                        int n = 0, i = 1, x = 0, b = 0, p = 0, e = 0;
                        int disp = stoi(check2);

                        if(!(disp>=-2048&&disp<=2047))
                            error+="Line Number "+to_string(line_number)+": Displacement field for Format 3 is not in the range\n";
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(disp, 5);
                    }
                    else if (check3 != "-1")
                    {
                        int n = 1, i = 1, x = 0, e = 0, p, b;
                        int disp;
                        if (check3[0] == '*')
                            check3 += IntToHexString(LOCCTR);

                        string f = check3;
                        int location = *((LITTAB[f].address).lower_bound(LOCCTR)) + BLOCKTAB[block_mapping[LITTAB[f].blocknumber].second].start;
                        disp = location - (LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + OPTAB[opcode].format);
                        if (disp >= -2048 && disp <= 2047)
                            p = 1, b = 0;
                        else
                        {
                            if (!used_BASE)
                                error+="Line Number "+to_string(line_number)+": BASE is not used when required\n";
                            else
                            {
                                disp = disp + LOCCTR - base + OPTAB[opcode].format;
                                if (disp >= 0 && disp <= 4095)
                                    b = 1, p = 0;
                                else
                                    error+="Line Number "+to_string(line_number)+": Invalid Displacement\n";
                            }
                        }
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(disp, 5);
                    }

                    else
                       error+="Line Number "+to_string(line_number)+": Invalid operand for format 3\n";
                }
            }

            else
            {
                auto check1 = ValidateMemReferences(operand);
                auto check2 = ValidateImmUsage(operand);
                auto check3 = ValidateLiteralReferences(operand);
                if (check1 != "-1")
                {
                    char c = check1.back();
                    int len = sz(check1);
                    int n, i, x = 0, e = 1, p = 0, b = 0;
                    if (c == 'S')
                        n = 1, i = 1;
                    else if (c == 'I')
                        n = 0, i = 1;
                    else if (c == 'D')
                        n = 1, i = 0;
                    else
                        n = 1, i = 1, x = 1;
                    string f = check1.substr(0, len - 1);
                    int value = SYMTAB[f].address;
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;

                    if (SYMTAB[f].relative == 0)
                        temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(value);
                    else
                    {
                        temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(value + BLOCKTAB[block_mapping[SYMTAB[f].block].second].start);
                        m.push_back("M^" + IntToHexString(LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + 1, 2) + "^05");
                    }
                }
                else if (check2 != "-1")
                {
                    int n = 0, i = 1, x = 0, b = 0, p = 0, e = 1;
                    int disp = stoi(check2);
                    if(!(disp>=0&&disp<(1<<20)))
                            error+="Line Number "+to_string(line_number)+": Address field for Format 4 is not in the range\n";
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;
                    temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(disp, 3);
                }

                else if (check3 != "-1")
                {
                    int n = 1, i = 1, x = 0, e = 1, p, b;
                    if (check3 == "*")
                        check3 += to_string(LOCCTR);

                    string f = check3;
                    int location = *((LITTAB[f].address).lower_bound(LOCCTR)) + BLOCKTAB[block_mapping[LITTAB[f].blocknumber].second].start;
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = StringHexToInt(OPTAB[opcode].opcode) + 2 * n + i;
                    temp = IntToHexString(first_plus_second, 6) + IntToHexString(third, 7) + IntToHexString(location, 3);
                    m.push_back("M^" + IntToHexString(LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + 1, 2) + "^05");
                }
            }
            t.Insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "RESB" || opcode == "RESW")
        {
            t.BreakTextRecords();
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "WORD")
        {
            string temp = IntToHexString(stoi(operand), 2);
            t.Insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "BYTE")
        {
            char c = operand[0];
            string temp;
            if (c == 'X')
                temp = operand.substr(2, sz(operand) - 3);
            else if (c == 'C')
            {
                string temp2 = operand.substr(2, sz(operand) - 3);
                for (auto &c : temp2)
                {
                    int x = c;
                    temp += IntToHexString(x, 6);
                }
            }
            t.Insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "LTORG")
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, "");

        else if (opcode == "EQU")
        {
            int val = EvaluateExpression(operand, 1);
            if(val!=-1){
            SYMTAB[label].address = val;
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, "");
            }
        }

        else if (opcode == "ORG" || opcode == "USE")
        {
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, "");
            t.BreakTextRecords();
        }

        else if (opcode == "BASE")
        {
            used_BASE = 1;
            base = SYMTAB[operand].address;
            PrintLineV2(-1, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "NOBASE")
        {
            used_BASE = 0;
            PrintLineV2(-1, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "END")
            PrintLineV2(-1, current_block_number, label, opcode, operand, "");

        else if (opcode == "*")
        {
            int cnt;
            string temp;
            if(operand[0]=='C')
                cnt = 2*sz(operand) - 6;
            if (operand[0] == 'X')
                cnt = sz(operand) - 3;
            if(operand[0] == '*'){
                int i;
                for(i=1;i<sz(operand);i++)
                    if(operand[i]!='0')
                    break;
                cnt=max(1,(6-i)/2);
                if(cnt%2!=0)
                cnt++;
            }
            temp = IntToHexString(EvalByteSequence(operand), 8 - cnt);
            PrintLineV2(LOCCTR, current_block_number, label, opcode, operand, temp);
            t.Insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
        }
        line_number++;
    }
    e = "E^" + IntToHexString(start, 2);
    fout << h << endl;
    t.PrintTextRecords(fout);
    for (auto &md : m)
        fout << md << endl;
    fout << e << endl;

    fout2<<"\n/**********************************************************************************/\nERRORS in the SIC/XE Program:\n"<<endl;
    fout2<<error<<endl;
}