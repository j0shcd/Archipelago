// gui.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <iostream>
#include <cairomm/context.h>
#include <sstream>
#include <iomanip>

#include "gui.h"

using namespace std;

namespace { double zoom(1); 
			char type_noeud('L'); 
			double init_x(0);
			double init_y(0);
			static int state_node (0); 
}

Win::Win() :
	big_box(Gtk::ORIENTATION_HORIZONTAL,10), box_btns(Gtk::ORIENTATION_VERTICAL,10),
	box_general(Gtk::ORIENTATION_VERTICAL,10),
	box_display(Gtk::ORIENTATION_VERTICAL,10),
	box_editor(Gtk::ORIENTATION_VERTICAL,10),
	box_info(Gtk::ORIENTATION_VERTICAL,10), btn_exit("exit"), btn_new("new"), 
	btn_open("open"), btn_save("save"), btn_shortest_path("shortest path"), 
	btn_zoom_in("zoom in"), btn_zoom_out("zoom out"), btn_zoom_reset("zoom reset"),
	btn_edit_link("edit link"), btn_logement("logement"), btn_transport("transport"), 
	btn_production("production"), lbl_general("General"),lbl_display("Display"),
	lbl_editor("Editor"), lbl_info("Informations")
{
	set_title("Archipelago"); 
	set_border_width(0);
	
	add(big_box);
	box_btns.set_border_width(0);
	big_box.pack_start(box_btns, false, false); 
	big_box.pack_start(box_area);
	area.set_size_request(default_drawing_size, default_drawing_size); 
	box_area.pack_start(area);
	box_btns.pack_start(sep1); 
	box_btns.pack_start(box_general);
	box_btns.pack_start(sep2); 
	box_btns.pack_start(box_display);
	box_btns.pack_start(sep3); 
	box_btns.pack_start(box_editor);
	box_btns.pack_start(sep4); 
	box_btns.pack_start(box_info);
	box_btns.pack_start(sep5);
	
	box_general.pack_start(lbl_general, false, false); 
	box_general.pack_start(btn_exit,false,false); 
	box_general.pack_start(btn_new,false,false);
	box_general.pack_start(btn_open,false,false); 
	box_general.pack_start(btn_save,false,false);
	
	update_affichage();
	box_display.pack_start(lbl_display); 
	box_display.pack_start(btn_shortest_path,false,false);
	box_display.pack_start(btn_zoom_in,false,false);
	box_display.pack_start(btn_zoom_out,false,false); 
	box_display.pack_start(btn_zoom_reset,false,false);
	box_display.pack_start(lbl_zoom);
	
	
	box_editor.pack_start(lbl_editor);
	box_editor.pack_start(btn_edit_link,false,false);
	btn_transport.join_group(btn_logement);
	btn_production.join_group(btn_logement);
	box_editor.pack_start(btn_logement,false, false);
	box_editor.pack_start(btn_transport,false, false);
	box_editor.pack_start(btn_production,false, false);
	
	box_info.pack_start(lbl_info); 
	box_info.pack_start(lbl_ENJ); 
	box_info.pack_start(lbl_CI); 
	box_info.pack_start(lbl_MTA);
	
	btn_open.signal_clicked().connect(sigc::mem_fun(*this, &Win::on_btn_open));
	btn_new.signal_clicked().connect(sigc::mem_fun(*this,  &Win::on_btn_new) );
	btn_save.signal_clicked().connect(sigc::mem_fun(*this, &Win::on_btn_save));
	btn_exit.signal_clicked().connect(sigc::mem_fun(*this, &Win::on_btn_exit));
	btn_shortest_path.signal_clicked().connect(sigc::mem_fun(*this,
											&Win::on_btn_shortest_path));
	btn_zoom_in.signal_clicked().connect(sigc::mem_fun(*this, &Win::on_btn_zoom_in));
	btn_zoom_out.signal_clicked().connect(sigc::mem_fun(*this, 
											&Win::on_btn_zoom_out));
	btn_zoom_reset.signal_clicked().connect(sigc::mem_fun(*this, 
											&Win::on_btn_zoom_reset));
	btn_edit_link.signal_clicked().connect(sigc::mem_fun(*this, 
											&Win::on_btn_edit_link));
	btn_logement.signal_clicked().connect(sigc::mem_fun(*this,&Win::on_btn_logement));
	btn_transport.signal_clicked().connect(sigc::mem_fun(*this, 
											&Win::on_btn_transport));
	btn_production.signal_clicked().connect(sigc::mem_fun(*this,
											&Win::on_btn_production));
	show_all_children();
}

Win::~Win() {}

void Win::on_btn_open(){
	
	unselect(state_node);
	btn_edit_link.set_active(false);
	btn_shortest_path.set_active(false);
	area.clear();
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();
	
	switch (result) {
		
		case (Gtk::RESPONSE_OK) : {
			std::string filename = dialog.get_filename();
			lecture(filename);
			area.zoom_reset();
			update_affichage();
			area.show();
			break;
		}
		case (Gtk::RESPONSE_CANCEL) : {
			break;
		} 
		default : {
			cout << "unexpected button clicked" << endl;
			break;
	}
  }
}

void Win::on_btn_new() {
	unselect(state_node);
	btn_edit_link.set_active(false);
	btn_shortest_path.set_active(false);
	area.zoom_reset();
	update_affichage();
	area.clear();
}

