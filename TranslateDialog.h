#ifndef TRANSLATEDIALOG_H
#define TRANSLATEDIALOG_H

#include <QDialog>

namespace Ui {
class TranslateDialog;
}

class TranslateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TranslateDialog(QWidget *parent = 0);
    ~TranslateDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void translateEvent(int id, int dx, int dy);

private:
    Ui::TranslateDialog *ui;
};

#endif // TRANSLATEDIALOG_H
