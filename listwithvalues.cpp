#include "listwithvalues.h"

ListWithValues::ListWithValues(QObject *parent)
    : QAbstractListModel(parent)
{
    // initialize our data (QList<QString>) with a list of color names
    m_roleNames[TimeRole] = "time";
    m_roleNames[AverageAmplitudeRole] = "averageAmplitude";
}

ListWithValues::~ListWithValues()
{
}

int ListWithValues::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // return our data count
    return m_data.count();
}

QHash<int,QByteArray>  ListWithValues::roleNames() const
{
    return m_roleNames;
}

QVariant ListWithValues::data(const QModelIndex &index, int role) const
{
    // the index returns the requested row and column information.
    // we ignore the column and only use the row information
    int row = index.row();

    // boundary check for the row
    if(row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    // A model can return data for different roles.
    // The default role is the display role.
    // it can be accesses in QML with "model.display"
    switch(role) {
        case TimeRole:
            // Return the color name for the particular row
            // Qt automatically converts it to the QVariant type
            return m_data.value(row).time;
        case AverageAmplitudeRole:
            return m_data.value(row).averageAmplitude;
    }

    // The view asked for other data, just return an empty QVariant
    return QVariant();
}

void ListWithValues::insert(int index, const A &a)
{
    if(index < 0 || index > m_data.count()) {
        return;
    }

    // view protocol (begin => manipulate => end]
    emit beginInsertRows(QModelIndex(), index, index);
    m_data.insert(index, a);
    emit endInsertRows();
    // update our count property
    //emit countChanged(m_data.count());
}

void ListWithValues::append(const A &a)
{
    //auto itr = std::find_if(m_data.begin(), m_data.end(), [&a](const A& aList) {return (aList.time == a.time) || aList.time > a.time;});
    //insert(std::distance(m_data.begin(), itr), a);
    insert(m_data.count(), a);
}
