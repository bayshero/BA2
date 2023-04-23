#include "gui.h"
#include <iostream>
#include <gtkmm/filechooserdialog.h>
#include "constantes.h"
#include "graphic.h"
#include "simulation.h"
using namespace std;



Gui::Gui(const std::string& filename):
	big_box(Gtk::Orientation::HORIZONTAL,2), 
	general_box(Gtk::Orientation::HORIZONTAL,2), 
	buttons_box(Gtk::Orientation::VERTICAL,2),
	info_box(Gtk::Orientation::VERTICAL,2), 
	general("General"), info("Info : nombre de..."),
	button_exit("exit"), button_open("open"), button_save("save"),
	button_start("start"), button_step("step")
{
	//initialisation simulation**
	this->filename = filename;
	if (!filename.empty()) { 
		simu.lecture(filename.c_str()); //c_str renvoie un pointeur const char*
		simu.error_check();
	}
	
	//interface***********************
	set_child(big_box);
	
	//big box
	big_box.append(buttons_box);
	m_area.set_size_request(taille_dessin);
	m_area.set_expand(true); //super cool a fixer distortion
	area_box.append(m_area);
	big_box.append(area_box);
	
	//general box**
	general_box.append(general);
	
	//info box**
	info_box.append(info);
	info_box.append(nbUpdate);
	info_box.append(nbP);
	info_box.append(nbRs);
	info_box.append(nbRr);
	info_box.append(nbNs);
	info_box.append(nbNp);
	info_box.append(nbNd);
	info_box.append(nbNr);
	
	info.set_halign(Gtk::Align::START);
	nbUpdate.set_halign(Gtk::Align::START);
	nbP.set_halign(Gtk::Align::START);
	nbRs.set_halign(Gtk::Align::START);
	nbRr.set_halign(Gtk::Align::START);
	nbNs.set_halign(Gtk::Align::START);
	nbNp.set_halign(Gtk::Align::START);
	nbNd.set_halign(Gtk::Align::START);
	nbNr.set_halign(Gtk::Align::START);
	
	//buttons box**
	buttons_box.append(general_box);
	buttons_box.append(button_exit);
	buttons_box.append(button_open);
	buttons_box.append(button_save);
	buttons_box.append(button_start);
	buttons_box.append(button_step);
	buttons_box.append(info_box);
	
	//fonctions appelées par les boutons
	button_exit.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_exit_clicked));
	button_open.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_open_clicked));
	//button_save.signal_clicked().connect(sigc::mem_fun(*this,
		//			     &Gui::on_button_save_clicked));
	button_start.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_start_clicked));				     
	button_step.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_step_clicked));				     				     
	//mettre a jour les labels
	update_infos();
}

Gui::~Gui(){}

void Gui::update_infos()
{
	std::stringstream s1, s2, s3, s4, s5, s6, s7, s8;
    int nbUpdate_ = simu.GetRs().GetNbUpdate();
    s1 << "mises à jour:" << nbUpdate_;
    nbUpdate.set_text(s1.str());
    
    int nbP_ = simu.GetParticules().size();
    s2 << "particules:" << nbP_;
    nbP.set_text(s2.str());  
    
    int nbRs_ = simu.GetRs().GetNbRs();
    s3 << "robots réparateurs en service:" << nbRs_;
    nbRs.set_text(s3.str()); 

    int nbRr_ = simu.GetRs().GetNbRr();
    s4 << "robots réparateurs en réserve:" << nbRr_;
    nbRr.set_text(s4.str()); 

    int nbNs_ = simu.GetRs().GetNbNs();
    s5 << "robots neutraliseurs en service:" << nbNs_;
    nbNs.set_text(s5.str());
    
    int nbNp_ = simu.GetRs().GetNbNp();
    s6 << "robots neutraliseurs en panne:" << nbNp_;
    nbNp.set_text(s6.str()); 
    
    int nbNd_ = simu.GetRs().GetNbNd();
    s7 << "robots neutraliseurs détruits:" << nbNd_;
    nbNd.set_text(s7.str()); 
    
    int nbNr_ = simu.GetRs().GetNbNr();
    s8 << "robots neutraliseurs en réserve:" << nbNr_;
    nbNr.set_text(s8.str());  
      
}

void Gui::on_button_exit_clicked(){
    hide();
}

void Gui::on_button_open_clicked() {
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
								    Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(sigc::mem_fun(
				*this, &Gui::on_file_dialog_response_open), dialog));
	
    // Add response buttons to the dialog
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);
    
    //filters
    auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);

    // Show the dialog
    dialog->show();
}

