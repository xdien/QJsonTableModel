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
#include "JsonTableModel.h"

//! [0]
JsonTableModel::JsonTableModel(const QStringList &headers, QObject *parent)
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
JsonTableModel::~JsonTableModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int JsonTableModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant JsonTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags JsonTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
TreeItem *JsonTableModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QStringList JsonTableModel::getChildSelectList() const
{
    return m_childSelectList;
}

void JsonTableModel::setChildSelectList(const QString &childSelectList)
{
    m_childSelectList = childSelectList.trimmed().split(",");
}

QStringList JsonTableModel::getParentSelectList() const
{
    return m_parentSelectList;
}

void JsonTableModel::setParentSelectList(const QString &parentSelectList)
{
    m_parentSelectList = parentSelectList.trimmed().split(",");
}

int JsonTableModel::getGroupByColumn() const
{
    return m_groupByColumn;
}

void JsonTableModel::setGroupByColumn(int groupByColumn, Qt::SortOrder s)
{
    m_groupByColumn = groupByColumn;
    if(s==Qt::AscendingOrder)
        m_statemtOrder = " order by "%QString::number(m_groupByColumn+1)%" asc ";
    if(s==Qt::DescendingOrder)
        m_statemtOrder = " order by "%QString::number(m_groupByColumn+1)%" desc ";
}


int JsonTableModel::getCalulatorCol() const
{
    return m_calulatorCol;
}

void JsonTableModel::setCalulatorCol(const _calulator &calulatorCol)
{
    m_calulatorCol = calulatorCol;
}
bool JsonTableModel::insertNewRow(const QJsonObject &jsonObject, TreeItem *parent){

    QStringList listKeyWithSubArray;
    QVector<QVariant> m_data;
    foreach(const QString key, jsonObject.keys()){
        if(jsonObject.value(key).isArray()){
            listKeyWithSubArray << key;
        }else {
            m_data << jsonObject.value(key);
        }
    }
    bool thanhCong = parent->insertChildren(1,m_data);
    if(listKeyWithSubArray.size()>0){
        TreeItem *child;
        child = nullptr;
        child = parent->child(parent->childCount()-1);
        insertNewArray(parent->childCount(),jsonObject.value(listKeyWithSubArray.at(0)).toArray(),child);
    }
    return thanhCong;
}

void JsonTableModel::insertNewArray(int row,const QJsonArray &jsonArray, TreeItem *parentItem){
    foreach(const QJsonValue jsonValue, jsonArray){
        if(jsonValue.isArray()){
            insertNewArray(row,jsonValue.toArray(), parentItem);
        }else if(jsonValue.isObject()){
            insertNewRow(jsonValue.toObject(),parentItem);
        }
    }
}

void JsonTableModel::setJsonArrayString(const QString *jsonArrayString)
{
    QJsonDocument *jsonResponse = new QJsonDocument(QJsonDocument::fromJson(jsonArrayString->toUtf8()));
    if(jsonResponse->isArray()){
        m_dataArray = new QJsonArray(jsonResponse->array());
        int row_ = 0;
        insertNewArray(row_,*m_dataArray,rootItem);
    }
    emit dataChanged(QModelIndex(),QModelIndex());
}

bool JsonTableModel::setJsonArray(const QJsonArray *jsonArray)
{
    if(jsonArray == nullptr)
        return false;
    m_dataArray = new QJsonArray(*jsonArray);
    int row_ = 0;
    insertNewArray(row_,*m_dataArray,rootItem);
    emit dataChanged(QModelIndex(),QModelIndex());
}
//! [4]

QVariant JsonTableModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        return  section;
    return QVariant();
}

//! [5]
QModelIndex JsonTableModel::index(int row, int column, const QModelIndex &parent) const
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

bool JsonTableModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool JsonTableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = false;

    beginInsertRows(parent, position, position + rows - 1);
//    success = parentItem->insertChildren(1,QVector<QVariant>());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex JsonTableModel::parent(const QModelIndex &index) const
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

bool JsonTableModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool JsonTableModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int JsonTableModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool JsonTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool JsonTableModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void JsonTableModel::setupModelData(const QStringList &lines, TreeItem *parent)
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
