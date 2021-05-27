#include "chdvplot.h"
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>

static const QMap<int, QColor> GraphColors = {
  {0, Qt::red},
  {1, Qt::blue},
  {2, Qt::cyan},
  {3, Qt::black},
  {4, Qt::yellow},
  {5, Qt::darkCyan},
  {6, Qt::darkRed},
  {7, Qt::darkBlue},
  {8, Qt::darkGreen},
  {9, Qt::darkYellow},
};
CHDVPlot::CHDVPlot(QWidget* parent)
{
  setGeometry(400, 250, 550, 400);
  int h = this->geometry().height();
  int w = this->geometry().width();


  _pcr = new chdvReader();
  _pchbx = new chdvCheckBoxWgt(this);
  auto pbutton = new QPushButton("File", this);
  pbutton->setMaximumSize(h / 10, w / 10);
  pbutton->setMinimumSize(h / 10, w / 10);
  _pCustomPlot = new QCustomPlot();
  _pCustomPlot->legend->setVisible(true);
  _pCustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
  _pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  _pvSlider = new QSlider(Qt::Vertical);
  _phSlider = new QSlider(Qt::Horizontal);

  _data.clear();

  auto hLayout = new QHBoxLayout();
  auto vLayout = new QVBoxLayout();
  auto vButtonLayout = new QVBoxLayout();
  auto ChBoxLayout = new QVBoxLayout();
  auto vGrafLayout = new QVBoxLayout();
  auto hGrafLayout = new QHBoxLayout();


  vLayout->addSpacerItem(new QSpacerItem(w / 10, h / 100));
  vLayout->addLayout(ChBoxLayout);

  vButtonLayout->addWidget(pbutton);

  hGrafLayout->addWidget(_pCustomPlot);
  hGrafLayout->addWidget(_pvSlider);
  vGrafLayout->addLayout(hGrafLayout);
  vGrafLayout->addWidget(_phSlider);


  hLayout->addLayout(vLayout);
  hLayout->addLayout(vGrafLayout);
  hLayout->addLayout(vButtonLayout);

  setLayout(hLayout);

  connect(pbutton, &QPushButton::clicked, this, [ = ] {
    execute(ChBoxLayout);
  });

  connect(_phSlider, SIGNAL(valueChanged(int)), this, SLOT(horzSliderChanged(int)));
  connect(_pvSlider, SIGNAL(valueChanged(int)), this, SLOT(vertSliderChanged(int)));
  connect(_pCustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
  connect(_pCustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

}

void CHDVPlot::execute(QVBoxLayout* layout)
{
  QString appDir = QCoreApplication::applicationDirPath();
  QString fileName = QFileDialog::getOpenFileName(this,
                     "Открыть Файл", appDir);

  _pcr->setFile(fileName);
  _data.clear();
  _params.clear();

  _data = _pcr->readAll(",");
  _params = _pcr->getParam();

  int size = _params.size();

  if(layout->isEmpty()) {
    _pchbx->makeCBoxes(layout, _params);
    _bVect.resize(layout->count());


    for(int i = 0; i < size; ++i) {
      connect(dynamic_cast<QCheckBox*>(layout->itemAt(i)->widget())
              , &QCheckBox::stateChanged
              , this
      , [ = ] {
        _bVect.replace(i, dynamic_cast<QCheckBox*>(layout->itemAt(i)->widget())->isChecked());
        drawPlots();
      });
    }
  }
}

void CHDVPlot::drawPlots()
{
  _pCustomPlot->clearGraphs();

  if(!_data.isEmpty()) {
    QVector<double> x1, y1, xMinMax, yMinMax;

    for (int  i = 0; i < _data.size(); ++i) {
      x1.clear();
      y1.clear();

      if(!_bVect[i]) {
        x1.append(0.0);
        y1.append(0.0);
      }
      else {
        for(auto d : _data.at(i).second) {
          x1.append(d.x());
          y1.append(d.y());
        }

        if(!x1.isEmpty() && !y1.isEmpty()) {
          auto resultX = std::minmax_element(x1.begin(), x1.end());
          xMinMax << *resultX.first << *resultX.second;
          auto resultY = std::minmax_element(y1.begin(), y1.end());
          yMinMax << *resultY.first << *resultY.second;
        }
      }

      _pCustomPlot->addGraph(_pCustomPlot->xAxis, _pCustomPlot->yAxis);
      _pCustomPlot->graph(i)->setData(x1, y1);
      _pCustomPlot->graph(i)->setPen(QPen(GraphColors[i]));
      _pCustomPlot->graph(i)->setName(_data.at(i).first);
    }

    auto resultXTotal = std::minmax_element(xMinMax.begin(), xMinMax.end());
    auto resultYTotal = std::minmax_element(yMinMax.begin(), yMinMax.end());
    _minX = (*resultXTotal.first);
    _maxX = (*resultXTotal.second);
    _minY = (*resultYTotal.first);
    _maxY = (*resultYTotal.second);

    _pCustomPlot->xAxis->setLabel("Time"); /*можно подтягивать так же из файла
    , но нужно тогда немного переписать в ридере*/
    _pCustomPlot->yAxis->setLabel("Values");

    _pCustomPlot->xAxis->setRange((_minX - (abs(_minX * 0.2))), (_maxX * 1.2));
    _pCustomPlot->yAxis->setRange((_minY - (abs(_minY * 0.2))), (_maxY * 1.2));
    //    _pCustomPlot->rescaleAxes();
  }

  _pvSlider->setRange(static_cast<int>(_minY), static_cast<int>(_maxY));
  _phSlider->setRange(static_cast<int>(_minX), static_cast<int>(_maxX));
  _pvSlider->setValue((static_cast<int>(_maxY + _minY)) / 2);
  _phSlider->setValue((static_cast<int>(_maxX + _minX)) / 2);
  _pCustomPlot->replot();

}

void CHDVPlot::horzSliderChanged(int value)
{
  if (qAbs(_pCustomPlot->xAxis->range().center() - value) >
      0.01) {
    _pCustomPlot->xAxis->setRange(value, _pCustomPlot->xAxis->range().size(), Qt::AlignCenter);
    _pCustomPlot->replot();
  }
}

void CHDVPlot::vertSliderChanged(int value)
{
  if (qAbs(_pCustomPlot->yAxis->range().center() - value) >
      0.01) {
    _pCustomPlot->yAxis->setRange(value, _pCustomPlot->yAxis->range().size(), Qt::AlignCenter);
    _pCustomPlot->replot();
  }
}

void CHDVPlot::xAxisChanged(QCPRange range)
{
  _phSlider->setValue(qRound(range.center()));
  _phSlider->setPageStep(qRound(range.size()));
}

void CHDVPlot::yAxisChanged(QCPRange range)
{
  _pvSlider->setValue(qRound(range.center()));
  _pvSlider->setPageStep(qRound(range.size()));
}
