#include "pracaSoSubormi.h"
#include <qfile.h>
#include <qjsondocument.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>


void readJsonData(std::string filePath)
{
	QFile file(QString::fromStdString(filePath));

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QString jsonData = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
	QJsonObject jsonObject = doc.object();

	if (fyzicky_hostia.size() != 0)
	{
		fyzicky_hostia.clear();
	}

	if (firemny_hostia.size() != 0)
	{
		firemny_hostia.clear();
	}

	if (administratori.size() != 0)
	{
		administratori.clear();
	}

	if (recepcne.size() != 0)
	{
		recepcne.clear();
	}

	if (jsonObject.contains("employees")) {
		QJsonArray employeesArray = jsonObject["employees"].toArray();

		for (const QJsonValue& employeeValue : employeesArray)
		{
			QJsonObject employeeObject = employeeValue.toObject();
			QString role = employeeObject["role"].toString();

			if (role == "Host")
			{
				QString name = employeeObject["first_name"].toString();
				QString surname = employeeObject["last_name"].toString();
				QString address = employeeObject["address"].toString();
				QString username = employeeObject["username"].toString();
				QString password = employeeObject["password"].toString();

				GUEST new_quest = GUEST(username.toStdString(), password.toStdString(), name.toStdString(), surname.toStdString(), address.toStdString());
				fyzicky_hostia.push_back(new_quest);
			}

			if (role == "Firma")
			{
				QString name = employeeObject["first_name"].toString();
				QString surname = employeeObject["last_name"].toString();
				QString address = employeeObject["address"].toString();
				QString companyName = employeeObject["company_name"].toString();
				bool isVatPayer = employeeObject["is_vat_payer"].toBool();
				QString username = employeeObject["username"].toString();
				QString password = employeeObject["password"].toString();

				BUSSINES_GUEST new_quest = BUSSINES_GUEST(username.toStdString(), password.toStdString(), name.toStdString(), surname.toStdString(), address.toStdString(), companyName.toStdString(), isVatPayer);
				firemny_hostia.push_back(new_quest);
			}

			if (role == "admin")
			{
				QString position = employeeObject["role"].toString();
				QString username = employeeObject["username"].toString();
				QString password = employeeObject["password"].toString();

				HOTEL_EMPLOYER new_employer = HOTEL_EMPLOYER(username.toStdString(), password.toStdString(), position.toStdString());
				administratori.push_back(new_employer);
			}

			if (role == "recepcny")
			{
				QString position = employeeObject["role"].toString();
				QString username = employeeObject["username"].toString();
				QString password = employeeObject["password"].toString();

				HOTEL_EMPLOYER new_employer = HOTEL_EMPLOYER(username.toStdString(), password.toStdString(), position.toStdString());
				recepcne.push_back(new_employer);
			}

		}
	}
}


void readJsonToRoom(std::string filePath)
{
	QFile file(QString::fromStdString(filePath));

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QString jsonData = file.readAll();

	QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());

	QJsonObject jsonObject = doc.object();

	if (!jsonObject.contains("rooms") || !jsonObject["rooms"].isArray())
	{
		return;
	}

	QJsonArray roomsArray = jsonObject["rooms"].toArray();

	for (const QJsonValue& roomValue : roomsArray)
	{
		QJsonObject roomObject = roomValue.toObject();

		ROOM room;

		if (roomObject.contains("typeOfRoom") && roomObject["typeOfRoom"].isString())
			room.set_typeOfRoom(roomObject["typeOfRoom"].toString().toStdString());

		if (roomObject.contains("dates") && roomObject["dates"].isArray())
		{
			QJsonArray datesArray = roomObject["dates"].toArray();
			std::vector<QDate> dates;

			for (const QJsonValue& dateValue : datesArray)
			{
				QDate date = QDate::fromString(dateValue.toString(), "yyyy,d,M");
				if (date.isValid())
					dates.push_back(date);
			}

			room.set_reservedDays(dates);
		}

		if (roomObject.contains("roomNumber") && roomObject["roomNumber"].isDouble())
			room.set_roomNumber(static_cast<int>(roomObject["roomNumber"].toDouble()));

		if (roomObject.contains("price") && roomObject["price"].isDouble())
			room.set_pricePerNight(roomObject["price"].toDouble());

		rooms.push_back(room);
	}
}



void ulozIzby(std::string filePath)
{
	QJsonArray roomsArray;

	for (int i = 0; i < rooms.size();i++)
	{
		QJsonObject roomObject;
		roomObject["typeOfRoom"] = QString::fromStdString(rooms[i].typeOfRoom());

		// Saving dates as strings in the format "yyyy,M,d"
		QJsonArray datesArray;
		for (const QDate& date : rooms[i].reservedDays())
		{
			datesArray.append(date.toString("yyyy,d,M"));
		}
		roomObject["dates"] = datesArray;

		roomObject["roomNumber"] = rooms[i].roomNumber();
		roomObject["price"] = rooms[i].pricePerNight();

		roomsArray.append(roomObject);
	}

	QJsonObject jsonObject;
	jsonObject["rooms"] = roomsArray;

	QFile file(QString::fromStdString(filePath));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QJsonDocument doc(jsonObject);
		file.write(doc.toJson());
		file.close();
	}
}

