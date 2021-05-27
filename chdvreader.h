#ifndef CHDVREADER_H
#define CHDVREADER_H

#include <QObject>
#include <iostream>
#include <QFile>
#include <QPolygonF>

class chdvReader : public QObject {
    Q_OBJECT
  public:
    ~chdvReader();
    chdvReader();
    chdvReader(const QString &fileName);
    void setFile(const QString &fileName);
    QList<QString> getParam();
    QVector<std::pair<QString, QPolygonF>> readAll(const QString &sep);

  private:
    QFile* _pFile = nullptr;
    QList<QString>* _pParam = nullptr;

};

#endif // CHDVREADER_H
