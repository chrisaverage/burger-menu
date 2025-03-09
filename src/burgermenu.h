#pragma once
#include <QWidget>
#include <QList>
#include <QSize>
#include <QIcon>
#include <QPushButton>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyle>

class QActionGroup;
class QAction;
class QString;

extern const QString BurgerButtonObjectName;
extern const QString BurgerMenuName;
extern const QString MainBurgerButtonObjectName;

class BurgerButton : public QPushButton
{
    Q_OBJECT
public:
    BurgerButton(QAction* action, QWidget* parent)
        : QPushButton(parent)
        , mIconSize(QSize(64,64))
        , mAction(action)
    {
        setObjectName(BurgerButtonObjectName);
        connect(action, &QAction::destroyed, this, &BurgerButton::deleteLater);
        setCursor(Qt::PointingHandCursor);

        connect(mAction, SIGNAL(changed()), this, SLOT(update()));
        connect(this, &BurgerButton::clicked, [&]{
            if(mAction->isCheckable() && !mAction->isChecked())
                mAction->toggle();

            mAction->trigger();
        });
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        QStyleOptionButton opt;
        opt.initFrom(this);
        opt.state |= (mAction->isChecked() ? QStyle::State_On : QStyle::State_Off);

        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        const QRect contentsRect = style()->subElementRect(QStyle::SE_PushButtonContents, &opt, this);
        if(!mAction->icon().isNull())
        {
            QIcon::Mode mode = ((opt.state & QStyle::State_MouseOver) == 0) ? QIcon::Normal : QIcon::Active;
            if(!isEnabled())
                mode = QIcon::Disabled;
            QIcon::State state = mAction->isChecked() ? QIcon::On : QIcon::Off;
            painter.drawPixmap(QRect(contentsRect.topLeft(), mIconSize), mAction->icon().pixmap(mIconSize, mode, state));
        }

        opt.rect = contentsRect.adjusted(mIconSize.width()+1, 0, 0, 0);
        opt.text = fontMetrics().elidedText(mAction->iconText(), Qt::ElideRight, opt.rect.width(), Qt::TextShowMnemonic);
        style()->drawControl(QStyle::CE_CheckBoxLabel, &opt, &painter, this);
    }

    void setIconSize(const QSize& size) { mIconSize = size; setFixedHeight(mIconSize.height()); update(); }

    QAction* action() const { return mAction; }

private:
    QSize mIconSize;
    QAction* mAction;
};

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
    void iconChanged() const;
    void iconSizeChanged(const QSize& size) const;
    void menuWidthChanged(int width) const;
    void animatedChanged(bool animated) const;
    void expandedChanged(bool expanded) const;
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
    void setExpanded(bool expanded);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void registerAction(QAction* action);
    void unRegisterAction(QAction* action);
    void setExpansionState(bool expanded);

    QActionGroup* mActions;
    QPushButton*  mBurgerButton;
    int           mMenuWidth;
    bool          mAnimated;
};
