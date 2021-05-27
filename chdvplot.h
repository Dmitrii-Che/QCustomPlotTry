#ifndef CHDVPLOT_H
#define CHDVPLOT_H

#include "chdvreader.h"
#include "chdvcheckboxwgt.h"
#include "qcustomplot.h"

class QPushButton;

class CHDVPlot : public QWidget {
    Q_OBJECT
  public:
    explicit CHDVPlot(QWidget* parent = nullptr);

  private:
    chdvReader* _pcr = nullptr;
    chdvCheckBoxWgt* _pchbx = nullptr;
    QCustomPlot* _pCustomPlot;
    QSlider* _pvSlider;
    QSlider* _phSlider;
    QVector<bool> _bVect;
    QVector<std::pair<QString, QPolygonF>> _data;
    QList<QString> _params;

    double _minX = 0;
    double _maxX = 100;
    double _minY = 0;
    double _maxY = 100;

    void execute(QVBoxLayout* layout);
    void drawPlots();

  private slots:
    void horzSliderChanged(int value);
    void vertSliderChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
};

#endif // CHDVPLOT_H
