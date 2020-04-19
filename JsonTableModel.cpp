#include "JsonTableModel.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

JsonTableModel::JsonTableModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

JsonTableModel::~JsonTableModel()
{

}

QString JsonTableModel::jsonString() const
{
    return m_jsonString;
}

void JsonTableModel::setJsonString(const QString &jsonString)
{
    m_jsonString = jsonString;
    QString str = "[{\"bookHeavyInfo\":{\"Qty\":100},\"bookLightInfo\":{\"Qty\":2}},"
                  "{\"bookHeavyInfo\":[{\"a\":1},{\"a\":2}]}]";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
    if(jsonResponse.isArray()){
        m_dataArray = jsonResponse.array();
        emit dataChanged(QModelIndex(),QModelIndex());
    }

}

QModelIndex JsonTableModel::index(int row, int column, const QModelIndex &parent) const
{
    QJsonObject jsonObject = m_dataArray.at(row).toObject();
    QString key = jsonObject.keys().at(column);

    if (!hasIndex(row, column, parent))
        return QModelIndex();

//    TreeItem *parentItem;
//    if (!parent.isValid())
//        parentItem = rootItem;
//    else
//        parentItem = static_cast<TreeItem*>(parent.internalPointer());

//    TreeItem *childItem = parentItem->child(row);
//    if (childItem)
//        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex JsonTableModel::parent(const QModelIndex &child) const
{
    int row, column;
    row = child.row();
    column =child.column();

    return QModelIndex();
}

int JsonTableModel::rowCount(const QModelIndex &parent) const
{
    return m_dataArray.size();
}

int JsonTableModel::columnCount(const QModelIndex &parent) const
{
    if(m_dataArray.isEmpty())
        return  0;
    return m_dataArray.at(0).toObject().keys().length();
}

QVariant JsonTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        QJsonObject jsonObject = m_dataArray.at(index.row()).toObject();
        QString key = jsonObject.keys().at(index.column());
    }
    return  QVariant();
}
