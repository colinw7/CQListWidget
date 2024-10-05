#ifndef CQLIST_WIDGET_COLUMNS_DIALOG_H
#define CQLIST_WIDGET_COLUMNS_DIALOG_H

#include <QDialog>

class QListWidget;
class CQListWidget;

class CQListWidgetColumnsDialog : public QDialog {
  Q_OBJECT

 public:
  CQListWidgetColumnsDialog(CQListWidget *parent);

 private:
  void addWidgets();

  void populate();
  void populateAll();
  void populateDisplayed();

 private slots:
  void addToDisplayed();
  void removeFromDisplayed();
  void moveUpDisplayed();
  void moveDownDisplayed();

 private:
  CQListWidget* list_          { nullptr };
  QListWidget*  allList_       { nullptr };
  QListWidget*  displayedList_ { nullptr };
};

#endif
