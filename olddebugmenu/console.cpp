#include "Console.h"

void Console::refresh() {
	inputField.refresh();
}

Console::Command *Console::getCommand(std::string name) {
	for (int i = 0; i < commands.size(); i++) {
		if (commands[i].name.compare(name) == 0) {
			return &commands[i];
		}
	}

	return nullptr;
}

void Console::initDefaultCommands() {
	addCommand(Console::Command("help", "prints out these lines or shows the usage of a particular command, type 'list' to show all commands", {
			Console::Command::Argument("commandName", "show the usage of this command", "")
		}, [this](std::vector<std::string> argvals) -> std::string {
		if (argvals.size() > 0) {
			Command *com = getCommand(argvals[0]);
			if (com == nullptr) { return "error: command '" + argvals[0] + "' not found"; }

			flog(com->toString());
		}

		//std::cout << "echo" << argvals.size() << "!\n";
		return "";
	}));

	addCommand(Console::Command("echo", "print a message on the console", {
			Console::Command::Argument("str", "the string to echo", "")
		}, [this](std::vector<std::string> argvals) -> std::string {
		if (argvals.size() > 0)
			flog(argvals[0]);

		//std::cout << "echo" << argvals.size() << "!\n";
		return "";
	}));

	addCommand(Console::Command("list", "list all possible commands", {},
		[this](std::vector<std::string> argvals) -> std::string {
		for (int i = 0; i < commands.size(); i++) {
			flog(commands[i].toString());
			flog("-----------------------------------");
		}
		return "";
	}));

	addCommand(Console::Command("clear", "clears all printed messages from the console", {},
		[this](std::vector<std::string> argvals) -> std::string {
		logs.clear();
		return "";
	}));

	addCommand(Console::Command("for", "repeats command n times, replaces every ' counterName ' (removes the spaces) with the actual value of the counter", {
			Console::Command::Argument("counterName", "the name of the counter"),
			Console::Command::Argument("begin", "initial value of the counter"),
			Console::Command::Argument("end", "final value of the counter"),
			Console::Command::Argument("inc", "how much to increment the counter every loop"),
			Console::Command::Argument("commands : array", "array of commands")
		},
		[this](std::vector<std::string> argvals) -> std::string {
		
		if (argvals.size() < 5) return "error: not enough arguments";

		/*std::cout << "argvals\n";
		for (int i = 0; i < argvals.size(); i++) {
			std::cout << argvals[i] << "\n";
		}*/

		int begin = atoi(argvals[1].c_str());
		int end = atoi(argvals[2].c_str());
		int inc = atoi(argvals[3].c_str());

		bool mustReplace = false;
		if (argvals.size() >= 6) {
			for (int i = 5; i < argvals.size() && !mustReplace; i++) {
				int idx = argvals[i].find(" " + argvals[0] + " ");
				mustReplace = idx != std::string::npos || argvals[i].compare(argvals[0]) == 0;
			}
			//std::cout << "mustreplace " << mustReplace << "\n";
		}

		for (int i = begin; i < end; i += inc) {
			std::vector<std::string> args(0);
			if (argvals.size() >= 6) {
				std::vector<std::string> targvals(argvals.begin() + 5, argvals.end());
				if (mustReplace) {
					for (int j = 0; j < targvals.size(); j++) {
						int idx = targvals[j].find(" " + argvals[0] + " ");
						if (idx != std::string::npos) {
							targvals[j].replace(idx, argvals[0].length() + 2, std::to_string(i));
						}
						else if (targvals[j].compare(argvals[0]) == 0) {
							targvals[j] = std::to_string(i);
							//std::cout << "found!" << targvals[j] << "\n";
						}
					}
				}
				/*std::cout << "targvals: \n";
				for (int i = 0; i < targvals.size(); i++) {
					std::cout << targvals[i] << "\n";
				}
				std::cout << "-\n";*/
				args = targvals;
			}

			/*std::cout << "args: \n";
			for (int i = 0; i < args.size(); i++) {
				std::cout << args[i] << "\n";
			}
			std::cout << "--\n";*/

			std::string result = queryCommand(argvals[4], args);
			if (result.length() > 0) return result;
		}

		return "";
	}));

	addCommand(Console::Command("startinfo", "prints the first message that appears when starting a session", {},
	[this](std::vector<std::string> argvals) -> std::string {
		std::time_t t = std::time(0);
		struct tm now;
		localtime_s(&now, &t);

		std::stringstream ss;
		ss << "<Parrlib OpenGL/C++ " << glGetString(GL_VERSION) << " (GLFW), session started " <<
			(now.tm_year + 1900) << "/" << (now.tm_mon + 1) << "/" << now.tm_mday << " " <<
			now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << ">\n" <<
			"OpenGL version: " << glGetString(GL_VERSION) << "\n" <<
			"OpenGL vendor: " << glGetString(GL_VENDOR) << "\n" <<
			"OpenGL renderer: " << glGetString(GL_RENDERER) << "\n" <<
			"GLFW version: " << glfwGetVersionString() << "\n" <<
			"this library (Parrlib) was developed by Alessandro Parrotta (alessandro.parrott@gmail.com)\n" <<
			"type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands\n";

		//sometimes flog causes a stack overflow with this string as param, could only be the hour in this case
		//cause that was the only thing changing
		//example with this:

		/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/7 23:54:30>
		type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/

		/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/8 1:48:30>
	type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/

	/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/8 23:6:32>
type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/

/*ss << "<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/7 23:54:30>\n"<<
	"type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands\n";*/

		std::cout << ss.str();
		return ss.str();
	}));
}