void Win::on_btn_save(){
	
	Gtk::FileChooserDialog dialog("Please choose a file",
			Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();
	
	switch (result) {
		
		case (Gtk::RESPONSE_OK) : {
			std::string filename = dialog.get_filename();
			save(filename);
			break;
		}
		case (Gtk::RESPONSE_CANCEL) : {
			cout << "cancelled" << endl;
			break;
		} 
		default : {
			cout << "unexpected button clicked" << endl;
			break;
		}
	}
}

void Win::on_btn_exit(){
	hide();
}

void Win::on_btn_shortest_path() {
	if (btn_shortest_path.get_active()) 
		direct_shortest_path();
		
	if (not btn_shortest_path.get_active())
		reset_colors();
		
	area.queue_draw();
}

void Win::on_btn_zoom_in() {
	area.zoom_in();
	update_affichage();
}

void Win::on_btn_zoom_out() {
	area.zoom_out();
	update_affichage();
}

void Win::on_btn_zoom_reset() {
	area.zoom_reset();
	update_affichage();
}

void Win::on_btn_edit_link() {
	if (not noeud_actif()) {
		btn_edit_link.set_active(false);
	}
}

void Win::on_btn_logement() {
	type_noeud = 'L';
}

void Win::on_btn_transport() {
	type_noeud = 'T';
}

void Win::on_btn_production() {
	type_noeud = 'P';
}

void Win::update_affichage() {
	ostringstream zoom_stream;
	zoom_stream << std::fixed;
	zoom_stream << std::setprecision(2);
	zoom_stream << zoom;
	string zoom = zoom_stream.str();
	lbl_zoom.set_text("zoom : x" + zoom);
	
	update_criteres();
	CriteresEval criteres_ville = get_criteres();
	
	ostringstream ENJ_stream;
	double ENJ_double = criteres_ville.ENJ;
	ENJ_stream << std::setprecision(4);
	ENJ_stream << ENJ_double;
	string ENJ = ENJ_stream.str();
	lbl_ENJ.set_text("ENJ : " + ENJ);
	
	ostringstream CI_stream;
	double CI_double = criteres_ville.CI;
	CI_stream << std::setprecision(6);
	CI_stream << CI_double;
	string CI = CI_stream.str();
	lbl_CI.set_text("CI : " + CI);
	
	ostringstream MTA_stream;
	double MTA_double = criteres_ville.MTA;
	MTA_stream << std::setprecision(6);
	MTA_stream << MTA_double;
	string MTA = MTA_stream.str();
	lbl_MTA.set_text("MTA : " + MTA);
}

bool Win::on_button_press_event(GdkEventButton * event) {
	if(event->type == GDK_BUTTON_PRESS) {
		double clic_x = event->x, clic_y = event->y;
		double origin_x = area.get_allocation().get_x();
		double origin_y = area.get_allocation().get_y();
		double width = area.get_allocation().get_width();
		double height= area.get_allocation().get_height();
		init_x = clic_x, init_y = clic_y;
		bool sp_active(btn_shortest_path.get_active());
			
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height) {
			double x_modele ((((clic_x-origin_x)/800)*(2000) -1000)/zoom);
			double y_modele ((1000 -(( (clic_y-origin_y)/800)*(2000)))/zoom);
			if (event->button == 1) {
				nbPos uid = create_uid();
				if (btn_edit_link.get_active())
					edit_link(x_modele, y_modele, sp_active);
				else {
					delete_noeud(x_modele, y_modele);
					if (sp_active)
						find_noeud_courant(x_modele, y_modele, state_node, true);
					else find_noeud_courant(x_modele, y_modele, state_node, false);
					create_noeud(uid,x_modele, y_modele, min_capacity, type_noeud, 
								state_node);
				}
				area.queue_draw();
			}
			else if (event->button == 3) {
				if (state_node == 2) {
					deplace_noeud(x_modele, y_modele);
					area.queue_draw();
				}
			}
		}
	}
	return true;
}

bool Win::on_button_release_event(GdkEventButton * event){
	if(event->type == GDK_BUTTON_RELEASE){
		double clic_x = event->x;
		double clic_y = event->y;
		double origin_x = area.get_allocation().get_x();
		double origin_y = area.get_allocation().get_y();
		double width = area.get_allocation().get_width();
		double height= area.get_allocation().get_height();
		
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height)
		{
			if (event ->button == 1){
				if(!(clic_x == init_x and clic_y == init_y) 
					 and btn_edit_link.get_active()== false){
					size_noeud(init_x/zoom, init_y/zoom, clic_x/zoom, clic_y/zoom);
					area.queue_draw();
				}
				else if (state_node == 1){
					state_node = 2;
				}
				else if (state_node == 2 and (not btn_edit_link.get_active())) {
					unselect(state_node);
					area.queue_draw();
				}
			}
		}
	}
	return true;
}

void DrawArea::zoom_reset() {
	zoom = 1;
	refresh();
}

void DrawArea::zoom_in() {
	if( zoom <= 3){
		zoom += 0.2;
	}
	refresh();
}

void DrawArea::zoom_out() {
	if(zoom >= 0.4){
		zoom -= 0.2;
	}
	refresh();
}

void DrawArea::clear() {
	empty = true; 
	supprime_vector();
	refresh(); 
}

void DrawArea::show() {
	empty = false;
	refresh();
}

void DrawArea::refresh() {
	auto win = get_window();
	if(win) {
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						 get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}

bool DrawArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	set_context(cr);
	orthographic_projection(cr);
	cr->scale(zoom, zoom);
	dessine_ville();
	return true;
}

void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr) {
	 
	double default_drawing_size2(default_drawing_size);

	cr->translate(default_drawing_size2/2, default_drawing_size2/2);
  
	cr->scale(default_drawing_size2/(2*dim_max),
             -default_drawing_size2/(2*dim_max)); 

	cr->translate(0,0);
}


