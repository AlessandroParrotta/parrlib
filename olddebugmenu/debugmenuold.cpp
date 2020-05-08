
#include "DebugMenuold.h"

namespace DebugMenuold {

	class Page {
	public:
		std::string title;
		UIList list;
		Page() {}
		Page(std::string title) : title(title) {}

		void resize() { list.resize(); }

		void update() { list.update(); }
		void draw() { list.draw(); }
	};

	class PagePair : public UIElement {
	public:
		std::string name;
		Label label;
		std::shared_ptr<UIElement> elem;
		PagePair() {}
		PagePair(std::string name, std::shared_ptr<UIElement> elem) : name(name), elem(elem) {
			label = Label(name, txr);
		}

		template <class T>
		std::shared_ptr<T> get() {
			if (elem == nullptr) return nullptr;
			return std::reinterpret_pointer_cast<T>(elem);
		}

		void setPos(vec2 pos) {
			UIElement::setPos(pos);
		}

		void setSize(vec2 size) {
			UIElement::setSize(size);
		}

		void update() {
			label.update();
			elem->update();
		}

		void draw() {
			label.draw();
			elem->draw();
		}
	};

	/*
		pages:
			-Video:
				-gpuinfo
				-res
				-xres
				-yres
				-aspect
				-vsync
				-fpscap
				-scaling
				-filtering
			-Input:
			-debug:
				-enabled
				-maxstrs
				-maxrtstrs
				-enablemenu
				-enablesaving
				-savesettings
			-Console
	*/
	std::unordered_map<std::string, Page> pages;

	TextRendererOld txr;
	bool resChangeMode;

	float contentAreaWidth;
	float contentHeight;

	void addConsoleCommand(Console::Command command) {

	}

	void startResizing() {

	}

	void resize() {
		for (auto& p : pages) p.second.resize();
	}

	void endResizing() {

	}

	void onOpen() {

	}

	void onClose() {

	}

	void resizeFunc(int sx, int sy) {
		prc::defStartResizing();
		prc::startResizing();

		prc::changeResolution(sx, sy);
		prc::contextFBO.resize(sx, sy);
		prc::defResize();
		prc::resize();

		prc::defEndResizing();
		prc::endResizing();
	}

	void addPair(std::string page, std::string id, std::string name, std::shared_ptr<UIElement> elem) {
		pages[page].list.add(id, std::make_shared<PagePair>(name, elem));
	}

	std::shared_ptr<PagePair> getPair(std::string page, std::string id) {
		return pages[page].list.get<PagePair>(id);
	}

	template<class T>
	std::shared_ptr<T> getPairElem(std::string page, std::string id) {
		return pages[page].list.get<PagePair>(id)->get<T>();
	}

	void init() {
		pages["vd"] = Page("Video");

			std::stringstream ssgpuinfo;
			ssgpuinfo << glGetString(GL_RENDERER) << " (" << glGetString(GL_VENDOR) << ")";
			addPair("vd", "gpuinfo", "GPU used: ", std::make_shared<Label>(ssgpuinfo.str(), txr));

			std::vector<Selector::Element> vmelems;
			for (int i = 0; i < prc::videoModesCount; i++) {
				if (i == 0 || (i > 0 && prc::videoModes[i - 1].width != prc::videoModes[i].width && prc::videoModes[i - 1].height != prc::videoModes[i].height)) {
					vmelems.push_back(Selector::Element(std::to_string(prc::videoModes[i].width) + "x" + std::to_string(prc::videoModes[i].height), [i] {
						if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) {
							//(std::static_pointer_cast<Selector>(pageContents[P_VIDEO][PV_SCALING][1]))->setSelectedElement(4, true);
							getPairElem<Selector>("vd", "scaling")->setSelectedElement(4, true);
						}

						pages["vd"].list.get<PagePair>("xres")->get<TextFieldInteger>()->setVal(prc::videoModes[i].width);
						pages["vd"].list.get<PagePair>("yres")->get<TextFieldInteger>()->setVal(prc::videoModes[i].height);
				
						resizeFunc(prc::videoModes[i].width, prc::videoModes[i].height);
					}));
					std::cout << "pushed video mode" << i << "\n";
				}
			}
			pages["vd"].list.add("res", std::make_shared<PagePair>("Resolution: ", std::make_shared<Selector>("", vmelems, txr, [](std::string title) {})));

