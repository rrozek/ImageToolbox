#pragma once

#include <QSettings>
#include <memory>
#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

private:
    Ui::ConfigDialog *m_ui;

    std::shared_ptr<QSettings> m_settings;
};

