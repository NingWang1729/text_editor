#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

StudentUndo::~StudentUndo() {
    StudentUndo::clear();
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if (!m_actions.empty() && action == Action::DELETE && m_actions.top().m_action == Action::DELETE) {
	if (row == m_actions.top().m_row) {
	    if (col == m_actions.top().m_col) {
		m_actions.top().m_text += ch;
		return;
	    } else if (col == m_actions.top().m_col - 1) {
		m_actions.top().m_text = ch + m_actions.top().m_text;
		m_actions.top().m_col--;
		return;
	    }
	}
    } else if (!m_actions.empty() && action == Action::INSERT && m_actions.top().m_action == Action::INSERT) {
	if (row == m_actions.top().m_row) {
	    if (col == m_actions.top().m_col + 1) {
		m_actions.top().m_text += ch;
		m_actions.top().m_col++;
		return;
	    }
	}
    } else {
	StudentUndo::action_object new_action;
	new_action.m_action = action;
	new_action.m_row = row;
	new_action.m_col = col;
	new_action.m_length = 1;
	new_action.m_text = "";
	if (action == Action::INSERT || action == Action::DELETE) {
	    new_action.m_text += ch;
	}
	m_actions.push(new_action);
    }
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if (m_actions.empty()) {
	return Action::ERROR;
    }
    switch (m_actions.top().m_action) {
    case Action::INSERT: {
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;
	count = m_actions.top().m_text.size();
	text = "";
	m_actions.pop();
	return Action::DELETE;
    }
    case Action::SPLIT: {
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;
	count = 1;
	text = "";
	m_actions.pop();
	return Action::JOIN;
    }
    case Action::DELETE: {
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;
	count = 1;
	text = m_actions.top().m_text;
	m_actions.pop();
	return Action::INSERT;
    }
    case Action::JOIN: {
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;
	count = 1;
	text = "";
	m_actions.pop();
	return Action::SPLIT;
    }
    default: {
	return Action::ERROR;
    }
    }
}

void StudentUndo::clear() {
    while (!m_actions.empty()) {
	m_actions.pop();
    }
}
