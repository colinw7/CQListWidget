#include <CQListWidget.h>
#include <CQListWidgetColumnsDialog.h>

#include <QHeaderView>
#include <QMenu>

CQListWidget::
CQListWidget(QWidget *parent) :
 QTableWidget(parent)
{
  verticalHeader()->hide();

  setSelectionBehavior(SelectRows);

  setShowGrid(false);

  QPixmap pixmap("select.xpm");

  icon_ = new QIcon(pixmap);
  menu_ = nullptr;

  populate();

  connect(horizontalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(headerClicked(int)));
}

void
CQListWidget::
setListModel(const CQListModel &model)
{
  model_ = &model;

  columnList_.clear();

  const CQListColumns &cols = model_->columns();

  for (bool ok = cols.begin(); ok; ok = cols.next()) {
    const QString &name = cols.value();

    Column column(name);

    columnList_.push_back(column);
  }
}

void
CQListWidget::
populate()
{
  int num_columns = 0;

  if (model_) {
    setRowCount(model_->rows().count());

    const CQListColumns &cols = model_->columns();

    for (bool ok = cols.begin(); ok; ok = cols.next()) {
      const QString &name = cols.value();

      Column &column = getColumn(name);

      if (! column.displayed) continue;

      ++num_columns;
    }
  }

  setColumnCount(num_columns + 1);

  int column_num = 0;

  if (model_) {
    const CQListRows    &rows = model_->rows();
    const CQListColumns &cols = model_->columns();

    for (bool ok = cols.begin(); ok; ok = cols.next()) {
      const QString &name = cols.value();

      Column &column = getColumn(name);

      if (! column.displayed) continue;

      QTableWidgetItem *item = new QTableWidgetItem(name);

      setHorizontalHeaderItem(column_num, item);

      QHeaderView::ResizeMode resizeMode =
       (column_num != num_columns - 1 ? QHeaderView::Interactive : QHeaderView::Stretch);

      //horizontalHeader()->setResizeMode(column_num, resizeMode);
      horizontalHeader()->setSectionResizeMode(column_num, resizeMode);

      int row_num = 0;

      for (bool ok1 = rows.begin(); ok1; ok1 = rows.next()) {
        const QString &value = rows.value(name);

        QTableWidgetItem *item = new QTableWidgetItem(value);

        setItem(row_num, column_num, item);

        ++row_num;
      }

      ++column_num;
    }
  }

  QTableWidgetItem *item = new QTableWidgetItem(*icon_, "");

  setHorizontalHeaderItem(column_num, item);

  //horizontalHeader()->setResizeMode(column_num, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode(column_num, QHeaderView::ResizeToContents);

  menu_column_ = column_num;

  // force resize
  resize(size() + QSize(1, 1));
  resize(size() - QSize(1, 1));
}

void
CQListWidget::
createMenu()
{
  delete menu_;

  menu_ = new QMenu;

  const CQListColumns &cols = model_->columns();

  for (bool ok = cols.begin(); ok; ok = cols.next()) {
    const QString &name = cols.value();

    QAction *action = menu_->addAction(name);

    action->setCheckable(true);

    Column &column = getColumn(name);

    action->setChecked(column.displayed);

    connect(menu_, SIGNAL(triggered(QAction *)), this, SLOT(selectMenuItem(QAction *)));
  }
}

void
CQListWidget::
showColumn(const QString &name)
{
  Column &column = getColumn(name);

  column.displayed = true;

  populate();
}

void
CQListWidget::
hideColumn(const QString &name)
{
  Column &column = getColumn(name);

  column.displayed = false;

  populate();
}

void
CQListWidget::
raiseColumn(const QString &name)
{
  ColumnList::iterator p2 = columnList_.begin();
  ColumnList::iterator p1 = p2++;
  ColumnList::iterator p3 = columnList_.end  ();

  for ( ; p2 != p3; p1 = p2, ++p2) {
    if ((*p2).name == name) {
      std::swap(*p1, *p2);
      break;
    }
  }

  populate();
}

void
CQListWidget::
lowerColumn(const QString &name)
{
  ColumnList::iterator p2 = columnList_.begin();
  ColumnList::iterator p1 = p2++;
  ColumnList::iterator p3 = columnList_.end  ();

  for ( ; p2 != p3; p1 = p2, ++p2) {
    if ((*p1).name == name) {
      std::swap(*p1, *p2);
      break;
    }
  }

  populate();
}

void
CQListWidget::
displayMenu(const QPoint &point)
{
  createMenu();

  menu_->popup(point);
}

void
CQListWidget::
displayDialog()
{
  if (! dialog_)
    dialog_ = new CQListWidgetColumnsDialog(this);

  dialog_->show();
}

void
CQListWidget::
headerClicked(int section)
{
  if (section != menu_column_)
    return;

  if (! columnsDialog_)
    displayMenu(QCursor::pos());
  else
    displayDialog();
}

void
CQListWidget::
selectMenuItem(QAction *action)
{
  if (action->isChecked())
    showColumn(action->text());
  else
    hideColumn(action->text());
}

QStringList
CQListWidget::
getAllColumns() const
{
  QStringList columnList;

  if (model_) {
    const CQListColumns &cols = model_->columns();

    for (bool ok = cols.begin(); ok; ok = cols.next())
      columnList.push_back(cols.value());
  }

  return columnList;
}

QStringList
CQListWidget::
getDisplayedColumns() const
{
  QStringList columnList;

  ColumnList::const_iterator p1 = columnList_.begin();
  ColumnList::const_iterator p2 = columnList_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1).displayed)
      columnList.push_back((*p1).name);

  return columnList;
}

CQListWidget::Column &
CQListWidget::
getColumn(const QString &name)
{
  static Column bad_column;

  ColumnList::iterator p1 = columnList_.begin();
  ColumnList::iterator p2 = columnList_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1).name == name)
      return *p1;

  return bad_column;
}
