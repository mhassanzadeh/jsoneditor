#include "addnodedlg.h"
#include "ui_addnodedlg.h"
#include <QMessageBox>

AddNodeDlg::AddNodeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNodeDlg)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okButton()));
    connect(ui->nodeType, SIGNAL(currentIndexChanged(QString)), this, SLOT(onCurrentIndexChanged(QString)));

    QStringList typeItems;
    typeItems.append(QString(tr("����")));
    typeItems.append(QString(tr("����")));
    typeItems.append(QString(tr("����")));
    typeItems.append(QString(tr("��ֵ")));
    typeItems.append(QString(tr("����")));
    typeItems.append(QString(tr("������")));
    typeItems.append(QString(tr("�ַ���")));
    ui->nodeType->addItems(typeItems);
}

AddNodeDlg::~AddNodeDlg()
{
    delete ui;
}

void AddNodeDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString AddNodeDlg::getValue()
{
    return m_value;
}

QString AddNodeDlg::getName()
{
    return m_name;
}

QString AddNodeDlg::getType()
{
    return m_type;
}

void AddNodeDlg::okButton()
{
    m_type = ui->nodeType->currentText();
    m_value = ui->nodeType->isEnabled() == true ? ui->nodeValue->text() : QString("");
    m_name = ui->nodeTitle->text();
}

void AddNodeDlg::onCurrentIndexChanged(QString currentText)
{
    if (currentText.compare(tr("����")) == 0
        || currentText.compare(tr("����")) == 0
        || currentText.compare(tr("��ֵ")) == 0)
    {
        ui->nodeValue->setEnabled(false);
    }
    else
    {
        ui->nodeValue->setEnabled(true);
    }
}

void AddNodeDlg::hideEvent(QHideEvent *e)
{
    if (ui->nodeTitle->text().length() == 0)
    {
        QMessageBox::information(this, tr("��ʾ"), tr("������д�ڵ�����"));
        return;
    }

    if (ui->nodeType->currentText().compare(tr("����")) == 0)
        if (ui->nodeValue->text().toLower().compare(tr("true")) != 0
            && ui->nodeValue->text().toLower().compare(tr("false")) != 0)
        {
            QMessageBox::information(this, tr("��ʾ"), tr("�������͵�ֵ������ true �� false"));
            return;
        }

    QDialog::hideEvent(e);
}
