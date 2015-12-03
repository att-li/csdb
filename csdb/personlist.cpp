#include <iostream>
#include <vector>
#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QVariant>
#include <algorithm>
#include <fstream>
#include <cstring>
#include "personlist.h"

personlist::personlist(){
    NOInList = 0;
    saved = 0;
    persondb = QSqlDatabase::addDatabase("QSQLITE");
}

personlist::~personlist(){

}

void personlist::addPerson(QString dbName, person newPerson){
    if(newPerson.getId() == -1){
        newPerson.setId(NOInList);
        writeToFile(dbName, newPerson);
    }
    else
        saved++;
    pList.push_back(newPerson);
    if(newPerson.getId() > NOInList){
        NOInList = newPerson.getId();
    }
    NOInList++;
}

void personlist::deletePerson(int index){
    vector<person> newList;
    for(int i = 0; i < NOInList; i++){
        if(pList[i].getId() != index)
            newList.push_back(pList[i]);
    }
    NOInList--;
    pList.swap(newList);
}

vector<person> personlist::getFullList()const{
    return pList;
}
//laga thetta
void personlist::editPerson(int i, person editPerson){
    if(i >= 0 && i < NOInList){
        for(int k = 0; k < NOInList; k++){
            if(pList[k].getId() == i){
                pList[k].setName(editPerson.getName());
                pList[k].setDateBirth(editPerson.getDateBirth());
                pList[k].setDateDeath(editPerson.getDateDeath());
                pList[k].setGender(editPerson.getGender());
                pList[k].setKnownFor(editPerson.getKnownFor());
                break;
            }

        }
    }
    else
        printf("Person with id %d was not found\n",i);
}

int personlist::getListSize()const{
    return NOInList;
}

bool personlist::idExists(int i){
    for(int k = 0; k < NOInList; k++){
        if(pList[k].getId() == i)
            return true;
    }
    return false;
}

void personlist::readFile(QString dbName){
    //temporary variables
    string tname, tgender, tbirth, tdeath, tknown;
    int tid;

    persondb.setDatabaseName(dbName);

    persondb.open();

    QSqlQuery query(persondb);

    query.exec("SELECT * from PersonData");

    while(query.next()){

        tid = query.value("id").toUInt();
        tname = query.value("name").toString().toStdString();
        tgender = query.value("gender").toString().toStdString();
        tbirth = query.value("dateBirth").toString().toStdString();
        tdeath = query.value("dateDeath").toString().toStdString();
        tknown = query.value("knownFor").toString().toStdString();

        addPerson(dbName, person(tid, tname, tgender, tbirth, tdeath, tknown));
    }

    persondb.close();

    /*
    ifstream data(fileName.c_str(), ios::in);

    if (data.is_open()){

        while(data >> tid >> tname >> tgender >> tbirth >> tdeath >> tknown){

        }


        data.close();
    }
    else{
        cout << strerror(errno) << endl;
    }
    */

}

void personlist::overwriteFile(QString dbName){
    /*
    ofstream data (fileName);
    saved = 0;
    for(int i = saved; i < NOInList; i++){
        data << pList[i].getData() << endl;
        saved++;
    }

    data.close();
    */
}

void personlist::writeToFile(QString dbName, person newPerson){

    persondb.setDatabaseName(dbName);

    persondb.open();

    QSqlQuery query(persondb);
    QString qname = QString::fromStdString(newPerson.getName());
    QString qgender = QString::fromStdString(newPerson.getGender());
    QString qbirth = QString::fromStdString(newPerson.getDateBirth());
    QString qdeath = QString::fromStdString(newPerson.getDateDeath());
    QString qknown= QString::fromStdString(newPerson.getKnownFor());

    query.prepare("INSERT INTO PersonData (id, name, gender, dateBirth, dateDeath, knownFor, age) "
                      "VALUES (:id, :name, :gender, :dateBirth, :dateDeath, :knownFor, :age)");
    query.bindValue(":id", newPerson.getId());
    query.bindValue(":name", qname);
    query.bindValue(":gender", qgender);
    query.bindValue(":dateBirth", qbirth);
    query.bindValue(":dateDeath", qdeath);
    query.bindValue(":knownFor", qknown);
    query.bindValue(":age", 0);
    query.exec();

    cout << newPerson.getName()<< " inserted to db" << endl;

    persondb.close();


}
