#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
	destroy(root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream infile(dictionaryFile);
	string s;
	root = addNode();
	Trie* temp = root;
	if (!infile)
		return false;
	while (getline(infile, s))
	{
		for (int i = 0; i < s.size(); i++)
		{
			int index = s[i] - 'a';
			if (index > 25 || index < 0) //for apostraphe
				index = 26;
			if (temp->child[index] == nullptr)
				temp->child[index] = addNode();
			temp = temp->child[index];
		}
		temp->leaf = true;
		temp = root;
	}
	root = temp;
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	Trie* temp = root;
	for (int i = 0; i < word.size(); i++)
	{
		word[i] = tolower(word[i]);
		int pos = word[i] - 'a';
		if (pos > 26 || pos < 0)
			pos = 26;
		if (temp->child[pos] == nullptr)
		{	
			temp = temp->child[pos];
			break;
		}
		temp = temp->child[pos];
	}
	if (temp != nullptr && temp->leaf == true)
		return true;
	suggestV2(max_suggestions, word, suggestions);
	return false; // TODO
}

 void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
	string temp;
	int pos = 0;
	int length = 0;
	for (int i = 0; i <= line.size(); i++)
	{
		if (!isalpha(line[i]) && line[i] != '\'')
		{
			temp = line.substr(pos, length);
			if (!check(temp))
			{
				length--;
				Position vec;
				vec.start = pos;
				vec.end = pos + length;
				problems.push_back(vec);
			}
			pos = i + 1;
			length = 0;
			continue;
		}
		length++;
	}
}