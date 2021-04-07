#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include <list>
#include "TextEditor.h"

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	std::list<std::string> text;
	std::list<std::string>::iterator it; //should be keeping track of y pos (row)
	int pos_x;
	int pos_y;
	bool redo = false;
};

#endif // STUDENTTEXTEDITOR_H_