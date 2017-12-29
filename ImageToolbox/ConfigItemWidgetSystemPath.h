#pragma once

#include "ConfigItemWidgetString.h"
#include <QFileDialog>
#include <QPushButton>

class ConfigItemWidgetSystemPath : public ConfigItemWidgetString
{
public:
    ConfigItemWidgetSystemPath(QWidget *parent = nullptr);
    ~ConfigItemWidgetSystemPath();

protected:
    void setValue(const QVariant &value) override;
    void updateValue() override;
    void applyLayout() override;

    QPushButton *m_browseButton;
    QFileDialog *m_browseDialog;
};

