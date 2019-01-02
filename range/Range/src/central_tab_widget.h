/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   central_tab_widget.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th December 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Main window's central tab widget class declaration  *
 *********************************************************************/

#ifndef CENTRAL_TAB_WIDGET_H
#define CENTRAL_TAB_WIDGET_H

#include <QTabWidget>
#include <QMdiArea>

#include <rmlib.h>

#include "text_browser.h"
#include "pick_details_tree.h"

class CentralTabWidget : public QTabWidget
{

    Q_OBJECT

    protected:

        static const QString informationIconFile;
        static const QString importantIconFile;
        static const QString severeIconFile;

        //! Model tab position;
        int modelTabPosition;
        //! Application output tab position;
        int applicationOutputTabPosition;
        //! Process output tab position;
        int processOutputTabPosition;
        //! Pick details tab position;
        int pickDetailsTabPosition;

        //! Application output last icon.
        QString applicationOutputLastIcon;
        //! Process output last icon.
        QString processOutputLastIcon;

        //! MDI Area.
        QMdiArea *mdiArea;
        //! Application output browser.
        TextBrowser *applicationOutputBrowser;
        //! Process output browser.
        TextBrowser *processOutputBrowser;
        //! Pick Details tree.
        PickDetailsTree *pickDetailsTree;

    public:

        enum Type
        {
            Model = 0,
            ApplicationOutput,
            ProcessOutput,
            PickDetails
        };

    public:

        //! Constructor.
        explicit CentralTabWidget(QWidget *parent = nullptr);

        //! return MDI Area.
        QMdiArea *getMdiArea(void);

    private:

        //! Set text and icon for given tab.
        void setTabTitle(CentralTabWidget::Type tabType, RMessageType messageType = R_MESSAGE_NONE, const QString &additionalText = QString());

    private slots:

        //! Print application info message.
        void onInfoPrinted(const QString &message);

        //! Print application notice message.
        void onNoticePrinted(const QString &message);

        //! Print application warning message.
        void onWarningPrinted(const QString &message);

        //! Print application error message.
        void onErrorPrinted(const QString &message);

        //! Print process standard output message.
        void onProcessReadyStandardOutput(const QString &message);

        //! Print process standard error message.
        void onProcessReadyStandardError(const QString &message);

        //! Central tab widget current changed.
        void onCurrentChanged(int tabPosition);

        //! Pick list has changed.
        void onPickLostChanged(void);

};

#endif // CENTRAL_TAB_WIDGET_H
