/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "treeitem.h"
#include "treemodel.h"

//! [0]
TreeModel::TreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    //setupModelData(data.split(QString("\n")), rootItem);
    lastQuery = "";
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QStringList TreeModel::getChildSelectList() const
{
    return m_childSelectList;
}

void TreeModel::setChildSelectList(const QString &childSelectList)
{
    m_childSelectList = childSelectList.trimmed().split(",");
}

QStringList TreeModel::getParentSelectList() const
{
    return m_parentSelectList;
}

void TreeModel::setParentSelectList(const QString &parentSelectList)
{
    m_parentSelectList = parentSelectList.trimmed().split(",");
}

int TreeModel::getGroupByColumn() const
{
    return m_groupByColumn;
}

void TreeModel::setGroupByColumn(int groupByColumn, Qt::SortOrder s)
{
    m_groupByColumn = groupByColumn;
    if(s==Qt::AscendingOrder)
        m_statemtOrder = " order by "%QString::number(m_groupByColumn+1)%" asc ";
    if(s==Qt::DescendingOrder)
        m_statemtOrder = " order by "%QString::number(m_groupByColumn+1)%" desc ";
}

void TreeModel::genChildData(TreeItem *child)
{
//    child->insertChildren(child->childCount(),1,query->record().count());
//    for (int c_col = 0; c_col < m_childSelectList.length(); c_col++) {
//        child->child(child->childCount()-1)->setData(c_col,query->value(m_childSelectList.at(c_col)));
//    }
}

int TreeModel::getCalulatorCol() const
{
    return m_calulatorCol;
}

void TreeModel::setCalulatorCol(const _calulator &calulatorCol)
{
    m_calulatorCol = calulatorCol;
}
void TreeModel::insertNewRow(int postion,int &row, int column,const QJsonObject &jsonObject, TreeItem *parent){

    int i = 0;
    i=1;
//    parent->insertChildren(parent->childNumber(),1,);
    QVector<QVariant> m_data;
    foreach(const QString key, jsonObject.keys()){
        if(jsonObject.value(key).isArray()){
            QVector<QVariant> nm_data;
            parent->insertChildren(0,1,0,nm_data);
//            TreeItem *child = new TreeItem(QVector<QVariant>(2),parent);
            TreeItem *child;
            child = nullptr;
            child = parent->child(0);
            insertNewArray(parent->childCount(),row,column,jsonObject.value(key).toArray(),child);
        }else {
            m_data << jsonObject.value(key);
        }
    }
    parent->insertChildren(0,1,0,m_data);
    postion++;
}
void TreeModel::insertNewArray(int postion, int &row, int column,const QJsonArray &jsonArray, TreeItem *parentItem){

    foreach(const QJsonValue &jsonValue, jsonArray){
        if(jsonValue.isArray()){
            insertNewArray(postion,row,column,jsonValue.toArray(), parentItem);
        }else if(jsonValue.isObject()){
            insertNewRow(postion,row,0,jsonValue.toObject(),parentItem);
//            row++;
        }
    }
}

void TreeModel::setQuery(const QString &query_str)
{
    if(rowCount()>0){
        removeRows(0,rowCount());
    }

    QVariant prevValue;
    int postion =0;
    TreeItem *child;
    child = nullptr;
    double sum;
    int count;
//    QString str = "[{\"bookHeavyInfo\":{\"Qty\":100},\"bookLightInfo\":{\"Qty\":2}},"
//                  "{\"bookHeavyInfo\":[{\"a\":1},{\"a\":2}]}]";
    QString str = "["
                  "{\"key1\": \"value1\", \"key2\":\"value2\", \"key3\":\"value3\",\"key4\":\"value4\" ,\"key5\": [{\"sub1\":\"vsub1\",\"sub2\":\"vsub2\",\"sub3\":\"vsub3\"}] },"
                  "{\"key1\": \"value1_2\", \"key2\":\"value2_2\", \"key3\":\"value3_2\",\"key4\":\"value4_2\"},"
                  "{\"key1\": \"value1_3\", \"key2\":\"value2_3\", \"key3\":\"value3_3\",\"key4\":\"value4_3\"},"
                  "{\"key1\": \"value1_4\", \"key2\":\"value2_4\", \"key3\":\"value3_4\",\"key4\":\"value4_4\" ,\"key5\": [{\"sub1\":\"vsub1_2\",\"sub2\":\"vsub2_2\",\"sub3\":\"vsub3_2\"}] },"
                  "{\"key1\": \"value1_5\", \"key2\":\"value2_5\", \"key3\":\"value3_5\",\"key4\":\"value4_5\" ,\"key5\": [{\"sub1\":\"vsub1_3\",\"sub2\":\"vsub2_3\",\"sub3\":\"vsub3_3\"}] },"
            "{\"key1\": \"value1_6\", \"key2\":\"value2_6\", \"key3\":\"value3_6\",\"key4\":\"value4_6\"},"
            "{\"key1\": \"value1_7\", \"key2\":\"value2_7\", \"key3\":\"value3_7\",\"key4\":\"value4_7\"}"
                  "]";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
    if(jsonResponse.isArray()){
        m_dataArray = jsonResponse.array();
        int row_ = 0;
        insertNewArray(0,row_,0,m_dataArray,rootItem);
    }
//    lastQuery = query->lastQuery();
//    while (query->next()) {
//        if(query->value(m_groupByColumn)!=prevValue){//neu phat hien item moi
//            prevValue = query->value(m_groupByColumn);
//            //set parrent
//            rootItem->insertChildren(postion,1,m_parentSelectList.length());
//            child = rootItem->child(postion);
//            for (int col = 0; col < m_parentSelectList.length(); col++) {
//               child->setData(col,query->value(m_parentSelectList.at(col)));
//            }
//            //set child
//            if(m_calulatorCol==SUM){
//                //chua hoan thanh
//                //if(postion>0) child->setData(m_parentSelectList)
//                sum = query->value(m_calulatorColName).toDouble();
//            }
//            if(m_calulatorCol==count){
//                count =1;
//            }
//            genChildData(child);
//            postion = postion +1;
//        }else{
//            if(child){
//                genChildData(child);
//                //tinh toan
//                if(m_calulatorCol==SUM){
//                    sum =sum+ query->value(m_calulatorColName).toDouble();
//                }
//                if(m_calulatorCol==COUNT){
//                    count++;
//                }
//            }
//        }
//    }
    emit dataChanged(QModelIndex(),QModelIndex());
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
//    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
//            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}