void Gui::on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog) {
    // Handle the response
    switch (response_id) {
        case Gtk::ResponseType::OK:
        {
            std::string new_filename = dialog->get_file()->get_path();
            Simulation new_simu;
            new_simu.lecture(new_filename.c_str());
            new_simu.error_check();
            simu = new_simu;
            filename = new_filename;
            update_infos();
            break;
        }
        case Gtk::ResponseType::CANCEL:
        {
			std::cout << "Cancel clicked." << std::endl;
			break;
		}

        default:
            // Do nothing if the user cancels the dialog
            break;
    }

    // Delete the dialog
    delete dialog;
}

/*
void Gui::on_button_save_clicked() {
    auto dialog = new Gtk::FileChooserDialog("Save file", Gtk::FileChooser::Action::SAVE);

    // Add response buttons to the dialog
    dialog->set_transient_for(*this);
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(
				*this, &Gui::on_file_dialog_response_save), dialog));

    //int result = dialog->show();

    // Handle the response
    switch (result) {
        case Gtk::ResponseType::OK:
        {
            std::string save_filename = dialog->get_file()->get_path();

            // Save the updated values to the chosen file
            simu.save(save_filename.c_str());

            break;
        }
        case Gtk::ResponseType::CANCEL:
        default:
            // Do nothing if the user cancels the dialog
            break;
    }
}

void Gui::on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog) {
    // Handle the response
    switch (response_id) {
        case Gtk::ResponseType::OK:
        {
            
        }
        case Gtk::ResponseType::CANCEL:
        {
			std::cout << "Cancel clicked." << std::endl;
			break;
		}

        default:
            // Do nothing if the user cancels the dialog
            break;
    }

    // Delete the dialog
    delete dialog;
}
*/

void Gui::on_button_start_clicked(){
	
}

void Gui::on_button_step_clicked(){
	
}	

// default Model Framing and window parameters
static Frame default_frame = {0, 500, 0, 500, 1.0, 500, 500}; 


MyArea::MyArea(){
	set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));   
	setFrame(default_frame);
}

MyArea::~MyArea(){
}

// defining the Model space frame to visualize in the window canvas
void MyArea::setFrame(Frame f){
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
	{
		f.asp = f.width/f.height;
		frame = f;
	}
	else
		std::cout << "incorrect Model framing or window parameters" << std::endl;
} 

void MyArea::adjustFrame(int width, int height){
	frame.width  = width;
	frame.height = height;

	// Preventing distorsion by adjusting the frame (cadrage)
	// to have the same proportion as the graphical area
	
    // use the reference framing as a guide for preventing distortion
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp)
    { // keep yMax and yMin. Adjust xMax and xMin
	    frame.yMax = default_frame.yMax ;
	    frame.yMin = default_frame.yMin ;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
        // the new frame is centered on the mid-point along X
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
    }
    else
    { // keep xMax and xMin. Adjust yMax and yMin
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
        // the new frame is centered on the mid-point along Y
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    }
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
								    Frame frame){
	// déplace l'origine au centre de la fenêtre
	cr->translate(frame.width/2, frame.height/2);
  
	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// ET inverse la direction de l'axe Y
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
  
	// décalage au centre du cadrage
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}
/*
#include <pango/pangocairo.h>//temporary

void draw_axes(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame) {//temporary
    // Save the current transformation matrix
    cr->save();

    // Set up the orthographic projection
    orthographic_projection(cr, frame);

    // Set line width and color for the axes
    cr->set_line_width(1.0);
    cr->set_source_rgb(0.0, 0.0, 0.0); // Black

    // Draw the X axis
    cr->move_to(frame.xMin, 0.0);
    cr->line_to(frame.xMax, 0.0);

    // Draw the Y axis
    cr->move_to(0.0, frame.yMin);
    cr->line_to(0.0, frame.yMax);

    // Stroke the axes
    cr->stroke();

    // Draw labels for the axes using Pango
    Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
    layout->set_font_description(Pango::FontDescription("Arial Bold 10"));

    // Label for the X axis
    layout->set_text("X");
    cr->move_to(frame.xMax + 5, -15);
    layout->show_in_cairo_context(cr);

    // Label for the Y axis
    layout->set_text("Y");
    cr->move_to(5, frame.yMax - 5);
    layout->show_in_cairo_context(cr);

    // Restore the transformation matrix
    cr->restore();
}
*/


void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr,const int width,const int height){
	adjustFrame(width, height);
	orthographic_projection(cr, frame);
	graphic_set_context(cr);
	
	
	cr->set_source_rgb(1.0, 1.0, 1.0 );
	cr->paint();
	//  draw_axes(cr, frame);
	
	//centre de fenetre
	//int xc, yc;
//	xc = width / 2;
	//yc = height / 2;
	
	empty_world(taille_dessin);
	draw_world();
}
