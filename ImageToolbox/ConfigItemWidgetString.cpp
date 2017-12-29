#include "ConfigItemWidgetString.h"

/*! \brief Constructor - creates and fills with data single settings field
 *
 * \param parent     - parent widget
 */
ConfigItemWidgetString::ConfigItemWidgetString(QWidget *parent)
    : ConfigItemWidget(parent)
    , m_lineEdit(new QLineEdit())
{
}

/*! \brief Destructor
 *
 */
ConfigItemWidgetString::~ConfigItemWidgetString()
{
    delete m_lineEdit;
}

/*! \brief sets new value on widget
 *
 * \param value - value to be set on widget
 */
void ConfigItemWidgetString::setValue(const QVariant &value)
{
    m_lineEdit->setText(value.toString());
}

/*! \brief updates local \ref m_value with current text from widget
 *
 */
void ConfigItemWidgetString::updateValue()
{
    m_value = m_lineEdit->text();
}

/*! \brief layout widget components inline
 *
 */
void ConfigItemWidgetString::applyLayout()
{
    m_layout->addWidget(m_labelWithKey);
    m_layout->addWidget(m_lineEdit);
}
