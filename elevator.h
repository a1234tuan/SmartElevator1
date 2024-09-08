#pragma once

#include <QObject>
#include <QVector>
#include <QTimer>

enum class Direction { UP, DOWN, IDLE };

class Button : public QObject {
    Q_OBJECT
public:
    explicit Button(int floor, Direction direction = Direction::IDLE, QObject* parent = nullptr);
    void click();
    bool isLit() const { return m_isLit; }
    int getFloor() const { return m_floor; }
    Direction getDirection() const { return m_direction; }

signals:
    void stateChanged();

private:
    int m_floor;
    Direction m_direction;
    bool m_isLit = false;
    int m_clickCount = 0;
};

class Cabin : public QObject {
    Q_OBJECT
public:
    explicit Cabin(int id, int totalFloors, QObject* parent = nullptr);
    void move();
    void requestFloor(int floor);
    int getCurrentFloor() const { return m_currentFloor; }
    Direction getDirection() const { return m_direction; }
    bool isDoorOpen() const { return m_doorOpen; }

signals:
    void floorChanged();
    void doorStateChanged();

private:
    int m_id;
    int m_currentFloor = 1;
    Direction m_direction = Direction::IDLE;
    QVector<Button*> m_buttons;
    bool m_doorOpen = false;
    QTimer m_moveTimer;

    void updateDirection();
    void openDoor();
    void closeDoor();
};

class ElevatorSystem : public QObject {
    Q_OBJECT
public:
    explicit ElevatorSystem(int numFloors, int numElevators, QObject* parent = nullptr);
    void requestElevator(int floor, Direction direction);
    Cabin* getCabin(int index) { return m_cabins[index]; }
    int getNumFloors() const { return m_numFloors; }

private:
    int m_numFloors;
    QVector<Cabin*> m_cabins;
    QVector<QVector<Button*>> m_externalButtons;

    Cabin* selectBestElevator(int floor, Direction direction);
};
