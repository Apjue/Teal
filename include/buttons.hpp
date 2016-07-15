// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef BUTTONS_H
#define BUTTONS_H

#include <QWidget>
#include <QSignalMapper>
#include <QPushButton>
#include <QHBoxLayout>
#include "graphicsscene.hpp"

class Buttons : public QWidget
{
    Q_OBJECT
public:
    Buttons(unsigned x, unsigned y, const QSize& size, QWidget* parent = nullptr);
    ~Buttons() = default;

    QPushButton* button1() const;
    QPushButton* button2() const;
    QPushButton* button3() const;

private:
    QHBoxLayout* m_layout{};
    QPushButton* m_button1{};
    QPushButton* m_button2{};
    QPushButton* m_button3{};
    QPushButton* m_button4{};
};

#endif // BUTTONS_H
