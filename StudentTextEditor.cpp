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
	// TODO
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
	    if (s.size() > 0 && s[s.size() - 1] == '\r') {
		s = s.substr(0, s.size() - 1);
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
    return true;
}

bool StudentTextEditor::save(std::string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	// TODO
}

void StudentTextEditor::move(Dir dir) {
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
	// TODO
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
