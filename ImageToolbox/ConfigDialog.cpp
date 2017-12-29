#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"

#include "ConfigItemWidget.h"
#include "ConfigItemWidgetBool.h"
#include "ConfigItemWidgetString.h"
#include "ConfigItemWidgetSystemPath.h"

#include <QDebug>
#include <QDir>

ConfigDialog::ConfigDialog(std::shared_ptr<QSettings> settings, QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::ConfigDialog)
    , m_settings(settings)
{
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, &ConfigDialog::slotButtonBoxClicked);
}

ConfigDialog::~ConfigDialog()
{
    delete m_ui;
}

void ConfigDialog::showEvent(QShowEvent *event)
{
    int i =0;
    for(QString group : m_settings->childGroups())
    {
        buildWidgets(group, i++);
    }
    QDialog::showEvent(event);
}

void ConfigDialog::closeEvent(QCloseEvent* event)
{
    m_settings->sync();
    for (ConfigItemWidget* pWidget : m_widgets)
        delete pWidget;
    m_widgets.clear();

    for (QGroupBox* pGroupBox : m_groupBoxes)
        delete pGroupBox;
    m_groupBoxes.clear();

    QDialog::closeEvent(event);
}

void ConfigDialog::buildWidgets(const QString &groupName, int row)
{
    m_settings->beginGroup(groupName);
    QGroupBox* pGroupBoxWidget = new QGroupBox(groupName, this);
    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    m_ui->gridLayout->addWidget(pGroupBoxWidget, row, 0);
    for (QString key : m_settings->allKeys())
    {
        ConfigItemWidget* pWidget = nullptr;

        QVariant value = m_settings->value(key);
        if (value.canConvert(QMetaType::QString))
        {
            if (value.toString() == "true" || value.toString() == "false")
                pWidget = new ConfigItemWidgetBool();
            else if (QDir(value.toString()).exists())
                pWidget = new ConfigItemWidgetSystemPath();
            else
                pWidget = new ConfigItemWidgetString();
        }
        else
        {
            qWarning() << "Unsupported QMetaType detected: " << value.userType();
        }

        if (pWidget != nullptr)
        {
            pWidget->applyLayout();
            pWidget->setSettingsFile(m_settings);
            pWidget->setContext(groupName, key);
            vBoxLayout->addWidget(pWidget);
            m_widgets.append(pWidget);
        }
    }
    pGroupBoxWidget->setLayout(vBoxLayout);
    m_groupBoxes.append(pGroupBoxWidget);
    m_settings->endGroup();
}

void ConfigDialog::slotButtonBoxClicked(QAbstractButton *button)
{
    QDialogButtonBox::ButtonRole buttonRole = m_ui->buttonBox->buttonRole(button);
    if (buttonRole == QDialogButtonBox::AcceptRole)
    {
        for(ConfigItemWidget* pWidget : m_widgets)
            pWidget->commitChanges();
        m_settings->sync();
        emit signalSettingsChanged();
    }
    else if (buttonRole == QDialogButtonBox::RejectRole)
    {
        for(ConfigItemWidget* pWidget : m_widgets)
            pWidget->revertChanges();
    }
    this->close();
}
