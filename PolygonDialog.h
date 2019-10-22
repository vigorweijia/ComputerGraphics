#ifndef POLYGONDIALOG_H
#define POLYGONDIALOG_H

#include <QDialog>

namespace Ui {
class PolygonDialog;
}

class PolygonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolygonDialog(QWidget *parent = 0);
    ~PolygonDialog();

private:
    Ui::PolygonDialog *ui;
};

#endif // POLYGONDIALOG_H
