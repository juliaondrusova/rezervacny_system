#include "SecondWindow.h"
#include <qpushbutton.h>
#include "MyFirstQtApp.h"
#include <qmessagebox.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "c_classy.h"
#include <QFileDialog>
#include "pracaSoSubormi.h"
#include <vector>


extern std::vector<GUEST> fyzicky_hostia;
extern std::vector<BUSSINES_GUEST> firemny_hostia;
extern std::vector<HOTEL_EMPLOYER> recepcne;
extern std::vector<HOTEL_EMPLOYER> administratori;
extern std::vector<RESERVATION> rezervacie;


extern std::string actualUserName;
std::vector <ROOM> rooms;
std::string typIzby = "";
extern USER* actualUser;


SecondWindow::SecondWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QWidget::setWindowTitle("Rezervačný systém pre hotel");
	settings();

	connect(ui.odhlasenie, &QPushButton::clicked, this, &SecondWindow::odhlasenie);
	connect(ui.typIzby, &QComboBox::currentIndexChanged, this, &SecondWindow::vyberTypuIzby);
	connect(ui.datumPrichodu, &QCalendarWidget::selectionChanged, this, &SecondWindow::zmenaPrichodu);
	connect(ui.datumOdchodu, &QCalendarWidget::selectionChanged, this, &SecondWindow::zmenaOdchodu);
	connect(ui.vytvoritRezervaciu, &QPushButton::clicked, this, &SecondWindow::rezervacia);
	connect(ui.vyhladatIzbu, &QPushButton::clicked, this, &SecondWindow::zobrazitIzby);
	connect(ui.zrusenieRezervacie, &QPushButton::clicked, this, &SecondWindow::zrusitRezervaciu);
	connect(ui.vytvoritFakturu, &QPushButton::clicked, this, &SecondWindow::vytvoritFakturu);
	connect(ui.zobrazitNaAktulizovanie, &QPushButton::clicked, this, &SecondWindow::zobrazNaAktualizovanie);
	connect(ui.zobrazitFormular, &QPushButton::clicked, this, &SecondWindow::zobrazNaVytvorenie);
	connect(ui.aktualizovatUsera, &QPushButton::clicked, this, &SecondWindow::aktualizovatUsera);
	connect(ui.vytvoritUsera, &QPushButton::clicked, this, &SecondWindow::vytvoritUsera);
	connect(ui.odstranitUsera, &QPushButton::clicked, this, &SecondWindow::odstranitUsera);
	connect(ui.odstranitIzbu, &QPushButton::clicked, this, &SecondWindow::odstranitIzbu);
	connect(ui.zobrazitIzbu, &QPushButton::clicked, this, &SecondWindow::zobrazitIzbu);
	connect(ui.pridatDatum, &QPushButton::clicked, this, &SecondWindow::pridatDatum);
	connect(ui.aktualizovatIzbu, &QPushButton::clicked, this, &SecondWindow::aktualizovatIzbu);
	connect(ui.vytvoritIzbu, &QPushButton::clicked, this, &SecondWindow::vytvoritIzbu);
	connect(ui.pridatDatumNova, &QPushButton::clicked, this, &SecondWindow::pridatDatumNova);
	connect(ui.zmenaStatusu, &QPushButton::clicked, this, &SecondWindow::zmenitStatus);
}

void SecondWindow::settings()
{

	for (int i = ui.mojeRezervacie->rowCount(); i > 0; i--)
	{
		ui.mojeRezervacie->removeRow(i - 1);
	}

	ui.tabWidget->setTabEnabled(1, true);
	ui.tabWidget->setTabEnabled(2, true);
	ui.tabWidget->setTabEnabled(3, true);

	if (actualUser->typeOfUser() == "BUSSINES_GUEST" || actualUser->typeOfUser() == "GUEST")
	{
		vyplnTabulkuRezervacii();
		ui.tabWidget->setTabEnabled(1, false);
		ui.tabWidget->setTabEnabled(2, false);
	}
	if (actualUser->typeOfUser() == "HOTEL_EMPLOYER" && reinterpret_cast<HOTEL_EMPLOYER*>(actualUser)->position() == "recepcny")
	{
		nacitatTabulkuRecepcie();
		ui.tabWidget->setTabEnabled(0, false);
		ui.tabWidget->setTabEnabled(2, false);
	}
	if (actualUser->typeOfUser() == "HOTEL_EMPLOYER" && reinterpret_cast<HOTEL_EMPLOYER*>(actualUser)->position() == "admin")
	{
		ui.tabWidget->setTabEnabled(0, false);
		ui.tabWidget->setTabEnabled(1, false);
	}

	for (int i = 0; i < rooms.size(); i++)
	{
		ui.cislaIzieb->addItem(QString::number(rooms[i].roomNumber()));
	}
	for (int i = 0; i < rooms.size(); i++)
	{
		ui.cisloIzbyComboBox->addItem(QString::number(rooms[i].roomNumber()));
	}

	zobrazitUSerov();

	ui.datumOdchodu->setDisabled(true);
	ui.datumOdchodu->setSelectedDate(QDate());
	ui.datumPrichodu->setSelectedDate(QDate());
	ui.typIzby->setCurrentIndex(-1);

	notVisibleActualise();
	notVisibleNew();

	ui.cislaIzieb->setCurrentIndex(-1);
	ui.cisloIzbyComboBox->setCurrentIndex(-1);
	roomSetting();
}

