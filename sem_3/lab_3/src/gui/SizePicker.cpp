#include "../../include/gui/SizePicker.hpp"

SizePicker::SizePicker(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
}

void SizePicker::paintEvent(QPaintEvent *) {
    QPainter p(this);
    int cellW = width() / maxW;
    int cellH = height() / maxH;

    for (int y = 0; y < maxH; ++y) {
        for (int x = 0; x < maxW; ++x) {
            QRect rect(x * cellW, y * cellH, cellW - 2, cellH - 2);
            
            // Если клетка попадает в выделение (от 0,0 до мышки)
            if (x <= hoverX && y <= hoverY) {
                p.setBrush(QColor(0, 120, 215)); // Цвет как в Word (синий)
            } else {
                p.setBrush(QColor(240, 240, 240)); // Обычный серый
            }
            p.drawRect(rect);
        }
    }

    // Рисуем текст с текущим размером под сеткой (опционально)
    if (hoverX != -1) {
        p.setPen(Qt::black);
        p.drawText(rect(), Qt::AlignCenter | Qt::AlignBottom, 
                   QString::number(hoverX + 1) + " x " + QString::number(hoverY + 1));
    }
}

void SizePicker::mouseMoveEvent(QMouseEvent *event) {
    // Получаем позицию через новый метод position()
    QPointF pos = event->position();

    // Преобразуем координаты в индексы сетки
    hoverX = static_cast<int>(pos.x()) / (width() / maxW);
    hoverY = static_cast<int>(pos.y()) / (height() / maxH);

    // Ограничиваем границы
    hoverX = std::max(0, std::min(hoverX, maxW - 1));
    hoverY = std::max(0, std::min(hoverY, maxH - 1));
    
    update();
}

void SizePicker::leaveEvent(QEvent *) {
    hoverX = -1;
    hoverY = -1;
    update();
}

void SizePicker::mousePressEvent(QMouseEvent *) {
    if (hoverX >= 4 && hoverY >= 4) { // Минимальный размер 5x5 (индексы 4,4)
        emit sizeSelected(hoverX + 1, hoverY + 1);
    }
}
