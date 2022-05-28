#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <controller.h>
#include <cryptocontroller.h>
#include <model.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <QClipboard>
#include "windows.h"
#include <QQmlContext>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QProcess>
#include <iostream>

using namespace std;
typedef unsigned long long QWORD;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    Controller controller;

    QQmlApplicationEngine engine;

    QQmlContext * context = engine.rootContext();

    context->setContextProperty("Controller", &controller); //поместить С++ объект в область видимости движка qml
    context->setContextProperty("sitesModel", &controller.sitesModel);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);




//    QProcess *satelliteProcess = new QProcess();
//    int pid = QApplication::applicationPid();
//    //qDebug() << pid;
//    QStringList arguments = {QString::number(pid)};
//    //qDebug() << arguments;
//    satelliteProcess -> start("C:/Users/User/Documents/QTProjects/build-DebugProtect-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/debug/DebugProtect.exe", arguments);
//    bool protectorStarted = satelliteProcess -> waitForStarted(1000);
//    qDebug() << protectorStarted;



    QWORD moduleBase = (QWORD)GetModuleHandle(NULL); // начальный адрес, с которого приложение разм. в вирт. памяти
    QWORD text_segment_start = moduleBase + 0x1000; // адрес сегмента .text
    //qDebug() << "text_segment_start:" << Qt::hex << text_segment_start << "\n";

    PIMAGE_DOS_HEADER pIDH = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase); // расположение и размер первого заголовка
    PIMAGE_NT_HEADERS pINH = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pIDH -> e_lfanew); // расположение и размер вторых двух заголовков
    QWORD size_of_text = pINH -> OptionalHeader.SizeOfCode; // величина .text
    //qDebug() << "size_of_text:" << size_of_text << "\n";

    QByteArray text_segment_contents = QByteArray((char*)text_segment_start, size_of_text);
    QByteArray current_hash = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);
    QByteArray current_hash_base64 = current_hash.toBase64();
    //qDebug() << "text_segment_contents:" << Qt::hex << text_segment_contents << "\n";
    qDebug() << "current_hash_base64:" << current_hash_base64 << "\n";

    const QByteArray hash0_base64 = QByteArray("FRay3jikdJsYLhJSzzDgD4T0yLKmrPKF+QhwZvnAEL0=");

//            bool checkresult = (current_hash_base64 == hash0_base64);
//            qDebug() << "checkresult:" << checkresult << "\n";
//                if (!checkresult){
//                    int result = QMessageBox::critical(nullptr, "Внимание!", "Приложение модифицировано");
//                    return -1;
//                }


//                    bool debuggerFound = IsDebuggerPresent();
//                    if(debuggerFound){
//                        qDebug() << "FOUND!!!";
//                        cout << "FOUND!!!";
//                        return -1;
//                    }

    //QObject * mainWindow = engine.rootObjects().first();

    //    CryptoController crypto(mainWindow);
    //    context->setContextProperty("crypto", &crypto);

    //    qDebug() << QString::fromStdString(crypto.passwordToHash("12345").substr(0,32));

    //    crypto.encryptString((unsigned char *) (char*) "asdrfgbv1k3wrjfhasdrfgbv1k3wrjfh");
    //    crypto.decryptString((unsigned char *) (char*) "asdrfgbv1k3wrjfhasdrfgbv1k3wrjfh");
    return app.exec();
}


