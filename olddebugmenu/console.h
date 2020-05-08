#pragma once

#include "../OldUI/UIElement.h"
#include "../OldUI/TextField/TextField.h"

class Console : public UIElement{
public:
	struct Command {
		std::string name;
		std::string description;

		struct Argument {
			std::string name;
			std::string description;
			std::string value;
			Argument() {}
			Argument(std::string name, std::string description) { this->name = name; this->description = description; }
			Argument(std::string name, std::string description, std::string value) { this->name = name; this->description = description; this->value = value; }
			std::string toString() {
				return "<" + name + ">: " + description + ( value.length()>0 ? (", value = '" + value + "'") : "");
			}
		};
		std::vector<Argument> args;

		std::function<std::string(std::vector<std::string>)> action;

		Command() {}
		Command(std::string name, std::string description, std::vector<Argument> args, std::function<std::string(std::vector<std::string>)> action) { this->name = name; this->description = description; this->args = args; this->action = action; }

		std::string toString() {
			std::string str = "Command <" + name + ">:" + 
							  "\n\tdescription: " + description + 
							  "\n\tsyntax: \'" + name;

			for (int i = 0; i < args.size(); i++) {
				str += " <" + args[i].name + ">";
			}

			if (args.size() > 0) {
				str += "\'\n\targuments for \'" + name + "\':\n";

				for (int i = 0; i < args.size(); i++) {
					str += "\t\t" + args[i].toString() + "\n";
				}
			}
			else {
				str += "\'\n\tno arguments";
			}

			return str;
		}
	};
	std::vector<Command> commands;

public:
	int maxLogs = 100;
	std::vector<std::string> logs;

	std::vector<std::string> inputLogs;
	std::string prevInputLog;
	int curInputLog = 0;

	float slider = 0.0f;

	TextRendererOld txr;

	TextField inputField;

	Console();
	Console(TextRendererOld &txr, Vector2f pos, Vector2f size);
	Console(TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f color);

	~Console();

	void addCommand(Command command);
	Command *getCommand(std::string name);

	std::string queryCommand(std::string commandName, std::vector<std::string> args);

	virtual void initDefaultCommands();

	std::stringstream log;
	virtual void flogOld(std::string str);
	virtual void flog(std::string str);

	void setPos(Vector2f pos);
	Vector2f getPos();

	void setSize(Vector2f size);
	Vector2f getSize();

	virtual void setColor(Vector4f color);

	void refresh();

	void init();
	void update();
	void draw();
};

