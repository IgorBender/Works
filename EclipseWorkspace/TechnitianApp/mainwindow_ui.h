/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOW_UI_H
#define MAINWINDOW_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *MainTab;
    QWidget *tab;
    QGroupBox *StatusGroupBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *FpfaLabel;
    QLabel *FpgaStatus;
    QSpacerItem *horizontalSpacer;
    QLabel *PnaLabel;
    QLabel *PnaStatus;
    QWidget *ConfigTab;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *CongListWidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *ConfStackedWidget;
    QWidget *PnaPage;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *IpAddrabel;
    QLabel *PortLabel;
    QSpinBox *PortSpinBox;
    QLineEdit *IpEdit;
    QWidget *widget1;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *page_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(600, 500);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(600, 500));
        MainWindow->setMaximumSize(QSize(600, 500));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        MainTab = new QTabWidget(centralWidget);
        MainTab->setObjectName(QStringLiteral("MainTab"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        StatusGroupBox = new QGroupBox(tab);
        StatusGroupBox->setObjectName(QStringLiteral("StatusGroupBox"));
        StatusGroupBox->setGeometry(QRect(30, 20, 261, 71));
        widget = new QWidget(StatusGroupBox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 30, 221, 32));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        FpfaLabel = new QLabel(widget);
        FpfaLabel->setObjectName(QStringLiteral("FpfaLabel"));

        horizontalLayout_2->addWidget(FpfaLabel);

        FpgaStatus = new QLabel(widget);
        FpgaStatus->setObjectName(QStringLiteral("FpgaStatus"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(FpgaStatus->sizePolicy().hasHeightForWidth());
        FpgaStatus->setSizePolicy(sizePolicy1);
        FpgaStatus->setMinimumSize(QSize(30, 30));
        FpgaStatus->setMaximumSize(QSize(30, 30));

        horizontalLayout_2->addWidget(FpgaStatus);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        PnaLabel = new QLabel(widget);
        PnaLabel->setObjectName(QStringLiteral("PnaLabel"));

        horizontalLayout_2->addWidget(PnaLabel);

        PnaStatus = new QLabel(widget);
        PnaStatus->setObjectName(QStringLiteral("PnaStatus"));
        PnaStatus->setMinimumSize(QSize(30, 30));
        PnaStatus->setMaximumSize(QSize(30, 30));

        horizontalLayout_2->addWidget(PnaStatus);

        MainTab->addTab(tab, QString());
        ConfigTab = new QWidget();
        ConfigTab->setObjectName(QStringLiteral("ConfigTab"));
        gridLayout_2 = new QGridLayout(ConfigTab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        CongListWidget = new QListWidget(ConfigTab);
        CongListWidget->setObjectName(QStringLiteral("CongListWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(CongListWidget->sizePolicy().hasHeightForWidth());
        CongListWidget->setSizePolicy(sizePolicy2);
        CongListWidget->setMaximumSize(QSize(100, 16777215));
        CongListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        CongListWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
        CongListWidget->setDefaultDropAction(Qt::IgnoreAction);
        CongListWidget->setIconSize(QSize(40, 40));
        CongListWidget->setMovement(QListView::Static);
        CongListWidget->setResizeMode(QListView::Adjust);
        CongListWidget->setViewMode(QListView::IconMode);

        horizontalLayout->addWidget(CongListWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ConfStackedWidget = new QStackedWidget(ConfigTab);
        ConfStackedWidget->setObjectName(QStringLiteral("ConfStackedWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ConfStackedWidget->sizePolicy().hasHeightForWidth());
        ConfStackedWidget->setSizePolicy(sizePolicy3);
        PnaPage = new QWidget();
        PnaPage->setObjectName(QStringLiteral("PnaPage"));
        layoutWidget = new QWidget(PnaPage);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 211, 85));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        IpAddrabel = new QLabel(layoutWidget);
        IpAddrabel->setObjectName(QStringLiteral("IpAddrabel"));
        IpAddrabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, IpAddrabel);

        PortLabel = new QLabel(layoutWidget);
        PortLabel->setObjectName(QStringLiteral("PortLabel"));
        PortLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, PortLabel);

        PortSpinBox = new QSpinBox(layoutWidget);
        PortSpinBox->setObjectName(QStringLiteral("PortSpinBox"));
        PortSpinBox->setMaximum(65534);

        formLayout->setWidget(1, QFormLayout::FieldRole, PortSpinBox);

        IpEdit = new QLineEdit(layoutWidget);
        IpEdit->setObjectName(QStringLiteral("IpEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, IpEdit);

        widget1 = new QWidget(PnaPage);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(0, 310, 450, 70));
        sizePolicy1.setHeightForWidth(widget1->sizePolicy().hasHeightForWidth());
        widget1->setSizePolicy(sizePolicy1);
        widget1->setMinimumSize(QSize(0, 70));
        pushButton = new QPushButton(PnaPage);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(210, 370, 99, 27));
        pushButton_2 = new QPushButton(PnaPage);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(330, 370, 99, 27));
        ConfStackedWidget->addWidget(PnaPage);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        ConfStackedWidget->addWidget(page_2);

        verticalLayout->addWidget(ConfStackedWidget);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainTab->addTab(ConfigTab, QString());

        gridLayout->addWidget(MainTab, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered(bool)), MainWindow, SLOT(close()));

        MainTab->setCurrentIndex(0);
        ConfStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        StatusGroupBox->setTitle(QApplication::translate("MainWindow", "Status", 0));
        FpfaLabel->setText(QApplication::translate("MainWindow", "FPGA", 0));
        FpgaStatus->setText(QString());
        PnaLabel->setText(QApplication::translate("MainWindow", "PNA", 0));
        PnaStatus->setText(QString());
        MainTab->setTabText(MainTab->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
        IpAddrabel->setText(QApplication::translate("MainWindow", "IP address", 0));
        PortLabel->setText(QApplication::translate("MainWindow", "Port", 0));
        IpEdit->setInputMask(QString());
        IpEdit->setText(QString());
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", 0));
        MainTab->setTabText(MainTab->indexOf(ConfigTab), QApplication::translate("MainWindow", "Configuration", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_UI_H
