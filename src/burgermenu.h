#pragma once
#include <QWidget>
#include <QList>
#include <QSize>

class QPushButton;
class QActionGroup;
class QAction;
class QString;

class BurgerMenu : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize  READ iconSize  WRITE setIconSize  NOTIFY iconSizeChanged)
    Q_PROPERTY(int   menuWidth READ menuWidth WRITE setMenuWidth NOTIFY menuWidthChanged)
    Q_PROPERTY(bool  animated  READ animated  WRITE setAnimated  NOTIFY animatedChanged)

public:
    BurgerMenu(QWidget* parent = nullptr);

    QIcon burgerIcon() const;
    QSize iconSize() const;
    int menuWidth() const;
    QList<QAction*>	actions() const;
    bool animated() const;

signals:
    void iconSizeChanged(const QSize& size) const;
    void menuWidthChanged(int width) const;
    void animatedChanged(bool animated) const;
    void triggered(QAction* action) const;

public slots:
    QAction* addMenuAction(QAction* action);
    QAction* addMenuAction(const QString& label);
    QAction* addMenuAction(const QIcon& icon, const QString& label);
    void     removeMenuAction(QAction* action);

    void setBurgerIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    void setMenuWidth(int width);
    void setAnimated(bool animated);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void toggle(bool checked);
    void registerAction(QAction* action);
    void unRegisterAction(QAction* action);

    QActionGroup* mActions;
    QPushButton*  mBurgerButton;
    int           mMenuWidth;
    bool          mAnimated;
};
