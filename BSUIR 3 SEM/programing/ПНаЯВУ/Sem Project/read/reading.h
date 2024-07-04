#ifndef READING_H
#define READING_H
#include <QButtonGroup>
#include <QDialog>
#include <QWidget>

namespace Ui {
class Reading;
}

class Reading : public QDialog
{
    Q_OBJECT
private:

    int currentPage;
    QList<QString> pageList;

    QString content;
    int fontSize = 20;
    QString fontFamily = "Times New Roman";
    int currentPosition = 0;

public:
    QButtonGroup group1;
    QButtonGroup group2;
    QButtonGroup group3;

    explicit Reading(QString content,int id, QWidget *parentProxy, QWidget *parent = nullptr);
    ~Reading();
    void displayBook(QString html);

    void displayCurrentPage();
    void splitPages(const QString &html);
    void displayBookPageByPage(QString html);
    void on_fontStyleChanged(bool isChecked);
    void on_backgroundColorChanged(bool isChecked);
    void on_fontChanged(bool isChecked);
    QString generateStyledHTML(const QString& styleSheet,int fontSize, QString fontFamily);
public slots:
    void on_exitButton_clicked();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_spinBox_valueChanged(int newValue);
private:
    Ui::Reading *ui;
    QWidget *parentWindow;

};

#endif // READING_H
