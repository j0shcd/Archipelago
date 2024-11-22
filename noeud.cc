// noeud.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>

#include "error.h"
#include "noeud.h"
#include "tools.h"

namespace {
	std::vector<Noeud> noeuds;
	std::vector<Noeud> logement;
	std::vector<Noeud> transport;
	std::vector<Noeud> production;
	std::vector<Lien> liens;
}

using namespace std;

bool create_node (nbPos uid, double x, double y, nbPos nbp, char type, 
				  bool created_by_click){
	double dist(0);
	if (created_by_click) {
		dist = dist_min;
	}
	
	if (test_node(uid, x, y, nbp, dist)) {
		if (type == 'L') {
			add_logement(uid, x, y, nbp, type);
		}
		if (type == 'T') {
			add_transport(uid, x, y, nbp, type);
		}
		if (type == 'P') {
			add_production(uid, x, y, nbp, type);
		}
		return true;
	}
	else if (not created_by_click) {
		supprime_liens_noeuds();
		return false;
	}
	return false;
}

bool create_link (nbPos uid1, nbPos uid2, bool created_by_click) {
	double dist(0);
	if (created_by_click) {
		dist = dist_min;
	}
	
	int rank_node1, rank_node2;
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid3 = noeuds[i].get_uid();
		if(uid3 == uid1) {
			rank_node1 = i;
		}
	}

	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid4 = noeuds[i].get_uid();
		if(uid4 == uid2) {
			rank_node2 = i;
		}
	}

	if(test_link(uid1, uid2, dist)) {
		noeuds[rank_node1].set_link(uid2);
		noeuds[rank_node2].set_link(uid1);
		add_link(noeuds[rank_node1].get_x(),noeuds[rank_node1].get_y(), uid1,
				noeuds[rank_node2].get_x(),noeuds[rank_node2].get_y(), uid2);
	
		return true;
	}
	else if (not created_by_click) {
		supprime_liens_noeuds();
		return false;
	}
	
	return false;
}

void change_color_with_uid(nbPos uid, int couleur) {	
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid1 = noeuds[i].get_uid();
		noeuds[i].set_couleur(BLACK);
		if(uid == uid1) {
			noeuds[i].set_couleur(couleur);
		}
	}
	
}

bool test_node (nbPos uid, double x, double y, nbPos nbp, double dist) {
	
	if (noeuds.size()==0) {
		///test reserved uid
		if (uid==no_link) {
			cout << error::reserved_uid();
			return false;
		}
		
		///test too little capacity 
		
		if (nbp < min_capacity) {
			cout << error::too_little_capacity(nbp);
			return false;
		}; 
		
		///test too much capacity
		
		if (nbp > max_capacity) {
			cout << error::too_much_capacity(nbp);
			return false;
		};
		return true;
	}
	
	return success_node (uid, x, y, nbp, dist);
}

bool test_link (nbPos uid1, nbPos uid2, double dist){
	
	bool success = success_link_the_1st(uid1, uid2) 
				   and success_link_the_2nd(uid1, uid2, dist)
				   and success_link_the_3rd(uid1, uid2);
	
	return success;
}

void add_logement (nbPos const& uid, double const& x, double const& y, 
		nbPos const& nbp, char type) {

	Noeud node(uid, x, y, nbp, type);
	logement.push_back(node);
	noeuds.push_back(node);
}

void add_transport (nbPos const& uid, double const& x, double const& y, 
	   nbPos const& nbp, char type) {

	Noeud node(uid, x, y, nbp, type);
	transport.push_back(node);
	noeuds.push_back(node);
}

void add_production (nbPos const& uid, double const& x, double const& y, 
	  nbPos const& nbp, char type) {
						 
	Noeud node(uid, x, y, nbp, type);
	production.push_back(node);
	noeuds.push_back(node);
}

void add_link(double x1, double y1, nbPos uid1, double x2, double y2, nbPos uid2) {
	Lien lien({{x1,y1}, uid1,{x2,y2}, uid2, BLACK});
	liens.push_back(lien);
}


