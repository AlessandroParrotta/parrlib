#pragma once

#include <unordered_map>

#include "UIElement.h"

class UIList : public UIElement {
	protected:
		std::unordered_map<std::string, std::shared_ptr<UIElement>> elems;
		float elemHeight = 0.1f;
		float elemWidth = 0.6f;

		float spacing = 0.f;
		float padding = 0.f;

	public:

		UIList();
		UIList(vec2 pos, vec2 size);
		UIList(vec2 pos, vec2 size, vec2 elemSize);

		void set(std::string id, std::shared_ptr<UIElement> elem);
		void add(std::string id, std::shared_ptr<UIElement> elem);
		
		template <class T>
		std::shared_ptr<T> get(std::string id) {
			if (elems.find(id) == elems.end()) return nullptr;
			return std::reinterpret_pointer_cast<T>(elems[id]);
		}

		virtual void resize();
		virtual void refresh();

		virtual void update();
		virtual void draw();
		virtual void altDraw();

		virtual void setPos(vec2 pos);
		virtual void setSize(vec2 size);

		virtual void setElemHeight(float elemHeight);
		virtual float getElemHeight();

		virtual void setElemWidth(float elemWidth);
		virtual float getElemWidth();

		virtual void setSpacing(float spacing);
		virtual float getSpacing();

		virtual void setPadding(float padding);
		virtual float getPadding();

		std::shared_ptr<UIElement> operator[](std::string);
};

