#include "ImageManipulationToolbox.h"
#include "ui_ImageManipulationToolbox.h"

ImageManipulationToolbox::ImageManipulationToolbox(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ImageManipulationToolbox)
{
    m_ui->setupUi(this);
}