bool success_node(nbPos const& uid, double const& x, double const& y, 
				  nbPos const& nbp, double dist) {
		if (uid==no_link) {
			cout << error::reserved_uid();
			return false;
		}
		if (nbp < min_capacity) {
			cout << error::too_little_capacity(nbp);
			return false;
		}; 
		if (nbp > max_capacity) {
			cout << error::too_much_capacity(nbp);
			return false;
		};
	for (nbPos i(0); i < noeuds.size(); ++i) { 

		if (uid == noeuds[i].get_uid()) {
			cout << error::identical_uid(uid);
			return false;
		}
		Cercle NewNode = {x, y, sqrt(nbp)};
		Cercle ExistingNode = {noeuds[i].get_x(), noeuds[i].get_y(), 
							   sqrt(noeuds[i].get_nbp())};
		
		if (inter_cercles(ExistingNode, NewNode, dist)) {
			cout<< error::node_node_superposition(uid,noeuds[i].get_uid());
			return false;
		}
	}
	Cercle node; // node-link
	node.centre ={x,y};
	node.r = sqrt(nbp);
	for (nbPos j(0); j< liens.size(); ++j) {
		if (inter_seg_cer(node,liens[j].coord1.x,liens[j].coord2.x,liens[j].coord1.y,
						  liens[j].coord2.y,dist)) {
			cout << error::node_link_superposition(uid);
			return false;
		}
	}
	return true;
}

bool success_link_the_1st (nbPos uid1,nbPos uid2) {

	int j(0); //test link_vacuum
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (uid1 == noeuds[i].get_uid()){
			++j;
		}
	}
	if (j==0) {
		cout << error::link_vacuum(uid1);
		return false;
	}
	int h(0);
	for (nbPos k(0); k < noeuds.size(); ++k) {
		if (uid2==noeuds[k].get_uid()) {
			++h;
		}
	}
	if (h==0) {
		cout << error::link_vacuum(uid2);
		return false;
	}
	return true;
}

bool success_link_the_2nd (nbPos uid1, nbPos uid2, double dist) {
	
	nbPos rank_node1(0), rank_node2(0);
	for(nbPos i(0); i < noeuds.size(); ++i) {
		nbPos uid3 = noeuds[i].get_uid();
		if(uid1 == uid3) { rank_node1 = i; }
	}
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid4 = noeuds[i].get_uid();
		if(uid4 == uid2) { rank_node2 = i; }
	}
	
	int k(0); // test multiple same link
	for (nbPos i(0); i < noeuds[rank_node2].get_links().size() ; ++i) {
		if (noeuds[rank_node2].get_links()[i]==uid1) {++k;}
	}
	if (k>=1) {
		cout << error::multiple_same_link(uid1,uid2);
		return false;
	}
	
	Coord point1 = {noeuds[rank_node1].get_x(), noeuds[rank_node1].get_y()}; //node-
	Coord point2 = {noeuds[rank_node2].get_x(), noeuds[rank_node2].get_y()}; //link
	Coord vecteur_lien = vecteur(point1, point2); Segment lien;
	lien.point = point2; lien.vecteur = vecteur_lien;
	for(nbPos i(0); i < noeuds.size() ; ++i) {
		if (noeuds[rank_node1].get_uid() == noeuds[i].get_uid() or
		noeuds[rank_node2].get_uid() == noeuds[i].get_uid()) {continue;}
		Cercle node;
		node.centre = {noeuds[i].get_x(),noeuds[i].get_y()};
		node.r = sqrt(noeuds[i].get_nbp());
		if (inter_seg_cer(node,point1.x,point2.x,point1.y,point2.y,dist)) {
			cout << error::node_link_superposition(noeuds[i].get_uid());
			return false;
		}
	}
	return true;
}

bool success_link_the_3rd (nbPos uid1, nbPos uid2) {

	int rank_node1, rank_node2; //find node with uid x
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid3 = noeuds[i].get_uid();
		if(uid1 == uid3) {
			rank_node1 = i;
		}
	}
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid4 = noeuds[i].get_uid();
		if(uid4 == uid2) {
			rank_node2 = i;
		}
	}

	if (uid1 == uid2) { //test self link node
		cout << error::self_link_node(uid1);
		return false;
	}

	if (noeuds[rank_node1].get_type() == 'L') { //test max link
		if (noeuds[rank_node1].get_links().size() > (max_link-1) ){ 
			cout << error::max_link(uid1);
			return false;
		}
	}
	if (noeuds[rank_node2].get_type() == 'L') {
		if (noeuds[rank_node2].get_links().size() > (max_link-1) ){ 
			cout << error::max_link(uid2);
			return false;
		}
	}
	
	return true;
}
nbPos new_uid(){
	nbPos i(1);
	while(i>0){
		nbPos k(0);
		for(nbPos j(0); j < noeuds.size(); j++){
			if (noeuds[j].get_uid() == i){
				++k;
			}
		}
		if(k < 1){
			return i;
		}
		++i;
	}
	return 0;
}
		
