#include "controller.h"
#include <conio.h>
#include <stdio.h>
#include <cstdlib>

#include <QFile>
#include <QCoreApplication>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QClipboard>
#include <windows.h>



Controller::Controller(QObject *parent) : QObject(parent)
{
    //setDataToListView();

    //encryptLogPass();
    //encryptFile();
    clipboard = QGuiApplication::clipboard();
    //QString originalText = clipboard->text();

}


void Controller::setDataToListView() {
    if (sitesModel.rowCount() > 0) {
        sitesModel.clear();
    }

//    QFile data("C:/Users/User/Documents/QTProjects/Lab2Moskalchuk/data.json");

//    if (data.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        replyString = data.readAll();
//    }

    //qDebug() << replyString;

    crypto.decryptFile(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
    replyString = QString::fromStdString(crypto.getFileText());

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyString.toUtf8());

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray jsonArray = jsonObject["siteNames"].toArray();

    QJsonObject jsonObjectSitesData = jsonObject["sitesData"].toObject();


    foreach(QJsonValue jsonValueSiteNume, jsonArray){
        QString siteNume = jsonValueSiteNume.toString();
        QJsonObject jsonObjectCurrentSite = jsonObjectSitesData[jsonValueSiteNume.toString()].toObject();
        QString login = jsonObjectCurrentSite.value("login").toString();
        QString password = jsonObjectCurrentSite.value("password").toString();

        sitesModel.addItem(SiteObject(siteNume, login, password));
    }

    //data.close();
}


void Controller::encryptFile() {
    QFile data("C:/Users/User/Documents/QTProjects/Lab2Moskalchuk/data.json");
    QString jsonText;

    if (data.open(QIODevice::ReadOnly | QIODevice::Text)) {
        jsonText = data.readAll();
    }

    data.close();

    //qDebug() << jsonText;
    //crypto.strText = jsonText.toStdString();
    //string str = jsonText.toStdString();
    //crypto.setStrText(str);

    crypto.fileText = jsonText.toStdString();
    //crypto.encryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(keyHash).toLatin1().data()));

    crypto.encryptFile(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
//    crypto.decryptFile(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
//    QString::fromStdString(crypto.getFileText());

}


void Controller::encryptLogPass() {
    QFile data("C:/Users/User/Documents/QTProjects/Lab2Moskalchuk/data.json");

    if (data.open(QIODevice::ReadOnly | QIODevice::Text)) {
        replyString = data.readAll();
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyString.toUtf8());
    //qDebug() << jsonDoc;

    QJsonObject jsonObject = jsonDoc.object();

    QJsonArray jsonArray = jsonObject["siteNames"].toArray();

    QJsonObject jsonObjectSitesData = jsonObject["sitesData"].toObject();


    foreach(QJsonValue jsonValueSiteNume, jsonArray){
        QString siteNume = jsonValueSiteNume.toString();
        QJsonObject jsonObjectCurrentSite = jsonObjectSitesData[jsonValueSiteNume.toString()].toObject();
        QString login = jsonObjectCurrentSite.value("login").toString();
        QString password = jsonObjectCurrentSite.value("password").toString();

        crypto.setStrText(login.toLocal8Bit().constData());
        crypto.encryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
        jsonObjectCurrentSite["login"] = QString::fromLocal8Bit(crypto.getStrText().c_str());

        crypto.setStrText(password.toLocal8Bit().constData());
        crypto.encryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
        jsonObjectCurrentSite["password"] =  QString::fromLocal8Bit(crypto.getStrText().c_str());

//        QString::fromLocal8Bit(crypto.getStrText().c_str())
//        password.toLocal8Bit().constData()

        //crypto.setStrText(jsonObjectCurrentSite.value("password").toString().toLocal8Bit().constData());
        //crypto.setStrText( QString::fromStdString(crypto.getStrText()).toLatin1().data());
        //crypto.decryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));

        jsonObjectSitesData[jsonValueSiteNume.toString()] = jsonObjectCurrentSite;
    }

    jsonObject["sitesData"] = jsonObjectSitesData;

    jsonDoc.setObject(jsonObject);
//    qDebug() << "jsonDoc";
    qDebug() << jsonDoc;

    data.close();

    if (data.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        data.write(jsonDoc.toJson());
        data.close();
    }
}

void Controller::getPassword(QString password) {
    //qDebug() << password;
//                        bool debuggerFound = IsDebuggerPresent();
//                        if(debuggerFound){
//                            qDebug() << "FOUND!!!";
//                            cout << "FOUND!!!";
//                            //return -1;
//                        }

    crypto.setKeyHash(crypto.passwordToHash(password.toStdString()).substr(0,32));

    crypto.decryptFile(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
    QString text = QString::fromStdString(crypto.getFileText());


    if (text[0] == "{") {
        setDataToListView();
        emit sendAnswer(true);
    }

    else {
        emit sendAnswer(false);
    }
}



void Controller::copyData(QString site, QString typeString) {
    qDebug() << site;
    replyString = QString::fromStdString(crypto.getFileText());

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyString.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    QJsonObject jsonObjectSitesData = jsonObject["sitesData"].toObject();

    QJsonObject jsonObjectCurrentSite = jsonObjectSitesData[site].toObject();

    QString encrypt;
    if (typeString == "password")
    encrypt = jsonObjectCurrentSite.value("password").toString();

    else if (typeString == "login")
    encrypt = jsonObjectCurrentSite.value("login").toString();

    crypto.setStrText(encrypt.toLocal8Bit().constData());
    //crypto.encryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
    crypto.decryptString(reinterpret_cast<unsigned char *>(QString::fromStdString(crypto.getKeyHash()).toLatin1().data()));
    qDebug() << QString::fromStdString(crypto.getStrText());
    clipboard->setText(QString::fromStdString(crypto.getStrText()));
}


//void Controller::toClipboard(const std::string &s) {
//    OpenClipboard(0);
//    EmptyClipboard();
//    HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE, s.size()+1);
//    if (!hg){
//        CloseClipboard();
//        return;
//    }
//    memcpy(GlobalLock(hg),s.c_str(), s.size()+1);
//    GlobalUnlock(hg);
//    SetClipboardData(CF_TEXT,hg);
//    CloseClipboard();
//    GlobalFree(hg);
//}