void SecondWindow::vyplnTabulkuRezervacii()
{

		for (int i = ui.mojeRezervacie->rowCount(); i > 0; i--)
		{
			ui.mojeRezervacie->removeRow(i - 1);
		}
	

	if (actualUser->typeOfUser() == "BUSSINES_GUEST")
	{
		int index = najdiUsera(firemny_hostia);

		for (int i = 0;i < firemny_hostia[index].reservations().size();i++)
		{
			/*
			int row = ui.mojeRezervacie->rowCount();
			ui.mojeRezervacie->insertRow(row);
			ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].roomNumber())));
			ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].reservationNumber())));

			QString dateRange = (firemny_hostia[index].reservations()[i].date().front().toString() + "-" +
				firemny_hostia[index].reservations()[i].date().back().toString());

			ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(dateRange));
			ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].cena())));
			int status = firemny_hostia[index].reservations()[i].status();
			switch (status)
			{
			case 1:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));
				break;
			case 2:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Schválena"));
				break;
			case 3:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Zamietnutá"));
				break;
			default:
				break;
			}
			*/


			bool alreadyExists = false;
			int existingRow = -1; // Index riadku, kde sa nachádza rovnaké rezervačné číslo

			// Prechádzanie tabuľkou a hľadanie existujúceho rezervačného čísla
			for (int row = 0; row < ui.mojeRezervacie->rowCount(); row++)
			{
				if (ui.mojeRezervacie->item(row, 1)->text().toInt() == firemny_hostia[index].reservations()[i].reservationNumber())
				{
					alreadyExists = true;
					existingRow = row;
					break;
				}
			}

			// Ak rezervačné číslo ešte neexistuje v tabuľke, pridáme nový riadok
			if (!alreadyExists)
			{
				int row = ui.mojeRezervacie->rowCount();
				ui.mojeRezervacie->insertRow(row);
				ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].roomNumber())));
				ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].reservationNumber())));

				QString dateRange = (firemny_hostia[index].reservations()[i].date().front().toString() + "-" +
					firemny_hostia[index].reservations()[i].date().back().toString());

				ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(dateRange));
				ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(firemny_hostia[index].reservations()[i].cena())));
				int status = firemny_hostia[index].reservations()[i].status();
				switch (status)
				{
				case 1:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));
					break;
				case 2:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Schválená"));
					break;
				case 3:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Zamietnutá"));
					break;
				default:
					break;
				}
			}


			// Overenie, či rezervačné číslo už existuje v ui.cisloRusenejRezervacie
			alreadyExists = false;
			for (int j = 0; j < ui.cisloRusenejRezervacie->count(); j++)
			{
				if (ui.cisloRusenejRezervacie->itemText(j).toInt() == firemny_hostia[index].reservations()[i].reservationNumber())
				{
					alreadyExists = true;
					break;
				}
			}
			// Ak rezervačné číslo neexistuje, pridáme ho
			if (!alreadyExists)
			{
				ui.cisloRusenejRezervacie->addItem(QString::number(firemny_hostia[index].reservations()[i].reservationNumber()));
			}
		}
	}

	else if (actualUser->typeOfUser() == "GUEST")
	{
		int index = najdiUsera(fyzicky_hostia);

		for (int i = 0;i < fyzicky_hostia[index].reservations().size();i++)
		{
			/*
			int row = ui.mojeRezervacie->rowCount();
			ui.mojeRezervacie->insertRow(row);
			ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(fyzicky_hostia[index].reservations()[i].roomNumber())));
			ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(fyzicky_hostia[index].reservations()[i].reservationNumber())));

			QString dateRange = (fyzicky_hostia[index].reservations()[i].date().front().toString() + "-" +
				fyzicky_hostia[index].reservations()[i].date().back().toString());

			ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(dateRange));
			ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(fyzicky_hostia[index].reservations()[i].cena())));
			int status = fyzicky_hostia[index].reservations()[i].status();
			switch (status)
			{
			case 1:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));
				break;
			case 2:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Schválena"));
				break;
			case 3:
				ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Zamietnutá"));
				break;
			default:
				break;
			}
			*/

			int reservationNumber = fyzicky_hostia[index].reservations()[i].reservationNumber();

			// Check if reservation number already exists in ui.mojeRezervacie
			bool alreadyExists = false;
			for (int row = 0; row < ui.mojeRezervacie->rowCount(); row++)
			{
				if (ui.mojeRezervacie->item(row, 1)->text().toInt() == reservationNumber)
				{
					alreadyExists = true;
					break;
				}
			}

			if (!alreadyExists)
			{
				int row = ui.mojeRezervacie->rowCount();
				ui.mojeRezervacie->insertRow(row);
				ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(fyzicky_hostia[index].reservations()[i].roomNumber())));
				ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(reservationNumber)));

				QString dateRange = (fyzicky_hostia[index].reservations()[i].date().front().toString() + "-" +
					fyzicky_hostia[index].reservations()[i].date().back().toString());

				ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(dateRange));
				ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(fyzicky_hostia[index].reservations()[i].cena())));
				int status = fyzicky_hostia[index].reservations()[i].status();
				switch (status)
				{
				case 1:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));
					break;
				case 2:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Schválena"));
					break;
				case 3:
					ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Zamietnutá"));
					break;
				default:
					break;
				}
			}
			alreadyExists = false;
			for (int j = 0; j < ui.cisloRusenejRezervacie->count(); j++)
			{
				if (ui.cisloRusenejRezervacie->itemText(j).toInt() == fyzicky_hostia[index].reservations()[i].reservationNumber())
				{
					alreadyExists = true;
					break;
				}
			}
			// Ak rezervačné číslo neexistuje, pridáme ho
			if (!alreadyExists)
			{
				ui.cisloRusenejRezervacie->addItem(QString::number(fyzicky_hostia[index].reservations()[i].reservationNumber()));
			}
		}
	}
}

void SecondWindow::nacitatTabulkuRecepcie()
{
	if (ui.rezervacieTabulka->rowCount() != 0)
	{
		for (int i = ui.rezervacieTabulka->rowCount(); i > 0; i--)
		{
			ui.rezervacieTabulka->removeRow(i - 1);
		}
	}

	for (int i = 0; i < rezervacie.size(); i++)
	{
		int row = ui.rezervacieTabulka->rowCount();
		ui.rezervacieTabulka->insertRow(row);
		ui.rezervacieTabulka->setItem(row, 0, new QTableWidgetItem(QString::number(rezervacie[i].reservationNumber())));
		ui.rezervacieTabulka->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rezervacie[i].userName())));

		QString dateRange = (rezervacie[i].date().front().toString() + "-" +
			rezervacie[i].date().back().toString());

		ui.rezervacieTabulka->setItem(row, 2, new QTableWidgetItem(dateRange));
		ui.rezervacieTabulka->setItem(row, 3, new QTableWidgetItem(QString::number(rezervacie[i].cena())));
		int status = rezervacie[i].status();
		switch (status)
		{
		case 1:
			ui.rezervacieTabulka->setItem(row, 4, new QTableWidgetItem("Nová"));
			break;
		case 2:
			ui.rezervacieTabulka->setItem(row, 4, new QTableWidgetItem("Schválena"));
			break;
		case 3:
			ui.rezervacieTabulka->setItem(row, 4, new QTableWidgetItem("Zamietnutá"));
			break;
		default:
			break;
		}
		// Check if reservation number already exists in ui.cisloRezervRecep
		bool alreadyExists = false;
		for (int j = 0; j < ui.cisloRezervRecep->count(); j++)
		{
			if (ui.cisloRezervRecep->itemText(j).toInt() == rezervacie[i].reservationNumber())
			{
				alreadyExists = true;
				break;
			}
		}
		// Add reservation number to ui.cisloRezervRecep if it doesn't already exist
		if (!alreadyExists)
		{
			ui.cisloRezervRecep->addItem(QString::number(rezervacie[i].reservationNumber()));
		}
	}
}

