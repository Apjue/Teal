#include "util.hpp"

void setTextureRect(QGraphicsPixmapItem& item, const QPixmap& texture, const QRect& rect)
{
    QPixmap p { QSize{rect.width(), rect.height()} };
    p.fill(Qt::transparent);

    QPainter painter{&p};

    painter.drawPixmap(0, 0, rect.width(), rect.height(), texture,
                       rect.x(), rect.y(), rect.width(), rect.height());

    item.setPixmap(p);
}
