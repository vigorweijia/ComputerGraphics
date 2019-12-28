#ifndef GALAXYDIALOG_H
#define GALAXYDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class GalaxyDialog;
}

class GalaxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GalaxyDialog(QWidget *parent = 0);
    ~GalaxyDialog();

private:
    Ui::GalaxyDialog *ui;
};

#endif // GALAXYDIALOG_H
