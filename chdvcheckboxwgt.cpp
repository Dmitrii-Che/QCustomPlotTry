#include "chdvcheckboxwgt.h"

#include <QLayout>
#include <QCheckBox>

chdvCheckBoxWgt::chdvCheckBoxWgt(QWidget* parent)
{}

chdvCheckBoxWgt::~chdvCheckBoxWgt()
  = default;

void chdvCheckBoxWgt::makeCBoxes(QVBoxLayout* layout, const QList<QString> &vParams)
{
  for(const auto &i : vParams) {
    layout->addWidget(new QCheckBox(i));
  }
}
