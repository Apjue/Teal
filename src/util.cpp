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

QJsonDocument jsonFromFile(const QString& filename)
{
    QFile jsonFile { filename };
    jsonFile.open(QFile::ReadOnly);

    return QJsonDocument().fromJson(jsonFile.readAll());
}

void jsonToFile(const QJsonDocument& document, const QString& filename)
{
    QFile jsonFile { filename };
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}
