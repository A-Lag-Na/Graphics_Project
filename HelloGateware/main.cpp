// About as basic as Gateware gets... we use GLog to print to console & a file.
#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Some handy system level libraries
// Ignore some SYSTEM libraries GLog doesn't need to keep things at a minimal footprint
#define GATEWARE_DISABLE_GWINDOW // GLog doesn't need this, part of SYSTEM
// With what we want & what we don't defined we can include the API
#include "../Gateware/Gateware.h"
// lets just print a message to the console... (but also to a file)
int main()
{
	GW::SYSTEM::GLog log; // this is a proxy, its a smart handle to a library
	log.Create("MessageLog.txt"); // all proxies must be "Create"ed, before they function
	log.EnableConsoleLogging(true); // mirror output to the console
	log.Log("Hello Gateware!!!"); // say hello!
	return 0; // that's all folks
}