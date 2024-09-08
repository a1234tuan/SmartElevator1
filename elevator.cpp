#include "elevator.h"
#include <QDebug>

Button::Button(int floor, Direction direction, QObject* parent)
    : QObject(parent), m_floor(floor), m_direction(direction) {}

void Button::click() {
    m_clickCount++;
    if (m_clickCount == 1) {
        m_isLit = true;
    } else if (m_clickCount == 2) {
        m_isLit = false;
        m_clickCount = 0;
    }
    emit stateChanged();
}

Cabin::Cabin(int id, int totalFloors, QObject* parent)
    : QObject(parent), m_id(id) {
    for (int i = 1; i <= totalFloors; ++i) {
        m_buttons.push_back(new Button(i, Direction::IDLE, this));
    }
    connect(&m_moveTimer, &QTimer::timeout, this, &Cabin::move);
    m_moveTimer.start(2000); // Move every 2 seconds
}

void Cabin::move() {
    if (m_direction != Direction::IDLE) {
        m_currentFloor += (m_direction == Direction::UP) ? 1 : -1;
        emit floorChanged();
        updateDirection();
    }
}

void Cabin::requestFloor(int floor) {
    m_buttons[floor - 1]->click();
    updateDirection();
}

void Cabin::updateDirection() {
    Direction newDirection = Direction::IDLE;
    for (int i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]->isLit()) {
            if (i + 1 > m_currentFloor) newDirection = Direction::UP;
            else if (i + 1 < m_currentFloor) newDirection = Direction::DOWN;
            break;
        }
    }
    m_direction = newDirection;
}

void Cabin::openDoor() {
    m_doorOpen = true;
    emit doorStateChanged();
    QTimer::singleShot(3000, this, &Cabin::closeDoor);
}

void Cabin::closeDoor() {
    m_doorOpen = false;
    emit doorStateChanged();
}

ElevatorSystem::ElevatorSystem(int numFloors, int numElevators, QObject* parent)
    : QObject(parent), m_numFloors(numFloors) {
    for (int i = 0; i < numElevators; ++i) {
        m_cabins.push_back(new Cabin(i, numFloors, this));
    }
    for (int floor = 1; floor <= numFloors; ++floor) {
        QVector<Button*> floorButtons;
        floorButtons.push_back(new Button(floor, Direction::UP, this));
        floorButtons.push_back(new Button(floor, Direction::DOWN, this));
        m_externalButtons.push_back(floorButtons);
    }
}

void ElevatorSystem::requestElevator(int floor, Direction direction) {
    Cabin* bestElevator = selectBestElevator(floor, direction);
    bestElevator->requestFloor(floor);
}

Cabin* ElevatorSystem::selectBestElevator(int floor, Direction direction) {
    // Simple selection algorithm - choose the closest elevator
    Cabin* bestElevator = m_cabins[0];
    int minDistance = std::abs(bestElevator->getCurrentFloor() - floor);

    for (Cabin* cabin : m_cabins) {
        int distance = std::abs(cabin->getCurrentFloor() - floor);
        if (distance < minDistance ||
            (distance == minDistance && cabin->getDirection() == direction)) {
            minDistance = distance;
            bestElevator = cabin;
        }
    }
    return bestElevator;
}
