#include "shelfbooks.h"
#include "ui_shelfbooks.h"
#include "selectbookstoshelf.h"
#include "customapplication.h"

ShelfBooks::ShelfBooks(int shelfId, QString shelfName, QWidget *parentProxy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShelfBooks)
{
    ui->setupUi(this);
    this->shelfId = shelfId;
    this->shelfName = shelfName;
    this->parent = parentProxy;
    ui->shelfNameLabel->setText(shelfName);

    parentProxy->close();

    displayAllBooksForShelf();
    setWindowTitle(CustomApplication::applicationName());
}

ShelfBooks::~ShelfBooks()
{
    delete ui;
}

void ShelfBooks::on_shelvesButton_clicked()
{
    this->close();
    parent->show();
}

void ShelfBooks::displayAllBooksForShelf(QString searchTitle)
{
    books = db->getBooksFromShelf(this->shelfId);
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
            db->deleteBookFromShelf(this->shelfId, id);
            displayAllBooksForShelf();
        });

    }
    connect(ui->listWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {

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

void ShelfBooks::displayAllBooksForShelf()
{
    displayAllBooksForShelf("");
}

void ShelfBooks::onListItemClicked(int id,QString html)
{
    Reading *r=new Reading( html, id,this);
    r->show();
    ui->lineEdit->setText("");
}

bool ShelfBooks::isBookTitleMatch(QString bookTitle, QString searchTitle)
{

    bookTitle = bookTitle.toLower();
    searchTitle = searchTitle.toLower();

    return bookTitle.startsWith(searchTitle);
}


void ShelfBooks::on_addToShelf_clicked()
{
    SelectBooksToShelf *selectBooksToShelf = new SelectBooksToShelf(this->shelfId, this->books, this);
    selectBooksToShelf->exec();
    displayAllBooksForShelf();
    ui->lineEdit->setText("");
}


void ShelfBooks::on_lineEdit_textChanged(const QString &arg1)
{
    displayAllBooksForShelf(arg1);
}

