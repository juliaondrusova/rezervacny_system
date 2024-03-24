#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyFirstQtApp.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include "SecondWindow.h"

class MyFirstQtApp : public QMainWindow
{
	Q_OBJECT

public:
	MyFirstQtApp(QWidget* parent = nullptr);
	~MyFirstQtApp();

private:
	Ui::MyFirstQtAppClass ui;
	SecondWindow* sw;
	template <typename UserType>
	bool najdiUsera(const std::vector<UserType> users);

private slots:
	void openAnotherWindow();
	void zmenaTypuUzivatela();
	void zmenaTypuTypuUzivatela();
	void prihlasenie();

public:
	void settings();
};
