#ifndef READBOOK_H
#define READBOOK_H

#include <QMainWindow>

namespace Ui {
class readBook;
}

class readBook : public QMainWindow
{
    Q_OBJECT

public:
    explicit readBook(QWidget *parent = nullptr);
    ~readBook();
    void displayBook(const QString &content);

private slots:
    void on_nextButton_clicked();

    void on_prevButton_clicked();

private:
    Ui::readBook *ui;
};

#endif // READBOOK_H
