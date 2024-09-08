#pragma once

#include "Log.h"
#include "Application.h"
//#include <test/ClearColorTest.h>
//#include <test/TextureTest.h>

int main(int argc, char** argv)
{
	LN::Log::Init();

    auto app = LN::Application();

    //app.GetTestMenu().RegisterTest<LN::ClearColorTest>("clear color");
    //app.GetTestMenu().RegisterTest<LN::TextureTest>("kun 3D");

	app.Run();
}
