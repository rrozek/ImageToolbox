#pragma once

#include <QWidget>
#include <QSettings>
#include <QLabel>
#include <QHBoxLayout>
#include <memory>

/*! \class ConfigItemWidget
 *  \brief Simple widget responsible for displaying and manipulating
 * one settings property
 *
 */
class ConfigItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigItemWidget(QWidget *parent = nullptr);
    virtual ~ConfigItemWidget();

    void setSettingsFile(std::shared_ptr<QSettings> settings);
    void setContext(const QString& groupName, const QString& key);

    void commitChanges();
    void revertChanges();

    virtual void applyLayout() = 0;
signals:

protected:
    void setLabelText(const QString &labelText);
    virtual void setValue(const QVariant &value) = 0;
    virtual void updateValue() = 0;

    QVariant m_value;

    QLabel *m_labelWithKey;
    QHBoxLayout *m_layout;

    std::shared_ptr<QSettings> m_settings;
    QString m_fullKey;

};

