#pragma once

#define NOMINMAX
#include <algorithm>

#include "..\Context.h"
#include "..\debug.h"
#include "..\OldUI\Label.h"
#include "..\OldUI\Button.h"
#include "..\OldUI\TextField\TextFieldInteger.h"
#include "..\OldUI\CheckBox.h"
#include "..\OldUI\Selector.h"

#include  "Console.h"

namespace DebugDisplay_Old {
	extern TextRendererOld txr;
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
