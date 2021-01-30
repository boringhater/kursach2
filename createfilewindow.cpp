#include "createfilewindow.h"
#include "ui_createfilewindow.h"
#include "PiNoteLib.h"
#include <fstream>
#include "QMessageBox"
#include <string>
#include "QInputDialog"
#include <iostream>

using namespace std;


bool FileExists(std::string filePath)
{
    bool exists = false;
    std::ifstream fin(filePath.c_str(), std::ios::binary);

    if (fin.is_open())
    {
        exists = true;
    }
    fin.close();
    return exists;
}

createFileWindow::createFileWindow(NotesRepository * globalRepPointer, string *newNamePointer,bool* changingItemPtr,QWidget *parent) :
    QDialog(parent),
    cfw_ui(new Ui::createFileWindow())
{
    cfw_ui->setupUi(this);
    this->globalRepPointer = globalRepPointer;
    this->newNamePointer = newNamePointer;
    this->changingItemPtr = changingItemPtr;
    cfw_ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    vector<string> tagsVect = this->globalRepPointer->getGlobalTagsVector();
    for_each(tagsVect.begin(),tagsVect.end(),[&](string tag) -> void
    {
        cfw_ui->listWidget->addItem(QString::fromStdString(tag));
    });

    QPixmap createButtonPixmap("createIcon.png");
    QIcon createButtonIcon(createButtonPixmap);
    cfw_ui->addTagButton->setIcon(createButtonIcon);
    cfw_ui->addTagButton->setIconSize(createButtonPixmap.rect().size());
    cfw_ui->addTagButton->setStyleSheet("QPushButton {background-color: transparent;}");
}

createFileWindow::~createFileWindow()
{
    delete cfw_ui;
}


void createFileWindow::on_dialogButtonBox_accepted()  //ДОДЕЛАТЬ: ТЕГИ, ИХ БЛЯТЬ НЕТ; РЕПОЗИТОРИЙ
{
    bool createOrChangeFile = true;
    std::string strFileName = cfw_ui->nameLineEdit->text().toUtf8().constData();
    QList<QListWidgetItem *> currentSelectedItems = cfw_ui->listWidget->selectedItems();
    vector<string> tags;
    foreach(QListWidgetItem* item, currentSelectedItems)
    {
        tags.push_back(item->text().toUtf8().constData());
    }
    if(QFile::exists(QString::fromStdString(strFileName+noteExt)))
    {
        createOrChangeFile = QMessageBox::question(this,"Создать заметку", "Файл с таким названием существует. Хотите заменить его?") == QMessageBox::Yes;
        *changingItemPtr = createOrChangeFile;
    }
    if(createOrChangeFile)
    {
        globalRepPointer->createNote(strFileName,cfw_ui->authorLineEdit->text().toUtf8().constData(),tags);
        *newNamePointer = strFileName;
        accept();
    }
}

void createFileWindow::on_dialogButtonBox_rejected()
{
    reject();
}

void createFileWindow::on_createFileWindow_destroyed()
{
    reject();
}

void createFileWindow::on_addTagButton_clicked()
{
    bool bOk;
    QString tag = QInputDialog::getText(this, "Создание тега",  "Тег:", QLineEdit::Normal,  "", &bOk );
    if (bOk)
    {
        globalRepPointer->addTag(tag.toUtf8().constData());
    }
    cfw_ui->listWidget->addItem(tag);
}
