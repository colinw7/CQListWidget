#ifndef CQLIST_WIDGET_H
#define CQLIST_WIDGET_H

#include <QTableWidget>
#include <map>
#include <vector>

class CQListColumns {
 public:
  CQListColumns() { }

  virtual ~CQListColumns() { }

  virtual bool begin() const = 0;
  virtual bool next () const = 0;

  virtual int count() const = 0;

  virtual const QString &value() const = 0;

  virtual int compare(const QString &lhs, const QString &rhs) {
    if (lhs < rhs) { return -1; }
    if (rhs > lhs) { return  1; }
    return 0;
  }
};

template<typename T>
class CQSTLColumns : public CQListColumns {
 public:
  CQSTLColumns() { }

  CQSTLColumns(T const& columns) :
   columns_(columns) {
  }

  bool begin() const override {
    p_ = columns_.begin();

    return p_ != columns_.end();
  }

  bool next() const override {
    ++p_;

    return p_ != columns_.end();
  }

  int count() const override { return columns_.size(); }

  const QString &value() const override { return *p_; }

  void add(const QString &value) {
    columns_.push_back(value);
  }

 private:
  typedef typename T::const_iterator const_iterator;

  T                      columns_;
  mutable const_iterator p_;
};

//------

class CQListRows {
 public:
  CQListRows() { }

  virtual ~CQListRows() { }

  virtual bool begin() const = 0;
  virtual bool next () const = 0;

  virtual int count() const = 0;

  virtual const QString &value(const QString &property) const = 0;
};

template<typename T>
class CQSTLRows : public CQListRows {
 public:
  CQSTLRows() { }

  CQSTLRows(std::vector<T> const& rows) :
   rows_(rows) {
  }

  bool begin() const override {
    p_ = rows_.begin();

    return p_ != rows_.end();
  }

  bool next() const override {
    ++p_;

    return p_ != rows_.end();
  }

  int count() const override { return rows_.size(); }

  const QString &value(const QString &name) const override {
    return (*p_).value(name);
  }

  void add(const T &value) {
    rows_.push_back(value);
  }

 private:
  typedef typename std::vector<T>::const_iterator const_iterator;

  std::vector<T>         rows_;
  mutable const_iterator p_;
};

//-------

class CQListModel {
 public:
  CQListModel() :
   columns_(nullptr), rows_(nullptr) {
  }

  CQListModel(const CQListColumns &columns, const CQListRows &rows) :
   columns_(&columns), rows_(&rows) {
  }

  void setColumns(CQListColumns &columns) {
    columns_ = &columns;
  }

  void setRows(CQListRows &rows) {
    rows_ = &rows;
  }

  const CQListColumns &columns() const { return *columns_; }
  const CQListRows    &rows   () const { return *rows_   ; }

 private:
  CQListColumns const* columns_;
  CQListRows    const* rows_;
};

//-------------

class QIcon;
class QMenu;
class CQListWidgetColumnsDialog;

class CQListWidget : public QTableWidget {
  Q_OBJECT

  Q_PROPERTY(bool columnsDialog READ columnsDialog WRITE setColumnsDialog)

 private:
  struct Column {
    QString name;
    bool    displayed;

    Column(const QString &a_name="") :
     name(a_name), displayed(false) {
    }
  };

 public:
  CQListWidget(QWidget *parent = nullptr);

  void setListModel(const CQListModel &model);

  bool columnsDialog() const { return columnsDialog_; }

  void setColumnsDialog(bool flag) { columnsDialog_ = flag; }

  void populate();

  void showColumn(const QString &column);
  void hideColumn(const QString &column);

  void raiseColumn(const QString &column);
  void lowerColumn(const QString &column);

  QStringList getAllColumns() const;
  QStringList getDisplayedColumns() const;

 private:
  void createMenu();

  void displayMenu(const QPoint &point);
  void displayDialog();

  Column &getColumn(const QString &name);

 private slots:
  void headerClicked(int section);

  void selectMenuItem(QAction *action);

 private:
  typedef std::list<Column> ColumnList;

  CQListModel const*          model_;
  QIcon*                      icon_;
  QMenu*                      menu_;
  ColumnList                  columnList_;
  int                         menu_column_;
  bool                        columnsDialog_;
  CQListWidgetColumnsDialog * dialog_;
};

#endif
