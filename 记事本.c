#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName = "无标题";
    isUnTitled = true;   //是否命名
    isCaseDiffer = false;
    isSequence = true;
    findDialog = new FindDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 新建
 */
void MainWindow::on_action_new_triggered()
{
    if(ui->textEdit->document()->isModified())
    {
       int choice = QMessageBox::question(this,"提示信息",
                QString("是否将更改保存到%1?").arg(fileName),
                QMessageBox::Ok|QMessageBox::Cancel);
       if(choice==QMessageBox::Ok)
       {
           if(isUnTitled)
               on_action_saveAs_triggered();
           else
               saveFile(fileName);
       }
    }

    ui->textEdit->clear();
    setWindowTitle("无标题");
    isUnTitled = true;
}

/**
 * @brief 打开
 */
void MainWindow::on_action_open_triggered()
{
    if(ui->textEdit->document()->isModified())
    {
        int choice = QMessageBox::question(this,"提示信息",
                 QString("是否将更改保存到%1?").arg(fileName),
                 QMessageBox::Ok|QMessageBox::Cancel);

        if(choice==QMessageBox::Ok)
        {
            if(isUnTitled)
                on_action_saveAs_triggered();
            else
                saveFile(fileName);
        }
    }

    fileName = QFileDialog::getOpenFileName(this,"选择文件",
              "/","txt(*.txt);;excel(&.xls);;All Files(*.*)");
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("提示信息"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    }

    QTextStream in(&file);
    ui->textEdit->setText(in.readAll());
    this->fileName = fileName;
    isUnTitled = false;
    int begin = fileName.lastIndexOf("/") + 1;
    int count = fileName.indexOf(".") - begin;
    setWindowTitle(fileName.mid(begin,count));
}

/**
 * @brief 保存
 */
void MainWindow::on_action_save_triggered()
{
    if(isUnTitled)
        on_action_saveAs_triggered();
    else
        saveFile(fileName);
}

/**
 * @brief 另存为
 */
void MainWindow::on_action_saveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,tr("提示信息"),"/","(*.txt);;All File(*.*)");
    if (fileName.isEmpty())
    {
        QMessageBox::about(this,"提示信息","文件名为空");
        return;
    }
    else
        saveFile(fileName);
}

/**
 * @brief 保存文件
 */
void MainWindow::saveFile(QString fileName)
{
    if(fileName.isEmpty())
    {
        QMessageBox::about(this,"提示信息","文件路径不正确，请检查");
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    this->fileName = fileName;
    file.close();
}

/**
 * @brief 打印
 */
void MainWindow::on_action_print_triggered()
{
    QPrinter printer;
    QPrintDialog dlg(&printer,ui->textEdit);
    if(dlg.exec() == QDialog::Accepted)
    {
        //执行内容
    }
}

/**
 * @brief 打印预览
 */
void MainWindow::on_action_printView_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),this,
            SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void MainWindow::printPreview(QPrinter *printer)
{
    #ifdef QT_NO_PRINTER
        Q_UNUSEd(printer);
    #else
         ui->textEdit->document()->print(printer);
    #endif
}

/**
 * @brief 保存为pdf
 */
void MainWindow::on_action_pdf_triggered()
{
    QString name = QFileDialog::getSaveFileName(this,"Export PDF",
            this->fileName,"*.pdf");
    if (!fileName.isEmpty())
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(name);
        ui->textEdit->document()->print(&printer);
     }
}

/**
 * @brief 查找
 */
void MainWindow::on_action_find_triggered()
{
   findDialog->show();

}
