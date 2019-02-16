#include <iostream>
#include <iomanip>


/*
** This is copied from the vstgui standalone samples
*/
#include "vstgui/standalone/include/iapplication.h"
#include "vstgui/standalone/include/iuidescwindow.h"
#include "vstgui/standalone/include/helpers/appdelegate.h"
#include "vstgui/standalone/include/helpers/windowlistener.h"

using namespace VSTGUI::Standalone;
using namespace VSTGUI::Standalone::Application;

class VstGuiTestApp : public DelegateAdapter, public WindowListenerAdapter
{
public:
	VstGuiTestApp ()
	: DelegateAdapter ({"simple_standalone", "1.0.0", "vstgui.examples.simplestandalone"})
	{}
	
	void finishLaunching () override
	{
		UIDesc::Config config;
		config.uiDescFileName = "Window.uidesc";
		config.viewName = "Window";
		config.windowConfig.title = "Sample App";
		config.windowConfig.autoSaveFrameName = "SampleAppWindow";
		config.windowConfig.style.border ().close ().size ().centered ();
		if (auto window = UIDesc::makeWindow (config))
		{
			window->show ();
			window->registerWindowListener (this);
		}
		else
		{
			IApplication::instance ().quit ();
		}
	}
	void onClosed (const IWindow& window) override
	{
		IApplication::instance ().quit ();
	}
	
};

static Init gAppDelegate (std::make_unique<VstGuiTestApp> ());
