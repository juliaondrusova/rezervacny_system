#pragma once

#include <QMainWindow>
#include "ui_SecondWindow.h"
#include <vector>
#include <iostream>
#include "c_classy.h"


class SecondWindow : public QMainWindow
{
	Q_OBJECT

public:
	SecondWindow(QWidget* parent = nullptr);
	~SecondWindow();

private:
	Ui::SecondWindowClass ui;
	std::vector <QDate> selectedDays;
	void pridajPouzivatelovi(std::string typIzby);
	ROOM* vyberCisloIzby();
	bool dostupnost(ROOM room);
	void zrusitRiadok(int index);
	template <typename UserType>
	int najdiUsera(const std::vector<UserType> users);
	template <typename UserType>
	int najdiUsera1(const std::vector<UserType> users, std::string name);
	void zobrazitUSerov();
	void notVisibleActualise();
	void notVisibleNew();
	void roomSetting();
	void vyplnTabulkuRezervacii();
	void nacitatTabulkuRecepcie();

public:
	void settings();

private slots:
	void odhlasenie();
	void vyberTypuIzby();
	void zmenaPrichodu();
	void zmenaOdchodu();
	void rezervacia();
	void zobrazitIzby();
	void zrusitRezervaciu();
	void vytvoritFakturu();
	void zobrazNaAktualizovanie();
	void zobrazNaVytvorenie();
	void aktualizovatUsera();
	void vytvoritUsera();
	void odstranitUsera();
	void odstranitIzbu();
	void zobrazitIzbu();
	void pridatDatum();
	void pridatDatumNova();
	void aktualizovatIzbu();
	void vytvoritIzbu();
	void zmenitStatus();
};