///méthodes de la classe noeud

Noeud::Noeud(nbPos uid, double x, double y, nbPos nbp, char type, bool in, 
			 double access, nbPos parent, int couleur)
: uid(uid), nbp(nbp), type(type), in(true), access(infinite_time), parent(no_link), 
  couleur(BLACK) 
{
	position.x = x, position.y = y; 
}

nbPos Noeud::get_uid() const {
	return uid;
}

nbPos Noeud::get_nbp() const {
	return nbp;
}

double Noeud::get_x() const {
	return position.x;
}

double Noeud::get_y() const {
	return position.y;
}

Coord Noeud::get_position() const {
	return {position.x, position.y};
}

std::vector<nbPos> Noeud::get_links () const {
	return links;
}

char Noeud::get_type() const {
	return type;
}

bool Noeud::get_in()const {
	return in;
}

double Noeud::get_access()const{
	return access;
}

nbPos Noeud::get_parent()const{
	return parent;
}

int Noeud::get_couleur() const {
	return couleur;
}

void Noeud::set_uid (nbPos uid_set) {
	uid = uid_set;
}

void Noeud::set_nbp (nbPos nbp_set) { 
	nbp = nbp_set;
}

void Noeud::set_position(double position_set_x, double position_set_y){
	position.x = position_set_x;
	position.y = position_set_y;
}

void Noeud::set_link (nbPos uid) {
	links.push_back(uid);
}

void Noeud::set_type(char type_set) {
	type = type_set;
}

void Noeud::set_in(bool new_in) {
	in = new_in;
}

void Noeud::set_access(double new_access) {
	access = new_access;
}

void Noeud::set_parent(nbPos new_parent) {
	parent = new_parent;
}

void Noeud::set_couleur(int new_couleur) {
	couleur = new_couleur;
}

void Noeud::remove_link(nbPos uid) {
	for (nbPos i(0); i < links.size(); ++i) {
		if (uid == links[i]) {
			links.erase(links.begin() + i);
		}
	}
}

void supprime_liens_noeuds() {
	liens.clear();
	noeuds.clear();
	logement.clear();
	transport.clear();
	production.clear();
}

//sauvegarde

void sauvegarde_noeuds(ofstream& myfile) {
	
	string nblog = std::to_string(logement.size());
	myfile << nblog + "    # housing" + "\n";
	for (nbPos i(0); i < logement.size(); ++i) {
		string uid = std::to_string(logement[i].get_uid());
		string x = std::to_string(logement[i].get_x());
		string y = std::to_string(logement[i].get_y());
		string nbp = std::to_string(logement[i].get_nbp());
		myfile << "    " + uid + " " << x + " " << y + " " << nbp  + "\n"; 
	}
	string nbtra = std::to_string(transport.size());
	myfile << "\n" + nbtra + "    # transport" + "\n";
	for (nbPos j(0); j < transport.size(); ++j) {
		string uid = std::to_string(transport[j].get_uid());
		string x = std::to_string(transport[j].get_x());
		string y = std::to_string(transport[j].get_y());
		string nbp = std::to_string(transport[j].get_nbp());
		myfile << "     " + uid + " " << x + " " << y + " " << nbp  + "\n";
	}
	string nbpro = std::to_string(production.size());
	myfile << "\n" + nbpro + "    # production" + "\n";
	for (nbPos k(0); k < production.size(); ++k) {
		string uid = std::to_string(production[k].get_uid());
		string x = std::to_string(production[k].get_x());
		string y = std::to_string(production[k].get_y());
		string nbp = std::to_string(production[k].get_nbp());
		myfile << "     " + uid + " " << x + " " << y + " " << nbp  + "\n\n";
	}
}

void sauvegarde_liens(ofstream& myfile) {
	string nblin = std::to_string(liens.size());
	myfile << "\n" + nblin + "    # links" + "\n";
	for (nbPos i(0); i < liens.size(); i++) {
		string uid1 = std::to_string(liens[i].uid1);
		string uid2 = std::to_string(liens[i].uid2);
		myfile << "     " + uid1 + " " + uid2 + "\n";
	}
}

