// projet.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <iostream>
#include <fstream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "gui.h"
#include "ville.h"

using namespace std;

int main(int argc, char* argv[]) {	
	
	if (argv[1]) {
		std::string input_file = argv[1];
		lecture(input_file);
		auto app = Gtk::Application::create();
		Win win;
		return app->run(win);
	}
	
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	
	Win win;
	
	return app->run(win);
}
