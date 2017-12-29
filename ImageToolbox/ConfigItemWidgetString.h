#pragma once

#include "ConfigItemWidget.h"
#include <QLineEdit>

class ConfigItemWidgetString : public ConfigItemWidget
{
public:
    ConfigItemWidgetString(QWidget *parent = nullptr);
    ~ConfigItemWidgetString();

protected:
    void setValue(const QVariant &value) override;
    void updateValue() override;
    void applyLayout() override;

    QLineEdit *m_lineEdit;
};

