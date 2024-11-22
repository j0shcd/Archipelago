// gui.h  // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <gtkmm.h>

#include "graphic_gui.h"
#include "ville.h"

class DrawArea : public Gtk::DrawingArea {
	
public:

	DrawArea(){}
	virtual ~DrawArea() {}
	void clear();
	void show();
	void zoom_in();
	void zoom_out();
	void zoom_reset();
	void refresh_2();
	
protected:

	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  
private:

	bool empty;
	void refresh();
	
};	

class Win : public Gtk::Window {

public:
	Win();
	virtual ~Win();
	void conditions_intiales(std::string fichier);
	
private: /// il faut encore mettre indication du zoom (rendu3), 
		   /// prochain type de noeud(rendu3), affichage des critères(rendu2)
	void on_btn_open();
	void on_btn_new();
	void on_btn_save();
	void on_btn_exit();
	void on_btn_shortest_path();
	void on_btn_zoom_in(); 
	void on_btn_zoom_out(); 
	void on_btn_zoom_reset();
	void on_btn_edit_link();
	void on_btn_logement();
	void on_btn_transport();
	void on_btn_production();
	
	Gtk::Separator sep1;
	Gtk::Separator sep2;
	Gtk::Separator sep3;
	Gtk::Separator sep4;
	Gtk::Separator sep5;

	Gtk::Box big_box;
	Gtk::Box box_btns;
	Gtk::Box box_area;
	Gtk::Box box_general;
	Gtk::Box box_display;
	Gtk::Box box_editor;
	Gtk::Box box_info;
	
	//rendu2
	Gtk::Button btn_exit;
	Gtk::Button btn_new;
	Gtk::Button btn_open;
	Gtk::Button btn_save;
	//rendu3
	Gtk::ToggleButton btn_shortest_path;
	Gtk::Button btn_zoom_in;
	Gtk::Button btn_zoom_out;
	Gtk::Button btn_zoom_reset;
	Gtk::ToggleButton btn_edit_link;
	Gtk::RadioButton btn_logement;
	Gtk::RadioButton btn_transport;
	Gtk::RadioButton btn_production;
	
	Gtk::Label lbl_general; 
	Gtk::Label lbl_display;
	Gtk::Label lbl_zoom;
	Gtk::Label lbl_editor;
	Gtk::Label lbl_info;
	Gtk::Label lbl_ENJ;
	Gtk::Label lbl_CI;
	Gtk::Label lbl_MTA;

	DrawArea area;
	bool on_button_press_event(GdkEventButton * event);
	bool on_button_release_event(GdkEventButton * event);
	void update_affichage();
};

void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr);

#endif //GUI_HEADER_H
