#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    std::ifstream dictionary(dictionaryFile);
    std::ofstream debug_log("stderr.txt");
    if (!dictionary) {
        debug_log << "Dictionary not found!\n";
        return false;
    } else {
        debug_log << "Dictionary found!\n";
        m_dictionary.clear();
        std::string s;
        while (getline(dictionary, s)) {
            m_dictionary.push_back(s);
        }
    }
    debug_log << "First three words are:\n";
    debug_log << m_dictionary[0] << std::endl;
    debug_log << m_dictionary[1] << std::endl;
    debug_log << m_dictionary[2] << std::endl;
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}
