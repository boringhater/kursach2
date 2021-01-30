#include "filtrationwindow.h"
#include "ui_filtrationwindow.h"
#include "PiNoteLib.h"
#include "QFile"
#include "QFileInfo"
#include "QDateTime"


using namespace std;

bool changedToday(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    return(fileInfo.lastModified().date().day() == QDateTime::currentDateTime().date().day()
           && fileInfo.lastModified().date().month() == QDateTime::currentDateTime().date().month()
           && fileInfo.lastModified().date().year() == QDateTime::currentDateTime().date().year());
}
bool changedYesterday(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime yesterday = QDateTime::currentDateTime().addDays(-1);
    return(fileInfo.lastModified().date().day() == yesterday.date().day()
           && fileInfo.lastModified().date().month() == yesterday.date().month()
           && fileInfo.lastModified().date().year() == yesterday.date().year());
}
bool changedLastWeek(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime weekBefore = QDateTime::currentDateTime().addDays(-7);
    return(fileInfo.lastModified()>=weekBefore);
}
bool changedLastMonth(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime monthBefore = QDateTime::currentDateTime().addMonths(-1);
    return(fileInfo.lastModified()>=monthBefore);
}

bool createdToday(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    return(fileInfo.birthTime().date().day() == QDateTime::currentDateTime().date().day()
           && fileInfo.birthTime().date().month() == QDateTime::currentDateTime().date().month()
           && fileInfo.birthTime().date().year() == QDateTime::currentDateTime().date().year());
}
bool createdYesterday(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime yesterday = QDateTime::currentDateTime().addDays(-1);
    return(fileInfo.birthTime().date().day() == yesterday.date().day()
           && fileInfo.birthTime().date().month() == yesterday.date().month()
           && fileInfo.birthTime().date().year() == yesterday.date().year());
}
bool createdLastWeek(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime weekBefore = QDateTime::currentDateTime().addDays(-7);
    return(fileInfo.birthTime()>=weekBefore);
}
bool createdLastMonth(string filename)
{
    QFileInfo fileInfo(QString::fromStdString(filename+noteExt));
    QDateTime monthBefore = QDateTime::currentDateTime().addMonths(-1);
    return(fileInfo.birthTime()>=monthBefore);
}


bool containsAllTags(vector<string>* containerTagVectorPtr, vector<string> searchTagsVector)
{
    bool contains = true;
    bool* containsPtr = &contains;
    for_each(searchTagsVector.begin(),searchTagsVector.end(),[containerTagVectorPtr,containsPtr](string tag)->void
    {
        if(find(containerTagVectorPtr->begin(),containerTagVectorPtr->end(),tag) == containerTagVectorPtr->end())
        {
            *containsPtr = false;
        }
    });
    return contains;
}


filtrationWindow::filtrationWindow(NotesRepository* globalRepPointer, vector<string>* filterNamesList,QWidget *parent) :
    QDialog(parent),
    fw_ui(new Ui::filtrationWindow)
{
    fw_ui->setupUi(this);
    this->globalRepPointer = globalRepPointer;
    this->filterNamesList = filterNamesList;
    fw_ui->tagsListWidget->setSelectionMode(QAbstractItemView::MultiSelection);


    vector<string> currentAuthorsRep = globalRepPointer->getGlobalAuthorsVector();
    QListWidget* AuthorsListWidgetPtr = fw_ui->authorsListWidget;
    for_each(currentAuthorsRep.begin(),currentAuthorsRep.end(),[AuthorsListWidgetPtr](string author) -> void
    {
        AuthorsListWidgetPtr->addItem(QString::fromStdString(author));
    });


    vector<string> currentTagsRep = globalRepPointer->getGlobalTagsVector();
    QListWidget* tagsListWidgetPtr = fw_ui->tagsListWidget;
    for_each(currentTagsRep.begin(),currentTagsRep.end(),[tagsListWidgetPtr](string tag) -> void
    {
        tagsListWidgetPtr->addItem(QString::fromStdString(tag));
    });
}

filtrationWindow::~filtrationWindow()
{
    delete fw_ui;
}

void filtrationWindow::on_buttonBox_accepted()
{
    globalRepPointer->currentUnsaved = false;
    map<string,NoteInfo> currentRep = globalRepPointer->getNotesRep();
    vector<string>* filterListPtr = filterNamesList;
    QList<QListWidgetItem*> selectedTagsItems = fw_ui->tagsListWidget->selectedItems();

    vector<string> selectedTags;
    vector<string>* stPtr = &selectedTags;
    for_each(selectedTagsItems.begin(),selectedTagsItems.end(), [stPtr](QListWidgetItem* item) -> void
    {
        stPtr->push_back(item->text().toUtf8().constData());
    });

    string selectedAuthor = "";
    if (fw_ui->authorsListWidget->currentItem() != nullptr)
    {
        selectedAuthor = fw_ui->authorsListWidget->currentItem()->text().toUtf8().constData();
    }
    string searchName = fw_ui->nameSearchLineEdit->text().toUtf8().constData();
    QAbstractButton* checkedCreatedButton = fw_ui->createdButtonGroup->checkedButton();
    QAbstractButton* checkedChangedButton = fw_ui->changedButtonGroup->checkedButton();
    for_each(currentRep.begin(),currentRep.end(),[filterListPtr,searchName,selectedAuthor,selectedTags,checkedCreatedButton,checkedChangedButton](pair<string, NoteInfo> note) -> void
    {
        vector<string> tagsVect = note.second.getTags();
        vector<string>* tagsVectPtr = &tagsVect;
        if((searchName == ""  || note.first.find(searchName) != std::string::npos)      //поисковый if-овый монстр
                && (selectedAuthor == "" || note.second.getAuthor() == selectedAuthor)  //чек автора
                && (selectedTags.empty() || containsAllTags(tagsVectPtr,selectedTags))  //чек теги
                && (checkedCreatedButton == nullptr ||                                  //чек дату создания
                    (checkedCreatedButton->text() == "Сегодня" && createdToday(note.first))||
                    (checkedCreatedButton->text() == "Вчера" && createdYesterday(note.first))||
                    (checkedCreatedButton->text() == "За последнюю неделю" && createdLastWeek(note.first))||
                    (checkedCreatedButton->text() == "За последний месяц" && createdLastMonth(note.first))
                    )
                && ((checkedChangedButton == nullptr ||                                 //чек дату изменения
                     (checkedChangedButton->text() == "Сегодня" && changedToday(note.first))||
                     (checkedChangedButton->text() == "Вчера" && changedYesterday(note.first))||
                     (checkedChangedButton->text() == "За последнюю неделю" && changedLastWeek(note.first))||
                     (checkedChangedButton->text() == "За последний месяц" && changedLastMonth(note.first))
                     )
                    ))
        {
            filterListPtr->push_back(note.first);
        }
    });
    accept();
}

void filtrationWindow::on_buttonBox_rejected()
{
    reject();
}