void Console::init() {
	inputField = TextField("", 1.0f, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	//inputField.inputLayer = inputLayer;
	//inputField.disabled = false;
	//inputField.active = true;
	inputField.label.setMaxTextScale(1.0f);
	/*inputField.setChangeCallback([this] {
		std::cout << "asd" << prevInputLog << "\n";
		prevInputLog = inputField.getText();
	});*/
	inputField.action = [&] {
		if (inputField.getText().length() > 0) {
			if (inputLogs.size() == 0 || inputField.getText().compare(inputLogs.back()) != 0) {
				inputLogs.push_back(inputField.getText());
				if (inputLogs.size() > 20) {
					int toremove = inputLogs.size() - 20;
					inputLogs.erase(inputLogs.begin(), inputLogs.begin() + toremove);
				}
			}
			curInputLog = inputLogs.size();

			flog(inputField.getText());

			std::string commandName = "";
			std::stringstream ss;
			ss << inputField.getText();

			ss >> commandName;

			std::vector<std::string> args(0);
			while (ss) {
				std::string arg = "";
				
				int prevg = ss.tellg();
				ss >> arg;
				if (arg.length() > 0) {
					//std::cout << "arg " << arg << "\n";
					if (arg[0] == '\'') {
						//std::cout << "arg0 == '\n";
						std::string ssstr = ss.str();
						std::string str = ssstr.substr(prevg, ssstr.length() - prevg);
						//std::cout << "rem " << str << "\n";

						int initCut = 0;
						while (initCut < str.length() && str[initCut] != '\'') {
							initCut++;
						}
						//std::cout << "initCut " << initCut << "\n";
						if (initCut >= str.length()-1) continue;

						int cutPoint = initCut+1;
						while (cutPoint < str.length() && str[cutPoint] != '\'') {
							if (str[cutPoint] == '\\' && cutPoint + 1 < str.length() && str[cutPoint + 1] == '\''){
								str.erase(cutPoint, 1);
								//std::cout << "erase\n";
							}

							cutPoint++;
						}
						if (cutPoint > str.length()) continue;

						//std::cout << "cutPoint " << cutPoint << "\n";
						std::string combinedArg = str.substr(initCut+1, cutPoint-initCut-1);
						
						//std::cout << "combined " << combinedArg << "\n";
						args.push_back(combinedArg);
						ss.seekg(prevg + cutPoint+1);
					}
					else {
						args.push_back(arg);
					}
				}
			}

			
			std::string result = queryCommand(commandName, args);
			if (result.length() > 0) {
				flog(result);
			}

			inputField.setText("");
			slider = 0.0f;
		}
	};

	std::time_t t = std::time(0);
	struct tm now;
	localtime_s(&now, &t);

	std::stringstream ss;
	ss <<	"<Parrlib OpenGL/C++ " << glGetString(GL_VERSION) << " (GLFW), session started " << 
			 (now.tm_year + 1900) << "/" << (now.tm_mon + 1) << "/" << now.tm_mday << " " <<
			  now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << ">\n"<<
			"OpenGL version: "	<< glGetString(GL_VERSION)	<< "\n" <<
			"OpenGL vendor: "	<< glGetString(GL_VENDOR)	<< "\n" <<
			"OpenGL renderer: " << glGetString(GL_RENDERER) << "\n" <<
			"GLFW version: "	<< glfwGetVersionString()	<< "\n" <<
			"this library (Parrlib) was developed by Alessandro Parrotta (alessandro.parrott@gmail.com)\n"<<
			"type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands\n";

	//sometimes flog causes a stack overflow with this string as param, could only be the hour in this case
	//cause that was the only thing changing
	//example with this:

	/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/7 23:54:30>
	type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/
	
	/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/8 1:48:30>
type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/

	/*<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/8 23:6:32>
type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands*/
	
	/*ss << "<Parrlib OpenGL/C++ 4.6.0 NVIDIA 419.67 (GLFW), session started 2019/4/7 23:54:30>\n"<<
		"type 'help <commandName>' to show the usage of a command, or 'list' to show all possible commands\n";*/

	std::cout << ss.str();

	flog(ss.str());
}

void Console::setColor(Vector4f color) {
	if (this->color != color) {
		this->color = color;
		//inputField.setColor(color);
	}
}

Console::Console(){
	init();

	setPos(pos);
	setSize(size);
	setColor(color);
}
Console::Console(TextRendererOld &txr, Vector2f pos, Vector2f size) : Console()  {
	this->txr = txr;
	setPos(pos);
	setSize(size);
	setColor(color);
}
Console::Console(TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f color) : Console() {
	this->txr = txr;
	setPos(pos);
	setSize(size);
	setColor(color);
}


Console::~Console()
{
}

void Console::addCommand(Command command) {
	commands.push_back(command);
}

std::string Console::queryCommand(std::string commandName, std::vector<std::string> args) {
	/*std::cout << "args size " << args.size() << "\n";
	for (int i = 0; i < args.size(); i++) {
		std::cout << args[i] << " " << args[i].length() << "\n";
	}*/
	Command* selected = getCommand(commandName);
	if (selected == nullptr) return "error: command \'" + commandName + "\' not found";

	if (selected->args.size() > 0 && args.size() == 0) {
		flog("usage:\n" + selected->toString());
	}

	std::string result = selected->action(args);
	if (result.length() > 0) {
		flog(result);
	}
	//std::cout << result << "\n";

	return "";
}

void Console::flogOld(std::string str) {
	
	if (str.length() <= 0)return;

	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '\t') {
			str.replace(i, 1, "    ");
		}
	}

	int cutPoint = str.length();
	for (int i = 0; i < str.length() && cutPoint==str.length(); i++) {
		if (str[i] == '\n')cutPoint = i;
	}

	if (txr.getStringBounds(str, 1.0f).size.x > 2.0f) {
		int inPoint = 0;
		while (inPoint < str.length() && txr.getStringBounds(str.substr(0, inPoint), 1.0f).size.x < 2.0f) {
			inPoint++;
		}
		inPoint = max(inPoint - 1, 0);

		if (inPoint < cutPoint) {
			str.insert(inPoint, "\n");
			flog(str);
			return;
		}
	}

	logs.push_back(str.substr(0, cutPoint));

	if (cutPoint == str.length()) return;

	if (cutPoint < str.length()) flog(str.substr(cutPoint+1, str.length() - cutPoint-1));
}

