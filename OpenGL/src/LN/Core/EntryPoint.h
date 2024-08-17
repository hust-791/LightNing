#pragma once

#include "Log.h"
#include "Applicaton.h"

int main(int argc, char** argv)
{
	LN::Log::Init();

	auto app = LN::Application();

	app.Run();
}
