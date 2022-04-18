#pragma once

#include <parrlibcore/matrix3f.h>
#include "../globals.h"

class Sprite;

namespace prb {
	/*
		Immediate UI, a simple wrapper for drawing and managing ui elements in a very flexible way
		I would advise against using this system for 'serious' GUIs as it's not optimized for that task
	*/
	namespace imui {
		//structure that holds information about imui state machine
		struct statec {
			std::string atlas = "sprites/atlas.png";				//current sprite atlas
			int layout = 0;					//current layout
			bool manualLayoutOverride = false;//override flag in case layout != 0 but the user still sets coordinates manually
			vec2 nextPos = 0.f, nextSize = 0.f;
			vec2 pos = 0.f;					//position of the next element
			vec2 size = 0.f;				//size of the next element
			vec2 offset = 0.f;				//how much to move the next element
			aabb2 bound = 0.f;				//bound of the next element

			vec2 stroffset = 0.f;			//string offset
			vec2 strbound = 0.f;			//string bound (relative to NDC of ui bound -> means {-1,1} is equal to {-bound.size, bound.size})
			std::wstring modstr = L"...";

			bool indropmenu = false;
			std::string dropmenuid = "";
			aabb2 currentDropMenuBB = 0.f;	//the bounding box of the current active drop menu opened, it's for saving it later inside the drop menu struct itself
		};
		//current state of the imui state machine
		extern statec state;

		static const int NOLAYOUT = 0;
		static const int HORIZONTAL = 1;
		static const int VERTICAL = 2;
		static const int INCREMENTAL = 3;
		static const int CONTAINER = 4;

		void setLayout(int layout);
		int getLayout();

		void setPos(vec2 const& pos);
		void setSize(vec2 const& size);
		void move(vec2 const& delta);

		void setOffset(vec2 const& offset);

		void layoutSkip();

		void useAtlas(std::string const& name);

		void setSpace(mat3 const& sp);
		mat3 getSpace();

		void setTxr(std::vector<std::string> const& fonts, int const& fontSize);
		void setTxr(std::string const& font, int const& fontSize);
		TextRenderer& getTxr();

		void setTextColor(vec4 color);
		void setTextOutlineColor(vec4 color);
		vec4 getTextColor();
		vec4 getTextOutlineColor();

		bool button(std::wstring const& text);
		bool button(std::wstring const& text, vec2 const& pos, vec2 const& size);

		bool groupMenu(Sprite const& s, mat3 const& tr);
		bool groupMenu(std::string const& name, mat3 const& tr);

		void beginContainer(std::string const& id, aabb2 const& bb);
		void endContainer();

		/*
			returns:
				1 when focused
				2 on start focusing (when clicking on the textfield for the first time)
				3 when enter is pressed (once) or on end focusing (when clicking away from the textfield and it being still active)
				4 when the textfield is modified (entering or removing a character/s)
				0 otherwise

			if 'text' is null nothing will be drawn
			modstr is what to put at the end of the string when it doesn't fit the textfield (default is "...")
		*/
		int textField(std::string const& identifier, std::wstring const& hintText, std::wstring* text);
		int textField(std::string const& identifier, std::wstring const& hintText, std::wstring const& modstr, vec2 const& pos, vec2 const& size, std::wstring* text);

		int textFieldf(std::string const& identifier, float* val);
		int textFieldf(std::string const& identifier, vec2 const& pos, vec2 const& size, float* val);

		int textFieldi(std::string const& identifier, int* val);
		int textFieldi(std::string const& identifier, vec2 const& pos, vec2 const& size, int* val);

		int textFieldd(std::string const& identifier, double* val);
		int textFieldd(std::string const& identifier, vec2 const& pos, vec2 const& size, double* val);

		void label(std::wstring const& text);
		void label(std::wstring const& text, vec2 const& pos);

		//difference from label is mainly that the bound is relative to state.size instead of ui space coordinates
		void labelBound(std::wstring const& text, aabb2 const& bound, vec2 const& offset);
		void labelBound(std::wstring const& text, aabb2 const& bound);
		void labelBound(std::wstring const& text, vec2 const& offset);
		void labelBound(std::wstring const& text);

		/*
			returns:
				1 if being modified
				2 once on start modifying
				3 once on end modifying
				0 otherwise

		*/
		int slideri(std::string const& identifier, int* val, int minval, int maxval, bool textfield);
		int slideri(std::string const& identifier, vec2 const& pos, vec2 const& size, int* val, int minval, int maxval, bool textfield);

		int sliderd(std::string const& identifier, double* val, double minval, double maxval, bool textfield);
		int sliderd(std::string const& identifier, vec2 const& pos, vec2 const& size, double* val, double minval, double maxval, bool textfield);

		int sliderf(std::string const& identifier, float* val, float minval, float maxval, bool textfield);
		int sliderf(std::string const& identifier, vec2 const& pos, vec2 const& size, float* val, float minval, float maxval, bool textfield);

		/*
			fast drop menu (does not support multiple layers, use beginDropMenu() and endDropMenu() for that)
			opened body will always be on top of any other element independent of drawing order
			returns:
				if an option is selected, its index as it is in the array [ 0, opts.size() )
				-1 otherwise
		*/
		int dropMenu(std::string const& identifier, std::wstring const& text, std::vector<std::wstring> const& opts);
		int dropMenu(std::string const& identifier, std::wstring const& text, vec2 const& pos, vec2 const& size, std::vector<std::wstring> const& opts);

		/*
			default opt can be -1 to signal no default option (forcing the user to choose)
			opened body will always be on top of any other element independent of drawing order
			returns:
				if an option is selected, its index as it is in the array [ 0, opts.size() )
				-1 otherwise
		*/
		int comboBox(std::string const& identifier, std::wstring const& hintText, std::vector<std::wstring> const& opts, int const& defaultOpt = -1);
		int comboBox(std::string const& identifier, std::wstring const& hintText, vec2 const& pos, vec2 const& size, std::vector<std::wstring> const& opts, int const& defaultOpt = -1);

		int checkBox(std::string const& identifier, bool* val);
		int checkBox(std::string const& identifier, vec2 const& pos, vec2 const& size, bool* val);

		/*
			drop down menu
			body contents are not guaranteed to be on top of other elements, so drawing order matters
			returns:
				true if the drop down menu is opened
				false otherwise
			always call endDropMenu after beginDropMenu if that menu is opened
		*/
		bool beginDropMenu(std::string const& identifier, std::wstring const& text);
		bool beginDropMenu(std::string const& identifier, std::wstring const& text, vec2 const& pos, vec2 const& size);

		void endDropMenu(); //always call this after a beginDropMenu call!

		void reset();

		void init();
	}

}
