#include "editnotewindow.h"
#include "ui_editnotewindow.h"
#include "QInputDialog"

using namespace std;

editNoteWindow::editNoteWindow(NotesRepository* globalRepPointer, QString* currentName, QWidget *parent) :
    QDialog(parent),
    enw_ui(new Ui::editNoteWindow)
{
    enw_ui->setupUi(this);
    this->globalRepPointer = globalRepPointer;
    this->currentName = currentName;
    QPixmap createButtonPixmap("createIcon.png");
    QIcon createButtonIcon(createButtonPixmap);
    enw_ui->addTagButton->setIcon(createButtonIcon);
    enw_ui->addTagButton->setIconSize(createButtonPixmap.rect().size());
    enw_ui->addTagButton->setStyleSheet("QPushButton {background-color: transparent;}");
    enw_ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    QListWidget* listWidgetRep = enw_ui->listWidget;
    vector<string> currentRep = globalRepPointer->getGlobalTagsVector();
    for_each(currentRep.begin(),currentRep.end(),[listWidgetRep](string tag) -> void
             {
                 listWidgetRep->addItem(QString::fromStdString(tag));
             });
}

editNoteWindow::~editNoteWindow()
{
    delete enw_ui;
}

void editNoteWindow::on_addTagButton_clicked()
{
    bool bOk;
    QString tag = QInputDialog::getText(this, "Создание тега",  "Тег:", QLineEdit::Normal,  "", &bOk );
    if (bOk)
    {
        globalRepPointer->addTag(tag.toUtf8().constData());
    }
    enw_ui->listWidget->addItem(tag);
}

void editNoteWindow::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> currentSelectedItems = enw_ui->listWidget->selectedItems();
    vector<string> tags;
    foreach(QListWidgetItem* item, currentSelectedItems)
    {
        tags.push_back(item->text().toUtf8().constData());
    }
    globalRepPointer->editNote(currentName->toUtf8().constData(),enw_ui->newNameLineEdit->text().toUtf8().constData(),tags);
    *currentName = enw_ui->newNameLineEdit->text();
    accept();
}

void editNoteWindow::on_buttonBox_rejected()
{
    reject();
}
