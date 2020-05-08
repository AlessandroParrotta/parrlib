#pragma once

#include "../OldUI/UIElement.h"
#include "../OldUI/Label.h"

namespace PageSystem {
	TextRendererOld *txr;

	float rowStart = 0.2f;
	float rowHeight = .02f;
	float halfPoint = .0f;

	struct Row {
		vec2 pos, size;
		Label name;
		std::vector<std::shared_ptr<UIElement>> elems;

		Row(std::string name, std::vector<std::shared_ptr<UIElement>> elems, vec2 pos, vec2 size) : elems(elems) {
			this->name = Label(name, *txr, 0.f,0.f);
			this->name.setMaxTextScale(1.f);
			this->pos = pos;
			this->size = size;
			refresh();
		}

		Row(std::string name, std::shared_ptr<UIElement> elem, vec2 pos, vec2 size) : Row(name, std::vector<std::shared_ptr<UIElement>>{elem}, pos, size) {
			
		}

		template<class elemType>
		std::shared_ptr<elemType> getElem(int i) {
			return std::static_pointer_cast<elemType>(elems[i]);
		}

		void refresh() {
			name.setSize(vec2(halfPoint-pos.x, size.y));
			name.setPos(vec2(halfPoint-name.getSize().x, pos.y));
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->setSize(vec2(pos.x+size.x-halfPoint, size.y));
				elems[i]->setPos(vec2(halfPoint, pos.y));
			}
		}

		void update() {
			name.update();
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->update();
			}
		}

		void draw() {
			name.draw();
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->draw();
			}
		}
	};
	
	struct Page {
	public:
		std::string title;
		Page(std::string title) : title(title) {}
		virtual void refresh() {}
		virtual void onOpen() {}
		virtual void onClose() {}

		virtual void update() {}
		virtual void draw() {}
	};

	struct PageRows : public Page {
	public:
		std::vector<std::shared_ptr<Row>> elems;

	
		PageRows(std::string title, std::vector<std::shared_ptr<Row>> elems) : Page(title) {
			this->elems = elems;
		}

		void addRow(std::string rowName, std::vector<std::shared_ptr<UIElement>> elems) {
			std::shared_ptr<Row> row = std::make_shared<Row>(rowName, elems,0.f,0.f);
			row->pos = vec2(-2.f* prc::orthoAspect, 2.f-rowStart);
			row->size = vec2(2.f,rowHeight);
			row->refresh();
			this->elems.push_back(row);
		}

		void update() {
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->update();
			}
		}
		void draw() {
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->draw();
			}
		}
	};

	struct PageElems : public Page {
	public:
		std::vector<std::shared_ptr<UIElement>> elems;
	
		PageElems(std::string title, std::vector<std::shared_ptr<UIElement>> elems) : Page(title) {
			this->elems = elems;
		}

		template <class elemType>
		std::shared_ptr<elemType> getElem(int i) {
			return std::static_pointer_cast<elemType>(elems[i]);
		}

		void update() {
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->update();
			}
		}
		void draw() {
			for (int i = 0; i < elems.size(); i++) {
				elems[i]->draw();
			}
		}
	};
}