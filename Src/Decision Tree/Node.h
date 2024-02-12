#pragma once

#include <iostream>

struct node_t {
	// Constructor
	node_t(std::string info) {
		this->info = info;
	}

	virtual node_t* execute() = 0;

	std::string info;
	node_t* trueNode = nullptr;  // Pointer to true branch
	node_t* falseNode = nullptr; // Pointer to false branch
};

// Enum defining different types of predicates (Less, Greater, Equal, Not_Equal)
enum Predicate {
	LESS,
	GREATER,
	EQUAL,
	NOT_EQUAL
};

// Function to convert enum predicate to string representation
inline std::string getString(Predicate predicate) {
	switch(predicate) {
		case Predicate::LESS:
			return "<";
			break;
		case Predicate::GREATER:
			return ">";
			break;
		case Predicate::EQUAL:
			return "==";
			break;
		case Predicate::NOT_EQUAL:
			return "!=";
			break;
		default:
			return "";
			break;
	}
}

template <typename T>
struct decision_t : public node_t {
	// Constructor
	decision_t(const std::string& info, T& parameter, Predicate predicate, const T& value) : node_t{info},
		parameter{parameter},
		predicate{predicate},
		value{value} {
	}

	node_t* execute() {
		bool result = false;
		switch(this->predicate) {
			case Predicate::LESS:
				result = (parameter < value);
				break;
			case Predicate::GREATER:
				result = (parameter > value);
				break;
			case Predicate::EQUAL:
				result = (parameter == value);
				break;
			case Predicate::NOT_EQUAL:
				result = (parameter != value);
				break;
		}

		std::cout << "Decision: " << this->info << " : " << parameter << " " << getString(this->predicate) << " " << value << " (" << (result ? "true" : "false") << ")\n";

		// Return true or false branch node based on result
		return (result ? trueNode : falseNode);
	}

	T& parameter; // Reference to the parameter used for comparison
	T value; // Value for comparison
	Predicate predicate; // Predicate used for comparison
};

struct action_t : public node_t {
	// Constructor
	action_t(std::string info) : node_t{info} {
		//
	}

	virtual node_t* execute() override {
		std::cout << "Action: " << info << std::endl;

		// Action node is a leaf node (no children)
		return nullptr;
	}
};