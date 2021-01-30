#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"
#include "QMessageBox"
#include "QFile"
#include "QTextStream"
#include "QListWidgetItem"
#include "QPixmap"
#include "QToolButton"
#include "QListWidget"
#include "QListWidgetItem"
#include "QInputDialog"
#include "PiNoteLib.h"
#include <fstream>
#include <cstdio>
#include "editnotewindow.h"
#include "QFileInfo"
#include "QDateTime"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "filtrationwindow.h"

using namespace std;

MainWindow::MainWindow(NotesRepository *globalRepPointer,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->globalRepPointer = globalRepPointer;
    ui->setupUi(this);
    setWindowTitle("Kursach");
    QPalette darkPalette = palette();

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Background, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(150, 200, 238));
    darkPalette.setColor(QPalette::Highlight, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);

    //Иконки кнопок
    QPixmap createButtonPixmap("createIcon.png");
    QIcon createButtonIcon(createButtonPixmap);
    ui->createButton->setIcon(createButtonIcon);
    ui->createButton->setIconSize(createButtonPixmap.rect().size());

    QPixmap deleteButtonPixmap("deleteIcon.png");
    QIcon deleteButtonIcon(deleteButtonPixmap);
    ui->deleteButton->setIcon(deleteButtonIcon);
    ui->deleteButton->setIconSize(deleteButtonPixmap.rect().size());

    QPixmap saveButtonPixmap("saveIcon.png");
    QIcon saveButtonIcon(saveButtonPixmap);
    ui->saveButton->setIcon(saveButtonIcon);
    ui->saveButton->setIconSize(saveButtonPixmap.rect().size());

    QPixmap editButtonPixmap("editIcon.png");
    QIcon editIcon(editButtonPixmap);
    ui->editNoteButton->setIcon(editIcon);
    ui->editNoteButton->setIconSize(editButtonPixmap.rect().size());

    QPixmap filterButtonPixmap("filterIcon.png");
    QIcon filterButtonIcon(filterButtonPixmap);
    ui->filterButton->setIcon(filterButtonIcon);
    ui->filterButton->setIconSize(filterButtonPixmap.rect().size());

    QPixmap noFiltrationButtonPixmap("noFilterIcon.png");
    QIcon noFiltrationButtonIcon(noFiltrationButtonPixmap);
    ui->noFiltrationButton->setIcon(noFiltrationButtonIcon);
    ui->noFiltrationButton->setIconSize(noFiltrationButtonPixmap.rect().size());

    QPixmap infoButtonPixmap("infoIcon.png");
    QIcon infoButtonIcon(infoButtonPixmap);
    ui->infoButton->setIcon(infoButtonIcon);
    ui->infoButton->setIconSize(infoButtonPixmap.rect().size());

    ui->createButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->deleteButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->saveButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->editNoteButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->filterButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->noFiltrationButton->setStyleSheet("QPushButton {background-color: transparent;}");
    ui->infoButton->setStyleSheet("QPushButton {background-color: transparent;}");

    ui->listWidget->setMinimumWidth(createButtonPixmap.rect().size().width()+deleteButtonPixmap.rect().size().width());
    ui->listWidget->setMaximumWidth((createButtonPixmap.rect().size().width()+deleteButtonPixmap.rect().size().width())*3);
    QListWidget* listWidgetPtr = ui->listWidget;
    map<string, NoteInfo> currentRep =  globalRepPointer->getNotesRep();
    for_each(currentRep.begin(),currentRep.end(), [listWidgetPtr](pair<string, NoteInfo> note) -> void
             {
                 listWidgetPtr->addItem(QString::fromStdString(note.first));
             });
    ui->filtrationLabel->setText("Filtration: off");
    ui->plainTextEdit->setFont(QFont("Montseratt Light", 12));
}

MainWindow::~MainWindow()
{
    if(ui->listWidget->currentItem() == nullptr)
    {
        globalRepPointer->currentUnsaved = false;
    }
    if(globalRepPointer->currentUnsaved)
    {
        if(QMessageBox::question(this, "Сохранить изменения", "Сохранить изменения в текущем файле?") == QMessageBox::Yes)
        {
            globalRepPointer->saveNote(ui->listWidget->currentItem()->text().toUtf8().constData(), ui->plainTextEdit->toPlainText());
        }
    }
    globalRepPointer->saveRepositoryToFile();
    delete ui;
}

