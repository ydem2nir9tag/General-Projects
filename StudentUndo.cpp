#include "StudentUndo.h"
using namespace std;

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
	undo here;
	string c(1, ch);
	if (!undoData.empty())
	{
		if (action == DELETE)
		{
			if (undoData.top().act == action &&
				undoData.top().x == col &&
				undoData.top().y == row)
			{
				undoData.top().s += c;
				return;
			}
			else if (undoData.top().act == action &&
				undoData.top().Xnew == col + 1 &&
				undoData.top().y == row)
			{
				undoData.top().s.insert(0, c);
				undoData.top().Xnew = col;
				undoData.top().x = col;
				undoData.top().y = row;
				return;
			}

		}
		if (action == INSERT)
		{
			if (undoData.top().act == action &&
				undoData.top().Xnew == col - 1 &&
				undoData.top().y == row)
			{
				undoData.top().s += c;
				undoData.top().Xnew = col;
				return;
			} 
			/*else if (undoData.size() == 1 &&		this would have been needed if we were deleting things 
				undoData.top().act == action &&		word by word
				undoData.top().Xnew == col - 1 &&
				undoData.top().y == row &&
				undoData.top().s[1] == ' ' &&
				ch == ' ')
			{
				undoData.top().s += c;
				undoData.top().Xnew = col;
				return;
			}*/
		}
	}
	here.act = action;
	here.x = col;
	here.y = row;
	here.s = c;
	here.Xnew = col;
	undoData.push(here);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (undoData.empty())
		return Action::ERROR;
	Action q = undoData.top().act;
	switch(q)
	{
	case Action::INSERT:
		row = undoData.top().y;
		col = undoData.top().x;
		count = undoData.top().s.size();
		text = "";
		undoData.pop();
		return Action::DELETE;
	case Action::DELETE:
		row = undoData.top().y;
		col = undoData.top().x;
		count = 1;
		text = undoData.top().s;
		undoData.pop();
		return Action::INSERT;
	case Action::SPLIT:
		row = undoData.top().y;
		col = undoData.top().x;
		count = undoData.top().s.size();
		text = "";
		undoData.pop();
		return Action::JOIN;
	case Action::JOIN:
		row = undoData.top().y;
		col = undoData.top().x;
		count = undoData.top().s.size();
		text = "";
		undoData.pop();
		return Action::SPLIT;
	}
	return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
	// TODO
}
