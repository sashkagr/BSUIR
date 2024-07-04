#include "addshelfdialog.h"
#include "customapplication.h"
#include "shelveslist.h"
#include "ui_addshelfdialog.h"

AddShelfDialog::AddShelfDialog(std::vector<shelf> shelves, QWidget *parentProxy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddShelfDialog)
{
    this->shelves = shelves;

    ui->setupUi(this);
    parentWindow = parentProxy;
    parentProxy->close();
    setWindowTitle(CustomApplication::applicationName());
}

AddShelfDialog::~AddShelfDialog()
{
    delete ui;
}

void AddShelfDialog::on_addShelf_clicked()
{
    try
    {
        QString shelfName = ui->lineEdit->text();
        bool nameIsFree = true;

        for (shelf shelf : shelves)
        {
            if (shelf.getNaming() == shelfName)
            {
                nameIsFree = false;
                break;
            }
        }

        if (!nameIsFree)
        {
            throw std::runtime_error("Name is already in use");
        }

        db->addShelf(shelfName);
        parentWindow->show();
        this->close();
    }
    catch (const std::runtime_error &e)
    {
        ui->nameInUseWarning->setText(e.what());
    }
}



void AddShelfDialog::on_cancelButton_clicked()
{
    parentWindow->show();
    this->close();
}

