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

	void DrawDialogAnimated(Font* dialogueFont);
	void DrawDialog(Font* dialogueFont);

	bool LoadDialog(const char* filename);

	void StartDialog(const char* id);

	void NextDialog();

	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);

public:
	DialogNode* currentDialog;

	std::map<std::string, DialogNode*> dialogues;

	std::map<std::string, DialogNode*> nodeRoutes;

	std::map<std::string, std::function<void()>> callbacks;

	int selectedOption = 0;
	float timeStep;
	float timeStepAnswers;
	SDL_Color white;

	std::string folder;
	pugi::xml_document dialogFile;

	SDL_Texture* dialogGui;
	SDL_Rect dialogTextBox;
	SDL_Rect portraitBox;
	SDL_Rect shopKeeperPortrait;
	SDL_Rect catPortrait;
	SDL_Rect grandpaPortrait;
	SDL_Rect superheroPortrait;
	SDL_Rect hatsunePortrait;


	char savingText[TEXT_LEN] = { 0 };
	float savingCounter;
	bool savingBool = false;
	float itemCounter;

};

#endif // __DIALOGSYSTEM_H__