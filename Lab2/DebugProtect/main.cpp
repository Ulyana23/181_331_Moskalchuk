#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Windows.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    DWORD pid  = (DWORD) atoi(argv[1]); // получение pid процесса
    bool isAttached = DebugActiveProcess(pid); // подключиться как отладчик к нему
    if (!isAttached){ //проверка успешного подключения
        DWORD lastError = GetLastError();
        //qDebug() << lastError;
        return lastError;
    }

    DEBUG_EVENT DbgEvent;

    while(1){ //бесконечный цикл по ловле событий отладки
        WaitForDebugEvent(&DbgEvent, INFINITE);
        ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, DBG_CONTINUE);
        // сделать чтобы закрывался при закрытии менеджера паролей
    }
}