void ulozPouzivatelov(std::string filePath)
{
	QJsonObject jsonObject;

	QJsonArray employeesArray;

	// Assuming fyzicky_hostia is a vector of GUEST, firemny_hostia is a vector of BUSSINES_GUEST,
	// administratori is a vector of HOTEL_EMPLOYER, and recepcne is also a vector of HOTEL_EMPLOYER.

	// Writing physical guests (fyzicky_hostia)
	for (int i = 0; i < fyzicky_hostia.size();i++)
	{
		QJsonObject guestObject;
		guestObject["role"] = "Host";
		guestObject["first_name"] = QString::fromStdString(fyzicky_hostia[i].name());
		guestObject["last_name"] = QString::fromStdString(fyzicky_hostia[i].surname());
		guestObject["address"] = QString::fromStdString(fyzicky_hostia[i].address());
		guestObject["username"] = QString::fromStdString(fyzicky_hostia[i].username());
		guestObject["password"] = QString::fromStdString(fyzicky_hostia[i].password());

		employeesArray.append(guestObject);
	}

	for (int i = 0; i < firemny_hostia.size();i++)
	{
		QJsonObject guestObject;
		guestObject["role"] = "Firma";
		guestObject["first_name"] = QString::fromStdString(firemny_hostia[i].name());
		guestObject["last_name"] = QString::fromStdString(firemny_hostia[i].surname());
		guestObject["address"] = QString::fromStdString(firemny_hostia[i].address());
		guestObject["company_name"] = QString::fromStdString(firemny_hostia[i].companyName());
		guestObject["is_vat_payer"] = firemny_hostia[i].isVatPayer();
		guestObject["username"] = QString::fromStdString(firemny_hostia[i].username());
		guestObject["password"] = QString::fromStdString(firemny_hostia[i].password());

		employeesArray.append(guestObject);
	}

	for (int i = 0; i < administratori.size();i++)
	{
		QJsonObject adminObject;
		adminObject["role"] = "admin";
		adminObject["username"] = QString::fromStdString(administratori[i].username());
		adminObject["password"] = QString::fromStdString(administratori[i].password());

		employeesArray.append(adminObject);
	}

	// Writing receptionists (recepcne)
	for (int i = 0; i < recepcne.size();i++)
	{
		QJsonObject receptionistObject;
		receptionistObject["role"] = "recepcny";
		receptionistObject["username"] = QString::fromStdString(recepcne[i].username());
		receptionistObject["password"] = QString::fromStdString(recepcne[i].password());

		employeesArray.append(receptionistObject);
	}

	jsonObject["employees"] = employeesArray;

	QFile file(QString::fromStdString(filePath));
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QJsonDocument doc(jsonObject);
		file.write(doc.toJson());
		file.close();
	}
}


void loadReservationsFromFile(std::string filename)
{

	QFile file(QString::fromStdString(filename));

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QString jsonData = file.readAll();
	file.close();

	// Parse JSON data
	QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData.toUtf8());
	QJsonObject jsonObject = jsonDocument.object();

	// Extract reservations array
	QJsonArray reservationsArray = jsonObject["reservations"].toArray();

	// Convert JSON objects to Reservation objects
	for (const QJsonValue& reservationValue : reservationsArray)
	{
		QJsonObject reservationObject = reservationValue.toObject();

		RESERVATION reservation;
		reservation.set_reservationNumber(reservationObject["reservationNumber"].toInt()) ;
		reservation.set_userName(reservationObject["userName"].toString().toStdString());
		QStringList dateList = reservationObject["dates"].toVariant().toStringList();
		std::vector<std::string> datesVector;
		for (QString date : dateList) {
			datesVector.push_back(date.toStdString());
			std::cout << date.toStdString() << std::endl;
		}
		reservation.set_date(datesVector);
		reservation.set_roomNumber(reservationObject["roomNumber"].toInt());
		reservation.set_cena(reservationObject["totalPrice"].toDouble());
		reservation.set_status(reservationObject["status"].toInt());

		rezervacie.push_back(reservation);
	}
}

void saveReservationsToFile(std::string filename)
{
	QJsonArray reservationsArray;

	// Convert Reservation objects to JSON objects
	for (int i =0; i< rezervacie.size(); i++)
	{
		QJsonObject reservationObject;
		reservationObject["reservationNumber"] = rezervacie[i].reservationNumber();
		reservationObject["userName"] = QString::fromStdString(rezervacie[i].userName());

		QJsonArray dateList;
		const std::vector<QDate> datesVector = rezervacie[i].date();

		for (const QDate date : datesVector)
		{
			dateList.push_back(date.toString("yyyy,dd,MM"));
		}
		reservationObject["dates"] = dateList;

		reservationObject["roomNumber"] = rezervacie[i].roomNumber();
		reservationObject["totalPrice"] = rezervacie[i].cena();
		reservationObject["status"] = rezervacie[i].status();

		reservationsArray.append(reservationObject);
	}

	QJsonObject jsonObject;
	jsonObject["reservations"] = reservationsArray;

	QJsonDocument jsonDocument(jsonObject);
	QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);

	QFile file(QString::fromStdString(filename));
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << jsonString;
		file.close();
	}
}


void priradRezervacie()
{
	for (int i = 0; i < rezervacie.size(); i++)
	{
		for (int j = 0; j < fyzicky_hostia.size();j++)
		{
			if (rezervacie[i].userName() == fyzicky_hostia[j].username())
			{
				fyzicky_hostia[j].set_reservation(rezervacie[i]);
			}
		}
		for (int j = 0; j < firemny_hostia.size();j++)
		{
			if (rezervacie[i].userName() == firemny_hostia[j].username())
			{
				firemny_hostia[j].set_reservation(rezervacie[i]);
			}
		}
	}
}