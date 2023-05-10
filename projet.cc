/*!
  \file   projet.cc
  \author Charly Guardia 100%, Gauthier de Mercey 0%
  \date   avril 2023
  \version 2
*/

#include <iostream>
#include <gtkmm/application.h>
#include "simulation.h"
#include "gui.h"

using namespace std;

int main (int argc, char* argv[])
{
    auto app = Gtk::Application::create();
    std::string filename;
    if (argc == 2)
    {
        filename = argv[1];
    }
    return app->make_window_and_run<Gui>(1, argv, filename);
}
