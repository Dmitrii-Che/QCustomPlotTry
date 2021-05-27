#ifndef CHDVCHECKBOXWGT_H
#define CHDVCHECKBOXWGT_H

#include <QWidget>

class QVBoxLayout;

class chdvCheckBoxWgt : public QWidget {
    Q_OBJECT
  public:
    chdvCheckBoxWgt(QWidget* parent);
    ~chdvCheckBoxWgt();
    void makeCBoxes(QVBoxLayout* layout, const QList<QString> &vParams);

  private:
    QList<QString>_vParams;
    QVector<bool> _bVect;
};

#endif // CHDVCHECKBOXWGT_H
