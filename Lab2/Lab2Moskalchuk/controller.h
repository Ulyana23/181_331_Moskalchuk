#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QString>
#include "model.h"
#include <string>
#include <cstring>
#include "cryptocontroller.h"
#include <QGuiApplication>
#include <QTimer>

using namespace std;

class Controller : public QObject
{
    Q_OBJECT
public:
    //Model friendsModel;
    explicit Controller(QObject *parent = nullptr);
    QString replyString;
    Model sitesModel;
    QClipboard *clipboard;
    QTimer *timer;

    //Controller();
    void setDataToListView();
    void encryptFile();
    void encryptLogPass();
    void toClipboard(const std::string &s);
    CryptoController crypto;


public slots:
    void ClearClipboard();
    void getPassword(QString password);
    void copyData(QString site, QString typeString);

signals:
    void sendAnswer(bool flag);
};

#endif // CONTROLLER_H
