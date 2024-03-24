#pragma once
#include "c_classy.h"
#include <fstream>
#include <qmessagebox.h>
#include <iostream>
#include <sstream> 

extern std::vector<GUEST> fyzicky_hostia;
extern std::vector<BUSSINES_GUEST> firemny_hostia;
extern std::vector<HOTEL_EMPLOYER> recepcne;
extern std::vector<HOTEL_EMPLOYER> administratori;
extern std::vector <ROOM> rooms;
extern std::vector<RESERVATION> rezervacie;

void readJsonData(std::string filename);
void readJsonToRoom(std::string filePath);
void ulozPouzivatelov(std::string filename);
void ulozIzby(std::string filename);
void loadReservationsFromFile(std::string filename);
void saveReservationsToFile(std::string filename);
void priradRezervacie();