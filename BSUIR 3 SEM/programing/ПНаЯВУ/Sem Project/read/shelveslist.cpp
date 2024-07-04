#include "shelveslist.h"
#include "ui_shelveslist.h"

#include "addshelfdialog.h"
#include "customapplication.h"
#include "shelfbooks.h"

#include <QHBoxLayout>
#include <QLabel>

ShelvesList::ShelvesList(QWidget *parentProxy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShelvesList)
{
    ui->setupUi(this);
    parentWindow = parentProxy;
    parentProxy->close();
    showAllShalves();
    setWindowTitle(CustomApplication::applicationName());
}

ShelvesList::~ShelvesList()
{
    delete ui;
}

void ShelvesList::showAllShalves()
{
    shelves = db->getAllShelves();
    disconnect(ui->listWidget, &QListWidget::itemClicked, nullptr, nullptr);
    ui->listWidget->clear();

    for (shelf shef : shelves) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, shef.getId());
        QWidget *customWidget = new QWidget();
        QPushButton *buttonDelete = new QPushButton();
        QHBoxLayout *layout = new QHBoxLayout(customWidget);
        QLabel *textLayoutWidget = new QLabel();
        QVBoxLayout *textLayout = new QVBoxLayout(textLayoutWidget);

        QLabel *titleLabel = new QLabel(shef.getNaming());

        buttonDelete->setObjectName("deleteButton");
        buttonDelete->setFixedWidth(80);
        buttonDelete->setFixedHeight(24);
        buttonDelete->setText("DELETE");

        QString styles = "font-size: 20px";
        titleLabel->setStyleSheet(styles);
        titleLabel->setAlignment(Qt::AlignBottom);
        textLayout->addWidget(titleLabel);
        textLayout->setAlignment(Qt::AlignTop); // Align the text to the top
        textLayout->setContentsMargins(0, 0, 0, 0); // Remove any margins
        textLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

        layout->addWidget(textLayoutWidget);
        layout->addWidget(buttonDelete);
        customWidget->setLayout(layout);
        item->setSizeHint(customWidget->sizeHint());

        ui->listWidget->setItemWidget(item, customWidget);
        connect(buttonDelete, &QPushButton::clicked, [this, id = shef.getId()](){
            db->deleteShelf(id);
            showAllShalves();
        });

    }
    connect(ui->listWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        for (shelf shef : shelves) {
            QWidget *widget = ui->listWidget->itemWidget(item);
            int shelfId = item->data(Qt::UserRole).toInt();
            if (widget) {

                if (shelfId==shef.getId()) {

                    onListItemClicked(shef.getId(), shef.getNaming());
                    break;
                }
            }
        }
    });
}

void ShelvesList::onListItemClicked(int id, QString name)
{
    ShelfBooks *shelfBooks = new ShelfBooks(id, name, this);
    shelfBooks->show();
}

void ShelvesList::on_exitButton_clicked()
{
    parentWindow->show();
    this->close();
}


void ShelvesList::on_addShelfBtn_clicked()
{
    AddShelfDialog *addShelfDialog = new AddShelfDialog(shelves, this);
    addShelfDialog->exec();
    showAllShalves();
}

