#include "ConfigItemWidgetBool.h"

/*! \brief Constructor - creates and fills with data single settings field
 *
 * \param parent     - parent widget
 */
ConfigItemWidgetBool::ConfigItemWidgetBool(QWidget *parent)
    : ConfigItemWidget(parent)
    , m_checkBox(new QCheckBox())
{
}

/*! \brief Destructor
 *
 */
ConfigItemWidgetBool::~ConfigItemWidgetBool()
{
    delete m_checkBox;
}

/*! \brief sets new value on widget
 *
 * \param value - value to be set on widget
 */
void ConfigItemWidgetBool::setValue(const QVariant &value)
{
    m_checkBox->setChecked(value.toBool());
}

/*! \brief updates local \ref m_value with current text from widget
 *
 */
void ConfigItemWidgetBool::updateValue()
{
    m_value = m_checkBox->isChecked() ? "true" : "false";
}

/*! \brief layout widget components inline
 *
 */
void ConfigItemWidgetBool::applyLayout()
{
    m_layout->addWidget(m_labelWithKey);
    m_layout->addWidget(m_checkBox);
}
