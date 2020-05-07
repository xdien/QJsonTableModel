#ifndef JSONTABLEMODEL_H
#define JSONTABLEMODEL_H

#include <QAbstractItemModel>
#include <QJsonArray>
#include <QModelIndex>
#include <QVariant>

class  TreeItem;

class JsonTableModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    JsonTableModel(const QStringList &headers,
              QObject *parent = nullptr);
    ~JsonTableModel();
    QString lastQuery;
    enum _calulator{NOTHING,SUM,COUNT};
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    int getGroupByColumn() const;
    void setGroupByColumn(int groupByColumn, Qt::SortOrder s);
    void setJsonArrayString(const QString *jsonArrayString);
    bool setJsonArray(const QJsonArray *jsonArray);
    QStringList getParentSelectList() const;
    void setParentSelectList(const QString &parentSelectList);

    QStringList getChildSelectList() const;
    ///
    /// \brief setChildSelectList cach nhau boi dau ,
    /// \param childSelectList
    ///
    void setChildSelectList(const QString &childSelectList);


    int getCalulatorCol() const;
    void setCalulatorCol(const _calulator &calulatorCol);

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;
    TreeItem *rootItem;
    int m_groupByColumn;
    const QString ORDER_STR = " order by ";
    QString m_statemtOrder;
    //display data
    QStringList m_parentSelectList,m_childSelectList;
    _calulator m_calulatorCol;//if -1 nothing to do
    QString m_calulatorColName;
    QJsonArray *m_dataArray;

    bool insertNewRow(const QJsonObject &jsonObject, TreeItem *parent);
    void insertNewArray(int row, const QJsonArray &jsonArray, TreeItem *parentItem = nullptr);
};

#endif // JSONTABLEMODEL_H