void SecondWindow::notVisibleActualise()
{
	ui.menoAktualiz->setEnabled(false);
	ui.priezviskoAktualiz->setEnabled(false);
	ui.hesloAktualiz->setEnabled(false);
	ui.usernameAktualiz->setEnabled(false);
	ui.adresaAktualiz->setEnabled(false);
	ui.firmaAktualiz->setEnabled(false);
	ui.DPHAktualiz->setEnabled(false);
	ui.poziciaAktualiz->setEnabled(false);


	ui.aktualneMeno->setText("");
	ui.aktualnePriezvisko->setText("");
	ui.aktualneHeslo->setText("");
	ui.aktualneUsername->setText("");
	ui.aktualnaAdresa->setText("");
	ui.aktualnaFirma->setText("");
	ui.aktualnaPozicia->setCurrentIndex(-1);
	ui.aktualneDPH->setCurrentIndex(-1);
}

void SecondWindow::roomSetting()
{
	ui.aktualnyDruhIzby->setCurrentIndex(-1);
	ui.aktualnaCena->setValue(0);
	ui.novyDruhIzby->setCurrentIndex(-1);
	ui.novaCena->setValue(0);

	if (ui.aktualneDatumy->rowCount() != 0)
	{
		for (int i = ui.aktualneDatumy->rowCount(); i > 0; i--)
		{
			ui.aktualneDatumy->removeRow(i - 1);
		}
	}
	if (ui.noveDatumy->rowCount() != 0)
	{
		for (int i = ui.noveDatumy->rowCount(); i > 0; i--)
		{
			ui.noveDatumy->removeRow(i - 1);
		}
	}
}
void SecondWindow::notVisibleNew()
{
	ui.usernamePassword->setEnabled(false);
	ui.menoPreizviskoNovy->setEnabled(false);
	ui.novaFirma->setEnabled(false);
	ui.novyZamestnanec->setEnabled(false);
}

SecondWindow::~SecondWindow()
{}


void SecondWindow::zobrazitUSerov()
{
	for (int i = 0; i < administratori.size(); i++)
	{

		ui.vsetciAktualizovanie->addItem(QString::fromStdString(administratori[i].username()));
		ui.vsetciVymazanie->addItem(QString::fromStdString(administratori[i].username()));
	}
	for (int i = 0; i < recepcne.size(); i++)
	{

		ui.vsetciAktualizovanie->addItem(QString::fromStdString(recepcne[i].username()));
		ui.vsetciVymazanie->addItem(QString::fromStdString(recepcne[i].username()));
	}
	for (int i = 0; i < firemny_hostia.size(); i++)
	{

		ui.vsetciAktualizovanie->addItem(QString::fromStdString(firemny_hostia[i].username()));
		ui.vsetciVymazanie->addItem(QString::fromStdString(firemny_hostia[i].username()));
	}
	for (int i = 0; i < fyzicky_hostia.size(); i++)
	{

		ui.vsetciAktualizovanie->addItem(QString::fromStdString(fyzicky_hostia[i].username()));
		ui.vsetciVymazanie->addItem(QString::fromStdString(fyzicky_hostia[i].username()));
	}
}

void SecondWindow::odhlasenie()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že sa chcete odhlásiť?",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}


void SecondWindow::vyberTypuIzby()
{
	int index = ui.typIzby->currentIndex(); //zistenia vybrateho typu izby pomocou indexu

	switch (index)
	{
	case 0: //jednolozkova
		typIzby = "Jednolozkova";
		break;

	case 1: //dvojlozkova
		typIzby = "Dvojlozkova";
		break;

	case 2: //trojlozkova
		typIzby = "Trojlozkova";
		break;

	case 3: //stvorlozkova
		typIzby = "Stvorlozkova";
		break;

	case 4: //suita
		typIzby = "Suite";
		break;

	case 5: //apartman
		typIzby = "Apartman";
		break;

	case 6: //rodinna
		typIzby = "Rodinna";
		break;

	default:
		return;
	}
}

void SecondWindow::pridajPouzivatelovi(std::string typIzby)
{
	// ak sa v tabulke uz nachadzali nejake produkty z predchadzajuceho vyhladavanie, tak ich vymazem
	if (ui.infoIzby->rowCount() != 0)
	{
		for (int i = ui.infoIzby->rowCount(); i > 0; i--)
		{
			ui.infoIzby->removeRow(i - 1);
		}
	}

	//prehladavam pomedzi vsetky produkty
	for (int i = 0; i < rooms.size(); i++)
	{
		//ak sa zadany nazov zhoduje s nejakym zo struktury, tak ho vypisem do tabulky
		if (rooms[i].typeOfRoom() == typIzby)
		{
			int row = ui.infoIzby->rowCount();
			ui.infoIzby->insertRow(row);

			ui.infoIzby->setItem(row, 0, new QTableWidgetItem(QString::number(rooms[i].roomNumber())));
			ui.infoIzby->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rooms[i].typeOfRoom())));
			if (dostupnost(rooms[i]))
			{
				ui.infoIzby->setItem(row, 2, new QTableWidgetItem(QString::fromStdString("Dostupná")));
			}
			else
			{
				ui.infoIzby->setItem(row, 2, new QTableWidgetItem(QString::fromStdString("Nedostupná")));
			}

			if (actualUser->typeOfUser() == "BUSSINES_GUEST")
			{
				int index = najdiUsera(firemny_hostia);

				if (firemny_hostia[index].isVatPayer() == true)
				{
					ui.infoIzby->setItem(row, 3, new QTableWidgetItem(QString::number(rooms[i].pricePerNight() * 0.8, 'f', 2)));
				}
				else
				{
					ui.infoIzby->setItem(row, 3, new QTableWidgetItem(QString::number(rooms[i].pricePerNight(), 'f', 2)));
				}
			}
			else
			{
				ui.infoIzby->setItem(row, 3, new QTableWidgetItem(QString::number(rooms[i].pricePerNight(), 'f', 2)));
			}
		}
	}
}


