#ifndef CREATEFILEWINDOW_H
#define CREATEFILEWINDOW_H

#pragma once
#include <QDialog>
#include "PiNoteLib.h"

namespace Ui {
class createFileWindow;
}

class createFileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit createFileWindow(NotesRepository * globalRepPointer, std::string *newNamePointer,bool* changingItemPtr,QWidget *parent = nullptr);
    ~createFileWindow();

private slots:
    void on_dialogButtonBox_accepted();

    void on_dialogButtonBox_rejected();

    void on_createFileWindow_destroyed();

    void on_addTagButton_clicked();

private:
    NotesRepository * globalRepPointer;
    std::string *newNamePointer;
    bool* changingItemPtr;
    Ui::createFileWindow *cfw_ui;
};

#endif // CREATEFILEWINDOW_H
