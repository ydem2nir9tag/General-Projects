#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct Trie {
		struct Trie* child[27];
		bool leaf;
	};
	Trie* root;
	//helper functions
	Trie* addNode()
	{
		Trie* node = new Trie;
		for (int i = 0; i < 27; i++)
			node->child[i] = nullptr;
		node->leaf = false;
		return node;
	};

	/* this didn't work but i want it anyways, i spent a lot of time on this
	void suggest(int curPos, Trie* cur, std::string word, std::vector<std::string>& suggestions)
	{
		for (int j = 0; j < 27; j++)
		{
			Trie* temp = cur;
			std::string suggest = "";
			if (temp->child[j] != nullptr)
			{
				if (j == 26)
					suggest += '\'';
				else
				{
					char s = 'a' + j;
					suggest += s;
				}
				for (int i = curPos + 1; i < word.size(); i++)
				{
					word[i] = tolower(word[i]);
					int pos = word[i] - 'a';
					if (pos > 26 || pos < 0)
						break;
					if (temp->child[pos] == nullptr)
					{
						break;
					}
					char s = 'a' + pos;
					if (pos == 26)
						suggest += '\'';
					else
					{
						char s = 'a' + j;
						suggest += s;
					}
					temp = temp->child[pos];
				}
				if (temp != nullptr && temp->leaf == true)
					suggestions.push_back(suggest);
				continue;
			}
		}
	};*/

	void suggestV2(int max, std::string word, std::vector<std::string>& suggestions)
	{
		int suggested = 0;
		int size = word.size();
		int pos = 0;
		std::string temp = word;
		for (; pos < size; pos++)
		{
			temp = word;
			for (int i = 0; i < 27; i++)
			{
				char c = 'a' + i;
				if (i == 26)
					c = '\'';
				std::string s(1, c);
				temp.erase(pos, 1);
				temp.insert(pos, s);
				if (check(temp))
				{
					suggestions.push_back(temp);
					suggested++;
					if (suggested >= max)
						return;
				}
			}
		}
	}

	bool check(std::string word)
	{
		Trie* temp = root;
		for (int i = 0; i < word.size(); i++)
		{
			word[i] = tolower(word[i]);
			int pos = word[i] - 'a';
			if (pos > 26 || pos < 0)
				pos = 26;
			if (temp->child[pos] == nullptr)
			{
				return false;
			}
			temp = temp->child[pos];
		}
		if (temp != nullptr && temp->leaf == true)
			return true;
		return false; // TODO
	}

	void destroy(Trie* cur)
	{
		if (cur == nullptr)
			return;
		for (int i = 0; i < 27; i++)
		{
			destroy(cur->child[i]);
		}
		delete cur;
	}
};

#endif  // STUDENTSPELLCHECK_H_