void SecondWindow::zmenaPrichodu()
{
	selectedDays.clear();
	ui.datumOdchodu->setSelectedDate(QDate());
	ui.datumOdchodu->setMinimumDate(ui.datumPrichodu->selectedDate().addDays(1));
	ui.datumOdchodu->setDisabled(false);
}


void SecondWindow::zmenaOdchodu()
{
	QDate denPrichodu = ui.datumPrichodu->selectedDate();
	QDate denOdchodu = ui.datumOdchodu->selectedDate();

	while (denPrichodu != denOdchodu)
	{
		selectedDays.push_back(denPrichodu);
		denPrichodu = denPrichodu.addDays(1);
	}
}


void SecondWindow::rezervacia()
{
	bool izbaExistuje = false;

	for (int i = 0; i < ui.infoIzby->rowCount(); i++)
	{
		if (ui.cisloIzby->value() == ui.infoIzby->item(i, 0)->text().toInt())
		{
			izbaExistuje = true;
			break;
		}
	}

	if (!izbaExistuje)
	{
		QMessageBox::warning(this, "Chyba", "Izba nebola zvolená správne");
		if (ui.infoIzby->rowCount() != 0)
		{
			for (int i = ui.infoIzby->rowCount(); i > 0; i--)
			{
				ui.infoIzby->removeRow(i - 1);
			}
		}
		return;
	}
	zmenaOdchodu();
	if (actualUser->typeOfUser() == "GUEST")
	{
		int index = najdiUsera(fyzicky_hostia);
		ROOM* room = vyberCisloIzby();
		if (room == nullptr)
		{
			QMessageBox::warning(this, "Chyba", "Zvolená izba nie je dostupná!");
			if (ui.infoIzby->rowCount() != 0)
			{
				for (int i = ui.infoIzby->rowCount(); i > 0; i--)
				{
					ui.infoIzby->removeRow(i - 1);
				}
			}
			ui.typIzby->setCurrentIndex(-1);
			ui.datumPrichodu->setSelectedDate(QDate());
			ui.datumPrichodu->setSelectedDate(QDate());

			return;
		}
		double cena = room->pricePerNight() * selectedDays.size();
		int status = 1; //nova rezervacia
		int reservationNumber = rezervacie.size() + 1;
		RESERVATION reservation = RESERVATION(selectedDays, ui.cisloIzby->value(), cena, status, reservationNumber, actualUser->username());
		fyzicky_hostia[index].set_reservation(reservation);

		int row = ui.mojeRezervacie->rowCount();
		ui.mojeRezervacie->insertRow(row);
		ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(reservation.roomNumber())));
		ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(reservation.reservationNumber())));

		rezervacie.push_back(reservation);
		saveReservationsToFile("rezervacie.txt");

		std::string dateRange = fyzicky_hostia[index].reservations().back().date().front().toString().toStdString() + "-" +
			fyzicky_hostia[index].reservations().back().date().back().toString().toStdString();

		ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dateRange)));
		ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(cena)));
		ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));

		ui.cisloRusenejRezervacie->addItem(QString::number(reservation.reservationNumber()));


	}

	else if (actualUser->typeOfUser() == "BUSSINES_GUEST")
	{
		int index = najdiUsera(firemny_hostia);
		ROOM* room = vyberCisloIzby();
		if (room == nullptr)
		{
			QMessageBox::warning(this, "Chyba", "Zvolená izba nie je dostupná!");
			if (ui.infoIzby->rowCount() != 0)
			{
				for (int i = ui.infoIzby->rowCount(); i > 0; i--)
				{
					ui.infoIzby->removeRow(i - 1);
				}
			}
			return;
		}

		double cena;
		if (firemny_hostia[index].isVatPayer() == true)
		{
			cena = room->pricePerNight() * 0.8 * selectedDays.size();
		}
		else
		{
			cena = room->pricePerNight() * selectedDays.size();
		}

		int status = 1; //nova rezervacia
		int reservationNumber = rezervacie.size() + 1;
		RESERVATION reservation = RESERVATION(selectedDays, ui.cisloIzby->value(), cena, status, reservationNumber, actualUser->username());
		firemny_hostia[index].set_reservation(reservation);

		int row = ui.mojeRezervacie->rowCount();
		ui.mojeRezervacie->insertRow(row);
		ui.mojeRezervacie->setItem(row, 0, new QTableWidgetItem(QString::number(reservation.roomNumber())));
		ui.mojeRezervacie->setItem(row, 1, new QTableWidgetItem(QString::number(reservation.reservationNumber())));

		rezervacie.push_back(reservation);
		saveReservationsToFile("rezervacie.txt");

		std::string dateRange = firemny_hostia[index].reservations().back().date().front().toString().toStdString() + "-" +
			firemny_hostia[index].reservations().back().date().back().toString().toStdString();

		//QString dateRange = reservation.room().reservedDays().front().toString() + " - " + reservation.room().reservedDays().back().toString();
		ui.mojeRezervacie->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dateRange)));
		ui.mojeRezervacie->setItem(row, 3, new QTableWidgetItem(QString::number(cena)));
		ui.mojeRezervacie->setItem(row, 4, new QTableWidgetItem("Nová"));

		ui.cisloRusenejRezervacie->addItem(QString::number(reservation.reservationNumber()));
	}
}


ROOM* SecondWindow::vyberCisloIzby()
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (ui.cisloIzby->value() == rooms[i].roomNumber())
		{
			bool foundCommonElement = false;

			for (int j = 0; j < selectedDays.size(); j++)
			{
				for (const auto& day : rooms[i].reservedDays())
				{
					if (selectedDays[j] == day)
					{
						foundCommonElement = true;
						return nullptr;
					}
				}
			}
			if (!foundCommonElement)
			{
				return &rooms[i];
			}
		}
	}
}

bool SecondWindow::dostupnost(ROOM room)
{
	bool enable = true;

	for (int i = 0; i < selectedDays.size(); i++)
	{
		for (int j = 0; j < room.reservedDays().size(); j++)
		{
			if (selectedDays[i] == room.reservedDays()[j])
			{
				enable = false;
				return enable;
			}
		}
	}
	return enable;
}


void SecondWindow::zobrazitIzby()
{
	if (ui.typIzby->currentIndex() == -1)
	{
		QMessageBox::warning(this, "Chyba", "Nezvolili ste všetky údaje!");
		return;
	}

	pridajPouzivatelovi(typIzby);
}