// graphic

void dessine_quartier()
{
	for (nbPos j(0); j<liens.size();++j)
	{
		double x1 = liens[j].coord1.x;
		double y1 = liens[j].coord1.y;
		double x2 = liens[j].coord2.x;
		double y2 = liens[j].coord2.y;
		int couleur = liens[j].couleur;
		dessine_segment_tools(x1,y1,x2,y2,couleur);
	}
		
	for (nbPos i(0); i< noeuds.size(); ++i)
	{
		nbPos nbp = noeuds[i].get_nbp();
		double x = noeuds[i].get_x();
		double y = noeuds[i].get_y();
		char type = noeuds[i].get_type();
		int couleur = noeuds[i].get_couleur();
		dessine_noeud(nbp,x,y,couleur,type);
	}
}

void dessine_noeud(nbPos nbp, double x, double y,int couleur, char type) {
	
	nbPos r = sqrt(nbp);
	dessine_cercle_tools(x,y,r,couleur);
	
	switch (type) {
		case 'T':
			dessine_segment_tools(x-r,y,x+r,y,couleur);
			dessine_segment_tools(x,y-r,x,y+r,couleur);
			dessine_segment_tools(x+r*cos(PI/4),y+r*cos(PI/4),x-r*cos(PI/4),
								  y-r*cos(PI/4),couleur);
			dessine_segment_tools(x-r*cos(PI/4),y+r*cos(PI/4),x+r*cos(PI/4),
								 y-r*cos(PI/4),couleur);
			break;
			
		case 'P':
			dessine_rectangle({x-(r*0.75),y+(r/8)},r*2*0.75,(r*2)/8,couleur);
			break;
			
		case 'L':
		
		default:
			break;
	}
}

//Critères

double calcul_ENJ(){
	
	if (noeuds.size() == 0) {return 0;}
	
	double nbp_jour(0), nbp_nuit(0);
	for (nbPos i(0); i < logement.size(); ++i) {
		nbp_jour += logement[i].get_nbp();
	}

	for (nbPos j(0); j < transport.size(); ++j) {
		nbp_nuit += transport[j].get_nbp();
	}
	
	for (nbPos h(0); h < production.size(); ++h) {
		nbp_nuit += production[h].get_nbp();
	}
	
	return ((nbp_jour - nbp_nuit) / (nbp_jour + nbp_nuit));
}

double calcul_CI(){
	
	if (noeuds.size() == 0) {return 0;}
	double CI(0), dist_connexion(0), capacite(0), speed(default_speed);
	char type_A('A'), type_B('B');
	Coord position_A = {0, 0}, position_B = {0, 0};
	
	for (nbPos i(0); i < liens.size(); ++i) {
		for (nbPos j(0); j < noeuds.size(); ++j) {
			if (liens[i].uid1 == noeuds[j].get_uid()) {
				if (noeuds[j].get_type() == 'T') {
					type_A = 'T';
				}
				tests_CI (j, position_A, capacite);
			}
			if (liens[i].uid2 == noeuds[j].get_uid()) {
				if (noeuds[j].get_type() == 'T') {
					type_B = 'T';
				}
				tests_CI (j, position_B, capacite);
			}
		}
		if ((type_A == 'T') and (type_B == 'T')) {
			speed = fast_speed;
		}
		dist_connexion = norme(position_A, position_B);
		CI += (dist_connexion*capacite*speed);
		capacite = 0;
		speed = default_speed;
		type_A = 'A';
		type_B = 'B';
	}
	return CI;
}

void tests_CI (nbPos const& rank, Coord& position, double& capacite){	
	if (capacite != 0) {
		if (noeuds[rank].get_nbp() < capacite) {
			capacite = noeuds[rank].get_nbp();
		}
	} else capacite = noeuds[rank].get_nbp();
	
	position.x = noeuds[rank].get_x();
	position.y = noeuds[rank].get_y();
}
	
