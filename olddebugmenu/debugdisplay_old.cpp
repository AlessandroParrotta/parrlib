#define NOMINMAX

#include "DebugDisplay_Old.h"

#define P_VIDEO		0

#define PV_GPUINFO		0
#define PV_RES			PV_GPUINFO+1
#define PV_XRES			PV_RES+1
#define PV_YRES			PV_XRES+1
#define PV_ASPECT		PV_YRES+1
#define PV_VSYNC		PV_ASPECT+1
#define PV_FPSCAP		PV_VSYNC+1
#define	PV_SCALING		PV_FPSCAP+1
#define	PV_FILTERING	PV_SCALING + 1

#define P_INPUT		P_VIDEO+1

#define P_DEBUG P_INPUT+1

#define PD_ENABLEDEBUG 0
#define PD_MAXSTRS PD_ENABLEDEBUG+1
#define PD_MAXRTSTRS PD_MAXSTRS+1
#define PD_ENABLEMSGMENU PD_MAXRTSTRS+1
#define PD_ENABLESAVING PD_ENABLEMSGMENU+1
#define PD_SAVESETTINGS PD_ENABLESAVING+1

#define P_CONSOLE	P_DEBUG+1

#define PC_CONSOLE 0

#include "PageSystem.h"	

namespace DebugDisplay_Old {
	std::vector<std::shared_ptr<PageSystem::Page>> tpages;

	void addPageRows(std::shared_ptr<PageSystem::PageRows> page) {
		tpages.push_back(page);
	}

	/*template <class pageType>
	std::shared_ptr<pageType> getPage(int i) {
		return std::dynamic_pointer_cast<pageType>(tpages[i]);
	}

	template <class pageType>
	std::shared_ptr<pageType> getPage(std::string name) {
		std::shared_ptr<pageType> elem = nullptr;
		for (int i = 0; i < tpages.size(); i++) {
			if (tpages[i]->title.compare(name) == 0) elem = tpages[i];
		}
		return elem;
	}*/

	/*
		usage example i'm trying to mimic in the code:
		addPage<PageRows>(...)
		getPage<PageRows>(i)->getElem<Console>()->...
	*/

	template <class elemType>
	std::shared_ptr<elemType> getElem(int pageID, int rowID) {
		if (std::dynamic_pointer_cast<PageSystem::PageRows>(tpages[pageID]) != nullptr) {
			std::cout << "page rows!\n";
			return (std::dynamic_pointer_cast<elemType>(std::static_pointer_cast<PageSystem::PageRows>(tpages[pageID])->elems[rowID]->elems[0]));
		}
		else if (std::dynamic_pointer_cast<PageSystem::PageElems>(tpages[pageID]) != nullptr) {
			std::cout << "page elems!\n";
			return (std::dynamic_pointer_cast<elemType>(std::static_pointer_cast<PageSystem::PageElems>(tpages[pageID])->elems[rowID]));
		}
		
		std::cout << "not found!\n";
		return nullptr;
	}

	TextRendererOld txr;
	bool resChangeMode = false;

	Vector2f actualPos;
	Vector2f actualSize;

	Vector2f rawMouse;
	Vector2f wantedSize;
	Vector2f wantedPos;

	Label debugMenuEnabled;
	Label appRes;
	Label desiredRes;
	Label windowRes;

	Shader blendSh;
	Shader fastGaussian;

	FrameBufferObject blurwrite, blurread;

	float zoom=1.0f;
	
	bool resizing = false;
	bool oldResizing = resizing;
	bool blurEnabled = true;

	int page=0, numPages = 4;
	std::vector<Button> pages;

	float contentHeight = 60.0f/1080.0f*2.0f;
	float contentOffset = 30.0f / 1080.0f*2.0f;
	float contentSpacing = 20.0f / 1080.0f*2.0f;
	float standardOffset = 5.0f / 1080.0f*2.0f;
	float labelHeight = 36.0f / 1080.0f*2.0f;
	float pageOffset = 10.0f / 1080.0f*2.0f;
	
	float contentAreaWidth = 0.7f;

	//0:pages   1:rows       2:row of contents
	std::vector<std::vector<std::vector<std::shared_ptr<UIElement>>>> pageContents;

	int selectedContent = -1;

	//page 'video'
	/*Label l_resolution, l_resxxresy;
	TextFieldInteger txi_xres, txi_yres;

	Label l_vSync;
	CheckBox cb_vSync;

	Label l_aspect;
	Selector s_aspect;

	Label l_filtering;
	Selector s_filtering;*/

	void initPageVideo();

	//page 'input'

	void initPageInput();

	void initPageDebug();

	void initPageConsole();

	void setPageContentsCoords();

	float menuAlpha = 0.2f;

	void startResizing() {

	}

	void resize() {
		//getElem<Console>(std::make_shared<Page>(), 0)->;

		//debugMenuEnabled.refresh();
		txr.ortho.calcScale(prc::windowSizeX, prc::windowSizeY);
		setPageContentsCoords();

		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->setVal(constants::WIDTH);
		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->setVal(constants::HEIGHT);
	}

	void endResizing() {
		blurwrite.resize(prc::windowSizeX / 8.0f, prc::windowSizeY / 8.0f);
		blurread.resize(prc::windowSizeX / 8.0f, prc::windowSizeY / 8.0f);
	}

