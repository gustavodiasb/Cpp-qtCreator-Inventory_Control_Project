#include "fm_gestaovendas.h"
#include "ui_fm_gestaovendas.h"
#include <QtSql>
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QDebug>
#include <QDesktopServices>

fm_gestaoVendas::fm_gestaoVendas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_gestaoVendas)
{
    ui->setupUi(this);

    if (!con.aberto()) {
        if (!con.abrir()) {
            QMessageBox::warning(this, "Erro", "Erro ao conectar ao banco de dados!");
        }
    }

    // Tabela Produtos (itens) da venda
    ui->tw_listarProdutosVendas->horizontalHeader()->setVisible(true);
    ui->tw_listarProdutosVendas->setColumnCount(5);
    QStringList cabecalhoProdutos = {"ID movimentação", "Produto", "Quantidade", "Valor unitário", "Valor total"};
    ui->tw_listarProdutosVendas->setHorizontalHeaderLabels(cabecalhoProdutos);
    //ui->tw_listarProdutosVendas->verticalHeader()->setVisible(false);
    // redimencionar coluna e linha automaticamente
    ui->tw_listarProdutosVendas->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tw_listarProdutosVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tw_listarProdutosVendas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_listarProdutosVendas->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //QSqlQuery query;
    //int contLinha = 0;

    // Tabela Vendas
    ui->tw_listarVenda->horizontalHeader()->setVisible(true);
    ui->tw_listarVenda->setColumnCount(6);
    QStringList cabecalhoVendas = {"ID", "Data", "Hora", "Colaborador", "Valor total", "Tipo pagamento"};
    ui->tw_listarVenda->setHorizontalHeaderLabels(cabecalhoVendas);
    //ui->tw_listarVenda->verticalHeader()->setVisible(false);
    // redimencionar coluna e linha automaticamente
    ui->tw_listarVenda->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tw_listarVenda->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tw_listarVenda->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_listarVenda->setEditTriggers(QAbstractItemView::NoEditTriggers);

    definirDadosTabelaVenda("buscarTudo");

}

fm_gestaoVendas::~fm_gestaoVendas()
{
    delete ui;
}

void fm_gestaoVendas::on_tw_listarVenda_itemSelectionChanged()
{
    ui->tw_listarProdutosVendas->setRowCount(0);

    QSqlQuery query;
    int contLinha = 0;
    QString idVenda = ui->tw_listarVenda->item(ui->tw_listarVenda->currentRow(), 0)->text();

    query.prepare("SELECT id_venda, produto, quantidade, valor_unit, valor_total "
                  "FROM tb_produtosVendas "
                  "WHERE id_venda = "+idVenda+
                  " ORDER BY produto ASC");
    query.exec();
    query.first();

    do {
        ui->tw_listarProdutosVendas->insertRow(contLinha);
        ui->tw_listarProdutosVendas->setItem(contLinha, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tw_listarProdutosVendas->setItem(contLinha, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tw_listarProdutosVendas->setItem(contLinha, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tw_listarProdutosVendas->setItem(contLinha, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tw_listarProdutosVendas->setItem(contLinha, 4, new QTableWidgetItem(query.value(4).toString()));
        contLinha++;
    } while (query.next());
}


void fm_gestaoVendas::definirDadosTabelaVenda(QString tipoBusca) {
    QString sql = "";
    QString dataInicial = ui->de_dataInicial->text();
    QString dataFinal = ui->de_dataFinal->text();

    if (tipoBusca == "buscarTudo") {
        sql = "SELECT * FROM tb_vendas ORDER BY id_venda ASC";
    } else if (tipoBusca == "filtrarPorData") {
        sql = "SELECT * FROM tb_vendas "
              "WHERE data_venda BETWEEN '"+dataInicial+"' AND '"+dataFinal+
              "' ORDER BY id_venda ASC";
    }
    qInfo() << sql;
    ui->tw_listarVenda->setRowCount(0);

    con.abrir();
    QSqlQuery query;
    int contLinha = 0;

    query.prepare(sql);
    query.exec();
    qDebug() << query.lastError().text();
    query.first();

    do {
        ui->tw_listarVenda->insertRow(contLinha);
        ui->tw_listarVenda->setItem(contLinha, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tw_listarVenda->setItem(contLinha, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tw_listarVenda->setItem(contLinha, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tw_listarVenda->setItem(contLinha, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tw_listarVenda->setItem(contLinha, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tw_listarVenda->setItem(contLinha, 5, new QTableWidgetItem(query.value(5).toString()));
        contLinha++;
    } while (query.next());

}


void fm_gestaoVendas::on_btn_filtrarData_clicked()
{
    definirDadosTabelaVenda("filtrarPorData");
}


void fm_gestaoVendas::on_pushButton_clicked()
{
    definirDadosTabelaVenda("buscarTudo");
}


void fm_gestaoVendas::on_pushButton_2_clicked()
{
    QString idVenda = ui->tw_listarVenda->item(ui->tw_listarVenda->currentRow(), 0)->text();
    QString nomeRelatorio = idVenda+"_venda.pdf";
    QString local = QDir::currentPath()+"/reportsGenerated/"+nomeRelatorio;

    // Definindo formato da impressão
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(local);

    // Inserindo os dados no relatório
    QPainter painter;
    if (!painter.begin(&printer)) {
        qDebug() << "Erro ao abrir PDF "+nomeRelatorio;
        return;
    }

    int linha = 250;
    int salto = 20;
    QString dataVenda = ui->tw_listarVenda->item(ui->tw_listarVenda->currentRow(), 1)->text();

    painter.drawPixmap(25, 25, QPixmap(":/imagens/imgs/cfblogo.png"));

    painter.drawText(25, 200, "ID: "+idVenda);
    painter.drawText(150, 200, "Data: "+dataVenda);

    for (int i = 0; i < ui->tw_listarProdutosVendas->rowCount(); i++) {
        painter.drawText(25, linha, ui->tw_listarProdutosVendas->item(i, 0)->text());
        painter.drawText(50, linha, ui->tw_listarProdutosVendas->item(i, 1)->text());
        painter.drawText(300, linha, ui->tw_listarProdutosVendas->item(i, 2)->text());
        linha += salto;
    }

    printer.newPage();

    painter.drawText(25, 25, "Curso C++ Qt Creator");

    painter.end();

    // Ao gerar PDF, abrir o arquivo gerado na pasta
    QDesktopServices::openUrl(QUrl("file:///"+local));

}

