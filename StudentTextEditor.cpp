#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

// Constructor
StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
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
    m_text_editor.clear();
    std::ifstream current_file(file);
    std::ofstream debug_log("stderr.txt");
    if (!current_file) {
        debug_log << "Target textfile not found!\n";
        return false;
    } else {
        debug_log << "Target textfile found!\n";
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
    debug_log << "First three lines are:\n";
    std::list<std::string>::iterator it = m_text_editor.begin();
    for (int i = 0; i < 3; i++) {
        debug_log << *it << std::endl;
        it++;
        if (it == m_text_editor.end()) {
            break;
        }
    }
    m_text_iterator = m_text_editor.begin();
    m_row = 0;
    m_col = 0;
    return true;
}

bool StudentTextEditor::save(std::string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	// TODO
}

void StudentTextEditor::move(Dir dir) {
    std::ofstream debug_log("stderr.txt");
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
    debug_log << "row: " << m_row << " col: " << m_col << " rowlen: " << m_text_iterator->size() << std::endl;
	// TODO
}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
	// TODO
}

void StudentTextEditor::enter() {
	// TODO
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
	// TODO
}
