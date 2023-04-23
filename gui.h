#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

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
#include <gtkmm/drawingarea.h>

struct Frame // Model Framing and window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio
	int height;  // window height
	int width;   // window width
};



class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();
	void setFrame(Frame x);
	void adjustFrame(int width, int height);

protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, const int width, const int height);
private:
	Frame frame;
};


class Gui : public Gtk::Window
{
public:
	Gui(const std::string& filename = "");
	virtual ~Gui();
protected:
	Gtk::Box big_box, general_box, buttons_box, info_box, area_box;
	Gtk::Label general, info, nbUpdate, nbP, nbRs, nbRr, nbNs, nbNp, nbNd, nbNr;
	Gtk::Button button_exit, button_open, button_save,  button_start, button_step;
	Gtk::Separator separator1;
	
	void on_button_exit_clicked();
	void on_button_open_clicked();
	//void on_button_save_clicked();
	void on_button_start_clicked();
	void on_button_step_clicked();
	
	void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog);
	//void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog);
	void update_infos();
	
	Simulation simu;
	std::string filename;
	
	MyArea m_area;
};

std::string informations();

//void draw_axes(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame);

#endif



