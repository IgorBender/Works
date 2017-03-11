/*
 * MainWindow.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: igor
 */

#include <iostream>

#include "MainWindow.h"
#include "mainwindow_ui.h"
#include "IpAddressValidator.h"

using namespace std;


MainWindow::MainWindow(QWidget* pParent) : QMainWindow(pParent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ConfIpAddrPalete = ui->IpEdit->palette();
    ui->IpEdit->setValidator(new IpAddressValidator);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/Pna.png"), QSize(), QIcon::Selected, QIcon::On);
    QFont font;
    font.setPointSize(12);
    QListWidgetItem *PnaConfigListItem = new QListWidgetItem(ui->CongListWidget);
    PnaConfigListItem->setText(QString::fromUtf8("PNA"));
    PnaConfigListItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignTop);
    PnaConfigListItem->setFont(font);
    PnaConfigListItem->setIcon(icon);
    PnaConfigListItem->setSizeHint(QSize(100, 65));
    QListWidgetItem *Item2ConfigListItem = new QListWidgetItem(ui->CongListWidget);
    Item2ConfigListItem->setText(QString::fromUtf8("Item2"));
    Item2ConfigListItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignTop);
    Item2ConfigListItem->setFont(font);
    Item2ConfigListItem->setIcon(QIcon(":/icons/kcmpci.png"));
    Item2ConfigListItem->setSizeHint(QSize(100, 65));
    ui->CongListWidget->setCurrentItem(PnaConfigListItem, QItemSelectionModel::SelectCurrent);
    ui->MainTab->setCurrentWidget(ui->tab);
    ui->MainTab->setStyleSheet("QGroupBox {\
            border: 2px solid gray;\
        }");
    ui->StatusGroupBox->setStyleSheet("QGroupBox::title {\
            color: blue;\
            border: 2px solid gray;\
            border-bottom: none;\
            border-right: none;\
    padding: 2 5px;\
    }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CongListWidget_itemSelectionChanged()
{
    QListWidgetItem *item = ui->CongListWidget->currentItem();
    if("PNA" == item->text().toStdString())
    {
        ui->ConfStackedWidget->setCurrentWidget(ui->PnaPage);
    }
    else
    {
        ui->ConfStackedWidget->setCurrentWidget(ui->page_2);
    }
}

void MainWindow::on_MainTab_currentChanged(int index)
{
    if(1 == index)
    {
        ui->CongListWidget->setFocus();
    }
}

void MainWindow::on_IpEdit_textChanged(const QString &arg1)
{
    if(ui->IpEdit->hasAcceptableInput())
    {
        m_ConfIpAddrPalete.setColor(ui->IpEdit->backgroundRole(), QColor(100, 200, 100, 100));
        ui->IpEdit->setPalette(m_ConfIpAddrPalete);
    }
    else
    {
        m_ConfIpAddrPalete.setColor(ui->IpEdit->backgroundRole(), QColor(200, 100, 100, 100));
        ui->IpEdit->setPalette(m_ConfIpAddrPalete);
    }
}
