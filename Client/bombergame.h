#ifndef MENUGAME_H
#define MENUGAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QComboBox>
#include "menu/menuWidget.h"
#include "game/gameWidget.h"
#include "connection/client/Game.h"

class BomberGame: public QGraphicsView
{
    Q_OBJECT
public:
    explicit BomberGame(QWidget* parent = Q_NULLPTR);
    void initGame();

private:
    QStackedLayout* allScreens;
    MenuWidget* menu;
    GameWidget* game;
    QComboBox* pageComboBox;

    Game* gameNetwork;

public slots:
    void startNetworking();
};

#endif // MENUGAME_H