	void init() {
		PageSystem::txr = &txr;

		std::shared_ptr<CheckBox> cxb = std::make_shared<CheckBox>(0.f,0.f,1.f);
		addPageRows(std::make_shared<PageSystem::PageRows>("asd", std::vector<std::shared_ptr<PageSystem::Row>>{
			std::make_shared<PageSystem::Row>("sad", cxb,0.f,0.f)
		}));

		//getPage<PageSystem::PageRows>(0)->getElem<CheckBox>("sad", 0)->checked;
		getElem<CheckBox>(0, 0)->checked = false;
		std::cout << "checked " << getElem<CheckBox>(0, 0)->getPos() << "\n";

		blurwrite = FrameBufferObject(prc::windowSizeX/8.0f, prc::windowSizeY / 8.0f);
		blurread = FrameBufferObject(prc::windowSizeX / 8.0f, prc::windowSizeY / 8.0f);

		blurwrite.setWrapMode(GL_CLAMP_TO_EDGE);
		blurread.setWrapMode(GL_CLAMP_TO_EDGE);

		blendSh = Shader("assets/shaders/standard.vert", "assets/shaders/debugMenuBlend.frag");
		fastGaussian = Shader("assets/shaders/standard.vert", "assets/shaders/fastGaussian.frag");

		txr = TextRendererOld(prc::ftLibrary, "C:/Windows/Fonts/segoeuil.ttf", 32);
		txr.color = Vector4f(0.0f,0.0f,0.0f,1.0f);
		txr.customOrtho = true;
		txr.ortho = OrthoCoord(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
		txr.ortho.calcScale(prc::windowSizeX, prc::windowSizeY);
		
		debugMenuEnabled = Label("", txr, Vector2f(), Vector2f());
		debugMenuEnabled.setMaxTextScale(1.0f);
		debugMenuEnabled.drawBackground = [](UIElement *caller) {
			if (resChangeMode) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
				util::drawQuad(debugMenuEnabled.getPos(), debugMenuEnabled.getSize());
				glDisable(GL_BLEND);
			}
		};

		pages = std::vector<Button>(0);
		for (int i = 0; i < numPages; i++) {
			pages.push_back(Button(std::to_string(i), txr, Vector2f(),  Vector2f()));
			pages[i].label.setMaxTextScale(1.0f);
			//pages[i].customBackground = true;
			pages[i].action = [i] {
				page = i;
				selectedContent = -1;
				onClose();
				onOpen();
			};
			pages[pages.size() - 1].drawBackground = [i](UIElement *caller) {
				if (page == i || pages[i].mouseInside) {
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
					if (pages[i].mouseInside) {
						if (page == i) {
							glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
							menuAlpha = 0.3f;
						}
						else {
							glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
							menuAlpha = 0.2f;
						}
					}
					else {
						if (page == i) {
							menuAlpha = 0.2f;
						}
					}
					if(pages[i].locked){
						glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
						if (page == i) {
							menuAlpha = 0.4f;
						}
					}

					util::drawQuad(pages[i].getPos(), pages[i].getSize());
					glDisable(GL_BLEND);
				}
			};

		}

		pages[0].label.setText("Video");
		pages[1].label.setText("Input");
		pages[2].label.setText("Debug");
		pages[3].label.setText("Console");

		initPageVideo();
		initPageInput();
		initPageDebug();
		initPageConsole();
		setPageContentsCoords();

		/*for (int i = 0; i < pageContents.size(); i++) {
			for (int j = 0; j < pageContents[i].size(); j++) {
				for (int k = 0; k < pageContents[i][j].size(); k++) {
					pageContents[i][j][k]->inputLayer = "debugMenu";
				}
			}
		}*/

		(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->getSelectedElement().action();

		appRes = Label("", txr, Vector2f(), Vector2f());
		appRes.drawBackground = [](UIElement *caller) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
			util::drawQuad(appRes.getPos(), appRes.getSize());
			glDisable(GL_BLEND);
		};

		desiredRes = Label("", txr, Vector2f(), Vector2f());
		desiredRes.drawBackground = [](UIElement *caller) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
			util::drawQuad(desiredRes.getPos(), desiredRes.getSize());
			glDisable(GL_BLEND);
		};

