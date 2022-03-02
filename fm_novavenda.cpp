#include "fm_novavenda.h"
#include "ui_fm_novavenda.h"
#include <QMessageBox>
#include "fm_editarprodutovenda.h"
#include "fm_principal.h"
#include "variaveis_globais.h"
#include "funcoes_globais.h"

QString fm_novaVenda::g_idProduto;
QString fm_novaVenda::g_produto;
QString fm_novaVenda::g_quantidade;
QString fm_novaVenda::g_valorUnitario;
QString fm_novaVenda::g_valorTotal;
bool fm_novaVenda::alteracao;

fm_novaVenda::fm_novaVenda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_novaVenda)
{
    ui->setupUi(this);
    ui->txt_idProduto->setFocus();

    if (!con.aberto()) {
        if (!con.abrir()) {
            QMessageBox::warning(this, "Erro", "Erro ao conectar ao banco de dados!");
        }
    }

    ui->tw_listaProdutos->setColumnCount(5);

    ui->tw_listaProdutos->setColumnWidth(0, 100);
    ui->tw_listaProdutos->setColumnWidth(1, 200);
    ui->tw_listaProdutos->setColumnWidth(2, 100);
    ui->tw_listaProdutos->setColumnWidth(3, 100);
    ui->tw_listaProdutos->setColumnWidth(4, 100);

    QStringList cabecalho = {"Código", "Produto", "Valor Un.", "Quantidade", "Total"};
    ui->tw_listaProdutos->setHorizontalHeaderLabels(cabecalho);

    ui->tw_listaProdutos->setStyleSheet("QTableView{selection-background-color:red;}");
    ui->tw_listaProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_listaProdutos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_listaProdutos->verticalHeader()->setVisible(false);

    numLinha = 0;

    limparCampos();

}

fm_novaVenda::~fm_novaVenda()
{
    delete ui;
}

