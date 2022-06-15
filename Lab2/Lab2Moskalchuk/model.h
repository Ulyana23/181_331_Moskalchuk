#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QAbstractItemModel>


class SiteObject
{
public:
    SiteObject();
    SiteObject(const QString p_siteName,
               const QString p_login,
               const QString  p_password);

    QString getSiteName() const;
    QString getLogin() const;
    QString getPassword() const;

private:
    QString m_siteName;
    QString m_login;
    QString m_password;
};


//###########################################################################################


class Model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum enmRoles {
        siteName,
        login,
        password
    };

    explicit Model(QObject *parent = nullptr);
    void addItem(const SiteObject & newItem);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariantMap get(int idx) const;
    void clear();

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<SiteObject> sites;

};

#endif // MODEL_H
