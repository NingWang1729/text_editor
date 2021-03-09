#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
#include <string>

class StudentUndo : public Undo {
public:
    ~StudentUndo();
    void submit(Action action, int row, int col, char ch = 0);
    Action get(int& row, int& col, int& count, std::string& text);
    void clear();

private:
    struct action_object {
	Undo::Action m_action;
	unsigned long m_row;
	unsigned long m_col;
	unsigned long m_length;
	std::string m_text;
    };
    std::stack<action_object> m_actions;
};

#endif // STUDENTUNDO_H_
