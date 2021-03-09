#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct node {
	// Let ' be index 27, a-z be indexed 0-25
	node* m_children[27] = { nullptr };
	// If at end of word
	bool end = false;
	// Easy access to full word
	std::string word = "";
    };
    void insert(StudentSpellCheck::node* root, std::string key);
    bool search(StudentSpellCheck::node* root, std::string key);
    void display(StudentSpellCheck::node* root, int ticks);
    void clear(StudentSpellCheck::node* root);
    StudentSpellCheck::node* m_root = nullptr;
};

#endif  // STUDENTSPELLCHECK_H_
