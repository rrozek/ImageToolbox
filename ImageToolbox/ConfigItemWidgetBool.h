#pragma once

#include "ConfigItemWidget.h"
#include <QCheckBox>

class ConfigItemWidgetBool : public ConfigItemWidget
{
public:
    ConfigItemWidgetBool(QWidget *parent = nullptr);
    ~ConfigItemWidgetBool();

protected:
    void setValue(const QVariant &value) override;
    void updateValue() override;
    void applyLayout() override;

    QCheckBox *m_checkBox;
};

