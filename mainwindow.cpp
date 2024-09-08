#include "mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    m_elevatorSystem = new ElevatorSystem(30, 2, this);
    createUI();
    setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::createUI() {
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    for (int i = 0; i < 2; ++i) {
        QVBoxLayout* elevatorLayout = new QVBoxLayout();
        m_elevatorLayouts.push_back(elevatorLayout);

        QLabel* floorLabel = new QLabel(QString("Elevator %1 - Floor: 1").arg(i + 1));
        m_floorLabels.push_back(floorLabel);
        elevatorLayout->addWidget(floorLabel);

        QLabel* doorLabel = new QLabel("Door: Closed");
        m_doorLabels.push_back(doorLabel);
        elevatorLayout->addWidget(doorLabel);

        for (int floor = m_elevatorSystem->getNumFloors(); floor >= 1; --floor) {
            QPushButton* floorButton = new QPushButton(QString::number(floor));
            connect(floorButton, &QPushButton::clicked, [this, i, floor]() {
                m_elevatorSystem->getCabin(i)->requestFloor(floor);
            });
            elevatorLayout->addWidget(floorButton);
        }

        mainLayout->addLayout(elevatorLayout);

        connect(m_elevatorSystem->getCabin(i), &Cabin::floorChanged, [this, i]() {
            updateElevatorUI(i);
        });
        connect(m_elevatorSystem->getCabin(i), &Cabin::doorStateChanged, [this, i]() {
            updateElevatorUI(i);
        });
    }

    setCentralWidget(centralWidget);
}

void MainWindow::updateElevatorUI(int elevatorIndex) {
    Cabin* cabin = m_elevatorSystem->getCabin(elevatorIndex);
    m_floorLabels[elevatorIndex]->setText(QString("Elevator %1 - Floor: %2")
                                              .arg(elevatorIndex + 1)
                                              .arg(cabin->getCurrentFloor()));
    m_doorLabels[elevatorIndex]->setText(QString("Door: %1")
                                             .arg(cabin->isDoorOpen() ? "Open" : "Closed"));
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        int currentFloor = m_elevatorSystem->getCabin(0)->getCurrentFloor();
        Direction direction = (event->key() == Qt::Key_Up) ? Direction::UP : Direction::DOWN;
        m_elevatorSystem->requestElevator(currentFloor, direction);
    }
    QMainWindow::keyPressEvent(event);
}
