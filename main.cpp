#include "mainwindow.h"
#include "PiNoteLib.h"
#include <QApplication>
#include <QStyleFactory>
#include "QTextCodec"
#include "QFile"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    setlocale(LC_ALL, "ru-RU");
    NotesRepository globalRep = NotesRepository::constructRepositoryFromFile();
    NotesRepository* globalRepPointer = &globalRep;
    QApplication a(argc, argv);
    qApp->setStyle(QStyleFactory::create("Fusion"));
    MainWindow w(globalRepPointer);
    w.show();
    return a.exec();
}
