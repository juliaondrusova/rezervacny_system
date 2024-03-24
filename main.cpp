#include "MyFirstQtApp.h"
#include <QtWidgets/QApplication>
#include "c_classy.h"
#include <vector>
#include "c_classy.h"
#include <fstream>
#include <qmessagebox.h>
#include <iostream>
#include <sstream>   
#include "pracaSoSubormi.h"


std::vector<GUEST> fyzicky_hostia;
std::vector<BUSSINES_GUEST> firemny_hostia;
std::vector<HOTEL_EMPLOYER> recepcne;
std::vector<HOTEL_EMPLOYER> administratori;
std::vector<RESERVATION> rezervacie;


int main(int argc, char* argv[])
{
	readJsonData("uzivatelia.txt");
	readJsonToRoom("izbyNove.txt");
	loadReservationsFromFile("rezervacie.txt");
	priradRezervacie();

	QApplication a(argc, argv);
	MyFirstQtApp w;
	w.show();
	return a.exec();
}