double calcul_MTA() {
	if (logement.size() == 0) {return 0;}
	double MTA_prod(0);
	double MTA_tran(0);
	nbPos prod(0);
	nbPos tran(0);
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (noeuds[i].get_type() == 'L') {
			prod = Dijkstra(i, 'P');
			if (prod != no_link) 
				MTA_prod += noeuds[prod].get_access();
			else MTA_prod += infinite_time;
			
			tran = Dijkstra(i, 'T');
			if (tran != no_link) 
				MTA_tran += noeuds[tran].get_access();
			else MTA_tran += infinite_time;
		}
	}
	double MTA = (MTA_prod + MTA_tran)/(logement.size());
	return MTA;
}

bool compare_access (nbPos const& A, nbPos const& B) {
	return (noeuds[A].get_access()) < (noeuds[B].get_access());
}

nbPos Dijkstra(nbPos const& d, char const& type) {
	vector<nbPos> TA;
	for (nbPos j(0); j < noeuds.size(); ++j) {
		noeuds[j].set_in(true); noeuds[j].set_access(infinite_time);
		noeuds[j].set_parent(no_link); 
		TA.push_back(j);
	}
	noeuds[d].set_access(0);	
	sort(TA.begin(), TA.end(), compare_access); 									
	if (type == 'P') {
		if (production.size()==0) {return no_link;}
	}
	if (type == 'T') {
		if (transport.size()==0) {return no_link;}
		for (nbPos h(0); h < noeuds.size(); ++h) {										
			if (noeuds[h].get_type()=='P') {noeuds[h].set_in(false);}
		}
	}
	while (TA_empty() == false) {														
		nbPos n = find_min_access(TA); nbPos rank(0); double new_access_time(0);
		if (n == no_link) {return no_link;}
		if (noeuds[n].get_type() == type) {return n;}
		noeuds[n].set_in(false);
		for (nbPos k(0); k < noeuds[n].get_links().size(); ++k) {						
			for(nbPos i(0); i < noeuds.size(); ++i){ 									
				nbPos uid = noeuds[i].get_uid();
				if(uid == noeuds[n].get_links()[k]) {rank = i; break;}
			}
			if (rank != no_link and noeuds[rank].get_in() == true) {
				new_access_time = noeuds[n].get_access() + compute_access(n, rank); 
				if (noeuds[rank].get_access() > new_access_time) {
					noeuds[rank].set_access(new_access_time);
					noeuds[rank].set_parent(n);
					sort(TA.begin(), TA.end(), compare_access);
				}
			}
		}
	};
	TA.clear(); 
	return no_link;
}

nbPos find_min_access(std::vector<nbPos>& TA) 
{
	for (nbPos j(0); j < TA.size(); j++) {
		if (noeuds[TA[j]].get_in()) {
			return TA[j];
		}
	}
	return no_link;
}

double compute_access(nbPos const& n, nbPos const& rank_node) {
	double v(default_speed);
	double d = norme(noeuds[n].get_position(), noeuds[rank_node].get_position());
	if ((noeuds[n].get_type() == 'T') and (noeuds[rank_node].get_type() == 'T')) {
		v = fast_speed;
	}
	return d/v;
}

bool TA_empty() {
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (noeuds[i].get_in() == true) {
			return false;
		}
	}
	return true;
}

// Édition

Noeud find_node_with_uid(nbPos uid){
	for(nbPos i(0); i < noeuds.size(); ++i){
		nbPos uid1 = noeuds[i].get_uid();
		if(uid == uid1) {
			return noeuds[i];
		}
	}
	Noeud noeud_vide;
	return noeud_vide;
}

nbPos clicked_node(Coord click) {
	for (nbPos i(0); i < noeuds.size(); ++i) {
		Cercle node = {noeuds[i].get_position(), sqrt(noeuds[i].get_nbp())};
		if (point_dans_cercle(click, node)) {
			return noeuds[i].get_uid();
		}
	}
	return no_link;
}

void move_node (double x, double y, nbPos noeud_courant_uid) {
	nbPos indice_courant(0);
	
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (noeuds[i].get_uid() == noeud_courant_uid) {
			indice_courant = i;
			break;
		}
	}
	
	if (tests_move(x, y, indice_courant)) {
		noeuds[indice_courant].set_position(x, y);
		if (noeuds[indice_courant].get_type() == 'L') {
			logement[indice_courant].set_position(x, y);
		}
		else if (noeuds[indice_courant].get_type() == 'T') {
			transport[indice_courant - logement.size()].set_position(x,y);
		}
		else if (noeuds[indice_courant].get_type() == 'P') {
			production[indice_courant - logement.size() - 
					   transport.size()].set_position(x,y);
		}
		update_links(indice_courant);
	}
}

