#include "model.h"
#include <QDebug>



Model::Model(QObject *parent) : QAbstractListModel(parent)
{

}

void Model::addItem(const SiteObject &newItem)
{
    beginInsertRows(QModelIndex(),
    rowCount(), //номер строки вставки
    rowCount()); //номер строки, соответствующей концу вставляемого участка
    sites << newItem; //вставка нового элемента данных
    endInsertRows();
}

int Model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent); //скрывает сообщение о том, что параметр не используется
    return sites.count();
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[siteName] = "siteName";
    roles[login] = "login";
    roles[password] = "password";

    return roles;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || (index.row() >= sites.count()))
    return QVariant();

    const SiteObject & itemToReturn = sites[index.row()];

    if (role == siteName)
    return itemToReturn.getSiteName();
    else if (role == login)
    return itemToReturn.getLogin();
    else if (role == password)
    return itemToReturn.getPassword();

    return QVariant();
}

QVariantMap Model::get(int idx) const
{
    QVariantMap map;
    foreach(int k, roleNames().keys())
    {
    map[roleNames().value(k)] = data(index(idx, 0), k);
    }
    return map;

}

void Model::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);
    sites.clear();
    endRemoveRows();
}



SiteObject::SiteObject(const QString p_siteName,
                       const QString p_login,
                       const QString  p_password)

    :m_siteName(p_siteName), m_login(p_login), m_password(p_password)
{

}

QString SiteObject::getSiteName() const
{
    return m_siteName;
}

QString SiteObject::getLogin() const
{
    return m_login;
}

QString SiteObject::getPassword() const
{
    return m_password;
}
