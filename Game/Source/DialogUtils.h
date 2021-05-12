#ifndef __DIALOGUTILS_H__
#define __DIALOGUTILS_H__

#include "Defs.h"
#include "Log.h"

#include <vector>
#include <map>
#include <string>


using Attributes = std::map<std::string, std::string>;

struct DialogNode {
public:
	enum class NodeType {
		DIALOG,
		LINE,
		OPTIONS,
		OPTION,
		GOTO
	};

	DialogNode(Attributes* _attributes = nullptr) {
		type = NodeType::DIALOG;
		children = new std::vector<DialogNode*>();
		attributes = _attributes;
	}

	~DialogNode() {
		RELEASE(children);
		RELEASE(attributes);
	}

	void SetAttributes(Attributes* _attributes) {
		attributes = _attributes;
	}

	void AddChild(DialogNode* child)
	{
		children->push_back(child);
	}

	DialogNode* Next()
	{
		// If the node is OPTION we move back to the parent to continue with the next lines.
		if (type == NodeType::OPTION) return parent->Next();
		// If "next" exists, we go to next.
		if (next != nullptr) return next;
		// If next does not exist, and parent exists, we move to parent.
		if (parent != nullptr) return parent->Next();
		// If we get here, it means that the dialog has finished.
		return nullptr;
	}

	NodeType type;
	std::vector<DialogNode*>* children;
	std::map<std::string, std::string>* attributes;

	DialogNode* prev = nullptr;
	DialogNode* next = nullptr;
	DialogNode* parent = nullptr;
};



#endif // __DIALOGUTILS_H__