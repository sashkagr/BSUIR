#include "customapplication.h"
#include "selectbookstoshelf.h"
#include "ui_selectbookstoshelf.h"

#include <QHBoxLayout>
#include <QLabel>

SelectBooksToShelf::SelectBooksToShelf(int shelfId, std::vector<Book> booksOnShelf, QWidget *parentProxy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectBooksToShelf)
{
    this->shelfId = shelfId;
    this->parent = parentProxy;
    this->booksOnShelf = booksOnShelf;
    ui->setupUi(this);
    parentProxy->close();
    displayAllAvailableBooks();
    setWindowTitle(CustomApplication::applicationName());
}

SelectBooksToShelf::~SelectBooksToShelf()
{
    delete ui;
}

void SelectBooksToShelf::on_cancelButton_clicked()
{
    this->close();
    parent->show();
}

void SelectBooksToShelf::displayAllAvailableBooks()
{
    int maxImageWidth = 100;
    int maxImageHeight = 150;
    ui->listWidget->clear();

    std::vector<Book> books = db->getAllBooks();
    for (Book book : books) {
        if (!checkBookNotOnShelf(book.getId())) continue;
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, book.getId());
        QWidget *customWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(customWidget);
        QLabel *imageLabel = new QLabel();
        QLabel *textLayoutWidget = new QLabel();
        QVBoxLayout *textLayout = new QVBoxLayout(textLayoutWidget);

        QLabel *titleLabel = new QLabel(book.getTitle());
        QLabel *authorLabel = new QLabel(book.getAuthor());

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
        customWidget->setLayout(layout);
        item->setSizeHint(customWidget->sizeHint());

        ui->listWidget->setItemWidget(item, customWidget);

    }
    connect(ui->listWidget, &QListWidget::itemClicked, [this, books](QListWidgetItem *item) {

        for (Book book : books) {
            QWidget *widget = ui->listWidget->itemWidget(item);
            int bookId = item->data(Qt::UserRole).toInt();
            if (widget) {

                if (bookId==book.getId()) {

                    onListItemClicked(book.getId());
                    break;
                }
            }
        }
    });
}

bool SelectBooksToShelf::checkBookNotOnShelf(int bookId)
{
    bool result = true;
    for (Book book : booksOnShelf)
    {
        if (book.getId() == bookId)
        {
            result = false;
            break;
        }
    }
    return result;
}

void SelectBooksToShelf::onListItemClicked(int bookId)
{
    db->addBookOnShelf(this->shelfId, bookId);
    this->close();
    parent->show();
}

