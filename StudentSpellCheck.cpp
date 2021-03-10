#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ctype.h>
#include <regex>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck()
    : SpellCheck() {
    m_root = new StudentSpellCheck::node;
}

StudentSpellCheck::~StudentSpellCheck() {
    StudentSpellCheck::clear(m_root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    std::ifstream dictionary(dictionaryFile);
    if (!dictionary) {
        return false;
    } else {
        StudentSpellCheck::clear(m_root);
	m_root = new node;
        std::string s;
        while (getline(dictionary, s)) {
	    StudentSpellCheck::insert(m_root, s);
        }
    }
    // StudentSpellCheck::display(m_root, 6);
    return true;
}

void StudentSpellCheck::insert(StudentSpellCheck::node* root, std::string key) {
    if (root == nullptr) {
	return;
    }

    std::string new_key = "";
    for (auto &ch : key) {
	if (isalpha(ch)) {
	    new_key += tolower(ch);
	} else if (ch == '\'') {
	    new_key += ch;
	}
    }

    node* current = root;
    for (auto &ch : new_key) {
	if (ch == '\'') {
	    // It's a ' character
	    if (current->m_children[26] == nullptr) {
		current->m_children[26] = new node;
	    }
	    current = current->m_children[26];
	} else {
	    // Go to whatever character it is.
	    if (current->m_children[ch - 'a'] == nullptr) {
		current->m_children[ch - 'a'] = new node;
	    }
	    current = current->m_children[ch - 'a'];
	}
    }
    current->end = true;
    current->word = new_key;
}

bool StudentSpellCheck::search(StudentSpellCheck::node* root, std::string key) {
    if (root == nullptr) {
	return false;
    }

    std::string new_key = "";
    for (auto &ch : key) {
	if (isalpha(ch)) {
	    new_key += tolower(ch);
	} else if (ch == '\'') {
	    new_key += ch;
	}
    }

    node* current = root;
    for (auto &ch : new_key) {
	if (ch == '\'') {
	    // It's a ' character
	    if (current->m_children[26] == nullptr) {
		return false;
	    }
	    current = current->m_children[26];
	} else {
	    // Go to whatever character it is.
	    if (current->m_children[ch - 'a'] == nullptr) {
		return false;
	    }
	    current = current->m_children[ch - 'a'];
	}
    }
    return true;
}

void StudentSpellCheck::display(StudentSpellCheck::node* root, int ticks) {
    // To limit how deep to look
    if (ticks <= 0) {
	return;
    }

    std::ofstream debug_log("stderr.txt", std::ios::app);
    if (root->end) {
	debug_log << root->word << std::endl;
    }

    for (int i = 0; i < 27; i++) {
	if (root->m_children[i] == nullptr) {
	    continue;
	}
	StudentSpellCheck::display(root->m_children[i], ticks - 1);
    }
}

void StudentSpellCheck::clear(StudentSpellCheck::node* root) {
    if (root == nullptr) {
	return;
    }

    for (int i = 0; i < 27; i++) {
	if (root->m_children[i] != nullptr) {
	    StudentSpellCheck::clear(root->m_children[i]);
	}
    }
    delete root;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    suggestions.clear();
    if (StudentSpellCheck::search(m_root, word)) {
	return true;
    }
    
    int count = 0;
    for (int i = 0; i < word.size(); i++) {
	std::string temp = word;
	// all letters 1-z
	for (int j = 0; j < 27; j++) {
	    if (j == 26) {
		temp[i] = '\'';
	    } else {
		temp[i] = 'a' + j;
	    }
	    if (StudentSpellCheck::search(m_root, temp)) {
		if (count < max_suggestions) {
		    suggestions.push_back(temp);
		    count++;
		} else {
		    return false;
		}
	    }
	}
    }
    return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    problems.clear();
    std::string text = line.substr(0, line.size());
    std::regex word_pattern("[A-Za-z']+");
    std::smatch word_match;
    int pos = 0;
    while(std::regex_search(text, word_match, word_pattern)) {
	if (!(search(m_root, word_match[0].str()))) {
	    SpellCheck::Position error;
	    error.start = pos + word_match.position(0);
	    error.end = pos + word_match.position(0) + word_match[0].str().size() - 1;
	    problems.push_back(error);
	}
        pos += word_match.position(0) + word_match[0].str().size();
        text = word_match.suffix();
    }
}
