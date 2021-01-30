#ifndef EDITNOTEWINDOW_H
#define EDITNOTEWINDOW_H

#include <QDialog>
#include "PiNoteLib.h"

namespace Ui {
class editNoteWindow;
}

class editNoteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit editNoteWindow(NotesRepository* globalRepPointer, QString* currentName, QWidget *parent = nullptr);
    ~editNoteWindow();

private slots:
    void on_addTagButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::editNoteWindow *enw_ui;
    NotesRepository* globalRepPointer;
    QString* currentName;
};

#endif // EDITNOTEWINDOW_H