void Console::flog(std::string str) {
	if (str.compare("") == 0) return;

	std::vector<std::string> arr = stru::toLines(str);

	for (auto& s : arr) while (s.find("\t") != std::string::npos) s.replace(s.find("\t"), 1, "    ");

	for (auto& s : arr) logs.push_back(s);

	//std::stringstream logstr;
	//logstr << str;

	//while (logstr) {
	//	std::string line = "";
	//	std::getline(logstr, line);

	//	for (int i = 0; i < line.length(); i++) if (line[i] == '\t') line.replace(i, 1, "    ");

	//	while (line.compare("") != 0) {
	//		std::string slicedLine = line;
	//		if (txr.getStringBounds(line, 1.f).size.x > 2.f) {
	//			while (txr.getStringBounds(slicedLine, 1.f).size.x > 2.f) {
	//				slicedLine = slicedLine.substr(0, slicedLine.length() - 1);
	//			}
	//		}
	//		logs.push_back(slicedLine);

	//		if (slicedLine.length() == line.length()) line = "";
	//		else line = line.substr(slicedLine.length(), line.length() - slicedLine.length());
	//	}
	//}

	//logstr.clear();
	//logstr.str("");
}

void Console::setPos(Vector2f pos) {
	if (this->pos != pos) {
		UIElement::setPos(pos);
		inputField.setPos(pos);
	}

}