void fm_novaVenda::on_txt_idProduto_returnPressed()
{
    QString id = ui->txt_idProduto->text();
    QString qtde = ui->txt_qtde->text();
    double valorTotal;
    QSqlQuery query;

    query.prepare("SELECT id_produto, produto, valor_venda FROM tb_produtos WHERE id_produto = "+id);

    if (query.exec()) {
        query.first();

        if (query.value(0).toString() != "") {
            ui->tw_listaProdutos->insertRow(numLinha);
            ui->tw_listaProdutos->setItem(numLinha, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tw_listaProdutos->setItem(numLinha, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tw_listaProdutos->setItem(numLinha, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tw_listaProdutos->setItem(numLinha, 3, new QTableWidgetItem(qtde));
            valorTotal = ui->txt_qtde->text().toDouble() * query.value(2).toDouble();
            ui->tw_listaProdutos->setItem(numLinha, 4, new QTableWidgetItem(QString::number(valorTotal)));
            ui->tw_listaProdutos->setRowHeight(numLinha, 20);

            numLinha++;
            // somar valores do TW
            ui->lb_totalVenda->setText("Total: R$ "+QString::number(calculaTotal(ui->tw_listaProdutos, 4)));

        } else {
            QMessageBox::warning(this, "Erro", "Produto não encontrado!");
        }

        limparCampos();

    } else {
        QMessageBox::warning(this, "Erro", "Erro ao inserir produto!");
    }

}

void fm_novaVenda::limparCampos() {
    ui->txt_idProduto->clear();
    ui->txt_qtde->setText("1");
    ui->txt_idProduto->setFocus();
}

double fm_novaVenda::calculaTotal(QTableWidget *tw, int coluna) {
    int totalLinha;
    double total = 0;

    totalLinha = tw->rowCount();
    for (int i = 0; i < totalLinha; i++) {
        total += tw->item(i, coluna)->text().toDouble();
    }

    return total;

}

void fm_novaVenda::on_btn_excluir_clicked()
{
    if (ui->tw_listaProdutos->currentColumn() != -1) {
        int linha   = ui->tw_listaProdutos->currentRow();
        QString produto = ui->tw_listaProdutos->item(linha, 1)->text();

        QMessageBox::StandardButton opcao = QMessageBox::question(this, "Remoção", "Deseja remover o produto "+produto+" ?", QMessageBox::Yes|QMessageBox::No);

        if (opcao == QMessageBox::Yes) {
            ui->tw_listaProdutos->removeRow(linha);
            ui->lb_totalVenda->setText("Total: R$ "+QString::number(calculaTotal(ui->tw_listaProdutos, 4)));
            numLinha--;
        }
    } else {
        QMessageBox::warning(this, "Erro", "Selecione um produto primeiro");
    }
}

void fm_novaVenda::on_btn_editar_clicked()
{
    if (ui->tw_listaProdutos->currentColumn() != -1) {
        int linha = ui->tw_listaProdutos->currentRow();
        g_idProduto     = ui->tw_listaProdutos->item(linha, 0)->text();
        g_produto       = ui->tw_listaProdutos->item(linha, 1)->text();
        g_valorUnitario = ui->tw_listaProdutos->item(linha, 2)->text();
        g_quantidade    = ui->tw_listaProdutos->item(linha, 3)->text();

        fm_editarProdutoVenda f_editarProdutoVenda;
        f_editarProdutoVenda.exec();

        if (alteracao) {
            ui->tw_listaProdutos->item(linha, 2)->setText(g_valorUnitario);
            ui->tw_listaProdutos->item(linha, 3)->setText(g_quantidade);
            ui->tw_listaProdutos->item(linha, 4)->setText(g_valorTotal);
            ui->lb_totalVenda->setText("Total: R$ "+QString::number(calculaTotal(ui->tw_listaProdutos, 4)));
        }
    }
}

void fm_novaVenda::on_btn_venda_clicked()
{
    if (ui->tw_listaProdutos->rowCount() > 0) {

        int idVenda;
        QString msgFimVenda;
        double total = calculaTotal(ui->tw_listaProdutos, 4);
        //QString data = QDate::currentDate().toString("yyyy-MM-dd");
        QString data = QDate::currentDate().toString("dd/MM/yyyy");
        QString hora = QTime::currentTime().toString("hh:mm:ss");
        int idColaborador = variaveis_globais::id_colab;

        QSqlQuery query;
        query.prepare("INSERT INTO tb_vendas"
                        "(data_venda, hora_venda, id_colaborador, valor_total, id_tipo_pagamento)"
                       "VALUES "
                        "('"+data+"', '"+hora+"', '"+QString::number(idColaborador)+"', '"+QString::number(total)+"', 1)"
                      );
        if (!query.exec()) {
            QMessageBox::warning(this, "Erro", "Erro ao finalizar a venda!");
        } else {
            query.prepare("SELECT id_venda FROM tb_vendas ORDER BY id_venda DESC LIMIT 1");
            query.exec();
            query.first();
            idVenda = query.value(0).toInt();

            int totalLinhas = ui->tw_listaProdutos->rowCount();
            int linha = 0;
            while (linha < totalLinhas) {
                QString produto       = ui->tw_listaProdutos->item(linha, 1)->text();
                QString quantidade    = ui->tw_listaProdutos->item(linha, 3)->text();
                QString valorUnitario = ui->tw_listaProdutos->item(linha, 2)->text();
                QString valorTotal    = ui->tw_listaProdutos->item(linha, 4)->text();

                query.prepare("INSERT INTO tb_produtosVendas "
                                "(id_venda, produto, quantidade, valor_unit, valor_total)"
                               "VALUES "
                                "('"+QString::number(idVenda)+"', '"+produto+"', '"+quantidade+"', '"+valorUnitario+"', '"+valorTotal+"')"
                              );
                query.exec();
                linha++;
            }

            msgFimVenda = "ID venda: "+QString::number(idVenda)+"\nValor total: R$ "+QString::number(total);
            QMessageBox::information(this, "Venda Concluída", msgFimVenda);
            limparCampos();
            funcoes_globais::removerLinha(ui->tw_listaProdutos);
            ui->lb_totalVenda->setText("Total: R$ 0,00");
        }

    } else {
        QMessageBox::warning(this, "Erro", "Adicione produtos para prosseguir com a venda!");
    }
}

