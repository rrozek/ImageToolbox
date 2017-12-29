#include "ConfigItemWidgetSystemPath.h"

/*! \brief Constructor - creates and fills with data single settings field
 *
 * \param parent     - parent widget
 */
ConfigItemWidgetSystemPath::ConfigItemWidgetSystemPath(QWidget *parent)
    : ConfigItemWidgetString(parent)
    , m_browseButton(new QPushButton("browse..."))
    , m_browseDialog(new QFileDialog())
{
    QObject::connect(m_browseButton, &QPushButton::clicked, m_browseDialog, &QFileDialog::exec);
    QObject::connect(m_browseDialog, &QFileDialog::fileSelected, m_lineEdit, &QLineEdit::setText);
    m_browseDialog->setFileMode(QFileDialog::Directory);
}

/*! \brief Destructor
 *
 */
ConfigItemWidgetSystemPath::~ConfigItemWidgetSystemPath()
{
    delete m_browseButton;
    delete m_browseDialog;
}

/*! \brief sets new value on widget
 *
 * \param value - value to be set on widget
 */
void ConfigItemWidgetSystemPath::setValue(const QVariant &value)
{
    m_lineEdit->setText(value.toString());
    m_browseDialog->setDirectory(value.toString());
}

/*! \brief updates local \ref m_value with current text from widget
 *
 */
void ConfigItemWidgetSystemPath::updateValue()
{
    m_value = m_lineEdit->text();
}

/*! \brief layout widget components inline
 *
 */
void ConfigItemWidgetSystemPath::applyLayout()
{
    m_layout->addWidget(m_labelWithKey);
    m_layout->addWidget(m_lineEdit);
    m_layout->addWidget(m_browseButton);
}

