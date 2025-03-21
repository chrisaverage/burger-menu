#pragma once
#include <QWidget>
#include <QList>
#include <QSize>
#include <QIcon>

class QPushButton;
class QActionGroup;
class QAction;
class QString;
class BurgerButton;

class BurgerMenu : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon      READ burgerIcon WRITE setBurgerIcon NOTIFY iconChanged)
    Q_PROPERTY(QSize iconSize  READ iconSize   WRITE setIconSize   NOTIFY iconSizeChanged)
    Q_PROPERTY(int   menuWidth READ menuWidth  WRITE setMenuWidth  NOTIFY menuWidthChanged)
    Q_PROPERTY(bool  animated  READ animated   WRITE setAnimated   NOTIFY animatedChanged)
    Q_PROPERTY(bool  expanded  READ expanded   WRITE setExpanded   NOTIFY expandedChanged)

public:
    BurgerMenu(QWidget* parent = nullptr);

    QIcon burgerIcon() const;
    QSize iconSize() const;
    int menuWidth() const;
    QList<QAction*>	actions() const;
    bool animated() const;
    bool expanded() const;

signals:
    void iconChanged();
    void iconSizeChanged(const QSize& size);
    void menuWidthChanged(int width);
    void animatedChanged(bool animated);
    void expandedChanged(bool expanded);
    void triggered(QAction* action);

public slots:
    QAction* addMenuAction(QAction* action);
    QAction* addMenuAction(const QString& label);
    QAction* addMenuAction(const QIcon& icon, const QString& label);
    void     removeMenuAction(QAction* action);

    void setBurgerIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    void setMenuWidth(int width);
    void setAnimated(bool animated);
    void setExpanded(bool expanded);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void registerAction(QAction* action);
    void unRegisterAction(QAction* action);
    void setExpansionState(bool expanded);

    QActionGroup* mActions;
    QPushButton*  mBurgerButton;
    QList<BurgerButton*> mActionButtons;
    int           mMenuWidth;
    bool          mAnimated;
};
