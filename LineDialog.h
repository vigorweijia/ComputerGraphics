#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include <QWidget>

namespace Ui {
class LineDialog;
}

class LineDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LineDialog(QWidget *parent = 0);
    ~LineDialog();

private slots:
    void on_DDA_clicked();
    void on_Bresenham_clicked();
    void on_Cancel_clicked();

signals:
    void drawLineEvent(int id, float x0, float y0, float x1, float y1, int type);

private:
    Ui::LineDialog *ui;
};

#endif // LINEDIALOG_H
