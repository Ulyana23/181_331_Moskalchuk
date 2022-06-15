#ifndef CRYPTOCONTROLLER_H
#define CRYPTOCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QCryptographicHash>
#include <QByteArray>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <QFile>
#include <string>
#include <cstring>
#include <QBuffer>
#include <iostream>
#include <cstdlib>

using namespace std;

class CryptoController : public QObject {
    Q_OBJECT
public:
    explicit CryptoController(QObject *parent = nullptr);
    string passwordToHash(string password);
    bool encryptString(unsigned char* _key);
    bool decryptString(unsigned char* _key);
    bool encryptFile(unsigned char* _key);
    bool decryptFile(unsigned char* _key);
    void setStrText(string str);
    void setKeyHash(string _keyHash);
    string getStrText();
    string getKeyHash();
    string getFileText();
    QString file = "C:/Users/User/Documents/QTProjects/Lab2Moskalchuk/data.json";
    string strText = "";
    string fileText = "";

private:
    unsigned char * _iv = (unsigned char *)("12345678901234567890123456789090");
    string keyHash;

};

#endif // CRYPTOCONTROLLER_H
