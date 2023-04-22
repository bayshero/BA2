//GRAPHIC.CC, BENARAFA MANON: 100%/ GLASSEY ADELINE: 0%, V2
#include <iostream>
#include <fstream>
#include <vector>
#include "simulation.h"
#include "graphic_gui.h"
#include "gui.h"

using namespace std;

static Frame default_frame = {0, 500, 0, 500, 1.0, 500, 500}; 


Area::Area(){
	setFrame(default_frame);
}

Area::~Area(){
}

void Area::setFrame(Frame f){
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
	{
		f.asp = f.width/f.height;
		frame = f;
	}else{
		std::cout << "incorrect Model framing or window parameters" << std::endl;
	}
} 

void Area::adjustFrame(){
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp){ 
	    frame.yMax = default_frame.yMax;
	    frame.yMin = default_frame.yMin;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
     
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta;		  	  
    }else{ 
	    frame.xMax = default_frame.xMax;
	    frame.xMin = default_frame.xMin;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
        
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta;		  	  
    }
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame){
	cr->translate(frame.width/2, frame.height/2);
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
  
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

void Area::refresh(){
	auto win = get_window();
	if(win){
		Gdk::Rectangle r(0,0, get_allocation().get_width(),
							get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	adjustFrame();
	orthographic_projection(cr, frame);	
	graphic_set_context(cr);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->paint();
	
	graphic_empty_world();
	draw_world();
	return true;
}

Fenetre::Fenetre() :
	m_Box(Gtk::ORIENTATION_HORIZONTAL,10),
	m_Box_Left(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Right(Gtk::ORIENTATION_HORIZONTAL, 10),
	m_Box_Top(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Middle(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Bottom(Gtk::ORIENTATION_VERTICAL, 10),  
 
	Button_exit("Exit"),
	Button_open("Open"),
	Button_save("Save"),
	Button_start("Start"),
	Button_step("Step"),
	Button_previous("Previous"),
	Button_next("Next"),
  
	m_Frame_Top("General"),
	m_Frame_Middle("Info"),
	m_Frame_Bottom("Fourmiliere"),
	m_Label_nbn("Nb food= " + get_nourriture_string()),
	m_Label_infoF("None selected"),
  
	i(0),
	f_selected(false),
	timer_added(false),
	disconnect(false),
	timeout_value(100){
		
	set_border_width(0);

	add(m_Box);
 
	m_Box.pack_start(m_Box_Left,false,false);
	m_Box.pack_start(m_Box_Right);
  
	m_Area.set_size_request(200,200);
	m_Box_Right.pack_start(m_Area);
  
	m_Frame_Top.add(m_Box_Top);
	m_Box_Left.pack_start(m_Frame_Top);
  
	m_Frame_Middle.add(m_Box_Middle);
	m_Box_Middle.pack_start(m_Label_nbn);
	m_Box_Left.pack_start(m_Frame_Middle);
  
	m_Frame_Bottom.add(m_Box_Bottom);
	m_Box_Left.pack_start(m_Frame_Bottom);
  
	m_Box_Top.set_border_width(10);
	m_Box_Middle.set_border_width(10);
	m_Box_Bottom.set_border_width(10); 
  
	m_Box_Top.pack_start(Button_exit);
	m_Box_Top.pack_start(Button_open);
	m_Box_Top.pack_start(Button_save);
	m_Box_Top.pack_start(Button_start);
	m_Box_Top.pack_start(Button_step);
  
	m_Box_Bottom.pack_start(Button_previous);
	m_Box_Bottom.pack_start(Button_next);
	m_Box_Bottom.pack_start(m_Label_infoF);
  
	Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_exit) );
	Button_open.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_open) );
	Button_save.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_save) );     
	Button_start.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_start) );
	Button_step.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_step) );              
	Button_previous.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_previous) ); 
	Button_next.signal_clicked().connect(sigc::mem_fun(*this,
              &Fenetre::on_button_clicked_next) );               
	show_all_children();
}

Fenetre::~Fenetre(){
}

void Fenetre::on_button_clicked_exit(){
	exit(EXIT_FAILURE);
}