		windowRes = Label("", txr, Vector2f(), Vector2f());
		windowRes.drawBackground = [](UIElement *caller) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
			util::drawQuad(windowRes.getPos(), windowRes.getSize());
			glDisable(GL_BLEND);
		};
	}

	void update() {
		//resChangeMode = input::getKey(GLFW_KEY_LEFT_CONTROL, "debugMenu");

		//easy cheesy solution to mouseinquad
		input::getMouseInQuad(Vector2f(-10000.0f), Vector2f(20000.0f));

		if (resChangeMode) {
			if (input::getMouseScrollWheel() > 0) {
				zoom *= 2.0f;
			}
			else if (input::getMouseScrollWheel() < 0) {
				zoom /= 2.0f;
			}

			actualPos = Vector2f(0.0f - (constants::WIDTH / 2.0f)/((float)prc::windowSizeX), 0.0f - (constants::HEIGHT / 2.0f) / ((float)prc::windowSizeY));
			actualSize = Vector2f(constants::WIDTH / ((float)prc::windowSizeX)*2.0f, constants::HEIGHT / ((float)prc::windowSizeX)*2.0f);
			
			rawMouse = input::getRawMousePosInv();
			if (input::getKey(GLFW_KEY_LEFT_SHIFT, INPUT_LAYER_DEBUGMENU)) {
				float aspect = (float)constants::WIDTH / (float)constants::HEIGHT;
				//wantedSize = Vector2f(abs(rawMouse.x - prc::windowSizeX / 2.0f)*2.0f, abs(rawMouse.x - prc::windowSizeX / 2.0f)*2.0f*aspect);
				float dist = (Vector2f(prc::windowSizeX / 2.0f, prc::windowSizeY / 2.0f)-rawMouse).magnitude();
				float magn = Vector2f(prc::windowSizeX / 2.0f, prc::windowSizeY / 2.0f).magnitude();

				wantedSize = Vector2f(prc::windowSizeX, prc::windowSizeY) * (dist/magn);
			}
			else {
				wantedSize = Vector2f(abs(rawMouse.x-prc::windowSizeX/2.0f), abs(rawMouse.y - prc::windowSizeY / 2.0f))*2.0f;
			}
			
			wantedPos = Vector2f(0.0f - wantedSize.x / 2.0f, 0.0f - wantedSize.y / 2.0f);

			resizing = (input::getMouse(0) && prc::scalingMode != prc::SCALING_MODE::ONE_TO_ONE);

			if (!oldResizing && resizing) {
				prc::defStartResizing();
				prc::startResizing();
			}

			if (((int)wantedSize.x != constants::WIDTH || (int)wantedSize.y != constants::HEIGHT) && resizing){
				int maxX=std::max((int)wantedSize.x,1), maxY = std::max((int)wantedSize.y,1);
				prc::changeResolution(maxX, maxY);
				prc::contextFBO.resize(maxX, maxY);
				prc::defResize();
				prc::resize();

				(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->setSelectedElement(0);
				//debug::rtPrintln("resized!");
			}

			if (oldResizing && !resizing) {
				prc::defEndResizing();
				prc::endResizing();
			}
			oldResizing = resizing;
		}
		else {
			//std::cout << "ACTIVE LAUYER " << input::getActiveLayer() << "\n";
			for (int i = 0; i < pages.size(); i++) {
				pages[i].update();
			}

			for (int i = 0; i < pageContents[page].size(); i++) {
				if (i != selectedContent) {
					for (int j = 0; j < pageContents[page][i].size(); j++) {
						pageContents[page][i][j]->update();

						if (pageContents[page][i][j]->mouseInside) {
							selectedContent = i;
						}
					}
				}
			}

			if (selectedContent > -1) {
				for (int j = 0; j < pageContents[page][selectedContent].size(); j++) {
					pageContents[page][selectedContent][j]->update();
				}
			}

			/*if (page == P_CONSOLE) {
				(std::static_pointer_cast<Console>(pageContents[P_CONSOLE][0][0]))->inputField.update();
			}*/
			if (page == P_CONSOLE) {
				if (input::getMouseDown(0, INPUT_LAYER_DEBUGMENU)) {
					(std::static_pointer_cast<Console>(pageContents[P_CONSOLE][0][0]))->inputField.setActive(true);
				}
			}
		}
	}

	void renderResChange() {
		glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		Vector2f rActualSize = Vector2f(constants::WIDTH / (float)prc::windowSizeX * 2.0f, constants::HEIGHT / (float)prc::windowSizeY * 2.0f);
		Vector2f rActualPos = Vector2f(0.0f - rActualSize.x / 2.0f, 0.0f - rActualSize.y / 2.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		util::drawQuad(rActualPos, rActualSize, GL_LINE_LOOP);
		//txr.drawString("Application Resolution (" + std::to_string(constants::WIDTH) + "x" + std::to_string(constants::HEIGHT) + ")", Vector2f(rActualPos.x, rActualPos.y + rActualSize.y - prc::windowSizeY / txr.fontSize), 1.0f);
		appRes.setSize(Vector2f(rActualSize.x, std::min(rActualSize.y, txr.fontSize / (float)prc::windowSizeY * 2.0f)));
		appRes.setPos(Vector2f(rActualPos.x, rActualPos.y + rActualSize.y - std::min(rActualSize.y, prc::windowSizeY / (float)txr.fontSize)));
		appRes.setText("Application Resolution (" + std::to_string(constants::WIDTH) + "x" + std::to_string(constants::HEIGHT) + ")");
		appRes.draw();

		if (!resizing) {
			Vector2f rWantedSize = (wantedSize / Vector2f(prc::windowSizeX, prc::windowSizeY)) * 2.0f;
			Vector2f rWantedPos = Vector2f(0.0f - rWantedSize.x / 2.0f, 0.0f - rWantedSize.y / 2.0f);

			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			util::drawQuad(rWantedPos, rWantedSize, GL_LINE_LOOP);
			//txr.drawString("Desired New Resolution (" + std::to_string((int)wantedSize.x) + "x" + std::to_string((int)wantedSize.y) + ")", Vector2f(rWantedPos.x, rWantedPos.y + rWantedSize.y - prc::windowSizeY / txr.fontSize), 1.0f);
			desiredRes.setSize(Vector2f(rWantedSize.x, std::min(rWantedSize.y, txr.fontSize / (float)prc::windowSizeY * 2.0f)));
			desiredRes.setPos(Vector2f(rWantedPos.x, rWantedPos.y + rWantedSize.y - std::min(rWantedSize.y, prc::windowSizeY / (float)txr.fontSize)));
			desiredRes.setText("Desired New Resolution (" + std::to_string((int)wantedSize.x) + "x" + std::to_string((int)wantedSize.y) + ")");
			desiredRes.draw();
		}

		Vector2f rWindowSize = Vector2f(2.0f, 2.0f) * zoom;
		Vector2f rWindowPos = Vector2f(0.0f - rWindowSize.x / 2.0f, 0.0f - rWindowSize.y / 2.0f);

		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		util::drawQuad(rWindowPos, rWindowSize, GL_LINE_LOOP);
		//txr.drawString("Desired New Resolution (" + std::to_string((int)wantedSize.x) + "x" + std::to_string((int)wantedSize.y) + ")", Vector2f(rWantedPos.x, rWantedPos.y + rWantedSize.y - prc::windowSizeY / txr.fontSize), 1.0f);
		windowRes.setSize(Vector2f(2.0f * zoom, std::min(2.0f * zoom, txr.fontSize / (float)prc::windowSizeY * 2.0f)));
		windowRes.setPos(Vector2f(rWindowPos.x, std::min(rWindowPos.y + rWindowSize.y - std::min(rWindowSize.y, prc::windowSizeY / (float)txr.fontSize), debugMenuEnabled.getPos().y - txr.fontSize)));
		windowRes.setText("Window Resolution (" + std::to_string(prc::windowSizeX) + "x" + std::to_string(prc::windowSizeY) + ")");
		windowRes.draw();

		//debug::rtPrintln(zoom);
	}

	void renderBlurEffect() {
		glEnable(GL_TEXTURE_2D);
		blurread.bind();

		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glOrtho(0, prc::windowSizeX, 0, prc::windowSizeY, -1.0f, 1.0f);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		util::drawTexture(prc::contextFBO.getAttachment(GL_COLOR_ATTACHMENT0), prc::screenPosX, prc::screenPosY, prc::screenSizeX, prc::screenSizeY);

		blurread.unbind();

		//util::drawTexture(blurread.getAttachment(0), Vector2f(0.0f), Vector2f(prc::windowSizeX / 2.0f, prc::windowSizeY / 2.0f), 0.0f);

		int iters = 8;
		float radius = 10.0f;

		glActiveTexture(GL_TEXTURE0);

		fastGaussian.use();
		fastGaussian.setUniform("tex", 0);
		fastGaussian.setUniform("windowSize", vec2(prc::windowSizeX, prc::windowSizeY));
		fastGaussian.setUniform("screenSize", vec2(prc::screenSizeX, prc::screenSizeY));

		for (int i = 0; i < iters; i++) {
			fastGaussian.setUniform("direction", i % 2 == 0 ? Vector2f(0.0f, radius) : Vector2f(radius, 0.0f));
			blurwrite.bind();

			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glOrtho(0, prc::windowSizeX, 0, prc::windowSizeY, -1.0f, 1.0f);

			util::drawTexture(blurread.getAttachment(GL_COLOR_ATTACHMENT0), Vector2f(0.0f, 0.0f), Vector2f(prc::windowSizeX, prc::windowSizeY), 0.0f);
			
			blurwrite.unbind();

			FrameBufferObject tfbo = blurwrite;
			blurwrite = blurread;
			blurread = tfbo;
		}
		fastGaussian.release();

		util::drawTexture(blurread.getAttachment(GL_COLOR_ATTACHMENT0), Vector2f(prc::windowSizeX / 2.0f, 0.0f), Vector2f(prc::windowSizeX / 2.0f, prc::windowSizeY / 2.0f), 0.0f);
		glDisable(GL_TEXTURE_2D);
	}

	void draw() {

		if (resChangeMode) {
			renderResChange();
		}
		else {
			//glOrtho(0, prc::windowSizeX, 0, prc::windowSizeY, -1.0f, 1.0f);
			if (blurEnabled) {
				renderBlurEffect();
			}

			glEnable(GL_TEXTURE_2D);
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			util::drawTexture(blurread.getAttachment(GL_COLOR_ATTACHMENT0), Vector2f(-1.0f), Vector2f(2.0f), 0.0f);

			glDisable(GL_TEXTURE_2D);

			//util::setColor4f(vColor4f::red);
			//util::drawQuad(-1000.f, 2000.f);

			glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
				util::drawQuad(Vector2f(-1.0f), Vector2f(2.0f));
			glDisable(GL_BLEND);

			glEnable(GL_BLEND);
				glColor4f(1.0f, 1.0f, 1.0f, menuAlpha);
				util::drawQuad(Vector2f(-1.0f), Vector2f(2.0f, (debugMenuEnabled.getPos().y+1.0f - labelHeight - standardOffset)));
			glDisable(GL_BLEND);

			for (int i = 0; i < pages.size(); i++) {
				pages[i].draw();
			}

			for (int i = 0; i < pageContents[page].size(); i++) {
				//if (i != selectedContent) {
					for (int j = 0; j < pageContents[page][i].size(); j++) {
						pageContents[page][i][j]->draw();
					}
				//}
			}

			for (int i = 0; i < pageContents[page].size(); i++) {
				for (int j = 0; j < pageContents[page][i].size(); j++) {
					pageContents[page][i][j]->altDraw();
				}
			}

			//if (selectedContent > -1) {
			//	for (int j = 0; j < pageContents[page][selectedContent].size(); j++) {
			//		pageContents[page][selectedContent][j]->draw();
			//	}
			//}
		}

		//txr.drawString("debug Menu Enabled" + (resChangeMode  ? std::string(" (Resolution Changing Mode)")  : ""), Vector2f(prc::windowSizeX/2.0f-100.0f, prc::windowSizeY - prc::windowSizeY / txr.fontSize), 1.0f);
		debugMenuEnabled.setText("Debug Menu (F1) " + (resChangeMode ? std::string(" (Resolution Changing Mode)") : ""));
		debugMenuEnabled.setSize(Vector2f(2.0f*0.7f, labelHeight));
		debugMenuEnabled.setPos(Vector2f(0.0f - debugMenuEnabled.getSize().x / 2.0f, 1.0f - debugMenuEnabled.getSize().y - standardOffset));
		debugMenuEnabled.refreshTextBounds();
		debugMenuEnabled.draw();

		//util::drawTexture(blurwrite.getAttachment(GL_COLOR_ATTACHMENT0), 0.f, 1.f, 0.f);
		//util::drawTexture(blurread.getAttachment(GL_COLOR_ATTACHMENT0), -1.f, 1.f, 0.f);
		//util::drawTexture(prc::contextFBO.getAttachment(GL_COLOR_ATTACHMENT0), -1.f, 1.f, 0.f);
	}

	void setPageContentsCoords() {
		debugMenuEnabled.setSize(Vector2f(2.0f*0.7f, labelHeight));
		debugMenuEnabled.setPos(Vector2f(0.0f - debugMenuEnabled.getSize().x / 2.0f, 1.0f - debugMenuEnabled.getSize().y - standardOffset));

		for (int i = 0; i < pages.size(); i++) {
			pages[i].setPos(Vector2f(-1.0f+(2.0f / (float)numPages)*i, debugMenuEnabled.getPos().y - labelHeight - standardOffset));
			pages[i].setSize(Vector2f(2.0f / numPages, labelHeight));
			pages[i].refresh();
		}

		for (int i = 0; i < pageContents.size(); i++) {
			if (i == P_VIDEO || i == P_DEBUG) {
				for (int j = 0; j < pageContents[i].size(); j++) {
					//std::cout << pages[i].getPos().toString() << " " << pages[i].getSize().toString() << "\n";
					//std::cout << "debugmenupos " << debugMenuEnabled.getPos().toString() << "\n";
					float posy = pages[i].getPos().y - pageContents[i][j][0]->getSize().y - pageOffset - (contentHeight + contentSpacing) * j;

					pageContents[i][j][0]->setSize(Vector2f(2.0f*contentAreaWidth / 2.0f, contentHeight));
					pageContents[i][j][0]->setPos(Vector2f(0.0f - pageContents[i][j][0]->getSize().x, posy));
					(std::static_pointer_cast<Label>(pageContents[i][j][0]))->setMaxTextScale(1.0f);
					(std::static_pointer_cast<Label>(pageContents[i][j][0]))->setAlignment(Label::Alignment::RIGHT);

					float advx = standardOffset;
					for (int k = 1; k < pageContents[i][j].size(); k++) {
						pageContents[i][j][k]->setPos(Vector2f(0.0f + advx, posy));
						pageContents[i][j][k]->refresh();

						advx += pageContents[i][j][k]->getSize().x + standardOffset;
					}
				}
			}
		}

		pageContents[P_CONSOLE][PC_CONSOLE][0]->setPos(Vector2f(-1.0f, -1.0f));
		pageContents[P_CONSOLE][PC_CONSOLE][0]->setSize(Vector2f(2.0f,(pages[0].getPos().y+1.0f)));
	}


	void resizePageVideo() {
		
	}

	void resizePageInput() {
		
	}

	void resizePageDebug() {

	}

	void addDebugPageContent(std::string desc, std::vector<std::shared_ptr<UIElement>> elem) {
		pageContents[P_DEBUG].push_back(elem);

		std::shared_ptr<UIElement> l = std::make_shared<Label>(desc, txr, 0.0f, 0.0f);
		pageContents[P_DEBUG].back().insert(pageContents[P_DEBUG].back().begin(), l);

		resizePageDebug();
		setPageContentsCoords();
	}

	void addConsoleCommand(Console::Command command) {
		(std::static_pointer_cast<Console>(pageContents[P_CONSOLE][PC_CONSOLE][0]))->addCommand(command);
	}

	void resizePageConsole() {

	}

	void initPageVideo() {
		float aspectCorrect = (float)prc::windowSizeY/(float)prc::windowSizeX;

		pageContents.push_back(std::vector<std::vector<std::shared_ptr<UIElement>>>(0));

		//row: 'GPU used: ' + GPUused
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_GPUINFO].push_back(std::shared_ptr<UIElement>(new Label("GPU used: ", txr, Vector2f(), Vector2f())));

		std::stringstream ssgpuinfo;
		ssgpuinfo << glGetString(GL_RENDERER) << " (" << glGetString(GL_VENDOR) << ")";
		std::shared_ptr<Label> v_gpuinfo(new Label(ssgpuinfo.str(), txr, Vector2f(), Vector2f()));
		v_gpuinfo->setSize(Vector2f(contentAreaWidth, contentHeight));
		v_gpuinfo->setAlignment(Label::Alignment::LEFT);
		v_gpuinfo->setMaxTextScale(1.f);

		pageContents[P_VIDEO][PV_GPUINFO].push_back(v_gpuinfo);

		//row: 'Resolution: ' + selectorResolutions
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_RES].push_back(std::shared_ptr<UIElement>(new Label("Resolution: ", txr, Vector2f(), Vector2f())));

		std::vector<Selector::Element> pv_elemsRes = std::vector<Selector::Element>(0);
		pv_elemsRes.push_back(Selector::Element("Custom", [] {
			//pageContents[P_VIDEO][PV_XRES][1]->disabled = false;
			//pageContents[P_VIDEO][PV_YRES][1]->disabled = false;
		}));
		for (int i = 0; i < prc::videoModesCount; i++) {
			if (i == 0 || (i > 0 && prc::videoModes[i - 1].width != prc::videoModes[i].width && prc::videoModes[i - 1].height != prc::videoModes[i].height)) {
				pv_elemsRes.push_back(Selector::Element(std::to_string(prc::videoModes[i].width) + "x" + std::to_string(prc::videoModes[i].height), [i] {
					//pageContents[P_VIDEO][PV_XRES][1]->disabled = true;
					//pageContents[P_VIDEO][PV_YRES][1]->disabled = true;

					if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) {
						(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->setSelectedElement(4, true);
					}

					(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->setVal(prc::videoModes[i].width);
					(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->setVal(prc::videoModes[i].height);

					prc::defStartResizing();
					prc::startResizing();

					prc::changeResolution(prc::videoModes[i].width, prc::videoModes[i].height);
					prc::contextFBO.resize(prc::videoModes[i].width, prc::videoModes[i].height);
					prc::defResize();
					prc::resize();

					prc::defEndResizing();
					prc::endResizing();

				}));
			}
		}
		
		std::shared_ptr<Selector> v_res = std::shared_ptr<Selector>(new Selector("", pv_elemsRes,  txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f), [](std::string s) {
			
		}));
		v_res->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_res->txf.label.setMaxTextScale(1.0f);
		for (int i = 0; i < v_res->buttons.size(); i++) {
			v_res->buttons[i]->label.setMaxTextScale(1.0f);
		}
		v_res->setSelectedElement(0, false);

		pageContents[P_VIDEO][PV_RES].push_back(v_res);

		//row: 'Resolution X: ' + textfieldxresX
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_XRES].push_back(std::shared_ptr<UIElement>(new Label("Resolution X: ", txr, Vector2f(), Vector2f())));
		
		std::shared_ptr<TextFieldInteger> v_resX = std::shared_ptr<TextFieldInteger>(new TextFieldInteger(constants::WIDTH,1.0f,txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f,0.0f,0.0f,1.0f)));
		v_resX->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_resX->setVal(constants::WIDTH);
		v_resX->setModifier(false);
		v_resX->label.setMaxTextScale(1.0f);
		v_resX->action = [] {
			(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->setSelectedElement(0);
			
			if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) {
				(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->setSelectedElement(4, true);
			}

			prc::defStartResizing();
			prc::startResizing();
			
			prc::changeResolution((std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->getVal(), constants::HEIGHT);
			prc::contextFBO.resize((std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->getVal(), constants::HEIGHT);
			prc::defResize();
			prc::resize();

			prc::defEndResizing();
			prc::endResizing();
		};

		pageContents[P_VIDEO][PV_XRES].push_back(v_resX);

		//row: 'Resolution Y: ' + textfieldxresY
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_YRES].push_back(std::shared_ptr<UIElement>(new Label("Resolution Y: ", txr, Vector2f(), Vector2f())));

		std::shared_ptr<TextFieldInteger> v_resY = std::shared_ptr<TextFieldInteger>(new TextFieldInteger(constants::HEIGHT, 1.0f, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f)));
		v_resY->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_resY->setVal(constants::HEIGHT);
		v_resY->setModifier(false);
		v_resY->label.setMaxTextScale(1.0f);
		v_resY->action = [] {
			(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->setSelectedElement(0);

			if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) {
				(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->setSelectedElement(4, true);
			}

			prc::defStartResizing();
			prc::startResizing();
			
			prc::changeResolution(constants::WIDTH, (std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->getVal());
			prc::contextFBO.resize(constants::WIDTH, (std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->getVal());
			prc::defResize();
			prc::resize();

			prc::defEndResizing();
			prc::endResizing();
		};

		pageContents[P_VIDEO][PV_YRES].push_back(v_resY);

		//row: 'Aspect Ratio: ' + selectorAspectRatio
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_ASPECT].push_back(std::shared_ptr<UIElement>(new Label("Aspect Ratio: ", txr, Vector2f(), Vector2f())));
		
		std::shared_ptr<Selector> v_aspect = std::shared_ptr<Selector>(new Selector("", {
			Selector::Element("Auto",  [] { prc::rOrthoAspect = 0; }),
			Selector::Element("1:1",  [] { prc::rOrthoAspect = 1; }),
			Selector::Element("4:3",  [] { prc::rOrthoAspect = 2; }),
			Selector::Element("5:3",  [] { prc::rOrthoAspect = 3; }),
			Selector::Element("16:9",  [] { prc::rOrthoAspect = 4; }),
			Selector::Element("16:10",  [] { prc::rOrthoAspect = 5; }),
		}, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f), [](std::string s) {
			prc::defStartResizing();
			prc::startResizing();

			prc::defResize();
			prc::resize();

			prc::defEndResizing();
			prc::endResizing();
		}));
		v_aspect->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_aspect->txf.label.setMaxTextScale(1.0f);
		for (int i = 0; i < v_aspect->buttons.size(); i++) {
			v_aspect->buttons[i]->label.setMaxTextScale(1.0f);
		}

		/*int selElem = 0;
		if (prc::orthoAspect == 1.0f) selElem = 1;
		else if (prc::orthoAspect == 4.0f / 3.0f) selElem = 2;
		else if (prc::orthoAspect == 5.0f / 3.0f) selElem = 3;
		else if (prc::orthoAspect == 16.0f / 9.0f) selElem = 4;
		else if (prc::orthoAspect == 16.0f / 10.0f) selElem = 5;*/
		v_aspect->setSelectedElement(prc::rOrthoAspect);

		//v_aspect->buttons[v_aspect->getSelectedElementIdx()]->action();

		pageContents[P_VIDEO][PV_ASPECT].push_back(v_aspect);

		//row: 'VSync: ' + checkboxvSync
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_VSYNC].push_back(std::shared_ptr<UIElement>(new Label("VSync: ", txr, Vector2f(), Vector2f())));

		std::shared_ptr<CheckBox> v_checkbox = std::shared_ptr<CheckBox>(new CheckBox(Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f)));
		v_checkbox->setSize(Vector2f(contentHeight*aspectCorrect,  contentHeight));
		v_checkbox->checked = prc::vSync;
		/*v_checkbox->customBackground = true;
		v_checkbox->drawBackground = [] {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
			util::drawQuad(pageContents[P_VIDEO][PV_VSYNC][1]->getPos(), pageContents[P_VIDEO][PV_VSYNC][1]->getSize());
			glDisable(GL_BLEND);
		};*/
		v_checkbox->action = [] {
			prc::setVSync((std::static_pointer_cast<CheckBox>(pageContents[P_VIDEO][PV_VSYNC][1]))->checked);
		};

		pageContents[P_VIDEO][PV_VSYNC].push_back(v_checkbox);


		//row: 'Framerate cap: ' + textfieldframecap
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_FPSCAP].push_back(std::shared_ptr<UIElement>(new Label("Framerate cap: ", txr, Vector2f(), Vector2f())));


		std::shared_ptr<TextFieldInteger> v_framecap = std::shared_ptr<TextFieldInteger>(new TextFieldInteger(0, 1.0f, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f)));
		v_framecap->setSize(Vector2f(1.0f*contentAreaWidth/2.0f, contentHeight));
		//v_framecap->customBackground = true;
		v_framecap->setModifier(false);
		v_framecap->label.setMaxTextScale(1.0f);
		v_framecap->action = [] {
			int val = (std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_FPSCAP][1]))->getVal();
			if (val != 0) {
				val = std::max(val, 5);
				(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_FPSCAP][1]))->setVal(val);
			}

			prc::framerateCap = val;
		};
		v_framecap->setVal(prc::framerateCap);

		pageContents[P_VIDEO][PV_FPSCAP].push_back(v_framecap);

		//row:  'Scaling Mode: ' + selectorScalingMode
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_SCALING].push_back(std::shared_ptr<UIElement>(new Label("Scaling Mode: ", txr, Vector2f(), Vector2f())));
	
		std::shared_ptr<Selector> v_scalingmode = std::shared_ptr<Selector>(new Selector("", {
			Selector::Element("One to one", [] { 
				prc::scalingMode = prc::SCALING_MODE::ONE_TO_ONE; 

				//pageContents[P_VIDEO][PV_RES][1]->disabled = true;
				//(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->txf.disabled = true;

				(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->setSelectedElement(0);
				(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->setVal(prc::windowSizeX);
				(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->setVal(prc::windowSizeY);
				
				/*pageContents[P_VIDEO][PV_XRES][1]->disabled = true;
				pageContents[P_VIDEO][PV_YRES][1]->disabled = true;*/
		}	),
			Selector::Element("Real size", [] { prc::scalingMode = prc::SCALING_MODE::REAL_SIZE; }),
			Selector::Element("Total fill", [] { prc::scalingMode = prc::SCALING_MODE::TOTAL_FILL; }),
			Selector::Element("Integer ratio", [] { prc::scalingMode = prc::SCALING_MODE::INTEGER_RATIO; }),
			Selector::Element("Non-Integer ratio", [] { prc::scalingMode = prc::SCALING_MODE::NON_INTEGER_RATIO; }),
		}, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f), [](std::string s) {
				/*if ((std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->getSelectedElementIdx() != 0) {
					pageContents[P_VIDEO][PV_RES][1]->disabled = false;
					(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->txf.disabled = false;

					if ((std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_RES][1]))->getSelectedElementIdx() == 0) {
						pageContents[P_VIDEO][PV_XRES][1]->disabled = false;
						pageContents[P_VIDEO][PV_YRES][1]->disabled = false;
					}
				}*/

			prc::resizeApplication();
			//std::cout << "resize!\n";
		}));
		v_scalingmode->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_scalingmode->txf.label.setMaxTextScale(1.0f);
		for (int i = 0; i < v_scalingmode->buttons.size(); i++) {
			v_scalingmode->buttons[i]->label.setMaxTextScale(1.0f);
		}
		//std::cout << "v_scalingmode " << prc::scalingMode << "\n";
		v_scalingmode->setSelectedElement(prc::scalingMode, false);

		pageContents[P_VIDEO][PV_SCALING].push_back(v_scalingmode);

		//row:  'Scaling Mode: ' + selectorScalingMode
		pageContents[P_VIDEO].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		pageContents[P_VIDEO][PV_FILTERING].push_back(std::shared_ptr<UIElement>(new Label("Filtering: ", txr, Vector2f(), Vector2f())));

		std::shared_ptr<Selector> v_filtering = std::shared_ptr<Selector>(new Selector("", {
			Selector::Element("Nearest", [] {prc::contextFBO.setFiltering(GL_NEAREST); }),
			Selector::Element("Linear", [] { prc::contextFBO.setFiltering(GL_LINEAR); })
			}, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f), [](std::string s) {
		}));
		v_filtering->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		v_filtering->txf.label.setMaxTextScale(1.0f);
		for (int i = 0; i < v_filtering->buttons.size(); i++) {
			v_filtering->buttons[i]->label.setMaxTextScale(1.0f);
		}
		//std::cout << "v_scalingmode " << prc::scalingMode << "\n";
		v_filtering->setSelectedElement(prc::contextFBO.getMinFiltering()-GL_NEAREST, false);
		//std::cout << "magfiltering " << (prc::contextFBO.getMinFiltering()) <<  " " << GL_NEAREST << " " << GL_LINEAR <<  "\n";
		pageContents[P_VIDEO][PV_FILTERING].push_back(v_filtering);

		resizePageVideo();
	}

	void initPageInput() {
		pageContents.push_back(std::vector<std::vector<std::shared_ptr<UIElement>>>(0));

		resizePageInput();
	}

	void initPageDebug() {
		float aspectCorrect = (float)prc::windowSizeY / (float)prc::windowSizeX;

		pageContents.push_back(std::vector<std::vector<std::shared_ptr<UIElement>>>(0));

		//debug enabled

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_ENABLEDEBUG].push_back(std::shared_ptr<UIElement>(new Label("Debug Enabled: ", txr, 0.0f, 0.0f)));

		std::shared_ptr<CheckBox> pd_enabledebug = std::make_shared<CheckBox>(0.0f, 0.0f, vec4(1.0f).colMul(0.6f));
		pd_enabledebug->setSize(Vector2f(contentHeight*aspectCorrect, contentHeight));
		pd_enabledebug->checked = debug::enabled;
		pd_enabledebug->action = []{ debug::enabled = !debug::enabled; };

		pageContents[P_DEBUG][PD_ENABLEDEBUG].push_back(pd_enabledebug);

		//maxstrs

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_MAXSTRS].push_back(std::shared_ptr<UIElement>(new Label("Max Log Strings: ", txr, 0.0f, 0.0f)));
		
		std::shared_ptr<TextFieldInteger> pd_maxstrs = std::shared_ptr<TextFieldInteger>(new TextFieldInteger(0, 1.0f, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f)));
		pd_maxstrs->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		pd_maxstrs->setVal(debug::maxStrs);
		pd_maxstrs->setModifier(false);
		pd_maxstrs->label.setMaxTextScale(1.0f);
		pd_maxstrs->action = [] {
			std::shared_ptr<TextFieldInteger> txf = (std::static_pointer_cast<TextFieldInteger>(pageContents[P_DEBUG][PD_MAXSTRS][1]));
			debug::maxStrs = txf->getVal();
		};

		pageContents[P_DEBUG][PD_MAXSTRS].push_back(pd_maxstrs);

		//maxrtstrs

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_MAXRTSTRS].push_back(std::shared_ptr<UIElement>(new Label("Max RT-Log Strings: ", txr, 0.0f, 0.0f)));
		
		std::shared_ptr<TextFieldInteger> pd_maxrtstrs = std::shared_ptr<TextFieldInteger>(new TextFieldInteger(0, 1.0f, txr, Vector2f(), Vector2f(), Vector4f(0.6f, 0.6f, 0.6f, 1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f)));
		pd_maxrtstrs->setSize(Vector2f(1.0f*contentAreaWidth / 2.0f, contentHeight));
		pd_maxstrs->setVal(debug::maxRTStrs);
		pd_maxrtstrs->setModifier(false);
		pd_maxrtstrs->label.setMaxTextScale(1.0f);
		pd_maxrtstrs->action = [] {
			std::shared_ptr<TextFieldInteger> txf = (std::static_pointer_cast<TextFieldInteger>(pageContents[P_DEBUG][PD_MAXRTSTRS][1]));
			debug::maxRTStrs = txf->getVal();
		};

		pageContents[P_DEBUG][PD_MAXRTSTRS].push_back(pd_maxrtstrs);

		//enablemsgmenu

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_ENABLEMSGMENU].push_back(std::shared_ptr<UIElement>(new Label("Show logs on screen: ", txr, 0.0f, 0.0f)));

		std::shared_ptr<CheckBox> pd_enablemsg = std::make_shared<CheckBox>(0.0f, 0.0f, vec4(1.0f).colMul(0.6f));
		pd_enablemsg->setSize(Vector2f(contentHeight*aspectCorrect, contentHeight));
		pd_enablemsg->checked = debug::enableMsgCorner;
		pd_enablemsg->action = [] { debug::enableMsgCorner = !debug::enableMsgCorner; };

		pageContents[P_DEBUG][PD_ENABLEMSGMENU].push_back(pd_enablemsg);

		//enablesaving

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_ENABLESAVING].push_back(std::shared_ptr<UIElement>(new Label("Save settings automatically when closing the app: ", txr, 0.0f, 0.0f)));

		std::shared_ptr<CheckBox> pd_enablesaving = std::make_shared<CheckBox>(0.0f, 0.0f, vec4(1.0f).colMul(0.6f));
		pd_enablesaving->setSize(Vector2f(contentHeight*aspectCorrect, contentHeight));
		pd_enablesaving->checked = prc::enableSettingsSaving;
		pd_enablesaving->action = [] { prc::enableSettingsSaving = !prc::enableSettingsSaving; };

		pageContents[P_DEBUG][PD_ENABLESAVING].push_back(pd_enablesaving);

		//savesettings

		pageContents[P_DEBUG].push_back(std::vector<std::shared_ptr<UIElement>>(0));
		pageContents[P_DEBUG][PD_SAVESETTINGS].push_back(std::shared_ptr<UIElement>(new Label("Save Settings: ", txr, 0.0f, 0.0f)));

		std::shared_ptr<Button> pd_savesettings = std::make_shared<Button>("Save Settings", txr, 0.0f, 0.0f);
		pd_savesettings->setSize(Vector2f(contentHeight*2.0f, contentHeight));
		pd_savesettings->label.setMaxTextScale(1.0f);
		pd_savesettings->setColor(vec4(1.0f).colMul(0.6f));
		
		pd_savesettings->action = [] { prc::saveSettingsToFile("settings.cfg"); };

		pageContents[P_DEBUG][PD_SAVESETTINGS].push_back(pd_savesettings);

		resizePageDebug();
	}

	void initPageConsole() {
		pageContents.push_back(std::vector<std::vector<std::shared_ptr<UIElement>>>(0));

		pageContents[P_CONSOLE].push_back(std::vector<std::shared_ptr<UIElement>>(0));

		std::shared_ptr<Console> pc_console = std::make_shared<Console>(txr,0.0f,0.0f);
		pc_console->setColor(1.0f);
		//pc_console->inputField.barSizeReference = (float)prc::windowSizeX;
		pc_console->initDefaultCommands();

		pageContents[P_CONSOLE][PC_CONSOLE].push_back(pc_console);

		resizePageConsole();
	}

	void onOpen() {
		//video

		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_XRES][1]))->setVal(constants::WIDTH);
		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_VIDEO][PV_YRES][1]))->setVal(constants::HEIGHT);

		(std::static_pointer_cast<CheckBox>(pageContents[P_VIDEO][PV_VSYNC][1]))->checked = prc::vSync;

		(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->setSelectedElement(prc::scalingMode);
		(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_FILTERING][1]))->setSelectedElement(prc::contextFBO.getMagFiltering()-GL_NEAREST);

		//debug

		(std::static_pointer_cast<CheckBox>(pageContents[P_DEBUG][PD_ENABLEDEBUG][1]))->checked = debug::enabled;

		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_DEBUG][PD_MAXSTRS][1]))->setVal(debug::maxStrs);
		(std::static_pointer_cast<TextFieldInteger>(pageContents[P_DEBUG][PD_MAXRTSTRS][1]))->setVal(debug::maxRTStrs);
	
		(std::static_pointer_cast<CheckBox>(pageContents[P_DEBUG][PD_ENABLEMSGMENU][1]))->checked = debug::enableMsgCorner;

		//console

		if (page == P_CONSOLE) {
			//std::string il = (std::static_pointer_cast<Console>(pageContents[P_CONSOLE][0][0]))->inputField.inputLayer;
			//input::setActiveLayer(il);
			//debug::log << "set active!\n";
			(std::static_pointer_cast<Console>(pageContents[P_CONSOLE][0][0]))->inputField.setActive(true);
		}
	}

	void onClose() {

		//console
		if (page == P_CONSOLE) {
			(std::static_pointer_cast<Console>(pageContents[P_CONSOLE][0][0]))->inputField.setActive(false);
		}
	}
}