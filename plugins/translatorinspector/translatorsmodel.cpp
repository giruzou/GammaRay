/*
  translatorsmodel.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Jan Dalheimer <jan.dalheimer@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "translatorsmodel.h"

#include <core/util.h>

#include "translatorwrapper.h"

using namespace GammaRay;

TranslatorsModel::TranslatorsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}
int TranslatorsModel::columnCount(const QModelIndex &) const
{
  return 4;
}
int TranslatorsModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) {
    return 0;
  }
  return m_translators.size();
}
QVariant TranslatorsModel::data(const QModelIndex &index, int role) const
{
  if (role == TranslatorRole) {
    return QVariant::fromValue(m_translators.at(index.row()));
  }
  TranslatorWrapper *trans = m_translators.at(index.row());
  Q_ASSERT(trans);
  if (role == Qt::DisplayRole) {
    if (index.column() == 0) {
      return Util::addressToString(trans->translator());
    } else if (index.column() == 1) {
      return QString(trans->translator()->metaObject()->className());
    } else if (index.column() == 2) {
      return trans->translator()->objectName();
    } else if (index.column() == 3) {
      return trans->model()->rowCount(QModelIndex());
    }
  }
  return QVariant();
}
QVariant TranslatorsModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
  if (orientation == Qt::Horizontal && section >= 2) {
    if (role == Qt::DisplayRole) {
      if (section == 0) {
        return tr("Address");
      } else if (section == 1) {
        return tr("Object Name");
      } else if (section == 2) {
        return tr("Name");
      } else if (section == 3) {
        return tr("Translations");
      }
    }
  }
  return QVariant();
}
Qt::ItemFlags TranslatorsModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
TranslatorWrapper *TranslatorsModel::translator(const QModelIndex &index)
    const
{
  return m_translators.at(index.row());
}
void TranslatorsModel::sourceDataChanged()
{
  const QModelIndex
      tl = index(0, 2, QModelIndex()),
      br = index(rowCount()-1, 3, QModelIndex());
  if (!tl.isValid() || !br.isValid()) {
    return;
  }
  // needed to make sure these things also update
  emit dataChanged(tl, br,
                   QVector<int>() << Qt::DisplayRole << Qt::EditRole);
}

void TranslatorsModel::registerTranslator(TranslatorWrapper *translator)
{
  beginInsertRows(QModelIndex(), m_translators.size(), m_translators.size());
  m_translators.append(translator);
  endInsertRows();
  connect(translator->model(), SIGNAL(rowCountChanged()),
          SLOT(sourceDataChanged()));
}
void TranslatorsModel::unregisterTranslator(TranslatorWrapper *translator)
{
  const int index = m_translators.indexOf(translator);
  if (index == -1) {
    qWarning("TranslatorsModel::unregisterTranslator: translator not registered");
    return;
  }
  disconnect(translator->model(), 0, this, 0);
  beginRemoveRows(QModelIndex(), index, index);
  m_translators.removeAt(index);
  endRemoveRows();
}
