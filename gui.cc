#include "gui.h"
#include <iostream>
#include <gtkmm/filechooserdialog.h>
#include "constantes.h"
#include "graphic.h"
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

MyArea::MyArea()
{
	set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
}

MyArea::~MyArea()
{
}

void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr,const int width,const int height)
{
	graphic_set_context(cr);
	
	cr->set_source_rgb(255.0, 255.0, 255.0 );
	cr->paint();
	
	//centre de fenetre
	//int xc, yc;
//	xc = width / 2;
	//yc = height / 2;
	
	empty_world(taille_dessin);
}
