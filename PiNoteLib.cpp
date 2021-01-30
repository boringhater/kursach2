#include "PiNoteLib.h"
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <fstream>
#include <cstdio>
#include "QFile"
#include "QTextStream"
#include "QListWidgetItem"
#include <iostream>
#include "QMessageBox"
#include <string>

using namespace std;


NoteInfo::NoteInfo()
{
    author = "";
    tags = {};
}
NoteInfo::NoteInfo(std::string author, std::vector<std::string> tags)
{
    this->author = author;
    this->tags = tags;
}
std::string NoteInfo::getAuthor()
{
    return author;
}
std::vector<std::string> NoteInfo::getTags()
{
    return tags;
}


NotesRepository::NotesRepository(std::map<std::string, NoteInfo> &notesRep, std::vector<std::string> authorsVector, std::vector<std::string> tagsVector)
{
    this->notesRep = notesRep;
    this->authorsVector = authorsVector;
    this->tagsVector = tagsVector;
    this->currentUnsaved = false;
}

void NotesRepository::createNote(std::string name, std::string author, vector<string> tags)
{
    QFile file(QString::fromStdString(name+noteExt));
    file.open(QIODevice::WriteOnly);
    file.close();
    NoteInfo newNote(author,tags);
    notesRep[name] = newNote;
    if(find(authorsVector.begin(),authorsVector.end(),author) == authorsVector.end())
    {
        authorsVector.push_back(author);
    }
    for_each(tags.begin(),tags.end(),[&](string tag) -> void
    {
        if(find(tagsVector.begin(),tagsVector.end(),tag) == tagsVector.end())
        {
            tagsVector.push_back(tag);
        }
    });
}
void NotesRepository::deleteNote(std::string name)
{
    QFile::remove(QString::fromStdString(name+noteExt));
    auto oldInfo = find_if(notesRep.begin(), notesRep.end(),[name](pair<string, NoteInfo> i) -> bool {return i.first == name;});
    if(oldInfo!=notesRep.end())
    {
        notesRep.erase(oldInfo);
    }
}
void NotesRepository::editNote(std::string oldName, std::string newName, vector<string> newTags)
{
    if(newName != "")
    {
        createNote(newName,notesRep[oldName].author,newTags);
        QFile oldFile(QString::fromStdString(oldName+noteExt));
        QTextStream in(&oldFile);
        QString text = in.readAll();
        oldFile.close();
        QFile newFile(QString::fromStdString(newName+noteExt));
        newFile.open(QIODevice::WriteOnly);
        newFile.write(text.toUtf8().constData());
        deleteNote(oldName);
    }
    else
    {
        notesRep[oldName].tags = newTags;
    }
}
void NotesRepository::saveNote(std::string name, QString allText)
{
    QFile file(QString::fromStdString(name+noteExt));
    file.open(QIODevice::WriteOnly);
    file.write(allText.toUtf8().constData());
    file.close();
    currentUnsaved = false;
}

std::string NotesRepository::getAuthor(std::string name)
{
    return notesRep[name].author;
}
std::vector<std::string> NotesRepository::getTags(std::string name)
{
    return notesRep[name].tags;
}

void NotesRepository::addTag(std::string tag)
{
    if(find(tagsVector.begin(),tagsVector.end(),tag) == tagsVector.end())
    {
        tagsVector.push_back(tag);
    }
}
void NotesRepository::addAuthor(std::string author)
{
    if(find(authorsVector.begin(),authorsVector.end(),author) == authorsVector.end())
    {
        authorsVector.push_back(author);
    }
}
std::vector<std::string> NotesRepository::getGlobalTagsVector()
{
    return tagsVector;
}
std::vector<std::string> NotesRepository::getGlobalAuthorsVector()
{
    return authorsVector;
}


void NotesRepository::saveRepositoryToFile()
{
    QFile file(QString::fromStdString(mainRepositoryName));
    file.open(QFile::WriteOnly|QFile::Truncate);
    file.close();
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream writeStream(&file);
        writeStream<<QString::fromStdString(notesInfoFlag)<<Qt::endl;
        QTextStream* writeStreamPtr = &writeStream;
        for_each(notesRep.begin(),notesRep.end(), [writeStreamPtr](pair<string,NoteInfo> i) -> void
        {
            *writeStreamPtr<<QString::fromStdString(i.first)<<Qt::endl<<
                             QString::fromStdString(i.second.author)<<Qt::endl<<
                             "tags:"<<Qt::endl;
            for_each(i.second.tags.begin(),i.second.tags.end(), [writeStreamPtr](string tag) -> void
            {
                *writeStreamPtr<<QString::fromStdString(tag)<<Qt::endl;
            });
            *writeStreamPtr<<Qt::endl;
        });
        writeStream<<QString::fromStdString(authorsVectorFlag)<<Qt::endl;
        for_each(authorsVector.begin(),authorsVector.end(), [writeStreamPtr](string author) -> void
        {
            *writeStreamPtr<<QString::fromStdString(author)<<Qt::endl;
        });
        writeStream<<QString::fromStdString(tagsVectorFlag)<<Qt::endl;
        for_each(tagsVector.begin(),tagsVector.end(), [writeStreamPtr](string tag) -> void
        {
            *writeStreamPtr<<QString::fromStdString(tag)<<Qt::endl;
        });
        file.close();
    }

}
std::map<std::string,NoteInfo> NotesRepository::getNotesRep()
{
    return notesRep;
}

NotesRepository NotesRepository::constructRepositoryFromFile()
{
    map<string,NoteInfo> retNoteRep;
    vector<string> retAuthorsRep;
    vector<string> retTagsRep;
    QFile file(QString::fromStdString(mainRepositoryName));
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream readStream(&file);
        QString line = "";
        QString currentName = "";
        QString currentAuthor = "";
        vector<string> currentTags;
        line = readStream.readLine();
        while (!readStream.atEnd())
        {
            line = readStream.readLine();
            if(line.toUtf8().constData()[0] == '[')
            {
                break;
            }
            currentName = line;
            line = readStream.readLine();
            currentAuthor = line;
            vector<string> currentTags;
            line = readStream.readLine();
            while(!readStream.atEnd())
            {
                line = readStream.readLine();
                if(line == "")
                {
                    break;
                }
                currentTags.push_back(line.toUtf8().constData());
            }
            retNoteRep[currentName.toUtf8().constData()] = NoteInfo(currentAuthor.toUtf8().constData(), currentTags);
        }
        while (!readStream.atEnd())
        {
            line = readStream.readLine();
            if(line.toUtf8().constData()[0] == '[')
            {
                break;
            }
            retAuthorsRep.push_back(line.toUtf8().constData());
        }
        while(!readStream.atEnd())
        {
            line = readStream.readLine();
            retTagsRep.push_back(line.toUtf8().constData());
        }
        file.close();
    }
    return NotesRepository(retNoteRep,retAuthorsRep,retTagsRep);
}