void SecondWindow::zrusitRezervaciu()
{
	int cisloRezervacie = (ui.cisloRusenejRezervacie->currentText()).toInt();


	if (actualUser->typeOfUser() == "GUEST")
	{
		int index = najdiUsera(fyzicky_hostia);
		for (int j = 0; j < fyzicky_hostia[index].reservations().size(); j++)
		{
			if (fyzicky_hostia[index].reservations()[j].reservationNumber() == cisloRezervacie)
			{
				fyzicky_hostia[index].zrusitRezervaciu(j);
				zrusitRiadok(cisloRezervacie);
				int indexToRemove = ui.cisloRusenejRezervacie->findText(QString::number(cisloRezervacie));
				ui.cisloRusenejRezervacie->removeItem(indexToRemove);
				int indexRemove = ui.cisloRusenejRezervacie->findText(QString::number(cisloRezervacie));
				int indexRezervacie = 0;
				for (int i = 0;i < rezervacie.size();i++)
				{
					if (rezervacie[i].reservationNumber() == cisloRezervacie)
					{
						if (rezervacie[i].status() == 2)
						{
							for (int k = 0;k < rooms.size(); k++)
							{
								if (rezervacie[i].roomNumber() == rooms[k].roomNumber())
								{

									for (int l = 0; l < rezervacie[i].date().size();i++)
									{
										rooms[k].odstranDatum(rezervacie[i].date()[l]);
									}
								}
							}
						}
						indexRezervacie = i;
					}
				}
				rezervacie.erase(rezervacie.begin() + indexRezervacie);

				saveReservationsToFile("rezervacie.txt");
				ulozIzby("izbyNove.txt");
			}
		}
	}
	else if (actualUser->typeOfUser() == "BUSSINES_GUEST")
	{
		int index = najdiUsera(firemny_hostia);
		for (int j = 0; j < firemny_hostia[index].reservations().size(); j++)
		{
			if (firemny_hostia[index].reservations()[j].roomNumber() == cisloRezervacie)
			{
				firemny_hostia[index].zrusitRezervaciu(j);
				zrusitRiadok(cisloRezervacie);
				int indexToRemove = ui.cisloRusenejRezervacie->findText(QString::number(cisloRezervacie));
				ui.cisloRusenejRezervacie->removeItem(indexToRemove);
				int indexRemove = ui.cisloRusenejRezervacie->findText(QString::number(cisloRezervacie));
				int indexRezervacie = 0;
				for (int i = 0;i < rezervacie.size();i++)
				{
					if (rezervacie[i].reservationNumber() == cisloRezervacie)
					{
						if (rezervacie[i].status() == 2)
						{
							for (int k = 0;k < rooms.size(); k++)
							{
								if (rezervacie[i].roomNumber() == rooms[k].roomNumber())
								{

									for (int l = 0; l < rezervacie[i].date().size();i++)
									{
										rooms[k].odstranDatum(rezervacie[i].date()[l]);
									}
								}
							}
						}
						indexRezervacie = i;
					}
				}
				rezervacie.erase(rezervacie.begin() + indexRezervacie);

				saveReservationsToFile("rezervacie.txt");
				ulozIzby("izbyNove.txt");
			}
		}

	}
}

void SecondWindow::zrusitRiadok(int cisloIzby)
{
	for (int row = 0; row < ui.mojeRezervacie->rowCount(); row++)
	{
		int valueInColumn = ui.mojeRezervacie->item(row, 1)->text().toInt();

		if (valueInColumn == cisloIzby)
		{
			ui.mojeRezervacie->removeRow(row);
			return;
		}
	}
}


void SecondWindow::vytvoritFakturu()
{

	std::string filename = (actualUser->username() + "_faktura.txt");
	std::cout << filename << std::endl;
	std::fstream subor(filename, std::ios::out);

	if (actualUser->typeOfUser() == "GUEST")
	{
		int index = najdiUsera(fyzicky_hostia);
		int pcoetSchvalenych = 0;
		for (int i = 0; i < fyzicky_hostia[index].reservations().size(); i++)
		{
			if (fyzicky_hostia[index].reservations()[i].status() == 2)
			{
				pcoetSchvalenych++;
			}
		}
		if (pcoetSchvalenych == 0)
		{
			QMessageBox::information(this, "Upozornenie", "Nemáte žiadnu schválenu rezerváciu - nemôže byť vytvorená faktúra");
			return;
		}

		subor << "MENO: " << fyzicky_hostia[index].name() << " PRIEZVISKO: " << fyzicky_hostia[index].surname() << std::endl << std::endl;
		double vyslednaCena = 0;

		for (int j = 0;j < fyzicky_hostia[index].reservations().size(); j++)
		{
			if (fyzicky_hostia[index].reservations()[j].status() == 2)
			{
				vyslednaCena += fyzicky_hostia[index].reservations()[j].cena();
				subor << "DATUM: "
					<< fyzicky_hostia[index].reservations()[j].date().front().toString().toStdString() + "-" +
					fyzicky_hostia[index].reservations()[j].date().back().toString().toStdString()
					<< " CENA: "
					<< fyzicky_hostia[index].reservations()[j].cena() << std::endl;
			}
		}

		subor << std::endl << "CELKOVA SUMA: " << vyslednaCena;

	}
	else if (actualUser->typeOfUser() == "BUSSINES_GUEST")
	{
		int index = najdiUsera(firemny_hostia);
		int pocetSchvalenych = 0;
		for (int i = 0; i < firemny_hostia[index].reservations().size(); i++)
		{
			if (firemny_hostia[index].reservations()[i].status() == 2)
			{
				pocetSchvalenych++;

			}
		}

		if (pocetSchvalenych == 0)
		{
			QMessageBox::information(this, "Upozornenie", "Nemáte žiadnu schválenu rezerváciu - nemôže byť vytvorená faktúra");
			std::remove(filename.c_str());
			return;
		}

		std::fstream subor(filename, std::ios::out);

		subor << "MENO: " << firemny_hostia[index].name() << std::endl << "PRIEZVISKO: " << firemny_hostia[index].surname() << std::endl;
		double vyslednaCena = 0;

		for (int j = 0; j < firemny_hostia[index].reservations().size(); j++)
		{
			if (firemny_hostia[index].reservations().size() == 0)
			{
				QMessageBox::information(this, "Upozornenie", "Nemáte žiadnu schválenu rezerváciu - nemôže byť vytvorená faktúra");
				std::remove(filename.c_str());
				return;
			}
			if (firemny_hostia[index].reservations()[j].status() == 2)
			{
				vyslednaCena += firemny_hostia[index].reservations()[j].cena();
				subor << "DATUM: "
					<< firemny_hostia[index].reservations()[j].date().front().toString().toStdString() + "-" +
					firemny_hostia[index].reservations()[j].date().back().toString().toStdString()
					<< " CENA: "
					<< firemny_hostia[index].reservations()[j].cena() << std::endl;
			}
		}

		subor << std::endl << "CELKOVA SUMA: " << vyslednaCena;
	}
	subor.close();
}

