#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <gtkmm.h>

struct Frame{
	double xMin; 
	double xMax;
	double yMin;
	double yMax;
	double asp;  
	int height;  
	int width;   
};

class Area : public Gtk::DrawingArea{
public:
	Area();
	virtual ~Area();
	void setFrame(Frame x); 
	void adjustFrame();
	void refresh();
protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
	Frame frame;
};

class Fenetre : public Gtk::Window{
public:
	Fenetre();
	virtual ~Fenetre();
	void reset();
protected:
	bool on_key_press_event(GdkEventKey * key_event);
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	void on_button_clicked_previous();
	void on_button_clicked_next();
	Area m_Area;
	Gtk::Box m_Box, m_Box_Left, m_Box_Right;
	Gtk::Box m_Box_Top, m_Box_Middle, m_Box_Bottom;
	Gtk::Button Button_exit;
	Gtk::Button Button_open;
	Gtk::Button Button_save;
	Gtk::Button Button_start;
	Gtk::Button Button_step;
	Gtk::Button Button_previous;
	Gtk::Button Button_next;
	Gtk::Frame m_Frame_Top;
	Gtk::Frame m_Frame_Middle;
	Gtk::Frame m_Frame_Bottom;
	Gtk::Label m_Label_nbn;
	Gtk::Label m_Label_infoF;
	unsigned int i;
	bool f_selected;
	bool timer_added;
	bool disconnect;
	const int timeout_value;
	bool on_timeout();
private:
	void draw();	
};

#endif 

