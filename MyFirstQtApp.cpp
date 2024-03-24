#include "MyFirstQtApp.h"
#include "c_classy.h"
#include <qmessagebox.h>

extern std::vector<GUEST> fyzicky_hostia;
extern std::vector<BUSSINES_GUEST> firemny_hostia;
extern std::vector<HOTEL_EMPLOYER> recepcne;
extern std::vector<HOTEL_EMPLOYER> administratori;
extern std::vector<RESERVATION> rezervacie;

std::string actualUserName;
USER* actualUser;

MyFirstQtApp::MyFirstQtApp(QWidget* parent)
	: QMainWindow(parent)
{
	QWidget::setWindowTitle("Prihlasovanie");
	ui.setupUi(this);
	connect(ui.prihlasenie, SIGNAL(clicked()), this, SLOT(openAnotherWindow()));

	connect(ui.typPouzivatela, &QComboBox::currentIndexChanged, this, &MyFirstQtApp::zmenaTypuUzivatela);
	connect(ui.typTypuPouzivatela, &QComboBox::currentIndexChanged, this, &MyFirstQtApp::zmenaTypuTypuUzivatela);
	connect(ui.prihlasenie, &QPushButton::clicked, this, &MyFirstQtApp::prihlasenie);

	settings();

}


void MyFirstQtApp::settings()
{
	ui.typPouzivatela->setCurrentIndex(-1);
	ui.typTypuPouzivatela->setCurrentIndex(-1);
	ui.menoPouzivatela->setCurrentIndex(-1);
	ui.heslo->setText("");
}
MyFirstQtApp::~MyFirstQtApp()
{}

void MyFirstQtApp::openAnotherWindow() {

}

void MyFirstQtApp::zmenaTypuUzivatela()
{
	int index = ui.typPouzivatela->currentIndex(); //zistenia typu uzivatela (pomocou indexu)

	switch (index) {
	case 0:
		// host
		ui.typTypuPouzivatela->clear(); // vymazanie existujucich moznosti
		ui.typTypuPouzivatela->addItem("Fyzická osoba");
		ui.typTypuPouzivatela->addItem("Firma");
		ui.typTypuPouzivatela->setCurrentIndex(-1);
		break;
	case 1:
		// zamestnanec
		ui.typTypuPouzivatela->clear(); // vymazanie existujucich moznosti
		ui.typTypuPouzivatela->addItem("Recepčný");
		ui.typTypuPouzivatela->addItem("Administrátor");
		ui.typTypuPouzivatela->setCurrentIndex(-1);
		break;
	default:
		break;
	}
	ui.typTypuPouzivatela->setDisabled(false);
}


void MyFirstQtApp::zmenaTypuTypuUzivatela()
{
	int index = ui.typTypuPouzivatela->currentIndex(); //zistenia typu typu uzivatela (pomocou indexu)

	switch (index) {
	case 0:
		ui.menoPouzivatela->clear(); // vymazanie existujucich moznosti

		if (ui.typPouzivatela->currentIndex() == 0) //host, fyzicka osoba
		{
			actualUser = new GUEST();
			for (int i = 0; i < fyzicky_hostia.size(); i++)
			{
				ui.menoPouzivatela->addItem(QString::fromStdString(fyzicky_hostia[i].username()));
			}

		}

		else if (ui.typPouzivatela->currentIndex() == 1) //zamestnanec, recepcny
		{
			actualUser = new HOTEL_EMPLOYER("recepcny");
			for (int i = 0; i < recepcne.size(); i++)
			{
				ui.menoPouzivatela->addItem(QString::fromStdString(recepcne[i].username()));
			}
		}

		ui.menoPouzivatela->setCurrentIndex(-1);
		break;

	case 1:
		ui.menoPouzivatela->clear(); // vymazanie existujucich moznosti

		if (ui.typPouzivatela->currentIndex() == 0) //host, firma
		{
			actualUser = new BUSSINES_GUEST();
			for (int i = 0; i < firemny_hostia.size(); i++)
			{
				ui.menoPouzivatela->addItem(QString::fromStdString(firemny_hostia[i].username()));
			}
		}
		else if (ui.typPouzivatela->currentIndex() == 1) //zamestnanec, administrator
		{
			actualUser = new HOTEL_EMPLOYER("admin");
			for (int i = 0; i < administratori.size(); i++)
			{
				ui.menoPouzivatela->addItem(QString::fromStdString(administratori[i].username()));
			}
		}

		ui.menoPouzivatela->setCurrentIndex(-1);
		break;

	default:
		break;
	}

}

void MyFirstQtApp::prihlasenie()
{
	bool successfulLogin = false;

	if (actualUser->typeOfUser() == "GUEST")
	{
		successfulLogin = najdiUsera(fyzicky_hostia);
	}
	else if (actualUser->typeOfUser() == "BUSSINES_GUEST")
	{
		successfulLogin = najdiUsera(firemny_hostia);
	}

	else if (actualUser->typeOfUser() == "HOTEL_EMPLOYER" && reinterpret_cast<HOTEL_EMPLOYER*>(actualUser)->position()=="recepcny")
	{
		successfulLogin = najdiUsera(recepcne);

	}
	else if (actualUser->typeOfUser() == "HOTEL_EMPLOYER" && reinterpret_cast<HOTEL_EMPLOYER*>(actualUser)->position() == "admin")
	{
		successfulLogin = najdiUsera(administratori);
	}

	if (successfulLogin == true)
	{
		sw = new SecondWindow(this);
		sw->settings();
		this->close();
		sw->show();
		return;
	}
	else
	{
		QMessageBox::warning(this, "Chyba", "Zadané heslo nie je správne!");
	}
}


template <typename UserType>
bool MyFirstQtApp::najdiUsera(const std::vector<UserType> users)
{
	std::vector<USER> userPtrs(users.begin(), users.end());

	for (int i = 0;i < userPtrs.size(); i++)
	{
		if (QString::fromStdString(userPtrs[i].username()) == ui.menoPouzivatela->currentText())
		{
			if (QString::fromStdString(userPtrs[i].password()) == ui.heslo->text())
			{
				actualUser->set_username(userPtrs[i].username());
				return true;
			}
		} 
	}
	return false;
}
