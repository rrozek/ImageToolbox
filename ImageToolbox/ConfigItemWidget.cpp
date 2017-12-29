#include "ConfigItemWidget.h"

/*! \brief Constructor - creates and fills with data single settings field
 *
 * \param parent     - parent widget
 */
ConfigItemWidget::ConfigItemWidget(QWidget *parent)
    : QWidget(parent)
    , m_labelWithKey(new QLabel())
    , m_layout(new QHBoxLayout())
{
    setLayout(m_layout);
}

/*! \brief Destructor
 *
 */
ConfigItemWidget::~ConfigItemWidget()
{
    delete m_labelWithKey;
    delete m_layout;
}

/*! \brief sets reference to currently used settings file
 * and updates locally displayed value to the one read from settings
 *
 * \param settings     - pointer to used settings handle
 */
void ConfigItemWidget::setSettingsFile(std::shared_ptr<QSettings> settings)
{
    m_settings = settings;
}

/*! \brief sets reference to currently used settings file
 * and updates locally displayed value to the one read from settings
 *
 * \param groupName     - settings file group name that this item depends on
 * \param key           - settings file key name that this item depends on
 */
void ConfigItemWidget::setContext(const QString &groupName, const QString &key)
{
    m_labelWithKey->setText(key);
    m_value = m_settings->value(key);
    m_fullKey = groupName + "/" + key;
    setValue(m_value);
}

/*! \brief writes modified value into settings system
 *
 */
void ConfigItemWidget::commitChanges()
{
    updateValue();
    m_settings->setValue(m_fullKey, m_value);
}

/*! \brief reverts all user changes to sync displayed value with one from settings system
 *
 */
void ConfigItemWidget::revertChanges()
{
    setValue(m_value);
}

/*! \brief sets new setting text on label
 *
 * \param labelText     - setting name to display on corresponding label
 */
void ConfigItemWidget::setLabelText(const QString &labelText)
{
    m_labelWithKey->setText(labelText);
}
