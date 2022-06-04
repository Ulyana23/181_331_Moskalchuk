#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDebug>
#include "windows.h"
#include "fltuser.h"
#include <QMessageBox>
#include <system_error>
#include <string>


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    HRESULT hResult;
    DWORD EnablePrivilege(TCHAR * privilegeStr);
    LPCWSTR driverName;
    BOOL SetPrivilege(
        HANDLE hToken,          // access token handle
        LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
        BOOL bEnablePrivilege   // to enable or disable privilege
        );


public slots:
    void loadDriver();
    void unloadDriver();
};

#endif // CONTROLLER_H