			pages["vd"].list.add("resX", std::make_shared<PagePair>("Resolution X: ", std::make_shared<TextFieldInteger>(constants::WIDTH, txr, [] {
				pages["vd"].list.get<PagePair>("res")->get<Selector>()->setSelectedElement(0);
			
				if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) pages["vd"].list.get<PagePair>("scaling")->get<Selector>()->setSelectedElement(4, true);
			
				resizeFunc(pages["vd"].list.get<PagePair>("resx")->get<TextFieldInteger>()->getVal(), constants::HEIGHT);
			})));

			pages["vd"].list.add("resY", std::make_shared<PagePair>("Resolution Y: ", std::make_shared<TextFieldInteger>(constants::HEIGHT, txr, [] {
				pages["vd"].list.get<PagePair>("res")->get<Selector>()->setSelectedElement(0);
			
				if (prc::scalingMode == prc::SCALING_MODE::ONE_TO_ONE) pages["vd"].list.get<PagePair>("scaling")->get<Selector>()->setSelectedElement(4, true);

				resizeFunc(constants::WIDTH, pages["vd"].list.get<PagePair>("resy")->get<TextFieldInteger>()->getVal());
			})));

			pages["vd"].list.add("aspect", std::make_shared<PagePair>("Aspect Ratio: ", std::make_shared<Selector>("", std::vector<Selector::Element>{
				Selector::Element("Auto",	[] { prc::rOrthoAspect = 0; }),
				Selector::Element("1:1",	[] { prc::rOrthoAspect = 1; }),
				Selector::Element("4:3",	[] { prc::rOrthoAspect = 2; }),
				Selector::Element("5:3",	[] { prc::rOrthoAspect = 3; }),
				Selector::Element("16:9",	[] { prc::rOrthoAspect = 4; }),
				Selector::Element("16:10",	[] { prc::rOrthoAspect = 5; })
			}, txr, [](std::string s) {
					prc::defStartResizing();
					prc::startResizing();

					prc::defResize();
					prc::resize();

					prc::defEndResizing();
					prc::endResizing();
			})));

			pages["vd"].list.add("vsync", std::make_shared<PagePair>("VSync: ", std::make_shared<CheckBox>([] {
				prc::setVSync(pages["vd"].list.get<PagePair>("vsync")->get<CheckBox>()->checked);
			})));

			pages["vd"].list.add("fpscap", std::make_shared<PagePair>("Fps Cap: ", std::make_shared<TextFieldInteger>(0, txr, [] {
				int val = getPairElem<TextFieldInteger>("vd", "fpscap")->getVal();
				if (val != 0) {
					val = (int)std::fmax(val, 5);
					getPairElem<TextFieldInteger>("vd", "fpscap")->setVal(val);
				}

				prc::framerateCap = val;
			})));

			pages["vd"].list.add("scaling", std::make_shared<PagePair>("Scaling Mode: ", std::make_shared<Selector>("", std::vector<Selector::Element>{
				Selector::Element("One to One", [] {
					pages["vd"].list.get<PagePair>("res")->get<Selector>()->setSelectedElement(0);
					pages["vd"].list.get<PagePair>("resx")->get<TextFieldInteger>()->setVal(prc::windowSizeX);
					pages["vd"].list.get<PagePair>("resy")->get<TextFieldInteger>()->setVal(prc::windowSizeY);
				}),
				Selector::Element("Real Size",			[] { prc::scalingMode = prc::SCALING_MODE::REAL_SIZE;			}),
				Selector::Element("Total Fill",			[] { prc::scalingMode = prc::SCALING_MODE::TOTAL_FILL;			}),
				Selector::Element("Integer Ratio",		[] { prc::scalingMode = prc::SCALING_MODE::INTEGER_RATIO;		}),
				Selector::Element("Non-Integer Ratio",	[] { prc::scalingMode = prc::SCALING_MODE::NON_INTEGER_RATIO;	}),
			}, txr, [](std::string s) {
				prc::resizeApplication(); 
			})));

			pages["vd"].list.add("scaling", std::make_shared<PagePair>("Scaling Mode: ", std::make_shared<Selector>("", std::vector<Selector::Element>{
				Selector::Element("Nearest",	[] { prc::contextFBO.setFiltering(GL_NEAREST); }),
				Selector::Element("Linear",		[] { prc::contextFBO.setFiltering(GL_LINEAR); })
			}, txr, [](std::string s) {})));

		pages["in"] = Page("Input");

		pages["deb"] = Page("debug");

			addPair("deb", "enable", "debug Enabled: ", std::make_shared<CheckBox>([] { debug::enabled = getPair("deb", "enabled")->get<CheckBox>()->checked; }));

			addPair("deb", "maxstrs", "Max Log Strs: ", std::make_shared<TextFieldInteger>(0, txr, [] { debug::maxStrs = getPair("deb", "maxstrs")->get<TextFieldInteger>()->getVal(); }));
			addPair("deb", "maxtrstrs", "Max RT-Log Strs: ", std::make_shared<TextFieldInteger>(0, txr, [] { debug::maxRTStrs = getPair("deb", "maxrtstrs")->get<TextFieldInteger>()->getVal(); }));

			addPair("deb", "enablemsmenu", "Show logs on screen: ", std::make_shared<CheckBox>([] { debug::enableMsgCorner = !debug::enableMsgCorner; }));
		
			addPair("deb", "enablesaving", "Save Settings: ", std::make_shared<Button>("Save Settings", txr, [] { prc::saveSettingsToFile("settings.cfg"); }));

		pages["con"] = Page("Console");

			pages["con"].list.add("console", std::make_shared<Console>(txr,0.f,0.f));
			pages["con"].list.get<Console>("console")->initDefaultCommands();
	}

	void update() {
		for (auto& p : pages) p.second.update();
	}

	void draw() {
		for (auto& p : pages) p.second.draw();
	}
	 
}
