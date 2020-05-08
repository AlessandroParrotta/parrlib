#include "Selector.h"


void Selector::refresh() {
	txf.setTextScale(0.8f);
	description.setTextScale(txf.label.getTextScale());
	description.setPos(txf.label.getPos());
	description.setSize(txf.label.getSize());
	description.setColor(txf.label.getColor() * Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	if (txf.getText().compare("") == 0) {
		if (selectedElement >= 0) {
			description.setText(elements[selectedElement].text);
		}
		else {
			description.setText("Select an option...");
		}
	}
	else {
		description.setText("");
	}

	for (int i = 0; i < filteredButtons.size(); i++) {
		Vector2f tpos = pos + Vector2f(0.0f, -size.y - size.y*i);
		
		filteredButtons[i]->setPos(tpos);
		filteredButtons[i]->setSize(size);
		filteredButtons[i]->setTextScale(txf.getTextScale()*0.8f);
		filteredButtons[i]->refresh();
	}

	symbolSize = Vector2f(size.x*symbolOff, size.y);
	symbolPos = pos + Vector2f(size.x-symbolSize.x, 0.0f);

	txf.setSize(Vector2f(size.x - size.x*symbolOff, size.y));
	txf.refresh();

	/*if (txf.label.getText().length() == 0) {
		txf.label.setColor(labelCol / Vector4f(1.0f, 1.0f, 1.0f, 2.0f));
		txf.label.setText("Select an option...");
	}
	else {
		txf.label.setColor(labelCol);
	}*/

	symbolScale = std::fmin(txf.label.getTextScale(), txf.txr->getScaleToFit(std::string("v"), symbolSize));
	symbolBound = txf.txr->getStringBounds(std::string("v"), symbolScale);
}

Selector::Selector()
{
}


Selector::Selector(std::string text, std::vector<Element> elements, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void(std::string)> action) : UIElement(pos, size, backgroundColor) {
	this->elements = elements;
	txf = TextField(text, 0.7f, txr, pos, size, backgroundColor, textColor, [] {});
	txf.setChangeCallback([this] {
		filteredButtons.clear();
		for (int i = 0; i < buttons.size(); i++) {
			bool canFilter = true;

			std::stringstream ss;
			ss << txf.getText();
			while (ss && canFilter) {
				std::string w;
				ss >> w;
				if (!(buttons[i]->label.getText().find(w) != std::string::npos)) {
					//debug::println(w + " not found for " + buttons[i]->label.getText());
					canFilter = false;
				}
			}

			if (canFilter) {
				filteredButtons.push_back(buttons[i]);
			}
		}
		refresh();
	});

	description = Label(txr);
	description.checkOverlappingElems = false;
	description.setAlignment(Label::Alignment::LEFT);

	labelCol = txf.label.getColor();

	this->action = action;

	for (int i = 0; i < elements.size(); i++) {
		buttons.push_back(new Button(elements[i].text, txf.label.getTextScale(), txr, pos, size, backgroundColor, textColor, [this, i] {
			//this->txf.setText(this->buttons[i]->label.getText());
			this->txf.setText("");
			selectedElement = i;

			filteredButtons.clear();
			for (int i = 0; i < buttons.size(); i++) {
				filteredButtons.push_back(buttons[i]);
			}
			refresh();

			//buttons[i]->action = elements[i].action;
			//buttons[i]->action();
			this->elements[i].action();

			this->action(this->txf.getText());

			open = false;
		}));
		buttons[i]->inputLayer = this->inputLayer;
		//buttons[i]->action = elements[i].action;

		filteredButtons.push_back(buttons[buttons.size() - 1]);
	}

	refresh();

	txf.getChangeCallback()();

	txf.inputTag = inputTag;
}

Selector::Selector (std::string text, std::vector<Element> elements, TextRendererOld& txr, std::function<void(std::string)> action) : Selector(text, elements, txr, 0.f, 0.f, vColor4f::gray, vColor4f::black, action) {}



Selector::~Selector()
{
}

void Selector::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		txf.setPos(pos);

		refresh();
	}
}

void Selector::setSize(Vector2f size) {
	this->size = size;
	refresh();
}

void Selector::setSelectedElement(int selectedElement, bool invokeAction) {
	if (this->selectedElement != selectedElement) {
		this->selectedElement = selectedElement;

		if (invokeAction) {
			elements[selectedElement].action();
			action(elements[selectedElement].text);
			refresh();
		}
	}
}

void Selector::setSelectedElement(int selectedElement) {
	setSelectedElement(selectedElement, true);
}

int Selector::getSelectedElementIdx() {
	return selectedElement;
}

Selector::Element &Selector::getSelectedElement() {
	return elements[selectedElement];
}

Selector::Element &Selector::getElement(int elem) {
	return elements[elem];
}

void Selector::update(){
	UIElement::update();

	if (input::getMouseDown(0, inputLayer)) {
		if (mouseInside) {
			open = true;
		}
		else {
			if (open) {
				bool noChilds = false;
				for (int i = 0; i < filteredButtons.size() && !noChilds; i++) {
					noChilds = input::getMouseInQuad(filteredButtons[i]->getPos(), filteredButtons[i]->getSize());
				}

				open = noChilds;
			}
		}
	}

	if (open) {
		for (int i = 0; i < filteredButtons.size(); i++) {
			filteredButtons[i]->update();
		}
	}

	txf.update();
	description.update();
}

void Selector::draw() {
	drawBackground(this);

	txf.draw();
	description.draw();

	util::setColor4f(color.colDiv(1.1f).colDiv(!txf.isActive() && mouseInside ? (input::getMouse(0) ? (1.5f) : (1.1f)) : (1.f)));
	util::drawQuad(symbolPos, symbolSize);

	Vector4f col = labelCol;
	glColor4f(col.x, col.y, col.z, col.w);
	txf.txr->drawString(std::string("v"), symbolPos + symbolSize / 2.0f - symbolBound.size / 2.0f + symbolBound.offset, symbolScale);
	
	if (disabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		util::drawQuad(pos, size);
		glDisable(GL_BLEND);
	}
}

void Selector::altDraw() {
	if (open) {
		for (int i = 0; i < filteredButtons.size(); i++) {
			filteredButtons[i]->draw();
		}
	}
}

void Selector::dispose() {
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->dispose();
		delete buttons[i];
	}
}