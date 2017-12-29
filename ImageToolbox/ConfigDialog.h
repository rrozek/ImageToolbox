#pragma once

#include <QSettings>
#include <memory>
#include <QDialog>
#include <QAbstractButton>
#include <QGroupBox>

#include "ConfigItemWidget.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(std::shared_ptr<QSettings> settings, QWidget *parent = Q_NULLPTR);
    ~ConfigDialog();

signals:
    void signalSettingsChanged();

private:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    void buildWidgets(const QString& groupName, int row);

    void slotButtonBoxClicked(QAbstractButton* button);

    Ui::ConfigDialog *m_ui;

    std::shared_ptr<QSettings> m_settings;
    QList<ConfigItemWidget*> m_widgets;
    QList<QGroupBox*> m_groupBoxes;
};

