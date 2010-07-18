#ifndef ENDOSCOPE_OBJECTLISTMODEL_H
#define ENDOSCOPE_OBJECTLISTMODEL_H

#include <qabstractitemmodel.h>
#include <qvector.h>


namespace Endoscope {

class ObjectListModel : public QAbstractTableModel
{
  Q_OBJECT
  public:
    ObjectListModel( QObject *parent = 0 );
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void objectAdded( QObject *obj );
    void objectRemoved( QObject *obj );

  private:
    QVector<QObject*> m_objects;
};

}

#endif // ENDOSCOPE_OBJECTLISTMODEL_H
