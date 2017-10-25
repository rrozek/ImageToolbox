#pragma once

#include <QWidget>
#include <QGridLayout>

namespace Ui {
class ImageManipulationToolbox;
}

class ImageManipulationToolbox : public QWidget
{
    Q_OBJECT
public:
    explicit ImageManipulationToolbox(QWidget *parent = nullptr);

signals:

public slots:

private:
    Ui::ImageManipulationToolbox *m_ui;
};

