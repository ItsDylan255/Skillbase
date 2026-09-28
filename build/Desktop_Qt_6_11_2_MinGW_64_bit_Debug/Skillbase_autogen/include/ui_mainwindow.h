/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <exerciseprogresschartwidget.h>
#include <timelinebarwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_9;
    QWidget *sidebar;
    QVBoxLayout *verticalLayout_2;
    QLabel *sidebarTitleLabel;
    QPushButton *dashboardButton;
    QLabel *sidebarHobbySectionLabel;
    QListWidget *hobbyList;
    QPushButton *settingsButton;
    QFrame *line;
    QWidget *content;
    QVBoxLayout *contentLayout;
    QStackedWidget *pageStack;
    QWidget *dashboardPage;
    QVBoxLayout *dashboardPageLayout;
    QLabel *dashboardTitleLabel;
    QLabel *dashboardSubtitleLabel;
    QSpacerItem *dashboardHeaderSpacer;
    QFrame *dashboardDivider1;
    QHBoxLayout *dashboardTopRowLayout;
    QVBoxLayout *dashboardTodayColumnLayout;
    QLabel *todaySectionLabel;
    QLabel *todayLabel;
    QVBoxLayout *dashboardGoalsColumnLayout;
    QLabel *goalsSectionLabel;
    QLabel *goalsSummaryLabel;
    QSpacerItem *dashboardTopRowSpacer;
    QFrame *dashboardDivider2;
    QLabel *progressSectionLabel;
    QLabel *progressSummaryLabel;
    QFrame *dashboardDivider3;
    QLabel *recentActivitySectionLabel;
    QListWidget *recentActivityList;
    QWidget *hobbyPage;
    QVBoxLayout *hobbyPageLayout;
    QLabel *hobbyLabel;
    QHBoxLayout *hobbyNavigation;
    QToolButton *dashboardTab;
    QToolButton *routinesTab;
    QToolButton *exercisesTab;
    QToolButton *goalsTab;
    QToolButton *roadmapTab;
    QToolButton *historyTab;
    QStackedWidget *hobbyPageStack;
    QWidget *dashboardHobbyPage;
    QVBoxLayout *hobbyDashboardLayout;
    QFrame *hobbyDashboardDivider1;
    QHBoxLayout *hobbyStatusRowLayout;
    QFrame *hobbyPhaseSection;
    QVBoxLayout *hobbyPhaseColumnLayout;
    QLabel *hobbyPhaseSectionLabel;
    QLabel *hobbyPhaseNameLabel;
    QLabel *hobbyPhaseCountdownLabel;
    QProgressBar *hobbyPhaseProgressBar;
    QFrame *hobbyGoalSection;
    QVBoxLayout *hobbyGoalColumnLayout;
    QLabel *hobbyGoalSectionLabel;
    QLabel *hobbyGoalNameLabel;
    QLabel *hobbyGoalDeadlineLabel;
    QFrame *hobbyDashboardDivider2;
    QLabel *hobbyRoutinesSectionLabel;
    QWidget *hobbyRoutinesContainer;
    QHBoxLayout *hobbyRoutinesContainerLayout;
    QLabel *hobbyRoutinesEmptyLabel;
    QPushButton *hobbyRoutinesAddButton;
    QFrame *hobbyDashboardDivider3;
    QLabel *hobbyNoteSectionLabel;
    QPlainTextEdit *hobbyNotesTextEdit;
    QFrame *hobbyDashboardDivider4;
    QLabel *hobbyStatsSectionLabel;
    QHBoxLayout *hobbyStatsRowLayout;
    QVBoxLayout *hobbyStatExercisesLayout;
    QLabel *hobbyStatExercisesValueLabel;
    QLabel *hobbyStatExercisesCaptionLabel;
    QVBoxLayout *hobbyStatTimeLayout;
    QLabel *hobbyStatTimeValueLabel;
    QLabel *hobbyStatTimeCaptionLabel;
    QVBoxLayout *hobbyStatSessionsLayout;
    QLabel *hobbyStatSessionsValueLabel;
    QLabel *hobbyStatSessionsCaptionLabel;
    QVBoxLayout *hobbyStatGoalsLayout;
    QLabel *hobbyStatGoalsValueLabel;
    QLabel *hobbyStatGoalsCaptionLabel;
    QSpacerItem *hobbyStatsRowSpacer;
    QWidget *roadmapPage;
    QVBoxLayout *roadmapPageLayout;
    QHBoxLayout *roadmapHeaderLayout;
    QLabel *roadmapHeaderLabel;
    QSpacerItem *roadmapHeaderSpacer;
    QPushButton *addRoadmapGoalButton;
    QHBoxLayout *roadmapViewToggleLayout;
    QToolButton *roadmapTreeViewButton;
    QToolButton *roadmapDiagramViewButton;
    QSpacerItem *roadmapViewToggleSpacer;
    QSplitter *roadmapSplitter;
    QStackedWidget *roadmapViewStack;
    QWidget *roadmapTreePage;
    QVBoxLayout *roadmapTreePageLayout;
    QTreeWidget *roadmapTreeWidget;
    QWidget *roadmapDiagramPage;
    QVBoxLayout *roadmapDiagramPageLayout;
    QWidget *roadmapDiagramWidget;
    QWidget *roadmapInfoPanel;
    QVBoxLayout *roadmapInfoLayout;
    QLabel *roadmapInfoTitle;
    QFrame *roadmapInfoSeparator;
    QLabel *roadmapInfoNameLabel;
    QLabel *roadmapInfoStatusHeader;
    QLabel *roadmapInfoStatusLabel;
    QLabel *roadmapInfoDescriptionHeader;
    QLabel *roadmapInfoDescriptionLabel;
    QLabel *roadmapInfoParentHeader;
    QLabel *roadmapInfoParentLabel;
    QSpacerItem *roadmapInfoSpacer;
    QWidget *timelinePage;
    QVBoxLayout *timelinePageLayout;
    QHBoxLayout *timelineHeaderLayout;
    QLabel *timelineHeaderLabel;
    QSpacerItem *timelineHeaderSpacer;
    QPushButton *addTimelinePhaseButton;
    QScrollArea *timelineBarScrollArea;
    TimelineBarWidget *timelineBarWidget;
    QLabel *timelineEmptyLabel;
    QSpacerItem *timelinePageSpacer;
    QWidget *routinesPage;
    QVBoxLayout *routinesPageLayout;
    QListWidget *routineList;
    QPushButton *addRoutineButton;
    QWidget *exercisesPage;
    QVBoxLayout *exercisesPageLayout;
    QStackedWidget *exerciseViewStack;
    QWidget *exerciseOverviewPage;
    QVBoxLayout *exerciseOverviewPageLayout;
    QHBoxLayout *exercisesHeaderLayout;
    QLabel *exercisesHeaderLabel;
    QSpacerItem *exercisesHeaderSpacer;
    QPushButton *addExerciseButton;
    QHBoxLayout *exercisesFilterLayout;
    QLineEdit *exerciseSearchLineEdit;
    QComboBox *exerciseCategoryComboBox;
    QPushButton *manageCategoriesButton;
    QScrollArea *exerciseScrollArea;
    QWidget *exerciseCardsWidget;
    QGridLayout *exerciseCardsLayout;
    QWidget *exerciseDetailPage;
    QVBoxLayout *exerciseDetailPageLayout;
    QVBoxLayout *exerciseDetailHeaderLayout;
    QLabel *exerciseDetailNameLabel;
    QLabel *exerciseDetailDescriptionLabel;
    QHBoxLayout *exerciseDetailCategoryRow;
    QLabel *exerciseDetailCategoryTagLabel;
    QSpacerItem *exerciseDetailCategorySpacer;
    QGroupBox *exerciseDetailProgressCard;
    QVBoxLayout *exerciseDetailProgressCardLayout;
    QHBoxLayout *exerciseDetailValuesRow;
    QVBoxLayout *exerciseDetailStartValueColumn;
    QLabel *exerciseDetailStartHeaderLabel;
    QHBoxLayout *exerciseDetailStartValueRow;
    QLabel *exerciseDetailStartValueLabel;
    QPushButton *exerciseDetailEditStartValueButton;
    QVBoxLayout *exerciseDetailCurrentValueColumn;
    QLabel *exerciseDetailCurrentHeaderLabel;
    QLabel *exerciseDetailCurrentValueLabel;
    QVBoxLayout *exerciseDetailGoalValueColumn;
    QLabel *exerciseDetailGoalHeaderLabel;
    QLabel *exerciseDetailGoalValueLabel;
    QSpacerItem *exerciseDetailValuesSpacer;
    QProgressBar *exerciseDetailProgressBar;
    QLabel *exerciseDetailPercentLabel;
    QHBoxLayout *exerciseDetailBottomRow;
    QGroupBox *exerciseDetailDevelopmentCard;
    QVBoxLayout *exerciseDetailDevelopmentCardLayout;
    ExerciseProgressChartWidget *exerciseProgressChartWidget;
    QGroupBox *exerciseDetailStatisticsCard;
    QFormLayout *exerciseDetailStatisticsCardLayout;
    QLabel *exerciseDetailLastPerformedHeaderLabel;
    QLabel *exerciseDetailLastPerformedLabel;
    QLabel *exerciseDetailExecutionCountHeaderLabel;
    QLabel *exerciseDetailExecutionCountLabel;
    QLabel *exerciseDetailTotalTimeHeaderLabel;
    QLabel *exerciseDetailTotalTimeLabel;
    QWidget *historyPage;
    QVBoxLayout *historyPageLayout;
    QHBoxLayout *historyHeaderLayout;
    QLabel *historyHeaderLabel;
    QSpacerItem *historyHeaderSpacer;
    QHBoxLayout *historyToolbarLayout;
    QLineEdit *historySearchLineEdit;
    QSpacerItem *historyToolbarSpacer;
    QScrollArea *historyScrollArea;
    QWidget *historyEntriesWidget;
    QVBoxLayout *historyEntriesLayout;
    QLabel *historyEmptyStateLabel;
    QWidget *goalsPage;
    QVBoxLayout *goalsPageLayout;
    QHBoxLayout *goalsHeaderLayout;
    QLabel *goalsHeaderLabel;
    QSpacerItem *goalsHeaderSpacer;
    QPushButton *addGoalButton;
    QHBoxLayout *goalsFilterLayout;
    QLineEdit *goalSearchLineEdit;
    QToolButton *goalsFilterOpenButton;
    QToolButton *goalsFilterDoneButton;
    QStackedWidget *goalsViewStack;
    QWidget *goalsOpenPage;
    QVBoxLayout *goalsOpenPageLayout;
    QLabel *goalsOpenEmptyLabel;
    QScrollArea *goalsOpenScrollArea;
    QWidget *goalOpenCardsWidget;
    QGridLayout *goalOpenCardsLayout;
    QWidget *goalsDonePage;
    QVBoxLayout *goalsDonePageLayout;
    QLabel *goalsDoneEmptyLabel;
    QScrollArea *goalsDoneScrollArea;
    QWidget *goalDoneCardsWidget;
    QGridLayout *goalDoneCardsLayout;
    QWidget *settingsPage;
    QVBoxLayout *settingsPageLayout;
    QLabel *settingsTitleLabel;
    QSpacerItem *settingsPageSpacer;
    QWidget *statisticsPage;
    QVBoxLayout *statisticsPageLayout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1110, 656);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_9 = new QHBoxLayout(centralwidget);
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(200, 0));
        sidebar->setMaximumSize(QSize(200, 16777215));
        verticalLayout_2 = new QVBoxLayout(sidebar);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(12, 16, 12, 16);
        sidebarTitleLabel = new QLabel(sidebar);
        sidebarTitleLabel->setObjectName("sidebarTitleLabel");

        verticalLayout_2->addWidget(sidebarTitleLabel);

        dashboardButton = new QPushButton(sidebar);
        dashboardButton->setObjectName("dashboardButton");
        dashboardButton->setCheckable(true);
        dashboardButton->setChecked(true);
        dashboardButton->setAutoExclusive(true);

        verticalLayout_2->addWidget(dashboardButton);

        sidebarHobbySectionLabel = new QLabel(sidebar);
        sidebarHobbySectionLabel->setObjectName("sidebarHobbySectionLabel");

        verticalLayout_2->addWidget(sidebarHobbySectionLabel);

        hobbyList = new QListWidget(sidebar);
        hobbyList->setObjectName("hobbyList");

        verticalLayout_2->addWidget(hobbyList);

        settingsButton = new QPushButton(sidebar);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setCheckable(true);
        settingsButton->setAutoExclusive(true);

        verticalLayout_2->addWidget(settingsButton);


        horizontalLayout_9->addWidget(sidebar);

        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        horizontalLayout_9->addWidget(line);

        content = new QWidget(centralwidget);
        content->setObjectName("content");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(content->sizePolicy().hasHeightForWidth());
        content->setSizePolicy(sizePolicy);
        contentLayout = new QVBoxLayout(content);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        pageStack = new QStackedWidget(content);
        pageStack->setObjectName("pageStack");
        dashboardPage = new QWidget();
        dashboardPage->setObjectName("dashboardPage");
        dashboardPageLayout = new QVBoxLayout(dashboardPage);
        dashboardPageLayout->setSpacing(4);
        dashboardPageLayout->setObjectName("dashboardPageLayout");
        dashboardPageLayout->setContentsMargins(24, 24, 24, 24);
        dashboardTitleLabel = new QLabel(dashboardPage);
        dashboardTitleLabel->setObjectName("dashboardTitleLabel");

        dashboardPageLayout->addWidget(dashboardTitleLabel);

        dashboardSubtitleLabel = new QLabel(dashboardPage);
        dashboardSubtitleLabel->setObjectName("dashboardSubtitleLabel");

        dashboardPageLayout->addWidget(dashboardSubtitleLabel);

        dashboardHeaderSpacer = new QSpacerItem(0, 12, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        dashboardPageLayout->addItem(dashboardHeaderSpacer);

        dashboardDivider1 = new QFrame(dashboardPage);
        dashboardDivider1->setObjectName("dashboardDivider1");
        dashboardDivider1->setFrameShape(QFrame::Shape::HLine);
        dashboardDivider1->setFrameShadow(QFrame::Shadow::Plain);

        dashboardPageLayout->addWidget(dashboardDivider1);

        dashboardTopRowLayout = new QHBoxLayout();
        dashboardTopRowLayout->setSpacing(48);
        dashboardTopRowLayout->setObjectName("dashboardTopRowLayout");
        dashboardTodayColumnLayout = new QVBoxLayout();
        dashboardTodayColumnLayout->setSpacing(4);
        dashboardTodayColumnLayout->setObjectName("dashboardTodayColumnLayout");
        todaySectionLabel = new QLabel(dashboardPage);
        todaySectionLabel->setObjectName("todaySectionLabel");

        dashboardTodayColumnLayout->addWidget(todaySectionLabel);

        todayLabel = new QLabel(dashboardPage);
        todayLabel->setObjectName("todayLabel");
        todayLabel->setWordWrap(true);

        dashboardTodayColumnLayout->addWidget(todayLabel);


        dashboardTopRowLayout->addLayout(dashboardTodayColumnLayout);

        dashboardGoalsColumnLayout = new QVBoxLayout();
        dashboardGoalsColumnLayout->setSpacing(4);
        dashboardGoalsColumnLayout->setObjectName("dashboardGoalsColumnLayout");
        goalsSectionLabel = new QLabel(dashboardPage);
        goalsSectionLabel->setObjectName("goalsSectionLabel");

        dashboardGoalsColumnLayout->addWidget(goalsSectionLabel);

        goalsSummaryLabel = new QLabel(dashboardPage);
        goalsSummaryLabel->setObjectName("goalsSummaryLabel");
        goalsSummaryLabel->setWordWrap(true);

        dashboardGoalsColumnLayout->addWidget(goalsSummaryLabel);


        dashboardTopRowLayout->addLayout(dashboardGoalsColumnLayout);

        dashboardTopRowSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        dashboardTopRowLayout->addItem(dashboardTopRowSpacer);


        dashboardPageLayout->addLayout(dashboardTopRowLayout);

        dashboardDivider2 = new QFrame(dashboardPage);
        dashboardDivider2->setObjectName("dashboardDivider2");
        dashboardDivider2->setFrameShape(QFrame::Shape::HLine);
        dashboardDivider2->setFrameShadow(QFrame::Shadow::Plain);

        dashboardPageLayout->addWidget(dashboardDivider2);

        progressSectionLabel = new QLabel(dashboardPage);
        progressSectionLabel->setObjectName("progressSectionLabel");

        dashboardPageLayout->addWidget(progressSectionLabel);

        progressSummaryLabel = new QLabel(dashboardPage);
        progressSummaryLabel->setObjectName("progressSummaryLabel");
        progressSummaryLabel->setWordWrap(true);

        dashboardPageLayout->addWidget(progressSummaryLabel);

        dashboardDivider3 = new QFrame(dashboardPage);
        dashboardDivider3->setObjectName("dashboardDivider3");
        dashboardDivider3->setFrameShape(QFrame::Shape::HLine);
        dashboardDivider3->setFrameShadow(QFrame::Shadow::Plain);

        dashboardPageLayout->addWidget(dashboardDivider3);

        recentActivitySectionLabel = new QLabel(dashboardPage);
        recentActivitySectionLabel->setObjectName("recentActivitySectionLabel");

        dashboardPageLayout->addWidget(recentActivitySectionLabel);

        recentActivityList = new QListWidget(dashboardPage);
        recentActivityList->setObjectName("recentActivityList");

        dashboardPageLayout->addWidget(recentActivityList);

        pageStack->addWidget(dashboardPage);
        hobbyPage = new QWidget();
        hobbyPage->setObjectName("hobbyPage");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(hobbyPage->sizePolicy().hasHeightForWidth());
        hobbyPage->setSizePolicy(sizePolicy1);
        hobbyPageLayout = new QVBoxLayout(hobbyPage);
        hobbyPageLayout->setSpacing(16);
        hobbyPageLayout->setObjectName("hobbyPageLayout");
        hobbyPageLayout->setContentsMargins(24, 24, 24, 24);
        hobbyLabel = new QLabel(hobbyPage);
        hobbyLabel->setObjectName("hobbyLabel");

        hobbyPageLayout->addWidget(hobbyLabel);

        hobbyNavigation = new QHBoxLayout();
        hobbyNavigation->setObjectName("hobbyNavigation");
        hobbyNavigation->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        dashboardTab = new QToolButton(hobbyPage);
        dashboardTab->setObjectName("dashboardTab");
        dashboardTab->setCheckable(true);
        dashboardTab->setChecked(true);
        dashboardTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(dashboardTab);

        routinesTab = new QToolButton(hobbyPage);
        routinesTab->setObjectName("routinesTab");
        routinesTab->setCheckable(true);
        routinesTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(routinesTab);

        exercisesTab = new QToolButton(hobbyPage);
        exercisesTab->setObjectName("exercisesTab");
        exercisesTab->setCheckable(true);
        exercisesTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(exercisesTab);

        goalsTab = new QToolButton(hobbyPage);
        goalsTab->setObjectName("goalsTab");
        goalsTab->setCheckable(true);
        goalsTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(goalsTab);

        roadmapTab = new QToolButton(hobbyPage);
        roadmapTab->setObjectName("roadmapTab");
        roadmapTab->setCheckable(true);
        roadmapTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(roadmapTab);

        historyTab = new QToolButton(hobbyPage);
        historyTab->setObjectName("historyTab");
        historyTab->setCheckable(true);
        historyTab->setAutoExclusive(true);

        hobbyNavigation->addWidget(historyTab);


        hobbyPageLayout->addLayout(hobbyNavigation);

        hobbyPageStack = new QStackedWidget(hobbyPage);
        hobbyPageStack->setObjectName("hobbyPageStack");
        dashboardHobbyPage = new QWidget();
        dashboardHobbyPage->setObjectName("dashboardHobbyPage");
        hobbyDashboardLayout = new QVBoxLayout(dashboardHobbyPage);
        hobbyDashboardLayout->setSpacing(4);
        hobbyDashboardLayout->setObjectName("hobbyDashboardLayout");
        hobbyDashboardLayout->setContentsMargins(4, 8, 4, 12);
        hobbyDashboardDivider1 = new QFrame(dashboardHobbyPage);
        hobbyDashboardDivider1->setObjectName("hobbyDashboardDivider1");
        hobbyDashboardDivider1->setFrameShape(QFrame::Shape::HLine);
        hobbyDashboardDivider1->setFrameShadow(QFrame::Shadow::Plain);

        hobbyDashboardLayout->addWidget(hobbyDashboardDivider1);

        hobbyStatusRowLayout = new QHBoxLayout();
        hobbyStatusRowLayout->setSpacing(16);
        hobbyStatusRowLayout->setObjectName("hobbyStatusRowLayout");
        hobbyPhaseSection = new QFrame(dashboardHobbyPage);
        hobbyPhaseSection->setObjectName("hobbyPhaseSection");
        hobbyPhaseSection->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        hobbyPhaseSection->setFrameShape(QFrame::Shape::StyledPanel);
        hobbyPhaseSection->setFrameShadow(QFrame::Shadow::Plain);
        hobbyPhaseColumnLayout = new QVBoxLayout(hobbyPhaseSection);
        hobbyPhaseColumnLayout->setSpacing(4);
        hobbyPhaseColumnLayout->setObjectName("hobbyPhaseColumnLayout");
        hobbyPhaseColumnLayout->setContentsMargins(12, 12, 12, 12);
        hobbyPhaseSectionLabel = new QLabel(hobbyPhaseSection);
        hobbyPhaseSectionLabel->setObjectName("hobbyPhaseSectionLabel");

        hobbyPhaseColumnLayout->addWidget(hobbyPhaseSectionLabel);

        hobbyPhaseNameLabel = new QLabel(hobbyPhaseSection);
        hobbyPhaseNameLabel->setObjectName("hobbyPhaseNameLabel");
        hobbyPhaseNameLabel->setWordWrap(true);

        hobbyPhaseColumnLayout->addWidget(hobbyPhaseNameLabel);

        hobbyPhaseCountdownLabel = new QLabel(hobbyPhaseSection);
        hobbyPhaseCountdownLabel->setObjectName("hobbyPhaseCountdownLabel");

        hobbyPhaseColumnLayout->addWidget(hobbyPhaseCountdownLabel);

        hobbyPhaseProgressBar = new QProgressBar(hobbyPhaseSection);
        hobbyPhaseProgressBar->setObjectName("hobbyPhaseProgressBar");
        hobbyPhaseProgressBar->setValue(0);
        hobbyPhaseProgressBar->setTextVisible(false);

        hobbyPhaseColumnLayout->addWidget(hobbyPhaseProgressBar);


        hobbyStatusRowLayout->addWidget(hobbyPhaseSection);

        hobbyGoalSection = new QFrame(dashboardHobbyPage);
        hobbyGoalSection->setObjectName("hobbyGoalSection");
        hobbyGoalSection->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        hobbyGoalSection->setFrameShape(QFrame::Shape::StyledPanel);
        hobbyGoalSection->setFrameShadow(QFrame::Shadow::Plain);
        hobbyGoalColumnLayout = new QVBoxLayout(hobbyGoalSection);
        hobbyGoalColumnLayout->setSpacing(4);
        hobbyGoalColumnLayout->setObjectName("hobbyGoalColumnLayout");
        hobbyGoalColumnLayout->setContentsMargins(12, 12, 12, 12);
        hobbyGoalSectionLabel = new QLabel(hobbyGoalSection);
        hobbyGoalSectionLabel->setObjectName("hobbyGoalSectionLabel");

        hobbyGoalColumnLayout->addWidget(hobbyGoalSectionLabel);

        hobbyGoalNameLabel = new QLabel(hobbyGoalSection);
        hobbyGoalNameLabel->setObjectName("hobbyGoalNameLabel");
        hobbyGoalNameLabel->setWordWrap(true);

        hobbyGoalColumnLayout->addWidget(hobbyGoalNameLabel);

        hobbyGoalDeadlineLabel = new QLabel(hobbyGoalSection);
        hobbyGoalDeadlineLabel->setObjectName("hobbyGoalDeadlineLabel");

        hobbyGoalColumnLayout->addWidget(hobbyGoalDeadlineLabel);


        hobbyStatusRowLayout->addWidget(hobbyGoalSection);


        hobbyDashboardLayout->addLayout(hobbyStatusRowLayout);

        hobbyDashboardDivider2 = new QFrame(dashboardHobbyPage);
        hobbyDashboardDivider2->setObjectName("hobbyDashboardDivider2");
        hobbyDashboardDivider2->setFrameShape(QFrame::Shape::HLine);
        hobbyDashboardDivider2->setFrameShadow(QFrame::Shadow::Plain);

        hobbyDashboardLayout->addWidget(hobbyDashboardDivider2);

        hobbyRoutinesSectionLabel = new QLabel(dashboardHobbyPage);
        hobbyRoutinesSectionLabel->setObjectName("hobbyRoutinesSectionLabel");

        hobbyDashboardLayout->addWidget(hobbyRoutinesSectionLabel);

        hobbyRoutinesContainer = new QWidget(dashboardHobbyPage);
        hobbyRoutinesContainer->setObjectName("hobbyRoutinesContainer");
        hobbyRoutinesContainerLayout = new QHBoxLayout(hobbyRoutinesContainer);
        hobbyRoutinesContainerLayout->setSpacing(12);
        hobbyRoutinesContainerLayout->setObjectName("hobbyRoutinesContainerLayout");
        hobbyRoutinesContainerLayout->setContentsMargins(0, 0, 0, 0);

        hobbyDashboardLayout->addWidget(hobbyRoutinesContainer);

        hobbyRoutinesEmptyLabel = new QLabel(dashboardHobbyPage);
        hobbyRoutinesEmptyLabel->setObjectName("hobbyRoutinesEmptyLabel");
        hobbyRoutinesEmptyLabel->setWordWrap(true);

        hobbyDashboardLayout->addWidget(hobbyRoutinesEmptyLabel);

        hobbyRoutinesAddButton = new QPushButton(dashboardHobbyPage);
        hobbyRoutinesAddButton->setObjectName("hobbyRoutinesAddButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(hobbyRoutinesAddButton->sizePolicy().hasHeightForWidth());
        hobbyRoutinesAddButton->setSizePolicy(sizePolicy2);

        hobbyDashboardLayout->addWidget(hobbyRoutinesAddButton);

        hobbyDashboardDivider3 = new QFrame(dashboardHobbyPage);
        hobbyDashboardDivider3->setObjectName("hobbyDashboardDivider3");
        hobbyDashboardDivider3->setFrameShape(QFrame::Shape::HLine);
        hobbyDashboardDivider3->setFrameShadow(QFrame::Shadow::Plain);

        hobbyDashboardLayout->addWidget(hobbyDashboardDivider3);

        hobbyNoteSectionLabel = new QLabel(dashboardHobbyPage);
        hobbyNoteSectionLabel->setObjectName("hobbyNoteSectionLabel");

        hobbyDashboardLayout->addWidget(hobbyNoteSectionLabel);

        hobbyNotesTextEdit = new QPlainTextEdit(dashboardHobbyPage);
        hobbyNotesTextEdit->setObjectName("hobbyNotesTextEdit");
        hobbyNotesTextEdit->setMaximumSize(QSize(16777215, 72));

        hobbyDashboardLayout->addWidget(hobbyNotesTextEdit);

        hobbyDashboardDivider4 = new QFrame(dashboardHobbyPage);
        hobbyDashboardDivider4->setObjectName("hobbyDashboardDivider4");
        hobbyDashboardDivider4->setFrameShape(QFrame::Shape::HLine);
        hobbyDashboardDivider4->setFrameShadow(QFrame::Shadow::Plain);

        hobbyDashboardLayout->addWidget(hobbyDashboardDivider4);

        hobbyStatsSectionLabel = new QLabel(dashboardHobbyPage);
        hobbyStatsSectionLabel->setObjectName("hobbyStatsSectionLabel");

        hobbyDashboardLayout->addWidget(hobbyStatsSectionLabel);

        hobbyStatsRowLayout = new QHBoxLayout();
        hobbyStatsRowLayout->setSpacing(40);
        hobbyStatsRowLayout->setObjectName("hobbyStatsRowLayout");
        hobbyStatExercisesLayout = new QVBoxLayout();
        hobbyStatExercisesLayout->setSpacing(4);
        hobbyStatExercisesLayout->setObjectName("hobbyStatExercisesLayout");
        hobbyStatExercisesValueLabel = new QLabel(dashboardHobbyPage);
        hobbyStatExercisesValueLabel->setObjectName("hobbyStatExercisesValueLabel");

        hobbyStatExercisesLayout->addWidget(hobbyStatExercisesValueLabel);

        hobbyStatExercisesCaptionLabel = new QLabel(dashboardHobbyPage);
        hobbyStatExercisesCaptionLabel->setObjectName("hobbyStatExercisesCaptionLabel");

        hobbyStatExercisesLayout->addWidget(hobbyStatExercisesCaptionLabel);


        hobbyStatsRowLayout->addLayout(hobbyStatExercisesLayout);

        hobbyStatTimeLayout = new QVBoxLayout();
        hobbyStatTimeLayout->setSpacing(4);
        hobbyStatTimeLayout->setObjectName("hobbyStatTimeLayout");
        hobbyStatTimeValueLabel = new QLabel(dashboardHobbyPage);
        hobbyStatTimeValueLabel->setObjectName("hobbyStatTimeValueLabel");

        hobbyStatTimeLayout->addWidget(hobbyStatTimeValueLabel);

        hobbyStatTimeCaptionLabel = new QLabel(dashboardHobbyPage);
        hobbyStatTimeCaptionLabel->setObjectName("hobbyStatTimeCaptionLabel");

        hobbyStatTimeLayout->addWidget(hobbyStatTimeCaptionLabel);


        hobbyStatsRowLayout->addLayout(hobbyStatTimeLayout);

        hobbyStatSessionsLayout = new QVBoxLayout();
        hobbyStatSessionsLayout->setSpacing(4);
        hobbyStatSessionsLayout->setObjectName("hobbyStatSessionsLayout");
        hobbyStatSessionsValueLabel = new QLabel(dashboardHobbyPage);
        hobbyStatSessionsValueLabel->setObjectName("hobbyStatSessionsValueLabel");

        hobbyStatSessionsLayout->addWidget(hobbyStatSessionsValueLabel);

        hobbyStatSessionsCaptionLabel = new QLabel(dashboardHobbyPage);
        hobbyStatSessionsCaptionLabel->setObjectName("hobbyStatSessionsCaptionLabel");

        hobbyStatSessionsLayout->addWidget(hobbyStatSessionsCaptionLabel);


        hobbyStatsRowLayout->addLayout(hobbyStatSessionsLayout);

        hobbyStatGoalsLayout = new QVBoxLayout();
        hobbyStatGoalsLayout->setSpacing(4);
        hobbyStatGoalsLayout->setObjectName("hobbyStatGoalsLayout");
        hobbyStatGoalsValueLabel = new QLabel(dashboardHobbyPage);
        hobbyStatGoalsValueLabel->setObjectName("hobbyStatGoalsValueLabel");

        hobbyStatGoalsLayout->addWidget(hobbyStatGoalsValueLabel);

        hobbyStatGoalsCaptionLabel = new QLabel(dashboardHobbyPage);
        hobbyStatGoalsCaptionLabel->setObjectName("hobbyStatGoalsCaptionLabel");

        hobbyStatGoalsLayout->addWidget(hobbyStatGoalsCaptionLabel);


        hobbyStatsRowLayout->addLayout(hobbyStatGoalsLayout);

        hobbyStatsRowSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hobbyStatsRowLayout->addItem(hobbyStatsRowSpacer);


        hobbyDashboardLayout->addLayout(hobbyStatsRowLayout);

        hobbyPageStack->addWidget(dashboardHobbyPage);
        roadmapPage = new QWidget();
        roadmapPage->setObjectName("roadmapPage");
        roadmapPageLayout = new QVBoxLayout(roadmapPage);
        roadmapPageLayout->setSpacing(12);
        roadmapPageLayout->setObjectName("roadmapPageLayout");
        roadmapPageLayout->setContentsMargins(12, 12, 12, 12);
        roadmapHeaderLayout = new QHBoxLayout();
        roadmapHeaderLayout->setObjectName("roadmapHeaderLayout");
        roadmapHeaderLabel = new QLabel(roadmapPage);
        roadmapHeaderLabel->setObjectName("roadmapHeaderLabel");

        roadmapHeaderLayout->addWidget(roadmapHeaderLabel);

        roadmapHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        roadmapHeaderLayout->addItem(roadmapHeaderSpacer);

        addRoadmapGoalButton = new QPushButton(roadmapPage);
        addRoadmapGoalButton->setObjectName("addRoadmapGoalButton");

        roadmapHeaderLayout->addWidget(addRoadmapGoalButton);


        roadmapPageLayout->addLayout(roadmapHeaderLayout);

        roadmapViewToggleLayout = new QHBoxLayout();
        roadmapViewToggleLayout->setObjectName("roadmapViewToggleLayout");
        roadmapTreeViewButton = new QToolButton(roadmapPage);
        roadmapTreeViewButton->setObjectName("roadmapTreeViewButton");
        roadmapTreeViewButton->setCheckable(true);
        roadmapTreeViewButton->setChecked(true);
        roadmapTreeViewButton->setAutoExclusive(true);

        roadmapViewToggleLayout->addWidget(roadmapTreeViewButton);

        roadmapDiagramViewButton = new QToolButton(roadmapPage);
        roadmapDiagramViewButton->setObjectName("roadmapDiagramViewButton");
        roadmapDiagramViewButton->setCheckable(true);
        roadmapDiagramViewButton->setAutoExclusive(true);

        roadmapViewToggleLayout->addWidget(roadmapDiagramViewButton);

        roadmapViewToggleSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        roadmapViewToggleLayout->addItem(roadmapViewToggleSpacer);


        roadmapPageLayout->addLayout(roadmapViewToggleLayout);

        roadmapSplitter = new QSplitter(roadmapPage);
        roadmapSplitter->setObjectName("roadmapSplitter");
        roadmapSplitter->setOrientation(Qt::Orientation::Horizontal);
        roadmapSplitter->setChildrenCollapsible(false);
        roadmapViewStack = new QStackedWidget(roadmapSplitter);
        roadmapViewStack->setObjectName("roadmapViewStack");
        roadmapTreePage = new QWidget();
        roadmapTreePage->setObjectName("roadmapTreePage");
        roadmapTreePageLayout = new QVBoxLayout(roadmapTreePage);
        roadmapTreePageLayout->setObjectName("roadmapTreePageLayout");
        roadmapTreePageLayout->setContentsMargins(0, 0, 0, 0);
        roadmapTreeWidget = new QTreeWidget(roadmapTreePage);
        roadmapTreeWidget->setObjectName("roadmapTreeWidget");
        roadmapTreeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        roadmapTreeWidget->setHeaderHidden(true);

        roadmapTreePageLayout->addWidget(roadmapTreeWidget);

        roadmapViewStack->addWidget(roadmapTreePage);
        roadmapDiagramPage = new QWidget();
        roadmapDiagramPage->setObjectName("roadmapDiagramPage");
        roadmapDiagramPageLayout = new QVBoxLayout(roadmapDiagramPage);
        roadmapDiagramPageLayout->setObjectName("roadmapDiagramPageLayout");
        roadmapDiagramPageLayout->setContentsMargins(0, 0, 0, 0);
        roadmapDiagramWidget = new QWidget(roadmapDiagramPage);
        roadmapDiagramWidget->setObjectName("roadmapDiagramWidget");

        roadmapDiagramPageLayout->addWidget(roadmapDiagramWidget);

        roadmapViewStack->addWidget(roadmapDiagramPage);
        roadmapSplitter->addWidget(roadmapViewStack);
        roadmapInfoPanel = new QWidget(roadmapSplitter);
        roadmapInfoPanel->setObjectName("roadmapInfoPanel");
        roadmapInfoPanel->setMinimumSize(QSize(200, 0));
        roadmapInfoPanel->setMaximumSize(QSize(260, 16777215));
        roadmapInfoLayout = new QVBoxLayout(roadmapInfoPanel);
        roadmapInfoLayout->setSpacing(8);
        roadmapInfoLayout->setObjectName("roadmapInfoLayout");
        roadmapInfoTitle = new QLabel(roadmapInfoPanel);
        roadmapInfoTitle->setObjectName("roadmapInfoTitle");

        roadmapInfoLayout->addWidget(roadmapInfoTitle);

        roadmapInfoSeparator = new QFrame(roadmapInfoPanel);
        roadmapInfoSeparator->setObjectName("roadmapInfoSeparator");
        roadmapInfoSeparator->setFrameShape(QFrame::Shape::HLine);

        roadmapInfoLayout->addWidget(roadmapInfoSeparator);

        roadmapInfoNameLabel = new QLabel(roadmapInfoPanel);
        roadmapInfoNameLabel->setObjectName("roadmapInfoNameLabel");
        roadmapInfoNameLabel->setWordWrap(true);

        roadmapInfoLayout->addWidget(roadmapInfoNameLabel);

        roadmapInfoStatusHeader = new QLabel(roadmapInfoPanel);
        roadmapInfoStatusHeader->setObjectName("roadmapInfoStatusHeader");

        roadmapInfoLayout->addWidget(roadmapInfoStatusHeader);

        roadmapInfoStatusLabel = new QLabel(roadmapInfoPanel);
        roadmapInfoStatusLabel->setObjectName("roadmapInfoStatusLabel");

        roadmapInfoLayout->addWidget(roadmapInfoStatusLabel);

        roadmapInfoDescriptionHeader = new QLabel(roadmapInfoPanel);
        roadmapInfoDescriptionHeader->setObjectName("roadmapInfoDescriptionHeader");

        roadmapInfoLayout->addWidget(roadmapInfoDescriptionHeader);

        roadmapInfoDescriptionLabel = new QLabel(roadmapInfoPanel);
        roadmapInfoDescriptionLabel->setObjectName("roadmapInfoDescriptionLabel");
        roadmapInfoDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        roadmapInfoDescriptionLabel->setWordWrap(true);

        roadmapInfoLayout->addWidget(roadmapInfoDescriptionLabel);

        roadmapInfoParentHeader = new QLabel(roadmapInfoPanel);
        roadmapInfoParentHeader->setObjectName("roadmapInfoParentHeader");

        roadmapInfoLayout->addWidget(roadmapInfoParentHeader);

        roadmapInfoParentLabel = new QLabel(roadmapInfoPanel);
        roadmapInfoParentLabel->setObjectName("roadmapInfoParentLabel");

        roadmapInfoLayout->addWidget(roadmapInfoParentLabel);

        roadmapInfoSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        roadmapInfoLayout->addItem(roadmapInfoSpacer);

        roadmapSplitter->addWidget(roadmapInfoPanel);

        roadmapPageLayout->addWidget(roadmapSplitter);

        hobbyPageStack->addWidget(roadmapPage);
        timelinePage = new QWidget();
        timelinePage->setObjectName("timelinePage");
        timelinePageLayout = new QVBoxLayout(timelinePage);
        timelinePageLayout->setSpacing(12);
        timelinePageLayout->setObjectName("timelinePageLayout");
        timelinePageLayout->setContentsMargins(12, 12, 12, 12);
        timelineHeaderLayout = new QHBoxLayout();
        timelineHeaderLayout->setObjectName("timelineHeaderLayout");
        timelineHeaderLabel = new QLabel(timelinePage);
        timelineHeaderLabel->setObjectName("timelineHeaderLabel");

        timelineHeaderLayout->addWidget(timelineHeaderLabel);

        timelineHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        timelineHeaderLayout->addItem(timelineHeaderSpacer);

        addTimelinePhaseButton = new QPushButton(timelinePage);
        addTimelinePhaseButton->setObjectName("addTimelinePhaseButton");

        timelineHeaderLayout->addWidget(addTimelinePhaseButton);


        timelinePageLayout->addLayout(timelineHeaderLayout);

        timelineBarScrollArea = new QScrollArea(timelinePage);
        timelineBarScrollArea->setObjectName("timelineBarScrollArea");
        timelineBarScrollArea->setMinimumSize(QSize(0, 84));
        timelineBarScrollArea->setMaximumSize(QSize(16777215, 84));
        timelineBarScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        timelineBarScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        timelineBarScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        timelineBarScrollArea->setWidgetResizable(false);
        timelineBarWidget = new TimelineBarWidget();
        timelineBarWidget->setObjectName("timelineBarWidget");
        timelineBarWidget->setGeometry(QRect(0, 0, 400, 84));
        timelineBarScrollArea->setWidget(timelineBarWidget);

        timelinePageLayout->addWidget(timelineBarScrollArea);

        timelineEmptyLabel = new QLabel(timelinePage);
        timelineEmptyLabel->setObjectName("timelineEmptyLabel");
        timelineEmptyLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        timelinePageLayout->addWidget(timelineEmptyLabel);

        timelinePageSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        timelinePageLayout->addItem(timelinePageSpacer);

        hobbyPageStack->addWidget(timelinePage);
        routinesPage = new QWidget();
        routinesPage->setObjectName("routinesPage");
        routinesPageLayout = new QVBoxLayout(routinesPage);
        routinesPageLayout->setObjectName("routinesPageLayout");
        routineList = new QListWidget(routinesPage);
        routineList->setObjectName("routineList");

        routinesPageLayout->addWidget(routineList);

        addRoutineButton = new QPushButton(routinesPage);
        addRoutineButton->setObjectName("addRoutineButton");

        routinesPageLayout->addWidget(addRoutineButton);

        hobbyPageStack->addWidget(routinesPage);
        exercisesPage = new QWidget();
        exercisesPage->setObjectName("exercisesPage");
        exercisesPageLayout = new QVBoxLayout(exercisesPage);
        exercisesPageLayout->setSpacing(0);
        exercisesPageLayout->setObjectName("exercisesPageLayout");
        exercisesPageLayout->setContentsMargins(0, 0, 0, 0);
        exerciseViewStack = new QStackedWidget(exercisesPage);
        exerciseViewStack->setObjectName("exerciseViewStack");
        exerciseOverviewPage = new QWidget();
        exerciseOverviewPage->setObjectName("exerciseOverviewPage");
        exerciseOverviewPageLayout = new QVBoxLayout(exerciseOverviewPage);
        exerciseOverviewPageLayout->setSpacing(12);
        exerciseOverviewPageLayout->setObjectName("exerciseOverviewPageLayout");
        exerciseOverviewPageLayout->setContentsMargins(12, 12, 12, 12);
        exercisesHeaderLayout = new QHBoxLayout();
        exercisesHeaderLayout->setObjectName("exercisesHeaderLayout");
        exercisesHeaderLabel = new QLabel(exerciseOverviewPage);
        exercisesHeaderLabel->setObjectName("exercisesHeaderLabel");

        exercisesHeaderLayout->addWidget(exercisesHeaderLabel);

        exercisesHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        exercisesHeaderLayout->addItem(exercisesHeaderSpacer);

        addExerciseButton = new QPushButton(exerciseOverviewPage);
        addExerciseButton->setObjectName("addExerciseButton");

        exercisesHeaderLayout->addWidget(addExerciseButton);


        exerciseOverviewPageLayout->addLayout(exercisesHeaderLayout);

        exercisesFilterLayout = new QHBoxLayout();
        exercisesFilterLayout->setObjectName("exercisesFilterLayout");
        exerciseSearchLineEdit = new QLineEdit(exerciseOverviewPage);
        exerciseSearchLineEdit->setObjectName("exerciseSearchLineEdit");

        exercisesFilterLayout->addWidget(exerciseSearchLineEdit);

        exerciseCategoryComboBox = new QComboBox(exerciseOverviewPage);
        exerciseCategoryComboBox->addItem(QString());
        exerciseCategoryComboBox->setObjectName("exerciseCategoryComboBox");

        exercisesFilterLayout->addWidget(exerciseCategoryComboBox);

        manageCategoriesButton = new QPushButton(exerciseOverviewPage);
        manageCategoriesButton->setObjectName("manageCategoriesButton");

        exercisesFilterLayout->addWidget(manageCategoriesButton);


        exerciseOverviewPageLayout->addLayout(exercisesFilterLayout);

        exerciseScrollArea = new QScrollArea(exerciseOverviewPage);
        exerciseScrollArea->setObjectName("exerciseScrollArea");
        exerciseScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        exerciseScrollArea->setWidgetResizable(true);
        exerciseCardsWidget = new QWidget();
        exerciseCardsWidget->setObjectName("exerciseCardsWidget");
        exerciseCardsWidget->setGeometry(QRect(0, 0, 62, 16));
        exerciseCardsLayout = new QGridLayout(exerciseCardsWidget);
        exerciseCardsLayout->setSpacing(12);
        exerciseCardsLayout->setObjectName("exerciseCardsLayout");
        exerciseCardsLayout->setContentsMargins(0, 0, 0, 0);
        exerciseScrollArea->setWidget(exerciseCardsWidget);

        exerciseOverviewPageLayout->addWidget(exerciseScrollArea);

        exerciseViewStack->addWidget(exerciseOverviewPage);
        exerciseDetailPage = new QWidget();
        exerciseDetailPage->setObjectName("exerciseDetailPage");
        exerciseDetailPageLayout = new QVBoxLayout(exerciseDetailPage);
        exerciseDetailPageLayout->setSpacing(16);
        exerciseDetailPageLayout->setObjectName("exerciseDetailPageLayout");
        exerciseDetailPageLayout->setContentsMargins(16, 16, 16, 16);
        exerciseDetailHeaderLayout = new QVBoxLayout();
        exerciseDetailHeaderLayout->setSpacing(4);
        exerciseDetailHeaderLayout->setObjectName("exerciseDetailHeaderLayout");
        exerciseDetailNameLabel = new QLabel(exerciseDetailPage);
        exerciseDetailNameLabel->setObjectName("exerciseDetailNameLabel");
        exerciseDetailNameLabel->setWordWrap(true);

        exerciseDetailHeaderLayout->addWidget(exerciseDetailNameLabel);

        exerciseDetailDescriptionLabel = new QLabel(exerciseDetailPage);
        exerciseDetailDescriptionLabel->setObjectName("exerciseDetailDescriptionLabel");
        exerciseDetailDescriptionLabel->setWordWrap(true);

        exerciseDetailHeaderLayout->addWidget(exerciseDetailDescriptionLabel);

        exerciseDetailCategoryRow = new QHBoxLayout();
        exerciseDetailCategoryRow->setObjectName("exerciseDetailCategoryRow");
        exerciseDetailCategoryTagLabel = new QLabel(exerciseDetailPage);
        exerciseDetailCategoryTagLabel->setObjectName("exerciseDetailCategoryTagLabel");

        exerciseDetailCategoryRow->addWidget(exerciseDetailCategoryTagLabel);

        exerciseDetailCategorySpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        exerciseDetailCategoryRow->addItem(exerciseDetailCategorySpacer);


        exerciseDetailHeaderLayout->addLayout(exerciseDetailCategoryRow);


        exerciseDetailPageLayout->addLayout(exerciseDetailHeaderLayout);

        exerciseDetailProgressCard = new QGroupBox(exerciseDetailPage);
        exerciseDetailProgressCard->setObjectName("exerciseDetailProgressCard");
        exerciseDetailProgressCardLayout = new QVBoxLayout(exerciseDetailProgressCard);
        exerciseDetailProgressCardLayout->setObjectName("exerciseDetailProgressCardLayout");
        exerciseDetailValuesRow = new QHBoxLayout();
        exerciseDetailValuesRow->setObjectName("exerciseDetailValuesRow");
        exerciseDetailStartValueColumn = new QVBoxLayout();
        exerciseDetailStartValueColumn->setSpacing(4);
        exerciseDetailStartValueColumn->setObjectName("exerciseDetailStartValueColumn");
        exerciseDetailStartHeaderLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailStartHeaderLabel->setObjectName("exerciseDetailStartHeaderLabel");

        exerciseDetailStartValueColumn->addWidget(exerciseDetailStartHeaderLabel);

        exerciseDetailStartValueRow = new QHBoxLayout();
        exerciseDetailStartValueRow->setObjectName("exerciseDetailStartValueRow");
        exerciseDetailStartValueLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailStartValueLabel->setObjectName("exerciseDetailStartValueLabel");

        exerciseDetailStartValueRow->addWidget(exerciseDetailStartValueLabel);

        exerciseDetailEditStartValueButton = new QPushButton(exerciseDetailProgressCard);
        exerciseDetailEditStartValueButton->setObjectName("exerciseDetailEditStartValueButton");
        exerciseDetailEditStartValueButton->setMaximumSize(QSize(16777215, 24));

        exerciseDetailStartValueRow->addWidget(exerciseDetailEditStartValueButton);


        exerciseDetailStartValueColumn->addLayout(exerciseDetailStartValueRow);


        exerciseDetailValuesRow->addLayout(exerciseDetailStartValueColumn);

        exerciseDetailCurrentValueColumn = new QVBoxLayout();
        exerciseDetailCurrentValueColumn->setSpacing(4);
        exerciseDetailCurrentValueColumn->setObjectName("exerciseDetailCurrentValueColumn");
        exerciseDetailCurrentHeaderLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailCurrentHeaderLabel->setObjectName("exerciseDetailCurrentHeaderLabel");

        exerciseDetailCurrentValueColumn->addWidget(exerciseDetailCurrentHeaderLabel);

        exerciseDetailCurrentValueLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailCurrentValueLabel->setObjectName("exerciseDetailCurrentValueLabel");

        exerciseDetailCurrentValueColumn->addWidget(exerciseDetailCurrentValueLabel);


        exerciseDetailValuesRow->addLayout(exerciseDetailCurrentValueColumn);

        exerciseDetailGoalValueColumn = new QVBoxLayout();
        exerciseDetailGoalValueColumn->setSpacing(4);
        exerciseDetailGoalValueColumn->setObjectName("exerciseDetailGoalValueColumn");
        exerciseDetailGoalHeaderLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailGoalHeaderLabel->setObjectName("exerciseDetailGoalHeaderLabel");

        exerciseDetailGoalValueColumn->addWidget(exerciseDetailGoalHeaderLabel);

        exerciseDetailGoalValueLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailGoalValueLabel->setObjectName("exerciseDetailGoalValueLabel");

        exerciseDetailGoalValueColumn->addWidget(exerciseDetailGoalValueLabel);


        exerciseDetailValuesRow->addLayout(exerciseDetailGoalValueColumn);

        exerciseDetailValuesSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        exerciseDetailValuesRow->addItem(exerciseDetailValuesSpacer);


        exerciseDetailProgressCardLayout->addLayout(exerciseDetailValuesRow);

        exerciseDetailProgressBar = new QProgressBar(exerciseDetailProgressCard);
        exerciseDetailProgressBar->setObjectName("exerciseDetailProgressBar");
        exerciseDetailProgressBar->setValue(0);
        exerciseDetailProgressBar->setTextVisible(false);

        exerciseDetailProgressCardLayout->addWidget(exerciseDetailProgressBar);

        exerciseDetailPercentLabel = new QLabel(exerciseDetailProgressCard);
        exerciseDetailPercentLabel->setObjectName("exerciseDetailPercentLabel");
        exerciseDetailPercentLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        exerciseDetailProgressCardLayout->addWidget(exerciseDetailPercentLabel);


        exerciseDetailPageLayout->addWidget(exerciseDetailProgressCard);

        exerciseDetailBottomRow = new QHBoxLayout();
        exerciseDetailBottomRow->setSpacing(16);
        exerciseDetailBottomRow->setObjectName("exerciseDetailBottomRow");
        exerciseDetailDevelopmentCard = new QGroupBox(exerciseDetailPage);
        exerciseDetailDevelopmentCard->setObjectName("exerciseDetailDevelopmentCard");
        exerciseDetailDevelopmentCardLayout = new QVBoxLayout(exerciseDetailDevelopmentCard);
        exerciseDetailDevelopmentCardLayout->setObjectName("exerciseDetailDevelopmentCardLayout");
        exerciseProgressChartWidget = new ExerciseProgressChartWidget(exerciseDetailDevelopmentCard);
        exerciseProgressChartWidget->setObjectName("exerciseProgressChartWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(exerciseProgressChartWidget->sizePolicy().hasHeightForWidth());
        exerciseProgressChartWidget->setSizePolicy(sizePolicy3);

        exerciseDetailDevelopmentCardLayout->addWidget(exerciseProgressChartWidget);


        exerciseDetailBottomRow->addWidget(exerciseDetailDevelopmentCard);

        exerciseDetailStatisticsCard = new QGroupBox(exerciseDetailPage);
        exerciseDetailStatisticsCard->setObjectName("exerciseDetailStatisticsCard");
        exerciseDetailStatisticsCardLayout = new QFormLayout(exerciseDetailStatisticsCard);
        exerciseDetailStatisticsCardLayout->setObjectName("exerciseDetailStatisticsCardLayout");
        exerciseDetailLastPerformedHeaderLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailLastPerformedHeaderLabel->setObjectName("exerciseDetailLastPerformedHeaderLabel");

        exerciseDetailStatisticsCardLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, exerciseDetailLastPerformedHeaderLabel);

        exerciseDetailLastPerformedLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailLastPerformedLabel->setObjectName("exerciseDetailLastPerformedLabel");

        exerciseDetailStatisticsCardLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, exerciseDetailLastPerformedLabel);

        exerciseDetailExecutionCountHeaderLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailExecutionCountHeaderLabel->setObjectName("exerciseDetailExecutionCountHeaderLabel");

        exerciseDetailStatisticsCardLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, exerciseDetailExecutionCountHeaderLabel);

        exerciseDetailExecutionCountLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailExecutionCountLabel->setObjectName("exerciseDetailExecutionCountLabel");

        exerciseDetailStatisticsCardLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, exerciseDetailExecutionCountLabel);

        exerciseDetailTotalTimeHeaderLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailTotalTimeHeaderLabel->setObjectName("exerciseDetailTotalTimeHeaderLabel");

        exerciseDetailStatisticsCardLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, exerciseDetailTotalTimeHeaderLabel);

        exerciseDetailTotalTimeLabel = new QLabel(exerciseDetailStatisticsCard);
        exerciseDetailTotalTimeLabel->setObjectName("exerciseDetailTotalTimeLabel");

        exerciseDetailStatisticsCardLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, exerciseDetailTotalTimeLabel);


        exerciseDetailBottomRow->addWidget(exerciseDetailStatisticsCard);

        exerciseDetailBottomRow->setStretch(0, 3);
        exerciseDetailBottomRow->setStretch(1, 1);

        exerciseDetailPageLayout->addLayout(exerciseDetailBottomRow);

        exerciseDetailPageLayout->setStretch(2, 1);
        exerciseViewStack->addWidget(exerciseDetailPage);

        exercisesPageLayout->addWidget(exerciseViewStack);

        hobbyPageStack->addWidget(exercisesPage);
        historyPage = new QWidget();
        historyPage->setObjectName("historyPage");
        historyPageLayout = new QVBoxLayout(historyPage);
        historyPageLayout->setSpacing(16);
        historyPageLayout->setObjectName("historyPageLayout");
        historyPageLayout->setContentsMargins(12, 12, 12, 12);
        historyHeaderLayout = new QHBoxLayout();
        historyHeaderLayout->setObjectName("historyHeaderLayout");
        historyHeaderLabel = new QLabel(historyPage);
        historyHeaderLabel->setObjectName("historyHeaderLabel");

        historyHeaderLayout->addWidget(historyHeaderLabel);

        historyHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        historyHeaderLayout->addItem(historyHeaderSpacer);


        historyPageLayout->addLayout(historyHeaderLayout);

        historyToolbarLayout = new QHBoxLayout();
        historyToolbarLayout->setObjectName("historyToolbarLayout");
        historySearchLineEdit = new QLineEdit(historyPage);
        historySearchLineEdit->setObjectName("historySearchLineEdit");
        historySearchLineEdit->setMaximumSize(QSize(240, 16777215));

        historyToolbarLayout->addWidget(historySearchLineEdit);

        historyToolbarSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        historyToolbarLayout->addItem(historyToolbarSpacer);


        historyPageLayout->addLayout(historyToolbarLayout);

        historyScrollArea = new QScrollArea(historyPage);
        historyScrollArea->setObjectName("historyScrollArea");
        historyScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        historyScrollArea->setWidgetResizable(true);
        historyEntriesWidget = new QWidget();
        historyEntriesWidget->setObjectName("historyEntriesWidget");
        historyEntriesWidget->setGeometry(QRect(0, 0, 62, 160));
        historyEntriesLayout = new QVBoxLayout(historyEntriesWidget);
        historyEntriesLayout->setSpacing(16);
        historyEntriesLayout->setObjectName("historyEntriesLayout");
        historyEntriesLayout->setContentsMargins(0, 0, 0, 0);
        historyEmptyStateLabel = new QLabel(historyEntriesWidget);
        historyEmptyStateLabel->setObjectName("historyEmptyStateLabel");
        historyEmptyStateLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        historyEmptyStateLabel->setWordWrap(true);

        historyEntriesLayout->addWidget(historyEmptyStateLabel);

        historyScrollArea->setWidget(historyEntriesWidget);

        historyPageLayout->addWidget(historyScrollArea);

        hobbyPageStack->addWidget(historyPage);
        goalsPage = new QWidget();
        goalsPage->setObjectName("goalsPage");
        goalsPageLayout = new QVBoxLayout(goalsPage);
        goalsPageLayout->setSpacing(12);
        goalsPageLayout->setObjectName("goalsPageLayout");
        goalsPageLayout->setContentsMargins(12, 12, 12, 12);
        goalsHeaderLayout = new QHBoxLayout();
        goalsHeaderLayout->setObjectName("goalsHeaderLayout");
        goalsHeaderLabel = new QLabel(goalsPage);
        goalsHeaderLabel->setObjectName("goalsHeaderLabel");

        goalsHeaderLayout->addWidget(goalsHeaderLabel);

        goalsHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        goalsHeaderLayout->addItem(goalsHeaderSpacer);

        addGoalButton = new QPushButton(goalsPage);
        addGoalButton->setObjectName("addGoalButton");

        goalsHeaderLayout->addWidget(addGoalButton);


        goalsPageLayout->addLayout(goalsHeaderLayout);

        goalsFilterLayout = new QHBoxLayout();
        goalsFilterLayout->setObjectName("goalsFilterLayout");
        goalSearchLineEdit = new QLineEdit(goalsPage);
        goalSearchLineEdit->setObjectName("goalSearchLineEdit");

        goalsFilterLayout->addWidget(goalSearchLineEdit);

        goalsFilterOpenButton = new QToolButton(goalsPage);
        goalsFilterOpenButton->setObjectName("goalsFilterOpenButton");
        goalsFilterOpenButton->setCheckable(true);
        goalsFilterOpenButton->setChecked(true);
        goalsFilterOpenButton->setAutoExclusive(true);

        goalsFilterLayout->addWidget(goalsFilterOpenButton);

        goalsFilterDoneButton = new QToolButton(goalsPage);
        goalsFilterDoneButton->setObjectName("goalsFilterDoneButton");
        goalsFilterDoneButton->setCheckable(true);
        goalsFilterDoneButton->setAutoExclusive(true);

        goalsFilterLayout->addWidget(goalsFilterDoneButton);


        goalsPageLayout->addLayout(goalsFilterLayout);

        goalsViewStack = new QStackedWidget(goalsPage);
        goalsViewStack->setObjectName("goalsViewStack");
        goalsOpenPage = new QWidget();
        goalsOpenPage->setObjectName("goalsOpenPage");
        goalsOpenPageLayout = new QVBoxLayout(goalsOpenPage);
        goalsOpenPageLayout->setSpacing(12);
        goalsOpenPageLayout->setObjectName("goalsOpenPageLayout");
        goalsOpenPageLayout->setContentsMargins(0, 0, 0, 0);
        goalsOpenEmptyLabel = new QLabel(goalsOpenPage);
        goalsOpenEmptyLabel->setObjectName("goalsOpenEmptyLabel");
        goalsOpenEmptyLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        goalsOpenEmptyLabel->setWordWrap(true);

        goalsOpenPageLayout->addWidget(goalsOpenEmptyLabel);

        goalsOpenScrollArea = new QScrollArea(goalsOpenPage);
        goalsOpenScrollArea->setObjectName("goalsOpenScrollArea");
        goalsOpenScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        goalsOpenScrollArea->setWidgetResizable(true);
        goalOpenCardsWidget = new QWidget();
        goalOpenCardsWidget->setObjectName("goalOpenCardsWidget");
        goalOpenCardsWidget->setGeometry(QRect(0, 0, 62, 16));
        goalOpenCardsLayout = new QGridLayout(goalOpenCardsWidget);
        goalOpenCardsLayout->setSpacing(12);
        goalOpenCardsLayout->setObjectName("goalOpenCardsLayout");
        goalOpenCardsLayout->setContentsMargins(0, 0, 0, 0);
        goalsOpenScrollArea->setWidget(goalOpenCardsWidget);

        goalsOpenPageLayout->addWidget(goalsOpenScrollArea);

        goalsViewStack->addWidget(goalsOpenPage);
        goalsDonePage = new QWidget();
        goalsDonePage->setObjectName("goalsDonePage");
        goalsDonePageLayout = new QVBoxLayout(goalsDonePage);
        goalsDonePageLayout->setSpacing(12);
        goalsDonePageLayout->setObjectName("goalsDonePageLayout");
        goalsDonePageLayout->setContentsMargins(0, 0, 0, 0);
        goalsDoneEmptyLabel = new QLabel(goalsDonePage);
        goalsDoneEmptyLabel->setObjectName("goalsDoneEmptyLabel");
        goalsDoneEmptyLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        goalsDoneEmptyLabel->setWordWrap(true);

        goalsDonePageLayout->addWidget(goalsDoneEmptyLabel);

        goalsDoneScrollArea = new QScrollArea(goalsDonePage);
        goalsDoneScrollArea->setObjectName("goalsDoneScrollArea");
        goalsDoneScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        goalsDoneScrollArea->setWidgetResizable(true);
        goalDoneCardsWidget = new QWidget();
        goalDoneCardsWidget->setObjectName("goalDoneCardsWidget");
        goalDoneCardsWidget->setGeometry(QRect(0, 0, 86, 16));
        goalDoneCardsLayout = new QGridLayout(goalDoneCardsWidget);
        goalDoneCardsLayout->setSpacing(12);
        goalDoneCardsLayout->setObjectName("goalDoneCardsLayout");
        goalDoneCardsLayout->setContentsMargins(0, 0, 0, 0);
        goalsDoneScrollArea->setWidget(goalDoneCardsWidget);

        goalsDonePageLayout->addWidget(goalsDoneScrollArea);

        goalsViewStack->addWidget(goalsDonePage);

        goalsPageLayout->addWidget(goalsViewStack);

        hobbyPageStack->addWidget(goalsPage);

        hobbyPageLayout->addWidget(hobbyPageStack);

        pageStack->addWidget(hobbyPage);
        settingsPage = new QWidget();
        settingsPage->setObjectName("settingsPage");
        settingsPageLayout = new QVBoxLayout(settingsPage);
        settingsPageLayout->setSpacing(16);
        settingsPageLayout->setObjectName("settingsPageLayout");
        settingsPageLayout->setContentsMargins(24, 24, 24, 24);
        settingsTitleLabel = new QLabel(settingsPage);
        settingsTitleLabel->setObjectName("settingsTitleLabel");

        settingsPageLayout->addWidget(settingsTitleLabel);

        settingsPageSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        settingsPageLayout->addItem(settingsPageSpacer);

        pageStack->addWidget(settingsPage);
        statisticsPage = new QWidget();
        statisticsPage->setObjectName("statisticsPage");
        statisticsPageLayout = new QVBoxLayout(statisticsPage);
        statisticsPageLayout->setSpacing(16);
        statisticsPageLayout->setObjectName("statisticsPageLayout");
        statisticsPageLayout->setContentsMargins(24, 24, 24, 24);
        pageStack->addWidget(statisticsPage);

        contentLayout->addWidget(pageStack);


        horizontalLayout_9->addWidget(content);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        pageStack->setCurrentIndex(1);
        hobbyPageStack->setCurrentIndex(0);
        roadmapViewStack->setCurrentIndex(0);
        exerciseViewStack->setCurrentIndex(1);
        goalsViewStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Skillbase", nullptr));
        sidebarTitleLabel->setText(QCoreApplication::translate("MainWindow", "Skillbase", nullptr));
        dashboardButton->setText(QCoreApplication::translate("MainWindow", "\303\234bersicht", nullptr));
        sidebarHobbySectionLabel->setText(QCoreApplication::translate("MainWindow", "Hobbys", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "Einstellungen", nullptr));
        dashboardTitleLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bersicht", nullptr));
        dashboardSubtitleLabel->setText(QCoreApplication::translate("MainWindow", "Alle Hobbys im \303\234berblick", nullptr));
        todaySectionLabel->setText(QCoreApplication::translate("MainWindow", "HEUTE", nullptr));
        todayLabel->setText(QCoreApplication::translate("MainWindow", "Keine Aufgaben f\303\274r heute.", nullptr));
        goalsSectionLabel->setText(QCoreApplication::translate("MainWindow", "ZIELE", nullptr));
        goalsSummaryLabel->setText(QCoreApplication::translate("MainWindow", "Keine aktiven Ziele.", nullptr));
        progressSectionLabel->setText(QCoreApplication::translate("MainWindow", "FORTSCHRITT", nullptr));
        progressSummaryLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine Fortschrittsdaten vorhanden.", nullptr));
        recentActivitySectionLabel->setText(QCoreApplication::translate("MainWindow", "LETZTE AKTIVIT\303\204T", nullptr));
        hobbyLabel->setText(QCoreApplication::translate("MainWindow", "Hobby", nullptr));
        dashboardTab->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        routinesTab->setText(QCoreApplication::translate("MainWindow", "Routinen", nullptr));
        exercisesTab->setText(QCoreApplication::translate("MainWindow", "\303\234bungen", nullptr));
        goalsTab->setText(QCoreApplication::translate("MainWindow", "Ziele", nullptr));
        roadmapTab->setText(QCoreApplication::translate("MainWindow", "Roadmap", nullptr));
        historyTab->setText(QCoreApplication::translate("MainWindow", "Verlauf", nullptr));
        hobbyPhaseSectionLabel->setText(QCoreApplication::translate("MainWindow", "AKTUELLE PHASE", nullptr));
        hobbyPhaseNameLabel->setText(QCoreApplication::translate("MainWindow", "Keine aktive Phase", nullptr));
        hobbyPhaseCountdownLabel->setText(QString());
        hobbyGoalSectionLabel->setText(QCoreApplication::translate("MainWindow", "AKTUELLES ZIEL", nullptr));
        hobbyGoalNameLabel->setText(QCoreApplication::translate("MainWindow", "Keine aktiven Ziele", nullptr));
        hobbyGoalDeadlineLabel->setText(QString());
        hobbyRoutinesSectionLabel->setText(QCoreApplication::translate("MainWindow", "DEINE ROUTINEN", nullptr));
        hobbyRoutinesEmptyLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine Routinen angelegt. Lege deine erste Routine an, um sie hier auszuf\303\274hren.", nullptr));
        hobbyRoutinesAddButton->setText(QCoreApplication::translate("MainWindow", "+ Routine hinzuf\303\274gen", nullptr));
        hobbyNoteSectionLabel->setText(QCoreApplication::translate("MainWindow", "NOTIZ", nullptr));
        hobbyNotesTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Notizen zu diesem Hobby...", nullptr));
        hobbyStatsSectionLabel->setText(QCoreApplication::translate("MainWindow", "\303\234BERSICHT", nullptr));
        hobbyStatExercisesValueLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        hobbyStatExercisesCaptionLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bungen", nullptr));
        hobbyStatTimeValueLabel->setText(QCoreApplication::translate("MainWindow", "0 min", nullptr));
        hobbyStatTimeCaptionLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bungszeit", nullptr));
        hobbyStatSessionsValueLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        hobbyStatSessionsCaptionLabel->setText(QCoreApplication::translate("MainWindow", "Sessions", nullptr));
        hobbyStatGoalsValueLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        hobbyStatGoalsCaptionLabel->setText(QCoreApplication::translate("MainWindow", "Ziele", nullptr));
        roadmapHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Roadmap", nullptr));
        addRoadmapGoalButton->setText(QCoreApplication::translate("MainWindow", "+ Neues Ziel", nullptr));
        roadmapTreeViewButton->setText(QCoreApplication::translate("MainWindow", "Tree", nullptr));
        roadmapDiagramViewButton->setText(QCoreApplication::translate("MainWindow", "Diagram", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = roadmapTreeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Ziel", nullptr));
#if QT_CONFIG(tooltip)
        roadmapDiagramWidget->setToolTip(QCoreApplication::translate("MainWindow", "Hierarchie-Diagramm \342\200\223 wird per C++ gezeichnet", nullptr));
#endif // QT_CONFIG(tooltip)
        roadmapInfoTitle->setText(QCoreApplication::translate("MainWindow", "Informationen", nullptr));
        roadmapInfoNameLabel->setText(QString());
        roadmapInfoStatusHeader->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        roadmapInfoStatusLabel->setText(QString());
        roadmapInfoDescriptionHeader->setText(QCoreApplication::translate("MainWindow", "Beschreibung", nullptr));
        roadmapInfoDescriptionLabel->setText(QString());
        roadmapInfoParentHeader->setText(QCoreApplication::translate("MainWindow", "\303\234bergeordnetes Ziel", nullptr));
        roadmapInfoParentLabel->setText(QString());
        timelineHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Timeline", nullptr));
        addTimelinePhaseButton->setText(QCoreApplication::translate("MainWindow", "+ Phase hinzuf\303\274gen", nullptr));
#if QT_CONFIG(tooltip)
        timelineBarWidget->setToolTip(QCoreApplication::translate("MainWindow", "Lernphasen chronologisch, Breite entspricht der Dauer \302\267 L\303\274cken zeigen Pausen \302\267 Farbe entspricht dem Hobby", nullptr));
#endif // QT_CONFIG(tooltip)
        timelineEmptyLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine Lernphasen geplant.", nullptr));
        addRoutineButton->setText(QCoreApplication::translate("MainWindow", "+ Routine hinzuf\303\274gen", nullptr));
        exercisesHeaderLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bungen", nullptr));
        addExerciseButton->setText(QCoreApplication::translate("MainWindow", "+ \303\234bung", nullptr));
        exerciseSearchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\303\234bungen suchen...", nullptr));
        exerciseCategoryComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Alle Kategorien", nullptr));

        manageCategoriesButton->setText(QCoreApplication::translate("MainWindow", "Kategorien verwalten", nullptr));
        exerciseDetailNameLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bungsname", nullptr));
        exerciseDetailDescriptionLabel->setText(QCoreApplication::translate("MainWindow", "Beschreibung", nullptr));
        exerciseDetailCategoryTagLabel->setText(QCoreApplication::translate("MainWindow", "Kategorie", nullptr));
        exerciseDetailProgressCard->setTitle(QCoreApplication::translate("MainWindow", "Fortschritt", nullptr));
        exerciseDetailStartHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Startwert", nullptr));
        exerciseDetailStartValueLabel->setText(QCoreApplication::translate("MainWindow", "\342\200\223", nullptr));
        exerciseDetailEditStartValueButton->setText(QCoreApplication::translate("MainWindow", "Bearbeiten", nullptr));
        exerciseDetailCurrentHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Aktueller Wert", nullptr));
        exerciseDetailCurrentValueLabel->setText(QCoreApplication::translate("MainWindow", "\342\200\223", nullptr));
        exerciseDetailGoalHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Ziel", nullptr));
        exerciseDetailGoalValueLabel->setText(QCoreApplication::translate("MainWindow", "\342\200\223", nullptr));
        exerciseDetailPercentLabel->setText(QCoreApplication::translate("MainWindow", "0 %", nullptr));
        exerciseDetailDevelopmentCard->setTitle(QCoreApplication::translate("MainWindow", "Entwicklung", nullptr));
        exerciseDetailStatisticsCard->setTitle(QCoreApplication::translate("MainWindow", "Statistik", nullptr));
        exerciseDetailLastPerformedHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Letzte Ausf\303\274hrung", nullptr));
        exerciseDetailLastPerformedLabel->setText(QCoreApplication::translate("MainWindow", "\342\200\223", nullptr));
        exerciseDetailExecutionCountHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Ausf\303\274hrungen", nullptr));
        exerciseDetailExecutionCountLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        exerciseDetailTotalTimeHeaderLabel->setText(QCoreApplication::translate("MainWindow", "\303\234bungszeit", nullptr));
        exerciseDetailTotalTimeLabel->setText(QCoreApplication::translate("MainWindow", "\342\200\223", nullptr));
        historyHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Verlauf", nullptr));
        historySearchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Verlauf durchsuchen...", nullptr));
        historyEmptyStateLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine Aktivit\303\244ten\n"
"\n"
"Sobald du eine \303\234bung oder Routine ausf\303\274hrst, erscheint sie hier.", nullptr));
        goalsHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Ziele", nullptr));
        addGoalButton->setText(QCoreApplication::translate("MainWindow", "+ Ziel hinzuf\303\274gen", nullptr));
        goalSearchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ziele durchsuchen...", nullptr));
        goalsFilterOpenButton->setText(QCoreApplication::translate("MainWindow", "Offen", nullptr));
        goalsFilterDoneButton->setText(QCoreApplication::translate("MainWindow", "Geschafft", nullptr));
        goalsOpenEmptyLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine offenen Ziele\n"
"\n"
"Leg mit \"+ Ziel hinzuf\303\274gen\" dein erstes Ziel f\303\274r dieses Hobby an.", nullptr));
        goalsDoneEmptyLabel->setText(QCoreApplication::translate("MainWindow", "Noch keine geschafften Ziele\n"
"\n"
"Sobald du ein Ziel als geschafft markierst, landet es hier im Archiv.", nullptr));
        settingsTitleLabel->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
