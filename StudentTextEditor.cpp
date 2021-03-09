#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

// Constructor
StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
    m_text_editor.push_back("");
    m_text_iterator = m_text_editor.begin();
    m_row = 0;
    m_col = 0;
}

// Destructor
StudentTextEditor::~StudentTextEditor()
{
	// TODO
}

bool StudentTextEditor::load(std::string file) {
    StudentTextEditor::reset();
    std::ifstream current_file(file);
    if (!current_file) {
        return false;
    } else {
        std::string s;
        while (getline(current_file, s)) {
	    for (int i = 0; i < s.size(); i++) {
		if (s[i] == '\t') {
		    // Account for tab characters
		    s = s.substr(0, i) + "    " + s.substr(i + 1, s.size() - i - 1);
		} else if (s[i] == '\r') {
		    // Account for carriage returns
		    s = s.substr(0, i) + s.substr(i + 1, s.size() - i - 1);
		}
	    }
            m_text_editor.push_back(s);
        }
    }
    // Take into account empty text files
    if (m_text_editor.size() == 0) {
	m_text_editor.push_back("");
    }
    std::list<std::string>::iterator it = m_text_editor.begin();
    for (int i = 0; i < 3; i++) {
        it++;
        if (it == m_text_editor.end()) {
            break;
        }
    }
    m_text_iterator = m_text_editor.begin();
    return true;
}

bool StudentTextEditor::save(std::string file) {
    std::ofstream output_file(file);
    if (!output_file) {
	return false;
    }
    for (std::list<std::string>::iterator it = m_text_editor.begin(); it != m_text_editor.end(); it++) {
	output_file << *it << std::endl;
    }
	return true;
}

void StudentTextEditor::reset() {
    m_row = 0;
    m_col = 0;
    m_text_editor.clear();
    TextEditor::getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
    switch (dir) {
    case Dir::UP: {
	if (m_row > 0) {
	    m_row--;
	    m_text_iterator--;
	    if (m_col > m_text_iterator->size()) {
		m_col = m_text_iterator->size();
	    }
	}
	break;
    }
    case Dir::DOWN: {
	if (m_row < m_text_editor.size() - 1) {
	    m_row++;
	    m_text_iterator++;
	    if (m_col > m_text_iterator->size()) {
		m_col = m_text_iterator->size();
	    }
	}
	break;
    }
    case Dir::LEFT: {
	if (m_col > 0) {
	    m_col--;
	} else if (m_row > 0) {
	    m_row--;
	    m_text_iterator--;
	    m_col = m_text_iterator->size();
	}
	break;
    }
    case Dir::RIGHT: {
	if (m_col < m_text_iterator->size()) {
	    m_col++;
	} else if (m_row < m_text_editor.size() - 1) {
	    m_col = 0;
	    m_row++;
	    m_text_iterator++;
	}
	break;
    }
    case Dir::HOME: {
	m_col = 0;
	break;
    }
    case Dir::END: {
	m_col = m_text_iterator->size();
	break;
    }
    default:
	break;
    }
}

void StudentTextEditor::del() {
	// TODO
    if (m_col == m_text_iterator->size()) {
	if (m_row == m_text_editor.size() - 1) {
	    return;
	}
	std::string old = *m_text_iterator;
	m_text_iterator = m_text_editor.erase(m_text_iterator);
	*m_text_iterator = old + *m_text_iterator;
	TextEditor::getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\n');
    } else {
	TextEditor::getUndo()->submit(Undo::Action::DELETE, m_row, m_col, (*m_text_iterator)[m_col]);
        *m_text_iterator = m_text_iterator->substr(0, m_col) + m_text_iterator->substr(m_col + 1, m_text_iterator->size() - m_col - 1);
    }
}

void StudentTextEditor::backspace() {
	// TODO
    if (m_col == 0) {
	if (m_row == 0) {
	    return;
	}
	m_text_iterator--;
	m_row--;
	m_col = m_text_iterator->size();
	TextEditor::getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\n');
	std::string old = *m_text_iterator;
	m_text_iterator = m_text_editor.erase(m_text_iterator);
	*m_text_iterator = old + *m_text_iterator;
    } else {
	TextEditor::getUndo()->submit(Undo::Action::DELETE, m_row, m_col - 1, (*m_text_iterator)[m_col - 1]);
	*m_text_iterator = m_text_iterator->substr(0, m_col - 1) + m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_col--;
    }
}

void StudentTextEditor::insert(char ch) {
    if (ch != '\t') {
	*m_text_iterator = m_text_iterator->substr(0, m_col) + ch + m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_col++;
        TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
    } else {
	*m_text_iterator = m_text_iterator->substr(0, m_col) + "    " + m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_col++;
	TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
	m_col++;
	TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
	m_col++;
	TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
	m_col++;
	TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
    }
}

void StudentTextEditor::enter() {
    TextEditor::getUndo()->submit(Undo::Action::SPLIT, m_row, m_col, '\n');
    std::string s1 = m_text_iterator->substr(0, m_col);
    std::string s2 = m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
    m_text_editor.insert(m_text_iterator, s1);
    m_row++;
    m_col = 0;
    *m_text_iterator = s2;
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if (startRow < 0 || numRows < 0 || startRow >= m_text_editor.size()) {
	return -1;
    }

    // Clear all current lines
    lines.clear();

    // Go to the target start row
    std::list<std::string>::const_iterator it = m_text_editor.begin();
    for (int i = 0; i < startRow; i++) {
	it++;
	// Just in case somehow the iterator goes out of bounds
	if (it == m_text_editor.end()) {
	    return -1;
	}
    }
    // Start adding new lines
    int count = 0;
    for (int i = 0; i < numRows; i++) {
	lines.push_back(*it);
	count++;
	it++;
        if (it == m_text_editor.end()) {
	    break;
	}
    }
    return count;
}

void StudentTextEditor::undo() {
    int count;
    std::string text;
    int old_row = m_row;
    int old_col = m_col;
    auto undo_action = TextEditor::getUndo()->get(m_row, m_col, count, text);
    m_text_iterator = m_text_editor.begin();
    for (int i = 0; i < m_row; i++) {
	m_text_iterator++;
	if (m_text_iterator == m_text_editor.end()) {
	    m_row = old_row;
	    m_col = old_col;
	    return;
	}
    }
    switch (undo_action) {
    case Undo::Action::INSERT: {
        *m_text_iterator = m_text_iterator->substr(0, m_col) + text + m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_col += text.size();
	break;
    }
    case Undo::Action::SPLIT: {
	// Same as enter except without submitting undo
	std::string s1 = m_text_iterator->substr(0, m_col);
	std::string s2 = m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_text_editor.insert(m_text_iterator, s1);
	m_row++;
	m_col = 0;
	*m_text_iterator = s2;
	break;
    }
    case Undo::Action::DELETE: {
        *m_text_iterator = m_text_iterator->substr(0, m_col - count) + m_text_iterator->substr(m_col, m_text_iterator->size() - m_col);
	m_col -= count;
	break;
    }
    case Undo::Action::JOIN: {
	std::string old = *m_text_iterator;
	m_text_iterator = m_text_editor.erase(m_text_iterator);
	*m_text_iterator = old + *m_text_iterator;
    }
    default: {
	break;
    }
    }
}
