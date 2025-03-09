#include "burgermenu.h"

#include <QActionGroup>
#include <QBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QStyleOption>

namespace ObjectNames
{
    static const QLatin1String BurgerButton ("BurgerButton");
    static const QLatin1String BurgerMenu ("BurgerMenu");
    static const QLatin1String MainBurgerButton ("MainBurgerButton");
}

class BurgerButton : public QPushButton
{
public:
    BurgerButton(QAction* action, QWidget* parent)
        : QPushButton(parent)
        , mIconSize(QSize(64,64))
        , mAction(action)
    {
        connect(action, &QAction::destroyed, this, &BurgerButton::deleteLater);
        setCursor(Qt::PointingHandCursor);

        connect(mAction, &QAction::changed, this, static_cast<void(BurgerButton::*)()>(&BurgerButton::update));
        connect(this, &BurgerButton::clicked, mAction, [&]
        {
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

    void setIconSize(const QSize& size)
    {
        mIconSize = size;
        setFixedHeight(mIconSize.height());
        update();
    }

    QAction* action() const
    {
        return mAction;
    }

private:
    QSize mIconSize;
    QAction* mAction;
};

BurgerMenu::BurgerMenu(QWidget* parent)
    : QWidget(parent)
    , mActions(new QActionGroup(this))
    , mBurgerButton(new QPushButton(this))
    , mMenuWidth(200)
    , mAnimated(true)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mBurgerButton->setObjectName(ObjectNames::MainBurgerButton);
    mBurgerButton->setFlat(true);
    mBurgerButton->setIconSize(QSize(48,48));
    mBurgerButton->setFixedSize(48,48);
    mBurgerButton->setCheckable(true);
    mBurgerButton->setCursor(Qt::PointingHandCursor);
    mActions->setExclusive(true);

    auto burgerWidget = new QWidget(this);
    burgerWidget->setObjectName(ObjectNames::BurgerMenu);
    auto burgerLay = new QHBoxLayout();
    burgerLay->setContentsMargins(0, 0, 0, 0);
    burgerLay->setSpacing(0);
    burgerLay->addWidget(mBurgerButton);
    burgerLay->addWidget(burgerWidget);

    auto lay = new QVBoxLayout();
    setLayout(lay);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);

    lay->addLayout(burgerLay);
    lay->addStretch();
    setFixedWidth(48);

    connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::setExpansionState);
    connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::expandedChanged);
    connect(mActions, &QActionGroup::triggered, this, &BurgerMenu::triggered);
}

QIcon BurgerMenu::burgerIcon() const
{
    return mBurgerButton->icon();
}

QSize BurgerMenu::iconSize() const
{
    return mBurgerButton->iconSize();
}

int BurgerMenu::menuWidth() const
{
    return mMenuWidth;
}

QList<QAction*> BurgerMenu::actions() const
{
    return mActions->actions();
}

QAction* BurgerMenu::addMenuAction(QAction* action)
{
    mActions->addAction(action);
    registerAction(action);
    return action;
}

QAction*BurgerMenu::addMenuAction(const QString& label)
{
    auto action = mActions->addAction(label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}

QAction*BurgerMenu::addMenuAction(const QIcon& icon, const QString& label)
{
    auto action = mActions->addAction(icon, label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}

void BurgerMenu::removeMenuAction(QAction* action)
{
    mActions->removeAction(action);
    unRegisterAction(action);
}

void BurgerMenu::setBurgerIcon(const QIcon& icon)
{
    mBurgerButton->setIcon(icon);
    emit iconChanged();
}

void BurgerMenu::setIconSize(const QSize& size)
{
    if(size == mBurgerButton->iconSize())
        return;

    mBurgerButton->setIconSize(size);
    mBurgerButton->setFixedSize(size);
    for(auto btn : std::as_const(mActionButtons))
        btn->setIconSize(size);

    if(mBurgerButton->isChecked())
        setFixedWidth(mBurgerButton->width() + mMenuWidth);
    else
        setFixedWidth(mBurgerButton->width());

    emit iconSizeChanged(size);
}

void BurgerMenu::setMenuWidth(int width)
{
    if(width == mMenuWidth)
        return;

    mMenuWidth = width;

    if(mBurgerButton->isChecked())
        setFixedWidth(mBurgerButton->width() + mMenuWidth);
    else
        setFixedWidth(mBurgerButton->width());

    emit menuWidthChanged(mMenuWidth);
}

void BurgerMenu::setExpansionState(bool expanded)
{
    if(mAnimated)
    {
        auto anim = new QPropertyAnimation(this, "minimumWidth", this);
        anim->setDuration(250);
        anim->setStartValue(mBurgerButton->width());
        anim->setEndValue(mBurgerButton->width() + mMenuWidth);
        anim->setDirection(expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        anim->setEasingCurve(expanded ? QEasingCurve::OutCubic : QEasingCurve::InCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        if(expanded)
            setFixedWidth(mBurgerButton->width() + mMenuWidth);
        else
            setFixedWidth(mBurgerButton->width());
    }
}

void BurgerMenu::registerAction(QAction* action)
{
    auto button = new BurgerButton(action, this);
    button->setObjectName(ObjectNames::BurgerButton);
    button->setIconSize(mBurgerButton->iconSize());
    auto lay = static_cast<QVBoxLayout*>(layout());
    lay->insertWidget(lay->count() - 1, button);
    mActionButtons << button;
}

void BurgerMenu::unRegisterAction(QAction* action)
{
    auto btn = std::find_if(mActionButtons.begin(), mActionButtons.end(), [&](BurgerButton* btn){ return btn->action() == action; });
    if(btn != mActionButtons.end())
    {
        delete *btn;
        mActionButtons.erase(btn);
    }
}

bool BurgerMenu::animated() const
{
    return mAnimated;
}

bool BurgerMenu::expanded() const
{
    return mBurgerButton->isChecked();
}

void BurgerMenu::setAnimated(bool animated)
{
    if(mAnimated == animated)
        return;

    mAnimated = animated;
    emit animatedChanged(mAnimated);
}

void BurgerMenu::setExpanded(bool expanded)
{
    mBurgerButton->setChecked(expanded);
}

void BurgerMenu::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
