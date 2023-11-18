#include <iostream>
#include "ImageViewer.hpp"

int main(int argc, char* argv[])
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i)
	{
		args.push_back(argv[i]);
	}


	ImageViewer iv("tiv", 1280, 720, true, args);
	iv.Run();
}