template <typename UserType>
int SecondWindow::najdiUsera(const std::vector<UserType> users)
{
	std::vector<USER> userPtrs(users.begin(), users.end());

	for (int i = 0;i < userPtrs.size(); i++)
	{
		if (userPtrs[i].username() == actualUser->username())
		{
			return i;
		}
	}
	return -1;
}

template <typename UserType>
int SecondWindow::najdiUsera1(const std::vector<UserType> users, std::string name)
{
	std::vector<USER> userPtrs(users.begin(), users.end());

	for (int i = 0;i < userPtrs.size(); i++)
	{
		if (userPtrs[i].username() == name)
		{
			return i;
		}
	}
	return -1;
}

std::string vybratyUser = "";
int index_usera;
void SecondWindow::zobrazNaAktualizovanie()
{
	notVisibleActualise();
	std::string zvolenyUser = ui.vsetciAktualizovanie->currentText().toStdString();
	index_usera = najdiUsera1(firemny_hostia, zvolenyUser);
	if (index_usera != -1)
	{
		vybratyUser = "firma";
		ui.menoAktualiz->setEnabled(true);
		ui.priezviskoAktualiz->setEnabled(true);
		ui.hesloAktualiz->setEnabled(true);
		ui.usernameAktualiz->setEnabled(true);
		ui.firmaAktualiz->setEnabled(true);
		ui.DPHAktualiz->setEnabled(true);
		ui.adresaAktualiz->setEnabled(true);

		ui.aktualneMeno->setText(QString::fromStdString(firemny_hostia[index_usera].name()));
		ui.aktualnePriezvisko->setText(QString::fromStdString(firemny_hostia[index_usera].surname()));
		ui.aktualneHeslo->setText(QString::fromStdString(firemny_hostia[index_usera].password()));
		ui.aktualneUsername->setText(QString::fromStdString(firemny_hostia[index_usera].username()));
		ui.aktualnaAdresa->setText(QString::fromStdString(firemny_hostia[index_usera].address()));
		ui.aktualnaFirma->setText(QString::fromStdString(firemny_hostia[index_usera].companyName()));
		if (!firemny_hostia[index_usera].isVatPayer())
		{
			ui.aktualneDPH->setCurrentIndex(0);
		}
		else
		{
			ui.aktualneDPH->setCurrentIndex(1);
		}
		return;
	}
	index_usera = najdiUsera1(fyzicky_hostia, zvolenyUser);
	if (index_usera != -1)
	{
		vybratyUser = "osoba";
		ui.menoAktualiz->setEnabled(true);
		ui.priezviskoAktualiz->setEnabled(true);
		ui.hesloAktualiz->setEnabled(true);
		ui.usernameAktualiz->setEnabled(true);
		ui.adresaAktualiz->setEnabled(true);

		ui.aktualneMeno->setText(QString::fromStdString(fyzicky_hostia[index_usera].name()));
		ui.aktualnePriezvisko->setText(QString::fromStdString(fyzicky_hostia[index_usera].surname()));
		ui.aktualneHeslo->setText(QString::fromStdString(fyzicky_hostia[index_usera].password()));
		ui.aktualneUsername->setText(QString::fromStdString(fyzicky_hostia[index_usera].username()));
		ui.aktualnaAdresa->setText(QString::fromStdString(fyzicky_hostia[index_usera].address()));
		return;
	}
	index_usera = najdiUsera1(administratori, zvolenyUser);
	if (index_usera != -1)
	{
		vybratyUser = "admin";
		ui.hesloAktualiz->setEnabled(true);
		ui.usernameAktualiz->setEnabled(true);

		ui.aktualneHeslo->setText(QString::fromStdString(administratori[index_usera].password()));
		ui.aktualneUsername->setText(QString::fromStdString(administratori[index_usera].username()));
		if (administratori[index_usera].position() == "admin")
		{
			ui.aktualnaPozicia->setCurrentIndex(1);
		}
		else
		{
			ui.aktualnaPozicia->setCurrentIndex(-1);
		}
		return;
	}
	index_usera = najdiUsera1(recepcne, zvolenyUser);
	if (index_usera != -1)
	{
		vybratyUser = "recepcna";
		ui.hesloAktualiz->setEnabled(true);
		ui.usernameAktualiz->setEnabled(true);

		ui.aktualneHeslo->setText(QString::fromStdString(recepcne[index_usera].password()));
		ui.aktualneUsername->setText(QString::fromStdString(recepcne[index_usera].username()));
		if (recepcne[index_usera].position() == "recepcne")
		{
			ui.aktualnaPozicia->setCurrentIndex(0);
		}
		else
		{
			ui.aktualnaPozicia->setCurrentIndex(-1);
		}
		return;
	}
}

std::string druhNoveUsera = "";
void SecondWindow::zobrazNaVytvorenie()
{
	notVisibleNew();
	ui.usernamePassword->setEnabled(true);
	druhNoveUsera = ui.druhNovehoUsera->currentText().toStdString();
	if (druhNoveUsera == "Recepčná")
	{
		ui.novyZamestnanec->setEnabled(true);
	}
	else if (druhNoveUsera == "Administrátor")
	{
		ui.novyZamestnanec->setEnabled(true);
	}
	else if (druhNoveUsera == "Firemný hosť")
	{
		ui.menoPreizviskoNovy->setEnabled(true);
		ui.novaFirma->setEnabled(true);
	}
	else if (druhNoveUsera == "Fyzický hosť")
	{
		ui.menoPreizviskoNovy->setEnabled(true);
	}
}

