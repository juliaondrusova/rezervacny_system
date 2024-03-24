#pragma once

#include <iostream>
#include <vector>
#include <qdatetime.h>


class ROOM
{
private:
	std::string typeOfRoom_;
	std::vector <QDate> reservedDays_;
	int roomNumber_;
	double pricePerNight_;

public:
	//getery
	std::string typeOfRoom() { return typeOfRoom_; };
	std::vector <QDate> reservedDays() { return reservedDays_; };
	int roomNumber() { return roomNumber_; };
	double pricePerNight() { return pricePerNight_; };
	//settery
	void set_typeOfRoom(std::string typeOfRoom) { typeOfRoom_ = typeOfRoom; };
	void set_reservedDays(std::vector <QDate> reservedDays) { reservedDays_ = reservedDays; };
	void set_roomNumber(int roomNumber) { roomNumber_ = roomNumber; };
	void set_pricePerNight(double pricePerNight) { pricePerNight_ = pricePerNight; };
	//konstruktor
	ROOM(std::string typeOfRoom, std::vector <QDate> reservedDays, int roomNumber, double pricePerNight) :
		typeOfRoom_(typeOfRoom), reservedDays_(reservedDays), roomNumber_(roomNumber), pricePerNight_(pricePerNight) {}
	ROOM() {};
	//method
	void addReservedDay(QDate date) { reservedDays_.push_back(date); }
	void clearReservedDays() { reservedDays_.clear(); };
	void odstranDatum(QDate date) {
		for (int i = 0;i < reservedDays_.size(); i++)
		{
			if (reservedDays_[i] == date)
			{
				reservedDays_.erase(reservedDays_.begin() + i);
			}
		}
	}
};


class RESERVATION
{
private:
	std::vector <QDate> date_;
	int roomNumber_;
	double cena_;
	int status_; //1-nova 2-schvalena 3-zamietnuta 
	int reservationNumber_;
	std::string userName_;

public:
	//gettery
	std::vector <QDate> date() { return date_; };
	int roomNumber() { return roomNumber_;};
	double cena() { return cena_; };
	int status() { return status_; };
	int reservationNumber() { return reservationNumber_; };
	std::string userName() { return userName_; };
	//settery
	void set_date(std::vector <std::string> date)
	{
		for (int i = 0;i < date.size();i++)
		{
			date_.push_back(QDate::fromString(QString::fromStdString(date[i]), "yyyy,d,M"));
		}
	}
	void set_roomNumber(int roomNumber) { roomNumber_ = roomNumber; };
	void set_cena(double cena) { cena_ = cena; };
	void set_status(int status) { status_ = status; };
	void set_reservationNumber(int reservationNumber) { reservationNumber_ = reservationNumber; };
	void set_userName(std::string userName) { userName_ = userName; };
	//konstruktor
	RESERVATION(std::vector <QDate> date, int roomNumber, double cena, int status, int reservationNumber, std::string userName) :
		date_(date), roomNumber_(roomNumber), cena_(cena), status_(status), reservationNumber_(reservationNumber), userName_(userName) {}
	RESERVATION() {};
	//metody
};

class USER
{
protected:
	std::string userName_;
	std::string password_;

public:
	//getery
	std::string username() { return userName_; };
	std::string password() { return password_; };
	//setery
	void set_username(std::string username) { userName_ = username; };
	void set_password(std::string password) { password_ = password; };
	//konstruktor
	USER(std::string username, std::string password) : userName_(username), password_(password) {}
	USER() {};
	//metody
	virtual std::string typeOfUser() { return "USER"; };
};

class GUEST :public USER
{
protected:
	std::string name_;
	std::string surname_;
	std::string address_;
	std::vector <RESERVATION> reservations_;

public:
	//getery
	std::string name() { return name_; };
	std::string surname() { return surname_; };
	std::string address() { return address_; };
	std::vector <RESERVATION> reservations() { return reservations_; };
	//setery
	void set_name(std::string name) { name_ = name; };
	void set_surname(std::string surname) { surname_ = surname; };
	void set_address(std::string address) { address_ = address; };
	void set_reservation(RESERVATION reservation) { reservations_.push_back(reservation); };
	void set_reservation(std::vector <RESERVATION> reservations) { reservations_ = reservations; };
	//konstruktory
	GUEST(std::string username, std::string password, std::string name, std::string surname, std::string address) :
		USER(username, password), name_(name), surname_(surname), address_(address) {}
	GUEST() {};
	//metody
	void zrusitRezervaciu(int index) { reservations_.erase(reservations_.begin() + index); };
	void pridatRezervaciu(RESERVATION reservation) { reservations_.push_back(reservation); };
	virtual std::string typeOfUser() { return "GUEST"; };
};


class BUSSINES_GUEST :public GUEST
{
protected:
	std::string companyName_;
	bool isVatPayer_;

public:
	//getery
	std::string companyName() { return companyName_; };
	bool isVatPayer() { return isVatPayer_; };
	//setery
	void set_companyName(std::string companyName) { companyName_ = companyName; };
	void set_isVatPayer(bool isVatPayer) { isVatPayer_ = isVatPayer; };
	//konstruktor
	BUSSINES_GUEST(std::string username, std::string password, std::string name, std::string surname, std::string address, std::string companyName, bool isVatPayer)
		: GUEST(username, password, name, surname, address), companyName_(companyName), isVatPayer_(isVatPayer) {}
	//metody
	BUSSINES_GUEST() {};
	virtual std::string typeOfUser() { return "BUSSINES_GUEST"; };

};

class HOTEL_EMPLOYER :public USER
{
private:
	std::string position_;
public:
	//geter
	std::string position() { return position_; };
	//seter
	void set_position(std::string position) { position_ = position; };
	//konstruktor
	HOTEL_EMPLOYER(std::string username, std::string password, std::string position) :
		USER(username, password), position_(position) {}
	HOTEL_EMPLOYER(std::string position) { position_ = position; };
	HOTEL_EMPLOYER(std::string username, std::string password) :
		USER(username, password) {}
	//metody
	virtual std::string typeOfUser() { return "HOTEL_EMPLOYER"; };
};


