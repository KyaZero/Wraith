#pragma once

extern fw::Application* fw::CreateApplication();

int main(int argc, char** argv)
{
	auto app = fw::CreateApplication();
	app->Run();
	delete app;
	return 0;
}