#ifndef SIZE_PICKER
#define SIZE_PICKER

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class SizePicker : public QWidget {
    Q_OBJECT
public:
    explicit SizePicker(QWidget *parent = nullptr);

    signals:
        void sizeSelected(int w, int h);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int hoverX = -1;
    int hoverY = -1;
    const int maxW = 20;
    const int maxH = 15;
};

#endif