void SecondWindow::aktualizovatUsera()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete aktualizovať daného užívateľa? Po tejto operácií budete odhlásený",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		if (vybratyUser == "firma")
		{
			firemny_hostia[index_usera].set_address(ui.novaAdresa->text().toStdString());
			firemny_hostia[index_usera].set_companyName(ui.novaFirmaNazov->text().toStdString());
			firemny_hostia[index_usera].set_isVatPayer(ui.noveDPH->currentIndex());
			firemny_hostia[index_usera].set_name(ui.noveMeno->text().toStdString());
			firemny_hostia[index_usera].set_password(ui.noveHeslo->text().toStdString());
			firemny_hostia[index_usera].set_surname(ui.novePriezvisko->text().toStdString());
			firemny_hostia[index_usera].set_username(ui.noveUsername->text().toStdString());
			ulozPouzivatelov("uzivatelia.txt");
		}
		else if (vybratyUser == "osoba")
		{
			fyzicky_hostia[index_usera].set_address(ui.novaAdresa->text().toStdString());
			fyzicky_hostia[index_usera].set_name(ui.noveMeno->text().toStdString());
			fyzicky_hostia[index_usera].set_password(ui.noveHeslo->text().toStdString());
			fyzicky_hostia[index_usera].set_surname(ui.novePriezvisko->text().toStdString());
			fyzicky_hostia[index_usera].set_username(ui.noveUsername->text().toStdString());
			ulozPouzivatelov("uzivatelia.txt");
		}
		else if (vybratyUser == "admin")
		{
			administratori[index_usera].set_password(ui.noveHeslo->text().toStdString());
			administratori[index_usera].set_username(ui.noveUsername->text().toStdString());
			if (ui.novaPozicia->currentIndex() == 0)
			{
				administratori[index_usera].set_position(ui.noveUsername->text().toStdString());
			}
			ulozPouzivatelov("uzivatelia.txt");
		}

		else if (vybratyUser == "recepcna")
		{
			recepcne[index_usera].set_password(ui.noveHeslo->text().toStdString());
			recepcne[index_usera].set_username(ui.noveUsername->text().toStdString());
			if (ui.novaPozicia->currentIndex() == 0)
			{
				recepcne[index_usera].set_position(ui.noveUsername->text().toStdString());
			}
			ulozPouzivatelov("uzivatelia.txt");

		}

		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}


void SecondWindow::vytvoritUsera()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete vytvoriť daného užívateľa? Po tejto operácií budete odhlásený",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{

		if (druhNoveUsera == "Recepčná")
		{
			HOTEL_EMPLOYER new_employer = HOTEL_EMPLOYER(ui.novyUsername->text().toStdString(), ui.novyHeslo->text().toStdString(), "recepcna");
			recepcne.push_back(new_employer);
			ulozPouzivatelov("uzivatelia.txt");
		}
		else if (druhNoveUsera == "Administrátor")
		{
			HOTEL_EMPLOYER new_employer = HOTEL_EMPLOYER(ui.novyUsername->text().toStdString(), ui.novyHeslo->text().toStdString(), "admin");
			administratori.push_back(new_employer);
			ulozPouzivatelov("uzivatelia.txt");
		}
		else if (druhNoveUsera == "Firemný hosť")
		{
			BUSSINES_GUEST new_guest = BUSSINES_GUEST(
				ui.novyUsername->text().toStdString(),
				ui.novyHeslo->text().toStdString(),
				ui.newMeno->text().toStdString(),
				ui.newPriezvisko->text().toStdString(),
				ui.novaAdresa->text().toStdString(),
				ui.newFirma->text().toStdString(),
				ui.newDPH->currentIndex());
			firemny_hostia.push_back(new_guest);
			ulozPouzivatelov("uzivatelia.txt");
		}
		else if (druhNoveUsera == "Fyzický hosť")
		{
			GUEST new_guest = GUEST(
				ui.novyUsername->text().toStdString(),
				ui.novyHeslo->text().toStdString(),
				ui.newMeno->text().toStdString(),
				ui.newPriezvisko->text().toStdString(),
				ui.novaAdresa->text().toStdString());
			fyzicky_hostia.push_back(new_guest);
			ulozPouzivatelov("uzivatelia.txt");
		}

		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}



void SecondWindow::odstranitUsera()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete vytvoriť daného užívateľa? Po tejto operácií budete odhlásený",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		std::string zvolenyUser = ui.vsetciVymazanie->currentText().toStdString();
		int index = najdiUsera1(firemny_hostia, zvolenyUser);
		if (index != -1)
		{
			firemny_hostia.erase(firemny_hostia.begin() + index);
			ulozPouzivatelov("uzivatelia.txt");
			goto skipClosingWindow;
		}
		index = najdiUsera1(fyzicky_hostia, zvolenyUser);
		if (index != -1)
		{
			fyzicky_hostia.erase(fyzicky_hostia.begin() + index);
			ulozPouzivatelov("uzivatelia.txt");
			goto skipClosingWindow;
		}
		index = najdiUsera1(administratori, zvolenyUser);
		if (index != -1)
		{
			administratori.erase(administratori.begin() + index);
			ulozPouzivatelov("uzivatelia.txt");
			goto skipClosingWindow;
		}
		index = najdiUsera1(recepcne, zvolenyUser);
		if (index != -1)
		{
			recepcne.erase(recepcne.begin() + index);
			ulozPouzivatelov("uzivatelia.txt");
			goto skipClosingWindow;
		}
	skipClosingWindow:
		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}

void SecondWindow::odstranitIzbu()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete vymazať vybranú izbu? Po tejto operácií budete odhlásený",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		int cisloIzby = ui.cislaIzieb->currentText().toInt();

		for (int i = 0;i < rooms.size(); i++)
		{
			if (rooms[i].roomNumber() == cisloIzby)
			{
				rooms.erase(rooms.begin() + i);
				ulozIzby("izbyNove.txt");
			}
		}

		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}

void SecondWindow::zobrazitIzbu()
{
	roomSetting();

	int cisloIzby = ui.cisloIzbyComboBox->currentText().toInt();

	for (int i = 0;i < rooms.size(); i++)
	{
		if (rooms[i].roomNumber() == cisloIzby)
		{
			if (rooms[i].typeOfRoom() == "Jednolozkova")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(0);
			}
			else if (rooms[i].typeOfRoom() == "Dvojlozkova")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(1);
			}
			else if (rooms[i].typeOfRoom() == "Trojlozkova")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(2);
			}
			else if (rooms[i].typeOfRoom() == "Stvorlozkova")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(3);
			}
			else if (rooms[i].typeOfRoom() == "Suite")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(4);
			}
			else if (rooms[i].typeOfRoom() == "Apartman")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(5);
			}
			else if (rooms[i].typeOfRoom() == "Rodinna")
			{
				ui.aktualnyDruhIzby->setCurrentIndex(6);
			}

			ui.aktualnaCena->setValue(rooms[i].pricePerNight());

			ui.aktualneDatumy->setRowCount(0);

			for (int j = 0; j < rooms[i].reservedDays().size(); j++)
			{
				int row = ui.aktualneDatumy->rowCount();
				ui.aktualneDatumy->insertRow(row);
				ui.aktualneDatumy->setItem(row, 0, new QTableWidgetItem(rooms[i].reservedDays()[j].toString()));
			}

		}
	}
}


