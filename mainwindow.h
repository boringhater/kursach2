#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QListWidgetItem>
#include "createfilewindow.h"
#include "PiNoteLib.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NotesRepository * globalRepPointer = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createButton_clicked();

    void on_deleteButton_clicked();

    void on_filterButton_clicked();

    void on_noFiltrationButton_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_saveButton_clicked();

    void on_plainTextEdit_textChanged();

    void on_editNoteButton_clicked();

    void on_infoButton_clicked();

private:
    NotesRepository * globalRepPointer;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
