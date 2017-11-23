#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::ConfigDialog)
    , m_settings(std::make_shared<QSettings>("settings.ini", QSettings::IniFormat))
{
    m_ui->setupUi(this);
}

ConfigDialog::~ConfigDialog()
{
    delete m_ui;
}
