#ifndef JSONTABLEMODEL_H
#define JSONTABLEMODEL_H

#include <QAbstractItemModel>
#include <QJsonArray>

class JsonTableModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit JsonTableModel(QObject *parent = nullptr);
    ~JsonTableModel();
    QString jsonString() const;

    void setJsonString(const QString &jsonString);

private:
    QString m_jsonString;
    QJsonArray m_dataArray;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // JSONTABLEMODEL_H
