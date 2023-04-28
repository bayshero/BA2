/*!
  \file   gui.cc
  \author Charly Guardia 50%, Gauthier de Mercey 50%
  \date   avril 2023
  \version 2
*/

#include "gui.h"
#include <iostream>

using namespace std;

//constructeur de la classe gui
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
	timeout_value(250), // 250 ms = 0.250 seconds
	counter(0),
	started(false)
{
	//initialisation simulation
	this->filename = filename;
	if (!filename.empty()) { 
		simu.lecture(filename); //c_str renvoie un pointeur const char*
		simu.error_check();
		if(!simu.getError_simu()){
			//appeler fonction qui efface les données et crée un monde blanc 
			monde_faux();
		} 
	}
	
	int nbUpdate_ = simu.getRs().getNbUpdate(); 
	maj_label(nbUpdate_);
	update_infos();
	
	// Interface
	set_child(big_box);
	setup_big_box();
	setup_general_box();
	setup_info_box();
	setup_buttons_box();
	connect_buttons_signals();
	add_key_controller();		     				     
}

void Gui::setup_big_box()
{
	big_box.append(buttons_box);
	m_area.set_size_request(taille_dessin, taille_dessin);
	m_area.set_expand(true);
	area_box.append(m_area);
	big_box.append(area_box);
}

void Gui::setup_general_box()
{
	general_box.append(general);
}

void Gui::setup_info_box()
{
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
}

void Gui::setup_buttons_box()
{
	buttons_box.append(general_box);
	buttons_box.append(button_exit);
	buttons_box.append(button_open);
	buttons_box.append(button_save);
	buttons_box.append(button_start);
	buttons_box.append(button_step);
	buttons_box.append(info_box);
}

void Gui::connect_buttons_signals()
{
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

void Gui::add_key_controller()
{
	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Gui::on_window_key_pressed), false);
                  
	add_controller(controller);	
    
}
    
   

//destructeur de la classe gui
Gui::~Gui(){}



//actualise les valeurs affichées dans l'interface
void Gui::update_infos()
{
	std::stringstream s2, s3, s4, s5, s6, s7, s8;
    
	int nbP_ = simu.getParticules().size();
	s2 << "particules:" << nbP_;
	nbP.set_text(s2.str());  
    
	int nbRs_ = simu.getRs().getNbRs();
	s3 << "robots réparateurs en service:" << nbRs_;
	nbRs.set_text(s3.str()); 

	int nbRr_ = simu.getRs().getNbRr();
	s4 << "robots réparateurs en réserve:" << nbRr_;
	nbRr.set_text(s4.str()); 

	int nbNs_ = simu.getRs().getNbNs();
	s5 << "robots neutraliseurs en service:" << nbNs_;
	nbNs.set_text(s5.str());
    
	int nbNp_ = simu.getRs().getNbNp();
	s6 << "robots neutraliseurs en panne:" << nbNp_;
	nbNp.set_text(s6.str()); 
    
	int nbNd_ = simu.getRs().getNbNd();
	s7 << "robots neutraliseurs détruits:" << nbNd_;
	nbNd.set_text(s7.str()); 
    
	int nbNr_ = simu.getRs().getNbNr();
	s8 << "robots neutraliseurs en réserve:" << nbNr_;
	nbNr.set_text(s8.str());
}

//bouton exit
void Gui::on_button_exit_clicked(){
	hide();
}

//bouton open
void Gui::on_button_open_clicked() {
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
								    Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this); 
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(sigc::mem_fun(
				*this, &Gui::on_file_dialog_response_open), dialog));
	
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Open", Gtk::ResponseType::OK);
    
	//filtres
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	
	//ouvre la fenêtre et attend une réponse de l'utilisateur
	dialog->show();
}

