#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

bool checkSyntax(string infix);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	string s;
	
	for (int i = 0; i < infix.size(); i++)
		if (infix[i] != ' ')
			s += infix[i];

	if (!checkSyntax(s))
		return 1;

	postfix = "";

	stack<char> operatorStack;

	for (int i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			postfix += s[i];
			break;
		case '(':
			operatorStack.push(s[i]);
			break;
		case ')':
			while (operatorStack.top() != '(')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
			break;
		case '&':
			while (!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '|')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(s[i]);
			break;
		case '|':
			while (!operatorStack.empty() && operatorStack.top() != '(')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(s[i]);
			break;
		case '!':
			operatorStack.push(s[i]);
			break;
		}
	}
	while (!operatorStack.empty())
	{
		postfix += operatorStack.top();
		operatorStack.pop();
	}

	stack<bool> operandStack;
	bool operand1;
	bool operand2;
	for (int j = 0; j < postfix.size(); j++)
	{
		if (isdigit(postfix[j]))
		{
			operandStack.push(values[postfix[j]-'0']);
		}
		else if (postfix[j] == '&' || postfix[j] == '|')
		{
			operand1 = operandStack.top();
			operandStack.pop();
			operand2 = operandStack.top();
			operandStack.pop();
			if (postfix[j] == '&')
				operandStack.push(operand1 && operand2);
			else
				operandStack.push(operand1 || operand2);
		}
		else if (postfix[j] == '!')
		{
			operand1 = operandStack.top();
			operandStack.pop();
			operandStack.push(!operand1);
		}
	}
	result = operandStack.top();
	return 0;
}

bool checkSyntax(string infix)
{
	bool numberFound = false;
	int openParen = 0;
	int closedParen = 0;

	string s;

	for (int i = 0; i < infix.size(); i++)
		if (infix[i] != ' ')
			s += infix[i];

	for (int i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if ((i == s.size() - 1) || (i + 1 < s.size() && (s[i + 1] == '&' || s[i + 1] == '|' || s[i + 1] == ')')))
				numberFound = true;
			else
				return false;
			break;
		case '&':
		case '|':
			if (!(numberFound && i + 1 < s.size() && (isdigit(s[i + 1]) || s[i + 1] == '(' || s[i + 1] == '!')))
				return false;
			break;
		case '!':
			if (!(i + 1 < s.size() && (s[i + 1] == '(' || s[i + 1] == '!' || isdigit(s[i + 1]))))
				return false;
			break;
		case '(':
			if (i + 1 < s.size() && (isdigit(s[i + 1]) || s[i + 1] == '!' || s[i + 1] == '('))
				openParen++;
			else
				return false;
			break;
		case ')':
			if ((i == s.size() - 1) || (i + 1 < s.size() && (s[i + 1] == '&' || s[i + 1] == '|' || s[i + 1] == ')')))
				closedParen++;
			else
				return false;
			break;
		default:
			return false;
		}
		if (closedParen > openParen)
			return false;
	}
	return (numberFound && openParen == closedParen);
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer = true;
	assert(evaluate("!(7)|(8)", ba, pf, answer) == 0 && pf == "7!8|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("7)|(8", ba, pf, answer) == 1);
	assert(evaluate("(!(!7)|(8))", ba, pf, answer) == 0 && pf == "7!!8|" && answer);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);

	ba[2] = true;
	ba[9] = false;
	assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" && answer);
	assert(evaluate("(3)", ba, pf, answer) == 0 && pf == "3" && !answer);
	assert(evaluate("2&(3)", ba, pf, answer) == 0 && pf == "23&" && !answer);
	assert(evaluate("0 & !9", ba, pf, answer) == 0 && pf == "09!&" && answer);
	assert(evaluate("!(7|8)", ba, pf, answer) == 0 && pf == "78|!" && !answer);
	assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" && answer);
	assert(evaluate("6|4&5", ba, pf, answer) == 0 && pf == "645&|" && answer);
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && pf == "1911&|9|!&91&9&!!!|" && answer);
	cout << "Passed all tests" << endl;
}