void MainWindow::on_createButton_clicked()
{
    string newName;
    string *newNamePointer = &newName;
    bool changingItem = false;
    bool* changingItemPtr = &changingItem;
    createFileWindow cfw(globalRepPointer, newNamePointer,changingItemPtr);
    cfw.setWindowTitle("Создать заметку");
    cfw.setModal(true);
    if(cfw.exec() == QDialog::Accepted&&!changingItem)
    {
        globalRepPointer->currentUnsaved = false;
        ui->listWidget->addItem(QString::fromStdString(newName));
    }

}
void MainWindow::on_deleteButton_clicked()
{

    if(QMessageBox::question(this,"Удаление файла", "Уверены, что хотите удалить "+ui->listWidget->currentItem()->text().toUtf8()+"?") == QMessageBox::Yes)
    {
        QString currentName = ui->listWidget->currentItem()->text();
        globalRepPointer->currentUnsaved = false;
        globalRepPointer->deleteNote(currentName.toUtf8().constData());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
}

void MainWindow::on_filterButton_clicked()
{
    vector<string> filterNames;
    vector<string>* filterNamesPtr = &filterNames;
    filtrationWindow fw(globalRepPointer, filterNamesPtr);
    fw.setWindowTitle("Поиск");
    fw.setModal(true);
    QListWidget* listWidgetPtr = ui->listWidget;
    if(fw.exec() == QDialog::Accepted)
    {
        ui->listWidget->clear();
        for_each(filterNames.begin(),filterNames.end(), [listWidgetPtr](string note) -> void
                 {
                     listWidgetPtr->addItem(QString::fromStdString(note));
                 });
        ui->filtrationLabel->setText("Filtration: on");
        globalRepPointer->currentUnsaved = false;
    }
}

void MainWindow::on_noFiltrationButton_clicked()
{
    ui->listWidget->clear();
    QListWidget* listWidgetPtr = ui->listWidget;
    map<string, NoteInfo> currentRep =  globalRepPointer->getNotesRep();
    for_each(currentRep.begin(),currentRep.end(), [listWidgetPtr](pair<string, NoteInfo> note) -> void
             {
                 listWidgetPtr->addItem(QString::fromStdString(note.first));
             });
    ui->filtrationLabel->setText("Filtration: off");
    globalRepPointer->currentUnsaved = false;
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(ui->listWidget->currentItem() != nullptr)
    {
        if(globalRepPointer->currentUnsaved)
        {
            if(QMessageBox::question(this, "Сохранить изменения", "Сохранить изменения в текущем файле?") == QMessageBox::Yes)
            {
                globalRepPointer->saveNote(previous->text().toUtf8().constData(), ui->plainTextEdit->toPlainText());
            }
        }
        globalRepPointer->currentUnsaved = false;
        ui->plainTextEdit->setReadOnly(false);
        ui->plainTextEdit->clear();

        QString fileName = QString::fromStdString(ui->listWidget->currentItem()->text().toUtf8().constData() + noteExt);
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Ошибка", "Невозможно открыть файл : " + file.errorString());
            QString currentName = ui->listWidget->currentItem()->text();
            globalRepPointer->currentUnsaved = false;
            globalRepPointer->deleteNote(currentName.toUtf8().constData());
            delete ui->listWidget->takeItem(ui->listWidget->currentRow());
            return;
        }
        QTextStream in(&file);
        QString text = in.readAll();
        ui->plainTextEdit->insertPlainText(text);
        file.close();
        ui->nameLabel->setText(current->text());
        globalRepPointer->currentUnsaved = false;

    }
    else
    {
        ui->nameLabel->setText("");
        ui->plainTextEdit->setPlainText("");
        ui->plainTextEdit->setReadOnly(true);
    }
}

void MainWindow::on_saveButton_clicked()
{
    if(ui->listWidget->currentItem() != nullptr)
    {
        globalRepPointer->saveNote(ui->listWidget->currentItem()->text().toUtf8().constData(), ui->plainTextEdit->toPlainText());
    }
}

void MainWindow::on_plainTextEdit_textChanged()
{
    globalRepPointer->currentUnsaved = true;
}

void MainWindow::on_editNoteButton_clicked()
{
    if(ui->listWidget->currentItem()!=nullptr)
    {
        QString currentName = ui->listWidget->currentItem()->text();
        QString* currentNamePtr = &currentName;
        editNoteWindow enw(globalRepPointer, currentNamePtr);
        enw.setWindowTitle("Редактировать заметку");
        enw.setModal(true);
        if(enw.exec() == QDialog::Accepted &&currentName != "")
        {
            ui->listWidget->currentItem()->setText(currentName);
            ui->nameLabel->setText(currentName);
            globalRepPointer->saveNote(ui->listWidget->currentItem()->text().toUtf8().constData(), ui->plainTextEdit->toPlainText());
        }
    }
}

void MainWindow::on_infoButton_clicked()
{
    if(ui->listWidget->currentItem()!=nullptr)
    {
        QFileInfo fileInfo(QString::fromStdString(ui->listWidget->currentItem()->text().toUtf8().constData()+noteExt));
        vector<string> currentTags = globalRepPointer->getTags(ui->listWidget->currentItem()->text().toUtf8().constData());
        string delimiter = ", ";
        stringstream  s;
        copy(currentTags.begin(),currentTags.end(), ostream_iterator<string>(s,delimiter.c_str()));
        QMessageBox::information(this,ui->listWidget->currentItem()->text(),
                                 "Название: "+ui->listWidget->currentItem()->text()+
                                 "\nАвтор: "+ QString::fromStdString(globalRepPointer->getAuthor(ui->listWidget->currentItem()->text().toUtf8().constData()))+
                                 "\nТеги: "+ QString::fromStdString(s.str())+
                                 "\nДата создания: "+fileInfo.birthTime().toString()+
                                 "\nДата последнего изменения: "+fileInfo.lastModified().toString());
    }

}