void SecondWindow::pridatDatum()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete pridať daný dátum? Túto operáciu nemôžte vrátiť späť.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		QString dateString = ui.dateEdit->date().toString();

		int row = ui.noveDatumy->rowCount();
		ui.noveDatumy->insertRow(row);

		// Add the date to the table as a new QTableWidgetItem
		ui.noveDatumy->setItem(row, 0, new QTableWidgetItem(dateString));
	}
}

void SecondWindow::aktualizovatIzbu()
{
	if (ui.novyDruhIzby->currentIndex() == -1 || ui.novaCena->value() == 0)
	{
		QMessageBox::information(this, "Chyba", "Nie sú vyplnené všetky údaje!");
		return;
	}
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete aktualizovať danú izbu? Po tejto operácií budete odhlásený.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		int index = ui.novyDruhIzby->currentIndex(); //zistenia vybrateho typu izby pomocou indexu
		int cisloIzby = ui.cisloIzbyComboBox->currentText().toInt();
		for (int i = 0;i < rooms.size(); i++)
		{
			if (rooms[i].roomNumber() == cisloIzby)
			{
				switch (index)
				{
				case 0: //jednolozkova
					rooms[i].set_typeOfRoom("Jednolozkova");
					break;

				case 1: //dvojlozkova
					rooms[i].set_typeOfRoom("Dvojlozkova");
					break;

				case 2: //trojlozkova
					rooms[i].set_typeOfRoom("Trojlozkova");
					break;

				case 3: //stvorlozkova
					rooms[i].set_typeOfRoom("Stvorlozkova");
					break;

				case 4: //suita
					rooms[i].set_typeOfRoom("Suite");
					break;

				case 5: //apartman
					rooms[i].set_typeOfRoom("Apartman");
					break;

				case 6: //rodinna
					rooms[i].set_typeOfRoom("Rodinna");
					break;

				default:
					return;
				}
				rooms[i].set_pricePerNight(ui.novaCena->value());
				rooms[i].clearReservedDays();

				for (int row = 0; row < ui.noveDatumy->rowCount(); ++row)
				{
					QString dateString = ui.noveDatumy->item(row, 0)->text();
					QDate date = QDate::fromString(dateString);
					rooms[i].addReservedDay(date);
				}
			}
		}
		ulozIzby("izbyNove.txt");
		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}

void SecondWindow::vytvoritIzbu()
{
	if (ui.novaDruhIzby->currentIndex() == -1 || ui.novaIzbaCena->value() == 0 || ui.novaCisloIzby->value() == 0)
	{
		QMessageBox::information(this, "Chyba", "Nie sú vyplnené všetky údaje!");
		return;
	}
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete aktualizovať danú izbu? Po tejto operácií budete odhlásený.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		int index = ui.novaDruhIzby->currentIndex(); //zistenia vybrateho typu izby pomocou indexu

		std::string typeOfRoom;
		switch (index)
		{
		case 0: //jednolozkova
			typeOfRoom = "Jednolozkova";
			break;

		case 1: //dvojlozkova
			typeOfRoom = "Dvojlozkova";
			break;

		case 2: //trojlozkova
			typeOfRoom = "Trojlozkova";
			break;

		case 3: //stvorlozkova
			typeOfRoom = "Stvorlozkova";
			break;

		case 4: //suita
			typeOfRoom = "Suite";
			break;

		case 5: //apartman
			typeOfRoom = "Apartman";
			break;

		case 6: //rodinna
			typeOfRoom = "Rodinna";
			break;

		default:
			return;
		}
		int roomNumber = ui.novaCisloIzby->value();
		double pricePerNight = ui.novaIzbaCena->value();

		std::vector <QDate> reservedDays;
		for (int row = 0; row < ui.novaDatumy->rowCount(); ++row)
		{
			QString dateString = ui.novaDatumy->item(row, 0)->text();
			QDate date = QDate::fromString(dateString);
			reservedDays.push_back(date);
		}

		ROOM new_room = ROOM(typeOfRoom, reservedDays, roomNumber, pricePerNight);
		rooms.push_back(new_room);
		ulozIzby("izbyNove.txt");
		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}



void SecondWindow::pridatDatumNova()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete pridať daný dátum? Túto operáciu nemôžte vrátiť späť.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		QString dateString = ui.novaDateEdit->date().toString();

		int row = ui.novaDatumy->rowCount();
		ui.novaDatumy->insertRow(row);

		// Add the date to the table as a new QTableWidgetItem
		ui.novaDatumy->setItem(row, 0, new QTableWidgetItem(dateString));
	}
}
void SecondWindow::zmenitStatus()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this,
		"Potvrdenie",
		"Ste si istý, že chcete aktualizovať danú rezerváciu? Po tejto operácií budete odhlásený.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		for (int i = 0; i < rezervacie.size(); i++)
		{
			if (rezervacie[i].reservationNumber() == ui.cisloRezervRecep->currentText().toInt())
			{
				int status = ui.statusRecep->currentIndex() + 1;
				if (status == 2) //schvalena
				{
					for (int j = 0; j < rooms.size(); j++) // <-- Fixed the loop index variable from i to j
					{
						if (rooms[j].roomNumber() == rezervacie[i].roomNumber())
						{
							if (rooms[j].reservedDays() != rezervacie[i].date())
							{
								rooms[j].set_reservedDays(rezervacie[i].date());
								ulozIzby("izbyNove.txt");
							}
							else
							{
								QMessageBox::information(this, "Chyba", "Danú izbu nemôžete schváliť. V dané obdobie už je obsadená!"); // <-- Removed the unnecessary part
							}
						}
					}
				}
				rezervacie[i].set_status(status);
				saveReservationsToFile("rezervacie.txt");
			}
		}
		loadReservationsFromFile("rezervacie.txt");
		priradRezervacie();
		MyFirstQtApp* firstWindow = dynamic_cast<MyFirstQtApp*>(parent());
		if (firstWindow)
		{
			firstWindow->settings();
			firstWindow->show();
			this->close();
		}
	}
}
