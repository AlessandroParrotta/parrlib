#pragma once

#include <algorithm>

#include "..\MyContext.h"
#include "..\Debug.h"
#include "..\UI\Label.h"
#include "..\UI\Button.h"
#include "..\UI\TextField\TextFieldInteger.h"
#include "..\UI\CheckBox.h"
#include  "..\UI\Selector.h"

#include  "Console.h"

namespace DebugDisplay {
	extern TextRenderer txr;
	extern bool resChangeMode;

	extern float contentAreaWidth;
	extern float contentHeight;

	extern std::vector<std::vector<std::vector<std::shared_ptr<UIElement>>>> pageContents;
	void addDebugPageContent(std::string desc, std::vector<std::shared_ptr<UIElement>> elem);

	void addConsoleCommand(Console::Command command);

	void startResizing();
	void resize();
	void endResizing();

	void onOpen();
	void onClose();

	void init();

	void update();

	void draw();
}
