#include "burgermenu.h"
#include "burgermenuplugin.h"
#include <QtPlugin>

BurgerMenuPlugin::BurgerMenuPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void BurgerMenuPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool BurgerMenuPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *BurgerMenuPlugin::createWidget(QWidget *parent)
{
    return new BurgerMenu(parent);
}

QString BurgerMenuPlugin::name() const
{
    return QLatin1String("BurgerMenu");
}

QString BurgerMenuPlugin::group() const
{
    return QLatin1String("Menus");
}

QIcon BurgerMenuPlugin::icon() const
{
    return QIcon(QLatin1String(":/icon.png"));
}

QString BurgerMenuPlugin::toolTip() const
{
    return QLatin1String("Burger Menu");
}

QString BurgerMenuPlugin::whatsThis() const
{
    return QLatin1String("A widget implementing burger menu.");
}

bool BurgerMenuPlugin::isContainer() const
{
    return false;
}

QString BurgerMenuPlugin::domXml() const
{
    return QLatin1String("<widget class=\"BurgerMenu\" name=\"burgerMenu\">\n</widget>\n");
}

QString BurgerMenuPlugin::includeFile() const
{
    return QLatin1String("burgermenu.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(burgermenuplugin, BurgerMenuPlugin)
#endif // QT_VERSION < 0x050000
