#include "DialogSystem.h"
#include "DialogUtils.h"
#include "SceneCombat.h"
#include "SceneDevRoom.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Entity.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Font.h"
#include "Player.h"

#include <utility>

#include "Log.h"

void ShopKeeperFinished()
{
	app->entities->shopkeeperFinishedTalkRequest = true;
}
void CatFinished()
{
	app->entities->catFinishedTalkRequest = true;
}
void SuperheroFinished()
{
	app->entities->superheroFinishedTalkRequest = true;
}
void GrandpaFinished()
{
	app->entities->grandpaFinishedTalkRequest = true;
}

std::string ToUpperCase(std::string input)
{
	std::string output = "";

	std::string::iterator i = input.begin();

	for (i; i < input.end(); i++)
	{
		char c = *i;
		/*if (c >= 97 && c <= 122)
			c -= 32;*/
		output += c;
	}

	return output;
}

DialogSystem::DialogSystem()
{
	LoadDialog("Assets/Dialog/Cat.xml");
	LoadDialog("Assets/Dialog/ShopKeeper.xml");
	LoadDialog("Assets/Dialog/Grandpa.xml");
	LoadDialog("Assets/Dialog/Superhero.xml");

	// Register a callback functions
	callbacks[std::string("shopkeeper_finished")] = std::function<void()>(&ShopKeeperFinished);
	callbacks[std::string("cat_finished")] = std::function<void()>(&CatFinished);
	callbacks[std::string("superhero_finished")] = std::function<void()>(&SuperheroFinished);
	callbacks[std::string("grandpa_finished")] = std::function<void()>(&GrandpaFinished);
}

DialogSystem::~DialogSystem()
{
	nodeRoutes.clear();
	callbacks.clear();
	dialogues.clear();
}

void DialogSystem::DrawDialog(Font* dialogueFont)
{
	// Draw the background rectangle.
	//app->render->DrawRectangle(SDL_Rect({ 0, (app->render->camera.h / 3) * 2, app->render->camera.w, app->render->camera.h / 3 }), 43, 176, 132, 255, true, false);

	// Set the text to uppercase, since our font only supports uppercase. //CHANGE
	std::string text = ToUpperCase(currentDialog->attributes->at("value"));
	//std::string text = currentDialog->attributes->at("value");

	// Write the dialog line.
	//app->font->BlitText(10, (app->render->camera.h / 3) * 2 + 10, 0, text.c_str());
	app->render->DrawText(dialogueFont, text.c_str(), 60, (app->render->camera.h / 3) * 2 + 30, 34, 1, { 255,255,255,255 });

	// If the current node is a question, we should also draw the possible answers
	if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		std::vector<DialogNode*>::iterator i = currentDialog->children->begin();
		int y = 0;
		// Iterate through the answers.
		for (i; i < currentDialog->children->end(); i++)
		{
			// Set them to uppercase.
			text = ToUpperCase((*i)->attributes->at("value"));
			//text = (*i)->attributes->at("value");
			// Draw them, increasing the y offset at every iteration.
			// app->fonts->BlitText(30, (app->render->camera.h / 3) * 2 + 30 + (18 * y), 0, text.c_str());
			app->render->DrawText(dialogueFont, text.c_str(), 60, (app->render->camera.h / 3) * 2 + 82 + (30 * y), 30, 1, { 255,255,255,255 });
			y++;
		}
		// Draw a small black rectangle next to the selected option.
		SDL_Rect selectedRectangle = SDL_Rect({ 42, (app->render->camera.h / 3) * 2 + 94 + (30 * selectedOption), 6, 6 });
		app->render->DrawRectangle(selectedRectangle, 250, 0, 0, 255, true, false);
	}
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	// Get the file path.
	std::string path = folder + filename;   // CHNGE NAMES: NormalDialogs and CombatDialogs

	// Load the file.
	pugi::xml_parse_result result = dialogFile.load_file(path.c_str());

	if (result == NULL) {
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true) {

		/* Start TODO 1 */

		// Get the dialog root.
		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		// Parse the dialog XML into the tree structure.
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		// Insert the dialog into the dictionary.
		dialogues.insert(std::make_pair(id, dialog));

		/* End TODO 1 */
	}

	return ret;
}

