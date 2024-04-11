#include "tables.hpp"

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <string>
#define sz(x) int(x.size())

using namespace std;

int Precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}
int ApplyOperation(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return -1;
}
int GetValue(string s)
{
    int i;
    stack<int> values;
    stack<char> ops;
    for (i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
            continue;
        else if (s[i] == '(')
        {
            ops.push(s[i]);
        }
        else if (isdigit(s[i]))
        {
            int val = 0;
            while (i < s.length() &&
                   isdigit(s[i]))
            {
                val = (val * 10) + (s[i] - '0');
                i++;
            }

            values.push(val);
            i--;
        }
        else if (s[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(ApplyOperation(val1, val2, op));
            }
            if (!ops.empty())
                ops.pop();
        }

        else
        {
            while (!ops.empty() && Precedence(ops.top()) >= Precedence(s[i]))
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(ApplyOperation(val1, val2, op));
            }
            ops.push(s[i]);
        }
    }
    while (!ops.empty())
    {
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(ApplyOperation(val1, val2, op));
    }
    return values.top();
}

string IntToHexString(int num, int x = 3)
{
    string hex = "0123456789ABCDEF";
    string result = "";
    for (int i = sizeof(num) * 2 - 1; i >= 0; i--)
    {
        int x = (num >> (i * 4)) & 0xF;
        result += hex[x];
    }
    return result.substr(x);
}

int StringHexToInt(string &s)
{
    int ans = stoi(s, 0, 16);
    return ans;
}

bool IsValidByteSequence(string &s)
{
    char c = s[0];
    if (sz(s) <= 3)
        return 0;

    for (int i = 2; i < sz(s) - 1; i++)
    {
        if (c == 'C' && (!(s[i] >= 0 && s[i] <= 255)))
            return 0;

        if (c == 'X' && (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F')))
            return 0;
    }
    if (c == 'X' && (sz(s) - 3) % 2 != 0)
        return 0;
    return 1;
}
bool IsCommentLine(string &s)
{
    if (sz(s) == 0)
        return 1;

    int i = 0;
    while (i < sz(s))
    {
        if (s[i] != ' ')
            break;
        i++;
    }
    if (s[i] == '.' || i == sz(s))
        return 1;
    return 0;
}
void Capitalize(string &s)
{
    for (auto &c : s)
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
}