bool tests_move(double x, double y, nbPos indice_courant) {
	Coord centre = {x, y};
	Cercle noeud_courant = {centre, sqrt(noeuds[indice_courant].get_nbp())};
	for (nbPos j(0); j < liens.size(); j++) { //node_link
		if (liens[j].uid1 == noeuds[indice_courant].get_uid() or
			liens[j].uid2 == noeuds[indice_courant].get_uid()) {continue;}
		if (inter_seg_cer(noeud_courant, liens[j].coord1.x, liens[j].coord2.x,
						  liens[j].coord1.y, liens[j].coord2.y, dist_min)      ) { 
			cout << error::node_link_superposition(noeuds[indice_courant].get_uid());
			return false;
		}
	}
	Lien link = {{x,y}, noeuds[indice_courant].get_uid(), {0,0}, no_link}; //link_node
	nbPos indice_voisin(0);	Cercle cercle;
	for (nbPos i(0); i < noeuds[indice_courant].get_links().size(); ++i) {
		for (nbPos k(0); k < noeuds.size(); ++k) {
			if (noeuds[k].get_uid() == noeuds[indice_courant].get_links()[i]) {
				indice_voisin = k; break;
			}
		}
		link.coord2 = noeuds[indice_voisin].get_position();
		link.uid2 = noeuds[indice_voisin].get_uid();
		for (nbPos h(0); h < noeuds.size(); ++h) {
			if (noeuds[h].get_uid() == noeuds[indice_courant].get_uid() or 
				noeuds[h].get_uid() == noeuds[indice_voisin].get_uid() ) {continue;}	
			cercle = {noeuds[h].get_position(), sqrt(noeuds[h].get_nbp())};
			if (inter_seg_cer(cercle, link.coord1.x, link.coord2.x, link.coord1.y,
							  link.coord2.y, dist_min) ) {
			cout << error::node_link_superposition(noeuds[indice_courant].get_uid());
			return false;}
		}
	}
	for (nbPos n(0); n < noeuds.size(); ++n) {//node node
		if (noeuds[n].get_uid() == noeuds[indice_courant].get_uid()) {continue;}
		cercle = {noeuds[n].get_position(), sqrt(noeuds[n].get_nbp())};
		if (inter_cercles(cercle, noeud_courant, dist_min)) {
			cout << error::node_node_superposition(noeuds[n].get_uid(),
			noeuds[indice_courant].get_uid()) << endl; return false;}
	}
	return true;
}

void update_links(nbPos indice_courant) {
	nbPos indice_voisin(0);
	nbPos indice_lien(0);
	for (nbPos i(0); i < noeuds[indice_courant].get_links().size(); ++i) {
		for (nbPos j(0); j < noeuds.size(); ++j) {
			if (noeuds[j].get_uid() == noeuds[indice_courant].get_links()[i]) {
				indice_voisin = j; break;
			}
		}
		for (nbPos k(0); k < liens.size(); ++k) {
			if ((liens[k].uid1 == noeuds[indice_courant].get_uid() and
				liens[k].uid2 == noeuds[indice_voisin].get_uid()) or
				(liens[k].uid2 == noeuds[indice_courant].get_uid() and
				liens[k].uid1 == noeuds[indice_voisin].get_uid())) {
				indice_lien = k; break;
			}
		}
		liens[indice_lien] = {noeuds[indice_courant].get_position(), 
							  noeuds[indice_courant].get_uid(),
							  noeuds[indice_voisin].get_position(),
							  noeuds[indice_voisin].get_uid(), 
							  liens[indice_lien].couleur};
	}
}

void delete_link(nbPos& indice1, nbPos uid1, nbPos& indice2, nbPos uid2) {
	for (nbPos k(0); k < liens.size(); ++k) {
		if ((liens[k].uid1 == uid1 or liens[k].uid1 == uid2) and 
		    (liens[k].uid2 == uid1 or liens[k].uid2 == uid2)) {
			liens.erase(liens.begin() + k);
		}
	}
	for (nbPos j(0); j< noeuds.size(); ++j) {
		if (uid1 == noeuds[j].get_uid()) {
			indice1 = j;
			noeuds[j].remove_link(uid2);
		}
	}
	
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (uid2 == noeuds[i].get_uid()) {
			indice2 = i;
			noeuds[i].remove_link(uid1);
		}
	}
}