//cette fonction permet de gérer la réponse choisie pendant l'interaction avec open
void Gui::on_file_dialog_response_open(int response_id,Gtk::FileChooserDialog* dialog){
	// Handle the response
	switch (response_id) {
		case Gtk::ResponseType::OK:
		{
			auto new_filename = dialog->get_file()->get_path();
			//supprimer les anciennes données avant d'appeler lecture()
			simu.delete_simu();
			simu.lecture(new_filename);
			simu.error_check();

			if (simu.getError_simu()){
				//si il n'y a pas d'erreur dans la simulation
				//les infos et le dessin s'updatent à nouveau
				filename = new_filename;
				int nbUpdate_ = simu.getRs().getNbUpdate(); 
				maj_label(nbUpdate_);
				update_infos();
				m_area.queue_draw();
			} else {
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

	// hide the dialog
dialog->hide();
}

//bouton save
void Gui::on_button_save_clicked() {
	auto dialog=new Gtk::FileChooserDialog("Save file",Gtk::FileChooser::Action::SAVE);

	dialog->set_transient_for(*this);
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Save", Gtk::ResponseType::OK);
	dialog->signal_response().connect(sigc::bind(sigc::mem_fun(
				*this, &Gui::on_file_dialog_response_save), dialog));

	//ouvre la fenêtre et attend la réponse de l'utilisateur
	dialog->show();
}

//gére la réponse choisie pendant l'interaction avec save
void Gui::on_file_dialog_response_save(int response_id,Gtk::FileChooserDialog* dialog){
    //gère la réponse
    switch (response_id){
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
    dialog->hide();
}

//bouton start
void Gui::on_button_start_clicked(){
	
    started = !started; 

    if (started){
        button_start.set_label("stop");
        if (!timer_added){
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
	int nbUpdate_ = simu.getRs().getNbUpdate() + counter + 1;
    maj_label(nbUpdate_);
    
    // actualise la valeur de nbUpdate dans le robot spatial et update les infos
    simu.setRsNbUpdate(nbUpdate_);
    update_infos();
    
    //update le dessin
    m_area.queue_draw();
	
	return true;
}

void Gui::on_button_step_clicked(){
	//bouton step fonctionne que si started vaut false
    if (!started) {
		
        // fais une single update
        int nbUpdate_ = simu.getRs().getNbUpdate() + 1;
        maj_label(nbUpdate_);
        
        //actualise la valeur de nbUpdate dans le robot spatial et update les infos
        simu.setRsNbUpdate(nbUpdate_);
        simu.lance_simulation();
        m_area.queue_draw();
		update_infos();
    }
}	

void Gui::maj_label(int nbUpdate_){
	std::stringstream s1;
    s1 << "mises à jour:" << nbUpdate_;
    nbUpdate.set_text(s1.str());
}

//si le fichier rentré est faux, la fenêtre gui affiche un monde blanc
void Gui::monde_faux(){
	
	simu.delete_simu(); //efface les données de la simulation
	int nbUpdate_ = simu.getRs().getNbUpdate(); 
	maj_label(nbUpdate_);
	update_infos();
	m_area.queue_draw();
}

//entrées touches de clavier
bool Gui::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state){
	switch(gdk_keyval_to_unicode(keyval))
	{
		case 's':
			std::cout << "key 's' has been pressed !" << std::endl;
			on_button_start_clicked();
			return true;
		case '1':
			std::cout << " key '1' has been pressed !" << std::endl;
			on_button_step_clicked();
			return true;
	}
    //the event has not been handled
    return false;
}


// parametres par default du cadre et de la fenetre
static Frame default_frame = {-dmax, dmax, -dmax, dmax, 1, 500, 500}; 

MyArea::MyArea(){
	set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));   
	setFrame(default_frame);
}

MyArea::~MyArea(){
}

// definition du cadre de l'espace modele a visualiser dans le canvas de la fenetre
void MyArea::setFrame(Frame f){
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0)){
		f.asp = f.width/f.height;
		frame = f;
	}
	else
		std::cout << "incorrect Model framing or window parameters" << std::endl;
} 

void MyArea::adjustFrame(int width, int height){
	frame.width  = width;
	frame.height = height;

	// empeche la distortion en ajustant le cadre
	// pour avoir la meme proportion que la zone graphique
	double new_aspect_ratio((double)width/height);
	if(new_aspect_ratio > default_frame.asp){ 
		// conserver yMax et yMin. Ajuster xMax et xMin
		frame.yMax = default_frame.yMax ;
		frame.yMin = default_frame.yMin ;	
	  
		double delta(default_frame.xMax - default_frame.xMin);
		double mid((default_frame.xMax + default_frame.xMin)/2);
		// nouveau cadre est centre sur le point median suivant x
		frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
		frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
	}
	else{ // conserver xMax et xMin. Ajuster yMax et yMin
		frame.xMax = default_frame.xMax ;
		frame.xMin = default_frame.xMin ;
	  	
		double delta(default_frame.yMax - default_frame.yMin);
		double mid((default_frame.yMax + default_frame.yMin)/2);
        // le cadre est centre sur le point median suivant x
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

void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr,const int width,
					 const int height){
	adjustFrame(width, height);
	orthographic_projection(cr, frame);
	graphic_set_context(cr);
	//initaialise monde
	empty_world();
	//dessine les objets, s'il y en a
	draw_world();
}
