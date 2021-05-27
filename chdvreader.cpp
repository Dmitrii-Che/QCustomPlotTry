#include "chdvreader.h"
#include <QDebug>

chdvReader::chdvReader()
  = default;

chdvReader::~chdvReader()
  = default;

chdvReader::chdvReader(const QString &fileName)
{
  setFile(fileName);
}

void chdvReader::setFile(const QString &fileName)
{
  _pFile = new QFile(fileName);
  _pParam = new QList<QString>;
}

QList<QString> chdvReader::getParam()
{
  return *_pParam;
}

QVector<std::pair<QString, QPolygonF>> chdvReader::readAll(const QString &sep)
{
  QVector<std::pair<QString, QPolygonF>> plots;

  if (_pFile != nullptr) {
    if (!_pFile->open(QFile::ReadOnly | QFile::Text)) {
      qDebug() << "CHDV:: File Opening Erroe";
    }
    else {
      _pParam->clear();
      QTextStream stream(_pFile);
      QString line = stream.readLine();
      QVector<qreal> vTemp; /*вектор для хранения временных значений точек*/

      for(auto i : line.split(sep)) { /*собираем вектор из всех параметров*/
        _pParam->append(i);
      }

      _pParam->pop_front();


      QVector<QPolygonF> vPlotPoints(
        _pParam->size()); /*создаём вектор граффиков, туда будем писать точки*/

      while (!stream.atEnd()) { /*начинаем распределять значения*/
        line.clear();
        vTemp.clear();
        line = stream.readLine();

        for(auto i : line.split(sep)) {
          vTemp.append(i.toDouble());
        }

        for(int i = 0; i < vPlotPoints.size(); ++i) { /*Сборка кривых*/
          vPlotPoints[i] << QPointF(vTemp.at(0), vTemp.at(i + 1));
        }
      }

      _pFile->close();

      for(int i = 0; i < vPlotPoints.size(); ++i) { /*сборка в вектор пар*/
        plots.append(std::pair<QString, QPolygonF>(_pParam->at(i), vPlotPoints[i]));
      }
    }
  }

  return plots;
}
