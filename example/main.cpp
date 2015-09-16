#include <QApplication>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QIcon>
#include <QAction>
#include "../src/burgermenu.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setMinimumSize(500, 400);

    BurgerMenu* menu     = new BurgerMenu();
    QTextEdit*  textEdit = new QTextEdit();

    window.setLayout(new QHBoxLayout());
    window.layout()->setContentsMargins(0,0,0,0);
    window.layout()->setSpacing(0);
    window.layout()->addWidget(menu);
    window.layout()->addWidget(textEdit);

    menu->setMenuWidth(100);
    menu->setBurgerIcon(QIcon(":/images/burger.png"));
    menu->addMenuAction(QIcon(":/images/collections.png"), "Collection");
    menu->addMenuAction(QIcon(":/images/folders.png"),     "Folders");
    menu->addMenuAction(QIcon(":/images/albums.png"),      "Albums");

    window.setStyleSheet("BurgerMenu                  { background-color: black;   }"
                         "#BurgerMenu                 { background-color: black;   }"
                         "#BurgerButton               { background-color: black;   color: white; font-size: 18px; }"
                         "#BurgerButton:hover         { background-color: #3480D2; }"
                         "#BurgerButton:checked       { background-color: #106EBE; }"
                         "#BurgerButton:checked:hover { background-color: #3480D2; }"
                         "#MainBurgerButton           { background-color: black;   border: none; } "
                         "#MainBurgerButton:hover     { background-color: #333;    } "
                        );

    QObject::connect(menu, &BurgerMenu::triggered, [&](QAction* action)
    {
        textEdit->setText(QString("Action \"%1\" clicked.").arg(action->iconText()));
    });

    window.show();
    return a.exec();
}
