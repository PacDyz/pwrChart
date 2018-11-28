#ifndef LISTWITHVALUES_H
#define LISTWITHVALUES_H
#include <QtCore>
#include <QtGui>

struct A
{
    QString time;
    QString averageAmplitude;
};

class ListWithValues : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum RoleNames {
        TimeRole = Qt::UserRole,
        AverageAmplitudeRole = Qt::UserRole+1,
    };
    explicit ListWithValues(QObject *parent = 0);
    ~ListWithValues();

public: // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE void append(const A& a);
    Q_INVOKABLE void insert(int index, const A& A);
protected:
    virtual QHash<int,QByteArray>  roleNames() const override;
private:
    QList<A> m_data;
    QHash<int, QByteArray> m_roleNames;
};

#endif // LISTWITHVALUES_H
