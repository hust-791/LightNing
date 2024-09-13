#pragma once
#include "Log.h"
#include "Application.h"

extern LN::Application* LN::CreateApplication();

int main(int argc, char** argv)
{
	LN::Log::Init();
	LN_CORE_INFO("Log System Init!");
    auto app = LN::CreateApplication();

	LN_CORE_INFO("APP Running!");
	app->Run();

	LN_CORE_INFO("APP Shutdown!");
	delete app;
}
