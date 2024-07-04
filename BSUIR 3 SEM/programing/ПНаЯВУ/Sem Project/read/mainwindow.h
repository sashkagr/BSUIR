#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"

#include <QMainWindow>
#include <qlistwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showBooks(std::vector<Book> books);
    void showBooks(std::vector<Book> books, QString searchTitle);
    QString generateStyledHTML(const QString &htmlWithoutStyle, int fontSize, QString fontFamily);
public slots:
    void onListItemClicked(int id,QString html);
    void deleteBookClicked(int id);
private slots:
    void on_pushButton_clicked();

    void on_openShelves_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    std::vector<Book> books;
    database* db;

    bool isBookTitleMatch(QString bookTitle, QString searchTitle);
};
#endif // MAINWINDOW_H
