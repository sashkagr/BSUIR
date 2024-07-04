#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Fb2Helper.h"
#include "database.h"
#include "reading.h"
#include "shelveslist.h"
#include "customapplication.h"
#include <QListWidgetItem>
#include <qfiledialog.h>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db= new database();
    books = db->getAllBooks();
    showBooks(books);
    setWindowTitle(CustomApplication::applicationName());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
}

void MainWindow::showBooks(std::vector<Book> books, QString searchTitle)
{
    int maxImageWidth = 100;
    int maxImageHeight = 150;
    disconnect(ui->listWidget, &QListWidget::itemClicked, nullptr, nullptr);
    ui->listWidget->clear();

    for (Book book : books) {
        if (!searchTitle.isEmpty() && !isBookTitleMatch(book.getTitle(), searchTitle)) continue;
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, book.getId());
        QWidget *customWidget = new QWidget();
        QPushButton *buttonDelete = new QPushButton();
        QHBoxLayout *layout = new QHBoxLayout(customWidget);
        QLabel *imageLabel = new QLabel();
        QLabel *textLayoutWidget = new QLabel();
        QVBoxLayout *textLayout = new QVBoxLayout(textLayoutWidget);

        QLabel *titleLabel = new QLabel(book.getTitle());
        QLabel *authorLabel = new QLabel(book.getAuthor());

        buttonDelete->setObjectName("deleteButton");
        buttonDelete->setFixedWidth(80);
        buttonDelete->setFixedHeight(24);
        buttonDelete->setText("DELETE");

        // Extract the Base64 image data from the HTML tag
        QString imgTag = book.getImage();
        QString base64Data = imgTag.mid(imgTag.indexOf(',') + 1); // Extract the Base64 part
        QByteArray imageData = QByteArray::fromBase64(base64Data.toUtf8());
        QPixmap imagePixmap;
        imagePixmap.loadFromData(imageData);
        imagePixmap = imagePixmap.scaled(maxImageWidth, maxImageHeight, Qt::KeepAspectRatio);
        imageLabel->setPixmap(imagePixmap);

        QString labelStyle = "color: #333; font-size: 20px;";
        titleLabel->setStyleSheet(labelStyle+"align:top; ");
        authorLabel->setStyleSheet(labelStyle+"text-align:bottom;");
        titleLabel->setAlignment(Qt::AlignBottom);
        authorLabel->setAlignment(Qt::AlignTop);

        customWidget->setStyleSheet("background-color: #f7f7f7; padding: 0px; margin: 0px;");

        imageLabel->setFixedWidth(100);

        textLayout->addWidget(titleLabel);
        textLayout->addWidget(authorLabel);

        layout->addWidget(imageLabel);
        layout->addWidget(textLayoutWidget);
        layout->addWidget(buttonDelete);
        customWidget->setLayout(layout);
        item->setSizeHint(customWidget->sizeHint());

        ui->listWidget->setItemWidget(item, customWidget);
        connect(buttonDelete, &QPushButton::clicked, [this, id = book.getId()](){
            deleteBookClicked(id);
        });

    }
    connect(ui->listWidget, &QListWidget::itemClicked, [this, books](QListWidgetItem *item) {

        for (Book book : books) {
            QWidget *widget = ui->listWidget->itemWidget(item);
            int bookId = item->data(Qt::UserRole).toInt();
            if (widget) {

                if (bookId==book.getId()) {

                    onListItemClicked(book.getId(),book.getHtml());
                    break;
                }
            }
        }
    });
}

void MainWindow::showBooks(std::vector<Book> books)
{
    showBooks(books, QString(""));
}

void MainWindow::onListItemClicked(int id,QString html)
{
    Reading *r=new Reading( html, id,this);
    r->show();
    ui->lineEdit->setText("");
}

void MainWindow::deleteBookClicked(int id)
{
    db->deleteBook(id);
    books = db->getAllBooks();
    showBooks(books);
}

void MainWindow::on_pushButton_clicked()
{

    QString filePath = QFileDialog::getOpenFileName(this, "Open .fb2 File", QString(), "FB2 Files (*.fb2)");
    if (!filePath.isEmpty())
    {
        Book book=fb2helper::convertFb2ToHTML(filePath);

        db->addBook(book);
        books = db->getAllBooks();
        showBooks(books);
    }
    ui->lineEdit->setText("");
}



void MainWindow::on_openShelves_clicked()
{
    ShelvesList * sl = new ShelvesList(this);
    sl->show();
    ui->lineEdit->setText("");
}

bool MainWindow::isBookTitleMatch(QString bookTitle, QString searchTitle)
{

    bookTitle = bookTitle.toLower();
    searchTitle = searchTitle.toLower();

    return bookTitle.startsWith(searchTitle);
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    showBooks(books, arg1);
}

