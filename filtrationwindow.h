#ifndef FILTRATIONWINDOW_H
#define FILTRATIONWINDOW_H
#pragma once
#include <QDialog>
#include "PiNoteLib.h"

namespace Ui {
class filtrationWindow;
}

class filtrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit filtrationWindow(NotesRepository* globalRepPointer, std::vector<std::string>* filterNamesList,QWidget *parent = nullptr);
    ~filtrationWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    NotesRepository* globalRepPointer;
    std::vector<std::string>* filterNamesList;
    Ui::filtrationWindow *fw_ui;
};

#endif // FILTRATIONWINDOW_H