void Fenetre::on_button_clicked_open(){
	Gtk::FileChooserDialog dialog("Please choose a file", 
				Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();
  
	switch(result){
		case(Gtk::RESPONSE_OK):{
			std::string filename = dialog.get_filename();
			clear_simulation();
			reset();
			lecture(filename);
			m_Area.refresh();
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			break;
		}
		default:{
			break;
		}
	}
}
void Fenetre::reset(){
	i=0;
	f_selected=false;
	m_Label_infoF.set_text("None selected");
	if(timer_added){
		std::cout << "manually disconnecting the timer " << std::endl;
		disconnect  = true;   
		timer_added = false;
		Button_start.set_label("Start");
	}		
}

void Fenetre::on_button_clicked_save(){
	Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();
  
	switch(result){
		case(Gtk::RESPONSE_OK):{
			std::string filename = dialog.get_filename();
			save_simulation(filename);
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			break;
		}
		default:{
			break;
		}
	}
}

void Fenetre::on_button_clicked_start(){
	if(not timer_added){	  
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &Fenetre::on_timeout),
										  timeout_value );
		timer_added = true;
		std::cout << "Timer added" << std::endl;
		Button_start.set_label("Stop");
	}else{
		std::cout << "manually disconnecting the timer " << std::endl;
		disconnect  = true;   
		timer_added = false;
		Button_start.set_label("Start");
	}		
}

void Fenetre::on_button_clicked_step(){
	if(not timer_added){
		on_timeout();
	}
}

void Fenetre::on_button_clicked_previous(){
	unsigned int nbf(get_nbF());
	vector<string>info;
	if(nbf==0){
		m_Label_infoF.set_text("None selected");
		f_selected=false;
		return;
	}
	if(f_selected==false){
		i=nbf-1;
		info=get_fourmiliere_string(i);
		string si = to_string(i);
		m_Label_infoF.set_text("id: "+si+"\nTotal food: "+info[0]+"\n\n"
							+"nbC: "+info[1]+"\nnbD: "+ info[2]+"\nnbP: "+ info[3]);
		f_selected=true;
		return;
	}
	if(i==0){
		m_Label_infoF.set_text("None selected");
		f_selected=false;
	}else{
		i-=1;
		info=get_fourmiliere_string(i);
		string si = to_string(i);
		m_Label_infoF.set_text("id: "+si+"\nTotal food: "+info[0]+"\n\n"
							+"nbC: "+info[1]+"\nnbD: "+ info[2]+"\nnbP: "+ info[3]);
	}
}

void Fenetre::on_button_clicked_next(){
	unsigned int nbf(get_nbF());
	if(nbf==0){
		m_Label_infoF.set_text("None selected");
		f_selected=false;
		return;
	}
	vector<string>info;
		
	if(f_selected==false){
		i=0;
		info=get_fourmiliere_string(i);
		m_Label_infoF.set_text("id: 0\nTotal food: "+info[0]+"\n\n"
							+"nbC: "+info[1]+"\nnbD: "+ info[2]+"\nnbP: "+ info[3]);
		f_selected=true;
		return;
	}
	if(i==nbf-1){
		m_Label_infoF.set_text("None selected");
		f_selected=false;
		return;
	}else{
		i+=1;
		string si = to_string(i);
		info=get_fourmiliere_string(i);
		m_Label_infoF.set_text("id: "+si+"\nTotal food: "+info[0]+"\n\n"
							+"nbC: "+info[1]+"\nnbD: "+ info[2]+"\nnbP: "+ info[3]);
	}
}

bool Fenetre::on_timeout(){
	static unsigned int val(1);
	if(disconnect){
		disconnect = false; 
		return false; 
	}
	std::cout << "This is simulation update number : " << val << std::endl;
	++val; 
	maj();
	m_Label_nbn.set_text("Nb food= " + get_nourriture_string());
	
	unsigned int nbf(get_nbF());
	if(nbf and f_selected){
		vector<string>info(get_fourmiliere_string(i));
		m_Label_infoF.set_text("id: 0\nTotal food: "+info[0]+"\n\n"
			+"nbC: "+info[1]+"\nnbD: "+ info[2]+"\nnbP: "+ info[3]);
	}else{
		m_Label_infoF.set_text("None selected");
	}
	m_Area.refresh();
	return true; 
}

bool Fenetre::on_key_press_event(GdkEventKey * key_event){
	if(key_event->type == GDK_KEY_PRESS){
		switch(gdk_keyval_to_unicode(key_event->keyval)){
			case 's':
				on_button_clicked_start();
				break;
			case '1':
				on_button_clicked_step();
				break;
			case 'n':
				on_button_clicked_next();
				break;
			case 'p':
				on_button_clicked_previous();
				break;
			case 'q':
				on_button_clicked_exit();
				break;
		}
	}
	return Gtk::Window::on_key_press_event(key_event);
}
