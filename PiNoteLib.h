#ifndef PINOTELIB_H
#define PINOTELIB_H

#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <string>
#include "QFile"
#include "QTextStream"
#include "QListWidgetItem"

bool FileExists(std::string filePath);
static const std::string mainRepositoryName = "mainRep.txt";
static const std::string noteExt = ".txt";
static const std::string notesInfoFlag = "[NOTES]";
static const std::string authorsVectorFlag = "[AUTHORS]";
static const std::string tagsVectorFlag = "[TAGS]";
class NoteInfo
{
private:
    std::string author;
    std::vector<std::string> tags;
public:
    NoteInfo();
    friend class NotesRepository;
    NoteInfo(std::string author, std::vector<std::string> tags);
public:
    std::string getAuthor();
    std::vector<std::string> getTags();
};
class NotesRepository
{
    std::map<std::string, NoteInfo> notesRep;
    std::vector<std::string> authorsVector;
    std::vector<std::string> tagsVector;

    NotesRepository();
public:
    bool currentUnsaved;
    std::map<std::string,NoteInfo> getNotesRep();
    std::string getAuthor(std::string name);
    std::vector<std::string> getTags(std::string name);
    std::vector<std::string> getGlobalTagsVector();
    std::vector<std::string> getGlobalAuthorsVector();
    void createNote(std::string name, std::string author, std::vector<std::string> tags);
    void deleteNote(std::string name);
    void editNote(std::string oldName, std::string newName, std::vector<std::string> newTags);
    void saveNote(std::string name, QString allText);
    void addTag(std::string tag);
    void addAuthor(std::string author);
    void saveRepositoryToFile();
    static NotesRepository constructRepositoryFromFile();
    NotesRepository(std::map<std::string, NoteInfo> &notesRep, std::vector<std::string> authorsVector, std::vector<std::string> tagsVector);
};


#endif // PINOTELIB_H
