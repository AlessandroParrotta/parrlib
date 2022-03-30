#pragma once

#include "../ui/imui.h"

namespace prb {
	namespace debugmenu {
		extern bool enabled;

		void init();
		void update();
		void dispose();
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace dbm = debugmenu;
#endif
}
