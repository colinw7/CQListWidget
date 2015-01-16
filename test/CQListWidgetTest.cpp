#include <CQListWidgetTest.h>
#include <CQListWidget.h>
#include <QApplication>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QDateTime>

class ListFile {
 public:
  ListFile(const QString &name) :
   name_(name) {
  }

  const QString &value(const QString &name) const {
    static QString buffer;

    QFile file(name_);

    if      (name == "Perm") {
      QFile::Permissions perm = file.permissions();

      buffer = "";

      buffer += (perm & QFile::ReadOwner  ? "r" : "-");
      buffer += (perm & QFile::WriteOwner ? "w" : "-");
      buffer += (perm & QFile::ExeOwner   ? "x" : "-");
      buffer += (perm & QFile::ReadGroup  ? "r" : "-");
      buffer += (perm & QFile::WriteGroup ? "w" : "-");
      buffer += (perm & QFile::ExeGroup   ? "x" : "-");
      buffer += (perm & QFile::ReadOther  ? "r" : "-");
      buffer += (perm & QFile::WriteOther ? "w" : "-");
      buffer += (perm & QFile::ExeOther   ? "x" : "-");
    }
    else if (name == "Name")
      buffer = file.fileName();
    else if (name == "Type")
      buffer = QFileInfo(file).suffix();
    else if (name == "Date")
      buffer = QFileInfo(file).lastModified().toString();
    else if (name == "Owner")
      buffer = QFileInfo(file).owner();
    else if (name == "Group")
      buffer = QFileInfo(file).group();
    else if (name == "Size")
      buffer = QString("%1").arg(file.size());
    else
      buffer = "";

    return buffer;
  }

 private:
  QString name_;
};

int
main(int argc, char **argv)
{
  CQSTLColumns< std::vector<QString> > columns;

  columns.add("Perm" );
  columns.add("Name" );
  columns.add("Type" );
  columns.add("Date" );
  columns.add("Owner");
  columns.add("Group");
  columns.add("Size" );

  CQSTLRows<ListFile> rows;

  QDir dir(".");

  QStringList files = dir.entryList();

  for (int i = 0; i < files.size(); ++i)
    rows.add(files[i]);

  CQListModel model(columns, rows);

  //------

  QApplication app(argc, argv);

  CQListWidgetTest *test = new CQListWidgetTest;

  QVBoxLayout *layout = new QVBoxLayout;

  test->setLayout(layout);

  CQListWidget *list = new CQListWidget(test);

  list->setColumnsDialog(true);

  list->setModel(model);

  list->showColumn("Perm");
  list->showColumn("Name");
  list->showColumn("Date");

  list->populate();

  layout->addWidget(list);

  test->show();

  return app.exec();
}
