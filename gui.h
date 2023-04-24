#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/frame.h>
#include "gui.h"
#include "simulation.h"
#include "graphic.h"
#include <string>

class Gui : public Gtk::Window
{
public:
	Gui(const std::string& filename = "");
	virtual ~Gui();
protected:
	Gtk::Box big_box, general_box, buttons_box, info_box;
	Gtk::Label general, info, nbUpdate, nbP, nbRs, nbRr, nbNs, nbNp, nbNd, nbNr;
	Gtk::Button button_exit, button_open, button_save,  button_start, button_step;
	Gtk::Separator separator1;
	
	void on_button_exit_clicked();
	void on_button_open_clicked();
	void on_button_save_clicked();
	void on_button_start_clicked();
	void on_button_step_clicked();
	
	void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog);
	void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog);
	
	void maj_label(int nbUpdate_);
	void update_infos();
	
	Simulation simu;
	std::string filename;
	
	bool on_timeout();
	bool timer_added;
	bool disconnect;
	const int timeout_value; 
	unsigned int counter;
	bool started;

	void monde_faux();
	
};

std::string informations();

#endif

