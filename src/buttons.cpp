// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "buttons.hpp"

Buttons::Buttons(unsigned x, unsigned y, const QSize& size, QWidget* parent) : QWidget(parent)
{
    m_button1 = new QPushButton("activate");
    m_button2 = new QPushButton("deactivate");
    m_button3 = new QPushButton("Inventory");

    m_layout = new QHBoxLayout;
    m_layout->addWidget(m_button1);
    m_layout->addWidget(m_button2);
    m_layout->addWidget(m_button3);

    setLayout(m_layout);
    setFixedSize(size);
    setGeometry(x, y, size.width(), size.height());
}

QPushButton* Buttons::button1() const
{
    return m_button1;
}

QPushButton* Buttons::button2() const
{
    return m_button2;
}

QPushButton *Buttons::button3() const
{
    return m_button3;
}