void delete_node(nbPos uid) {
	nbPos indice(0);
	for (nbPos t(0); t < noeuds.size(); ++t) {
		if (noeuds[t].get_uid() == uid){
			indice = t;
			break;
		}
	}
	if (noeuds[indice].get_couleur() == RED) {
		clear_links(uid, indice);
		noeuds.erase(noeuds.begin() + indice);
		char type(noeuds[indice].get_type());
		if (type == 'L') {
			for (nbPos j(0); j < logement.size() ; ++j){
				if (logement[j].get_uid() == uid){
					logement.erase(logement.begin() + j);
				}
			}
		}
		if (type == 'T') {
			for (nbPos k(0); k < transport.size() ; ++k){
				if (transport[k].get_uid() == uid){
					transport.erase(transport.begin() + k);
				}
			}
		}
		if (type == 'P') {
			for (nbPos h(0); h < production.size() ; ++h){
				if (production[h].get_uid() == uid){
					production.erase(production.begin() + h);
				}
			}
		}
	}
}

void clear_links(nbPos uid, nbPos rank) {
	nbPos uid1(0);
	nbPos uid2(0);
	nbPos rank_voisin(0);

	for (nbPos i(0); i < liens.size(); ++i) {
		uid1 = liens[i].uid1;
		uid2 = liens[i].uid2;
		if (uid1 == uid) {
			noeuds[rank].remove_link(uid2);
			for (nbPos k(0); k < noeuds.size(); ++k) {
				if (uid2 == noeuds[k].get_uid())
					rank_voisin = k;
			}
			noeuds[rank_voisin].remove_link(uid1);
			liens.erase(liens.begin() + i);
			--i;
		}
		if (uid2 == uid) {
			noeuds[rank].remove_link(uid1);
			for (nbPos h(0); h < noeuds.size(); ++h) {
				if (uid1 == noeuds[h].get_uid())
					rank_voisin = h;
			}
			noeuds[rank_voisin].remove_link(uid2);
			liens.erase(liens.begin() + i);
			--i;
		}
	}
}

void add_or_remove_link (nbPos uid_courant, nbPos new_connection) {
	nbPos indice_courant(0);
	nbPos indice_new(0);
	if (link_does_not_exist(uid_courant, new_connection))
		create_link(new_connection, uid_courant, true);
	else delete_link(indice_courant, uid_courant, indice_new, new_connection);
}

bool link_does_not_exist(nbPos uid1, nbPos uid2) {
	nbPos rank(0);
	for (nbPos j(0); j < noeuds.size(); ++j) {
		if (uid1 == noeuds[j].get_uid()) {
			rank = j; break;
		}	
	}
	
	for (nbPos i(0); i < noeuds[rank].get_links().size(); ++i) {
		if (noeuds[rank].get_links()[i] == uid2) {
			return false;
		}
	}
	return true;
}

void shortest_path(nbPos noeud_courant_uid) {
	nbPos rank (0);
	for (nbPos j(0); j < noeuds.size(); ++j) {
		if (noeud_courant_uid == noeuds[j].get_uid()) {
			rank = j; 
			break;
		}	
	}
	bool pass_P(false);
	bool pass_T(false);
	if (noeuds[rank].get_type() == 'L') {
		if (noeuds[rank].get_links().size() == 1) {
			for (nbPos i(0); i < noeuds.size(); ++i) {
				if (noeuds[i].get_uid() == noeuds[rank].get_links()[0]) {
					if (noeuds[i].get_type() == 'P') {
						pass_T = true;
					}
					if (noeuds[i].get_type() == 'T') {
						pass_P = true;
					}
				}
			}
		}
		if (noeuds[rank].get_links().size() != 0) {
			if (not pass_P) {
				nbPos prod_parent (Dijkstra(rank, 'P')); 	
				if (prod_parent != no_link) {
					color_path(rank, prod_parent);
				}
			}
			if (not pass_T) {
				nbPos tran_parent (Dijkstra(rank, 'T'));
				if (tran_parent != no_link) {
					color_path(rank, tran_parent);
				}
			}
		}
	}
}

