#include "gui.h"
#include <iostream>

using namespace std;

Gui::Gui(const std::string& filename):
	big_box(Gtk::Orientation::HORIZONTAL,2), 
	general_box(Gtk::Orientation::HORIZONTAL,2), 
	buttons_box(Gtk::Orientation::VERTICAL,2),
	info_box(Gtk::Orientation::VERTICAL,2),
	general("General"), info("Info : nombre de..."),
	button_exit("exit"), button_open("open"), button_save("save"),
	button_start("start"), button_step("step"),
	timer_added(false),	// faux = timer non lancé
	disconnect(false), 	// sert de relai pour une demande d'arret du timer
	timeout_value(500), // 500 ms = 0.5 seconds
	counter(0),
	started(false)
{
	//initialisation simulation**
	this->filename = filename;
	if (!filename.empty()) { 
		simu.lecture(filename.c_str()); //c_str renvoie un pointeur const char*
		simu.error_check();
		if(!simu.getError_simu()){
			//appeler fonction qui efface les données et crée un monde blanc cleardata() et emptyworld()
			monde_faux();
		} else {
			std::stringstream s1;
			int nbUpdate_ = simu.GetRs().GetNbUpdate(); 
			s1 << "mises à jour:" << nbUpdate_;
			nbUpdate.set_text(s1.str());
			update_infos();
		}
	}
	
	//interface
	set_child(big_box);
	
	//big box
	big_box.append(buttons_box);
	
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
	button_save.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_save_clicked));
	button_start.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_start_clicked));				     
	button_step.signal_clicked().connect(sigc::mem_fun(*this,
					     &Gui::on_button_step_clicked));				     				     
}

Gui::~Gui(){}

void Gui::update_infos()
{
	
	std::stringstream s2, s3, s4, s5, s6, s7, s8;
    
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
            filename = new_filename;
            new_simu.lecture(filename.c_str());
            new_simu.error_check();
            cout<<new_simu.getError_simu()<<endl; //à enlever !!
            if (new_simu.getError_simu()){
                simu = new_simu;
                int nbUpdate_ = simu.GetRs().GetNbUpdate(); 
				maj_label(nbUpdate_);
                update_infos();
            } else {
                std::cerr << "Erreur lors de la lecture du nouveau fichier." << std::endl;
                cout<<"monde faux"<<endl;
                monde_faux();
            }
            break;
        }
        case Gtk::ResponseType::CANCEL:
        {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default:
            break;
    }

    // Delete the dialog
    delete dialog;
}



void Gui::on_button_save_clicked() {
    auto dialog = new Gtk::FileChooserDialog("Save file", Gtk::FileChooser::Action::SAVE);

    dialog->set_transient_for(*this);
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(
				*this, &Gui::on_file_dialog_response_save), dialog));

    dialog->show();
}

void Gui::on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog) {
    // Handle the response
    switch (response_id) {
        case Gtk::ResponseType::OK:
        {
            std::string save_filename = dialog->get_file()->get_path();
            simu.save(save_filename);
            break;
        }
        case Gtk::ResponseType::CANCEL:
        {
			std::cout << "Cancel clicked." << std::endl;
			break;
		}

        default:
            break;
    }
    delete dialog;
}

void Gui::on_button_start_clicked(){
	
    started = !started;

    if (started) {
        button_start.set_label("stop");
        if (!timer_added) {
            sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
                                         &Gui::on_timeout));
            auto conn = Glib::signal_timeout().connect(my_slot, timeout_value);
            timer_added = true;
        }
    } else {
        button_start.set_label("start");
        counter = 0; // le counter est remis à 0 quand on clique sur stop
        disconnect = true;
        timer_added = false;
    }
}


bool Gui::on_timeout(){

	if(disconnect){
		disconnect = false; //reinitialise pour la prochaine fois qu'un timer est créé
		return false; // fin du timer
	}	
	//lance la simulation d'un incrément de temps
	simu.lance_simulation();
	
	// affiche le nombre de mises à jour
	int nbUpdate_ = simu.GetRs().GetNbUpdate() + counter + 1;
    maj_label(nbUpdate_);
    
    // actualise la valeur de nbUpdate dans le robot spatial et des particules
    simu.setRsNbUpdate(nbUpdate_);
    update_infos();
	
	return true;
}

void Gui::on_button_step_clicked(){
    if (!started) {
		
        // fais une single update
        int nbUpdate_ = simu.GetRs().GetNbUpdate() + 1;
        maj_label(nbUpdate_);
        
        //actualise la valeur de nbUpdate dans le robot spatial et des particules
        simu.setRsNbUpdate(nbUpdate_);
        simu.lance_simulation();
		update_infos();
    }
}	

void Gui::maj_label(int nbUpdate_){
	std::stringstream s1;
    s1 << "mises à jour:" << nbUpdate_;
    nbUpdate.set_text(s1.str());
}

void Gui::monde_faux(){
	
	std::stringstream s1, s2, s3, s4, s5, s6, s7, s8;
	
	s1 << "mises à jour:" << 0;
    nbUpdate.set_text(s1.str());

    s2 << "particules:" << 0;
    nbP.set_text(s2.str());  
    
    s3 << "robots réparateurs en service:" << 0;
    nbRs.set_text(s3.str()); 

    s4 << "robots réparateurs en réserve:" << 0;
    nbRr.set_text(s4.str()); 

    s5 << "robots neutraliseurs en service:" << 0;
    nbNs.set_text(s5.str());
    
    s6 << "robots neutraliseurs en panne:" << 0;
    nbNp.set_text(s6.str()); 
    
    s7 << "robots neutraliseurs détruits:" << 0;
    nbNd.set_text(s7.str()); 
    
    s8 << "robots neutraliseurs en réserve:" << 0;
    nbNr.set_text(s8.str());  	
}

