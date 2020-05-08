#pragma once

#include "..\Context.h"
#include "..\debug.h"
#include "..\OldUI\Label.h"
#include "..\OldUI\Button.h"
#include "..\OldUI\TextField\TextFieldInteger.h"
#include "..\OldUI\CheckBox.h"
#include "../OldUI\Selector.h"
#include "../OldUI/UIList.h"

#include  "Console.h"

namespace DebugMenuold {
	extern TextRendererOld txr;
	extern bool resChangeMode;

	extern float contentAreaWidth;
	extern float contentHeight;

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
