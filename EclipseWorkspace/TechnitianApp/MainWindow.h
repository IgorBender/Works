/*
 * MainWindow.h
 *
 *  Created on: Oct 15, 2016
 *      Author: igor
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <qmainwindow.h>
#include <QtWidgets/QListWidget>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent = nullptr);
    ~MainWindow();

private slots:
    void on_CongListWidget_itemSelectionChanged();

    void on_MainTab_currentChanged(int index);

    void on_IpEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QPalette m_ConfIpAddrPalete;
};

#endif /* MAINWINDOW_H_ */
