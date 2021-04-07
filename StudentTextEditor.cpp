#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	pos_x = 0;
	pos_y = 0;
	text.push_back("");
	it = text.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
}

bool StudentTextEditor::load(std::string file) {
	ifstream infile(file);
	string s;
	if (!infile)
		return false;
	reset();
	while (getline(infile,s))
	{
		if (!s.empty() && s.back() == '\r')
			s.pop_back();
		text.push_back(s);
	}
	it = text.begin();
	pos_y = 0;
	pos_x = 0;
	return true;
}

bool StudentTextEditor::save(std::string file) {
	ofstream outfile(file);
	if (!outfile)
		return false;
	list<string>::iterator it1 = text.begin();
	while (it1 != text.end())
	{
		outfile << *it1 << '\n';
		it1++;
	}
	return true;
}

void StudentTextEditor::reset() {
	// TODO
	pos_x = 0;
	pos_y = 0;
	text.clear();
	it = text.begin();
}

void StudentTextEditor::move(Dir dir) {
	// TODO
	switch (dir)
	{
	case Dir::UP:
		if (pos_y == 0)
			break;
		pos_y--;
		it--;
		if (pos_x > it->size())
			pos_x = it->size();
		break;
	case Dir::DOWN:
		it++;
		if (it == text.end())
		{
			it--;
			break;
		}
		if (pos_x > it->size())
			pos_x = 0;
		pos_y++;
		break;
	case Dir::LEFT:
		pos_x--;
		if (pos_y == 0 && pos_x < 0)
		{
			pos_x = 0;
			break;
		}
		if (pos_x < 0)
		{
			StudentTextEditor::move(UP);
			pos_x = it->size();
		}
		break;
	case Dir::RIGHT:
		pos_x++;
		if (pos_x > it->size() && pos_y + 1 != text.size())
		{
			pos_x = 0;
			StudentTextEditor::move(DOWN);
		}
		else if (pos_x > it->size() && pos_y + 1 == text.size())
		{
			pos_x--;
		}
		break;
	case Dir::HOME:
		pos_x = 0;
		break;
	case Dir::END:
		pos_x = it->size();
		break;
	default:
		break;
	}
}

void StudentTextEditor::del() {
	// TODO
	if (pos_x < it->size())
	{
		if(!redo)
			getUndo()->submit(Undo::Action::DELETE, pos_y, pos_x, (*it)[pos_x]);
		it->erase(pos_x, 1);
		return;
	}
	if (pos_x == it->size())
	{
		list<string>::iterator it1 = it;
		it1++;
		if (it1 == text.end())
			return;
		if(!redo)
			getUndo()->submit(Undo::Action::JOIN, pos_y, pos_x, 'd');
		*it += *it1;
		text.erase(it1);
		return;
	}
}

void StudentTextEditor::backspace() {
	// TODO
	if (pos_x > 0)
	{
		pos_x--;
		del();
		return;
	}
	else if (pos_x == 0 && pos_y != 0)
	{
		it--;
		pos_y--;
		list<string>::iterator it1 = it;
		it1++;
		pos_x = it->size();
		*it += *it1;
		if(!redo)
			getUndo()->submit(Undo::Action::JOIN, pos_y, pos_x, 'b');
		text.erase(it1);
		return;
	}
	else return;
}

void StudentTextEditor::insert(char ch) {
	// TODO
	if (ch == '\t')
	{
		for (int i = 0; i < 4; i++)
		{
			insert(' ');
		}
		return;
	}
	string s(1, ch);
	it->insert(pos_x, s);
	move(RIGHT);
	if(!redo)
		getUndo()->submit(Undo::Action::INSERT, pos_y, pos_x, ch);
}

void StudentTextEditor::enter() {
	// TODO
	if (!redo)
		getUndo()->submit(Undo::Action::SPLIT, pos_y, pos_x, '\0');
	string str1 = it->substr(0, pos_x);
	string str2 = it->substr(pos_x, string::npos);
	*it = str1;
	list<string>::iterator it1 = it;
	it1++;
	text.insert(it1, str2);
	move(DOWN);
	pos_x = 0;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	// TODO
	row = pos_y;
	col = pos_x;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	// TODO
	if (startRow < 0 || numRows < 0 || startRow > text.size())
		return -1;
	list<string>::const_iterator it1 = it;
	lines.clear();
	int i = 0;
	int y = pos_y;
	if (pos_y > startRow)
	{
		while (y > startRow)
		{
			it1--;
			y--;
		}
	}
	if (pos_y < startRow)
	{
		while (y < startRow)
		{
			it1++;
			y++;
		}
	}
	while (it1 != text.end() && i < numRows)
	{
		lines.push_back(*it1);
		it1++;
		i++;
	}
	return i;
}

void StudentTextEditor::undo() {
	// TODO
	Undo::Action act;
	int x;
	int y;
	int count;
	string ntext;
	act = getUndo()->get(y, x, count, ntext);
	if (act == Undo::Action::ERROR)
		return;
	if (pos_y > y)
	{
		while (pos_y > y)
		{
			it--;
			pos_y--;
		}
	}
	if (pos_y < y)
	{
		while (pos_y < y)
		{
			it++;
			pos_y++;
		}
	
	}
	switch (act)
	{
	case Undo::Action::DELETE:
		pos_x = x - 1;
		pos_y = y;
		for (int i = 0; i < count; i++)
		{
			redo = true;
			del();
		}
		redo = false;
		break;
	case Undo::Action::INSERT:
		pos_x = x;
		pos_y = y;
		for (int i = 0; i < ntext.size(); i++)
		{
			redo = true;
			StudentTextEditor::insert(ntext[i]);
		}
		pos_x = x;
		redo = false;
		break;
	case Undo::Action::JOIN:
		pos_x = x;
		pos_y = y;
		redo = true;
		del();
		redo = false;
		break;
	case Undo::Action::SPLIT:
		pos_x = x;
		pos_y = y;
		redo = true;
		enter();
		redo = false;
		pos_x = x;
		pos_y = y;
		it--;
		break;
	}
}
