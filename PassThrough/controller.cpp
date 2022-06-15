#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    //BOOL isOK;
    HANDLE hToken;
    HANDLE hCurrentProcess;
    hCurrentProcess = GetCurrentProcess(); // 1
    OpenProcessToken(hCurrentProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    SetPrivilege(hToken, SE_LOAD_DRIVER_NAME, TRUE);

    driverName = L"passThrough";
}

BOOL Controller::SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue(
            NULL,            // lookup privilege on local system
            lpszPrivilege,   // privilege to lookup
            &luid ) )        // receives LUID of privilege
    {
        //printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
           hToken,
           FALSE,
           &tp,
           sizeof(TOKEN_PRIVILEGES),
           (PTOKEN_PRIVILEGES) NULL,
           (PDWORD) NULL) )
    {
          //printf("AdjustTokenPrivileges error: %u\n", GetLastError() );
          return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
          //printf("The token does not have the specified privilege. \n");
          return FALSE;
    }

    return TRUE;
}

void Controller::loadDriver()
{
    hResult = FilterLoad(driverName);

    if (!IS_ERROR(hResult)) {
        qDebug() << "загружен";
        QMessageBox::information(nullptr, "Ура!", "Драйвер загружен!");
    }

    else {
        std::string message = std::system_category().message(hResult);
        QString error = QString::fromLocal8Bit(message.c_str());
        qDebug() << error;

        QMessageBox::critical(nullptr, "Внимание!", "Ошибка: " + error);
    }

}

void Controller::unloadDriver()
{
    hResult = FilterUnload(driverName);
    if (!IS_ERROR(hResult)) {
        qDebug() << "выгружен";
        QMessageBox::information(nullptr, "Ура!", "Драйвер выгружен!");
    }

    else {
        std::string message = std::system_category().message(hResult);
        QString error = QString::fromLocal8Bit(message.c_str());
        qDebug() << error;

        QMessageBox::critical(nullptr, "Внимание!", "Ошибка: " + error);
    }
}


