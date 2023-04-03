#include <CQListWidgetColumnsDialog.h>
#include <CQListWidget.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "left.xpm"
#include "right.xpm"
#include "up.xpm"
#include "down.xpm"

class ListWidget : public QListWidget {
 public:
  ListWidget(QWidget *parent=nullptr) :
   QListWidget(parent) {
  }

  QSize sizeHint() const override { return QSize(125, 200); }
};

CQListWidgetColumnsDialog::
CQListWidgetColumnsDialog(CQListWidget *list) :
 QDialog(list), list_(list)
{
  addWidgets();

  populate();
}

void
CQListWidgetColumnsDialog::
addWidgets()
{
  QVBoxLayout *vbox = new QVBoxLayout;

  setLayout(vbox);

  QHBoxLayout *hbox = new QHBoxLayout;

  vbox->addLayout(hbox);

  // left list
  allList_ = new ListWidget;

  hbox->addWidget(allList_);

  allList_->setSelectionMode(QAbstractItemView::ExtendedSelection);

  // list movement arrows
  QVBoxLayout *vbox1 = new QVBoxLayout;

  hbox->addLayout(vbox1);

  QPushButton *r_button = new QPushButton();
  QPushButton *l_button = new QPushButton();

  r_button->setIcon(QIcon(QPixmap(const_cast<const char **>(right_xpm))));
  l_button->setIcon(QIcon(QPixmap(const_cast<const char **>(left_xpm ))));

  connect(r_button, SIGNAL(clicked()), this, SLOT(addToDisplayed()));
  connect(l_button, SIGNAL(clicked()), this, SLOT(removeFromDisplayed()));

  vbox1->addStretch();
  vbox1->addWidget(r_button);
  vbox1->addWidget(l_button);
  vbox1->addStretch();

  // right list
  displayedList_ = new ListWidget;

  displayedList_->setSelectionMode(QAbstractItemView::ExtendedSelection);

  hbox->addWidget(displayedList_);

  // list order arrows
  QVBoxLayout *vbox2 = new QVBoxLayout;

  hbox->addLayout(vbox2);

  QPushButton *u_button = new QPushButton();
  QPushButton *d_button = new QPushButton();

  u_button->setIcon(QIcon(QPixmap(const_cast<const char **>(up_xpm  ))));
  d_button->setIcon(QIcon(QPixmap(const_cast<const char **>(down_xpm))));

  connect(u_button, SIGNAL(clicked()), this, SLOT(moveUpDisplayed()));
  connect(d_button, SIGNAL(clicked()), this, SLOT(moveDownDisplayed()));

  vbox2->addStretch();
  vbox2->addWidget(u_button);
  vbox2->addWidget(d_button);
  vbox2->addStretch();

  // done button
  QHBoxLayout *hbox1 = new QHBoxLayout;

  vbox->addLayout(hbox1);

  hbox1->addStretch();

  QPushButton *done = new QPushButton("Done");

  hbox1->addWidget(done);

  connect(done, SIGNAL(clicked()), this, SLOT(accept()));
}

void
CQListWidgetColumnsDialog::
populate()
{
  populateAll();

  populateDisplayed();
}

void
CQListWidgetColumnsDialog::
populateAll()
{
  allList_->clear();

  QStringList allColumns = list_->getAllColumns();

  QStringList::const_iterator p1 = allColumns.constBegin();
  QStringList::const_iterator p2 = allColumns.constEnd  ();

  for ( ; p1 != p2; ++p1)
    allList_->addItem(*p1);
}

void
CQListWidgetColumnsDialog::
populateDisplayed()
{
  displayedList_->clear();

  QStringList displayedColumns = list_->getDisplayedColumns();

  QStringList::const_iterator p1 = displayedColumns.constBegin();
  QStringList::const_iterator p2 = displayedColumns.constEnd  ();

  for ( ; p1 != p2; ++p1)
    displayedList_->addItem(*p1);
}

void
CQListWidgetColumnsDialog::
addToDisplayed()
{
  QList<QListWidgetItem *> items = allList_->selectedItems();

  QList<QListWidgetItem *>::const_iterator p1 = items.constBegin();
  QList<QListWidgetItem *>::const_iterator p2 = items.constEnd  ();

  for ( ; p1 != p2; ++p1)
    list_->showColumn((*p1)->text());

  populateDisplayed();
}

void
CQListWidgetColumnsDialog::
removeFromDisplayed()
{
  QList<QListWidgetItem *> items = displayedList_->selectedItems();

  QList<QListWidgetItem *>::const_iterator p1 = items.constBegin();
  QList<QListWidgetItem *>::const_iterator p2 = items.constEnd  ();

  for ( ; p1 != p2; ++p1)
    list_->hideColumn((*p1)->text());

  populateDisplayed();
}

void
CQListWidgetColumnsDialog::
moveUpDisplayed()
{
  QList<QListWidgetItem *> items = displayedList_->selectedItems();

  QList<QListWidgetItem *>::const_iterator p1 = items.constBegin();
  QList<QListWidgetItem *>::const_iterator p2 = items.constEnd  ();

  for ( ; p1 != p2; ++p1)
    list_->raiseColumn((*p1)->text());

  populateDisplayed();
}

void
CQListWidgetColumnsDialog::
moveDownDisplayed()
{
  QList<QListWidgetItem *> items = displayedList_->selectedItems();

  QList<QListWidgetItem *>::const_iterator p1 = items.constBegin();
  QList<QListWidgetItem *>::const_iterator p2 = items.constEnd  ();

  for ( ; p1 != p2; ++p1)
    list_->lowerColumn((*p1)->text());

  populateDisplayed();
}
