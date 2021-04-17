#ifndef __DIALOGSYSTEM_H__
#define __DIALOGSYSTEM_H__

#include <map>
#include <string>
#include <functional>
#include "Font.h"

#include "Module.h"

class DialogNode;
class Entity;

class DialogSystem {
public:
	DialogSystem();

	// Destructor
	virtual ~DialogSystem();

	void DrawDialog(Font* dialogueFont);

	bool LoadDialog(const char* filename);

	void StartDialog(const char* id);

	void NextDialog();

	Font* dialogueFont = nullptr;

	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);

	DialogNode* currentDialog = nullptr;

	std::map<std::string, DialogNode*> dialogues;

	std::map<std::string, DialogNode*> nodeRoutes;

	std::map<std::string, std::function<void()>> callbacks;

	int selectedOption = 0;

	std::string folder;
	pugi::xml_document dialogFile;

};

#endif // __DIALOGSYSTEM_H__