void color_path (nbPos indice_courant, nbPos indice_parent) {
	nbPos parent_intermediaire = noeuds[indice_parent].get_parent();
	noeuds[indice_parent].set_couleur(GREEN);
	set_couleur_lien(noeuds[indice_parent].get_uid(),
						 noeuds[noeuds[indice_parent].get_parent()].get_uid(),
						 GREEN); 
	while (noeuds[parent_intermediaire].get_uid()!=noeuds[indice_courant].get_uid()){
		noeuds[parent_intermediaire].set_couleur(GREEN);
		set_couleur_lien(noeuds[parent_intermediaire].get_uid(),
						 noeuds[noeuds[parent_intermediaire].get_parent()].get_uid(),
						 GREEN); 
		parent_intermediaire = noeuds[parent_intermediaire].get_parent();
	}
}

void set_couleur_lien(nbPos uid1, nbPos uid2, int couleur) {
	for (nbPos i(0); i < liens.size(); ++i) {
		if (liens[i].uid1 == uid1) {
			if (liens[i].uid2 == uid2) {
				liens[i].couleur = couleur;
				break;
			}
		}
		if (liens[i].uid2 == uid1) {
			if (liens[i].uid1 == uid2) {
				liens[i].couleur = couleur;
				break;
			}
		}
	}
}

void reset_color_nodes(nbPos uid) {
	for (nbPos i(0); i < noeuds.size(); ++i) {
		if (noeuds[i].get_uid() != uid) {
			noeuds[i].set_couleur(BLACK);
		}
	}
}

void reset_color_links() {
	for (nbPos i(0); i < liens.size(); ++i) {
		set_couleur_lien(liens[i].uid1, liens[i].uid2, BLACK);
	}
}

void size_node(nbPos uid, double x_init, double y_init, double x_final, 
			   double y_final){
	Coord point_init = {x_init,y_init};
	Coord point_final = {x_final,y_final};
	nbPos rank_node(0), uid1(0);
	for (nbPos i(0); i < noeuds.size(); ++i){
		uid1 = noeuds[i].get_uid();
		if (uid1 == uid){
			rank_node = i;
		}
	}
	double rayon_debut = norme({noeuds[rank_node].get_position()}, point_init);
	double rayon_fin = norme({noeuds[rank_node].get_position()}, point_final);
	
	double r = sqrt(noeuds[rank_node].get_nbp()) + (rayon_fin - rayon_debut);
	nbPos new_nbp = pow(r,2);
	
	if (r > 0){					  
		if (success_node2(uid, noeuds[rank_node].get_x(), noeuds[rank_node].get_y(),
						  new_nbp, dist_min, rank_node)) {
			if (new_nbp <= min_capacity) {
				noeuds[rank_node].set_nbp(min_capacity);
			}
			else if (new_nbp >= max_capacity) {
				noeuds[rank_node].set_nbp(max_capacity);
			}	
			else noeuds[rank_node].set_nbp(new_nbp);
		}
	}
	else {
		if (success_node2(uid, noeuds[rank_node].get_x(), noeuds[rank_node].get_y(),
						  min_capacity, dist_min, rank_node)) {
			noeuds[rank_node].set_nbp(min_capacity);
		}
	}
}

bool success_node2(nbPos const& uid, double const& x, double const& y, 
				  nbPos nbp, double dist, nbPos const& rank_node) {

	for (nbPos i(0); i < noeuds.size(); ++i) { 
		if (!(i == rank_node)) {
			Cercle NewNode = {x, y, sqrt(nbp)};
			Cercle ExistingNode = {noeuds[i].get_x(), noeuds[i].get_y(), 
								   sqrt(noeuds[i].get_nbp())};
			
			if (inter_cercles(ExistingNode, NewNode, dist)) {
				cout<< error::node_node_superposition(uid,noeuds[i].get_uid());
				return false;
			}
		}
	}
	Cercle node; // node-link
	node.centre ={x,y};
	node.r = sqrt(nbp);
	for (nbPos j(0); j< liens.size(); ++j) {
		if (noeuds[rank_node].get_uid() == liens[j].uid1 or
			noeuds[rank_node].get_uid() == liens[j].uid2) {continue;}
		if (inter_seg_cer(node,liens[j].coord1.x,liens[j].coord2.x,liens[j].coord1.y,
						  liens[j].coord2.y,dist)) {
			cout << error::node_link_superposition(uid);
			return false;
		}
	}
	return true;
}
