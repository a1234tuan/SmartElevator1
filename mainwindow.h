#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "elevator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    ElevatorSystem* m_elevatorSystem;
    QVector<QVBoxLayout*> m_elevatorLayouts;
    QVector<QLabel*> m_floorLabels;
    QVector<QLabel*> m_doorLabels;

    void createUI();
    void updateElevatorUI(int elevatorIndex);
    void keyPressEvent(QKeyEvent* event) override;
};
