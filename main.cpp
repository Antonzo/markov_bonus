#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::ifstream;
using std::getline;
using std::cout;
using std::cin;
using std::cerr;
using std::exit;
struct rule
{
	string pattern; // ����� ����� �����������
	string replacement; // ������ ����� �����������
	bool terminal; // �������� �� ����������� ������������
	rule(string pat, string rep, bool term) :
		pattern(pat),
		replacement(rep),
		terminal(term)
	{
	}
};

//��������� ��� ���������� ������� ������
string const whitespace = " \t";
string::size_type const npos = string::npos;

bool is_whitespace(char c)
{
	return whitespace.find(c) != npos;
}

vector<rule> read_rules(ifstream& rulefile)
{
	vector<rule> rules;
	string line;
	while (getline(rulefile, line))
	{
		string::size_type pos;

		// ������� ������ �����
		if (line.find_first_not_of(whitespace) == npos)
			continue;

		// ����� "->", ����������� ���������
		pos = line.find("->");
		while (pos != npos && (pos == 0 || !is_whitespace(line[pos - 1])))
			pos = line.find("->", pos + 1);

		if (pos == npos || line.length() < pos + 3 || !is_whitespace(line[pos + 2]))
		{
			cerr << "invalid rule: " << line << "\n";
			exit(EXIT_FAILURE);
		}

		string pattern = line.substr(0, pos - 1);
		string replacement = line.substr(pos + 3);

		// �������� �������������� �������������� ��������
		pattern.erase(pattern.find_last_not_of(whitespace) + 1);
		replacement.erase(0, replacement.find_first_not_of(whitespace));

		// �������� ������������ �����������,����� ������� ����� � ������ �����\

		bool terminal = !replacement.empty() && replacement[0] == '.';
		if (terminal)
			replacement.erase(0, 1);

		rules.push_back(rule(pattern, replacement, terminal));
	}

	return rules;
}

string markov(vector<rule> rules, string input)
{
	string& output = input;
	vector<rule>::iterator iter = rules.begin();

    // ��������������� ��������� ������ �������
	while (iter != rules.end())
	{
		string::size_type pos = output.find(iter->pattern);
		if (pos != npos)
		{
			output.replace(pos, iter->pattern.length(), iter->replacement);
			// ���� ������������, �� ����� �������
			if (iter->terminal)
				break;
			iter = rules.begin();
		}
		else
			++iter;
	}

	return output;
}

int main()
{
	//���������� � ������ � ������� � ����� ReadMe(Markov).txt
	string input, filename;
	getline(cin, filename);
	ifstream rulefile(filename);
	vector<rule> rules = read_rules(rulefile);
	getline(cin, input);
	string output = markov(rules, input);
	cout << output << "\n";
	system("pause");
	return 0;
}