Vector2f Console::getPos() {
	return pos;
}

void Console::setSize(Vector2f size) {
	if (this->size != size) {
		UIElement::setSize(size);
		std::cout << "size!\n";
		inputField.setSize(Vector2f(size.x, ((float)(txr.fontSize+15)/ (float)size.y)*2.0f));
	}
}

Vector2f Console::getSize() {
	return size;
}

void Console::update() {
	UIElement::update();
	
	inputField.update();

	bool changeInputLog = false;
	if (input::getKeyDown(GLFW_KEY_UP, inputField.inputLayer)) {
		if (curInputLog == inputLogs.size()) prevInputLog = inputField.getText();
		curInputLog--;
		changeInputLog = true;
	}
	if (input::getKeyDown(GLFW_KEY_DOWN, inputField.inputLayer)) {
		curInputLog++;
		changeInputLog = true;
	}

	if (changeInputLog) {
		curInputLog = min(max(curInputLog, 0), inputLogs.size());
		if (curInputLog >= 0 && curInputLog < inputLogs.size()) {
			inputField.setText(inputLogs[curInputLog]);
		}
		else {
			inputField.setText(prevInputLog);
		}
		inputField.setCurIndex(inputField.getText().length());
	}
	
	unsigned int key = input::pollTextKey();
	if (key == GLFW_KEY_PAGE_UP || input::getKeyDown(GLFW_KEY_PAGE_UP, inputField.inputLayer) || input::getKeyDown(GLFW_KEY_PAGE_UP) || input::getMouseScrollWheel() > 0) {
		slider += 0.1f;
	}

	if (key == GLFW_KEY_PAGE_DOWN || input::getKeyDown(GLFW_KEY_PAGE_DOWN, inputField.inputLayer) || input::getKeyDown(GLFW_KEY_PAGE_DOWN) || input::getMouseScrollWheel() < 0) {
		slider -= 0.1f;
	}

	slider = max(slider, 0.0f);
}

void Console::draw() {
	if (log.str().length() > 0) {
		flog(log.str());
		log.str("");
		log.clear();
	}

	txr.customOrtho = true;
	txr.ortho = OrthoCoord(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
	txr.ortho.calcScale(size.x, size.y);
	for (int i = 0; i < logs.size(); i++) {
		float ypos = -1.0f + inputField.getSize().y + 0.05f +
					(txr.fontSize / (float)size.y)*2.0f*(logs.size() - 1 - i) -
					slider;

		if (ypos < -1.0f) break;
		if (ypos >= pos.y + size.y) continue;

		txr.drawString(logs[i], Vector2f(-1.0f + 0.01f, ypos), 1.0f);
	}

	inputField.draw();
}