void DialogSystem::StartDialog(const char* id)
{
	// If the dialog does not exist, return.
	if (dialogues.find(id) == dialogues.end()) return;
	// If it does exist, set it to currentDialog and run NextDialog() to start.
	currentDialog = dialogues.at(id);
	NextDialog();
}

void DialogSystem::NextDialog()
{
	/* Start TODO 4 */

	// If we have reached the end, currentDialog will be nullptr.
	if (currentDialog == nullptr)
	{
		return;
	}
	// If the currentDialog is DIALOG, it means we are at the root of the tree. We can just skip to the first child.
	if (currentDialog->type == DialogNode::NodeType::DIALOG)
	{
		currentDialog = *currentDialog->children->begin();
	}
	// If currentDialog is OPTIONS, we should skip to the selected child.
	else if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		currentDialog = *currentDialog->children->at(selectedOption)->children->begin();
	}
	// Else, we just skip to the next line.
	else
	{
		currentDialog = currentDialog->Next();
	}

	// We reset the selectedOption to 0.
	selectedOption = 0;

	// Again, if we have reached the end of the dialog we return.
	if (currentDialog == nullptr)
	{
		return;
	}

	// If the current line has a callback, we execute it.
	if (currentDialog->attributes->find("callback") != currentDialog->attributes->end())
	{
		callbacks.at(currentDialog->attributes->at("callback"))();
	}

	// If the current dialog is GOTO, we get the "route" and travel to the new line.
	if (currentDialog->type == DialogNode::NodeType::GOTO) {
		std::string route = currentDialog->attributes->at("route");
		currentDialog = nodeRoutes.at(route);
	}

	/* End TODO 4 */
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

	// We set the type variable acording to the XML node name. 
	std::string type = currentNode.name();
	if (type == "dialog") {}
	else if (type == "line")
	{
		dialogNode->type = DialogNode::NodeType::LINE;
	}
	else if (type == "options")
	{
		dialogNode->type = DialogNode::NodeType::OPTIONS;
	}
	else if (type == "option")
	{
		dialogNode->type = DialogNode::NodeType::OPTION;
	}
	else if (type == "goto")
	{
		dialogNode->type = DialogNode::NodeType::GOTO;
	}
	else
		return nullptr;


	Attributes* attributes = new Attributes();

	// We iterate over the attributes and store them in a dictionary.
	pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
	for (aIt; aIt != currentNode.attributes_end(); aIt++)
	{
		attributes->insert(std::make_pair(aIt->name(), aIt->value()));
	}

	dialogNode->SetAttributes(attributes);

	// If the node contains an "id", we store it in a dictionary that will be useful for GOTOs.
	if (dialogNode->attributes->find("id") != dialogNode->attributes->end())
	{
		nodeRoutes.insert(std::make_pair(dialogNode->attributes->at("id"), dialogNode));
	}

	// If the type is not LINE, we execute ParseDialogXML recursively over its children and store them in the "children" vector.
	if (dialogNode->type != DialogNode::NodeType::LINE)
	{
		pugi::xml_node_iterator cIt = currentNode.begin();
		for (cIt; cIt != currentNode.end(); cIt++)
		{
			DialogNode* child = ParseDialogXML(*cIt);
			child->parent = dialogNode;
			if (dialogNode->children->size() != 0)
			{
				child->prev = *(dialogNode->children->end() - 1);
				(*(dialogNode->children->end() - 1))->next = child;
			}
			dialogNode->AddChild(child);
		}
	}

	/* End TODO 2 */

	return dialogNode;
}