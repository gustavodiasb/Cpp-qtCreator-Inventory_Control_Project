#include "fm_gestaoestoque.h"
#include "ui_fm_gestaoestoque.h"
#include <QMessageBox>
#include <QtSql>
#include "funcoes_globais.h"

fm_gestaoEstoque::fm_gestaoEstoque(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_gestaoEstoque)
{
    ui->setupUi(this);

    if (!con.abrir()) {
        QMessageBox::warning(this, "Erro", "Erro ao abrir banco de dados");
    } else {
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_produtos");
        if (query.exec()) {

        } else {
            QMessageBox::warning(this, "Erro", "Erro ao listar os produtos");
        }
    }

    ui->tw_ge_produtos->setColumnCount(2);
    ui->tabWidget->setCurrentIndex(0);

}

fm_gestaoEstoque::~fm_gestaoEstoque()
{
    delete ui;
    con.fechar();
}

void fm_gestaoEstoque::on_btn_novoProduto_clicked()
{
    limparCampos();
}

void fm_gestaoEstoque::limparCampos() {
    ui->txt_codigoProduto->clear();
    ui->txt_descricaoProduto->clear();
    ui->txt_qtdeEstoque->clear();
    ui->txt_valorCompra->clear();
    ui->txt_valorVenda->clear();
    ui->txt_fornecedor->clear();
    ui->tw_ge_produtos->clear();
}


void fm_gestaoEstoque::on_btn_gravarNovoProduto_clicked()
{
    QString aux;
    QString idProduto    = ui->txt_codigoProduto->text();
    QString produto      = ui->txt_descricaoProduto->text();
    QString idFornecedor = ui->txt_fornecedor->text();
    QString qtdeEstoque  = ui->txt_qtdeEstoque->text();

    aux = ui->txt_valorCompra->text();
    std::replace(aux.begin(), aux.end(), ',', '.');
    QString valorCompra = aux;
    aux = ui->txt_valorVenda->text();
    std::replace(aux.begin(), aux.end(), ',', '.');
    QString valorVenda = aux;

    QSqlQuery query;
    query.prepare("INSERT INTO tb_produtos"
                  "(id_produto, produto, id_fornecedor, qtde_estoque, valor_compra, valor_venda)"
                  "VALUES("
                      +QString::number(idProduto.toInt())+", '"+ produto +"', "
                      +QString::number(idFornecedor.toInt())+", "
                      +QString::number(qtdeEstoque.toInt())+", "
                      +QString::number(valorCompra.toFloat())+", "
                      +QString::number(valorVenda.toFloat())+
                  ")");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erro", "Erro ao gravar registro no banco!");
    } else {
        limparCampos();
    }

}


void fm_gestaoEstoque::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        funcoes_globais::removerLinha(ui->tw_ge_produtos);
        int numLinha = 0;
        // Remover os produtos do TW
        QSqlQuery query;
        query.prepare("SELECT id_produto, produto FROM tb_produtos ORDER BY produto");
        if (query.exec()) {

            while (query.next()) {
                ui->tw_ge_produtos->insertRow(numLinha);
                ui->tw_ge_produtos->setItem(numLinha, 0, new QTableWidgetItem(query.value(0).toString()));
                ui->tw_ge_produtos->setItem(numLinha, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->tw_ge_produtos->setRowHeight(numLinha, 20);
                numLinha++;
            }

           ui->tw_ge_produtos->setColumnWidth(0, 150);
           ui->tw_ge_produtos->setColumnWidth(1, 230);
           QStringList cabecalho = {"Código", "Produto"};
           ui->tw_ge_produtos->setHorizontalHeaderLabels(cabecalho);
           // cor da linha selecionada e personalizações
           ui->tw_ge_produtos->setStyleSheet("QTableView {selection-background-color:red}");
           ui->tw_ge_produtos->setEditTriggers(QAbstractItemView::NoEditTriggers);
           ui->tw_ge_produtos->setSelectionBehavior(QAbstractItemView::SelectRows);
           ui->tw_ge_produtos->verticalHeader()->setVisible(false);
        } else {
            QMessageBox::warning(this, "Erro", "Erro ao listar produtos");
        }
    }
}


void fm_gestaoEstoque::on_tw_ge_produtos_itemSelectionChanged()
{
    int id = ui->tw_ge_produtos->item(ui->tw_ge_produtos->currentRow(), 0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_produtos WHERE id_produto="+QString::number(id));
    if (query.exec()) {
        query.first();
        ui->txt_ge_codigoProduto->setText(query.value(0).toString());
        ui->txt_ge_produto->setText(query.value(1).toString());
        ui->txt_ge_fornecedor->setText(query.value(2).toString());
        ui->txt_ge_quantidade->setText(query.value(3).toString());
        ui->txt_ge_valorCompra->setText(query.value(4).toString());
        ui->txt_ge_valorVenda->setText(query.value(5).toString());
    }
}


void fm_gestaoEstoque::on_btn_ge_gravar_clicked()
{
    if (ui->txt_ge_codigoProduto->text() == "") {
        QMessageBox::warning(this, "Erro", "Selecione na lista o produto que deseja alterar!");
    } else {
        int linha = ui->tw_ge_produtos->currentRow();

        int id = ui->tw_ge_produtos->item(linha, 0)->text().toInt();
        QString produto    = ui->txt_ge_produto->text();
        QString fornecedor = ui->txt_ge_fornecedor->text();
        QString quantidade = ui->txt_ge_quantidade->text();

        QString aux;

        aux = ui->txt_ge_valorCompra->text();
        std::replace(aux.begin(), aux.end(), ',', '.');
        QString valCompra = aux;

        aux = ui->txt_ge_valorVenda->text();
        std::replace(aux.begin(), aux.end(), ',', '.');
        QString valVenda = aux;

        QSqlQuery query;
        query.prepare("UPDATE tb_produtos SET "
                          "produto          = '"+produto+
                          "', id_fornecedor = '"+QString::number(fornecedor.toInt())+
                          "', qtde_estoque  = '"+QString::number(quantidade.toInt())+
                          "', valor_compra  = '"+QString::number(valCompra.toDouble())+
                          "', valor_venda   = '"+QString::number(valVenda.toDouble())+
                      "' WHERE id_produto   =  "+QString::number(id)
                      );
        if (query.exec()) {            
            ui->tw_ge_produtos->item(linha, 0)->setText(ui->txt_ge_codigoProduto->text());
            ui->tw_ge_produtos->item(linha, 1)->setText(produto);
            QMessageBox::information(this, "Atualizado", "Produto atualizado com sucesso");
        } else {
            QMessageBox::warning(this, "Erro", "Erro ao atualizar os dados no banco");
        }
    }
}


void fm_gestaoEstoque::on_btn_ge_excluir_clicked()
{
    if (ui->txt_ge_codigoProduto->text() == "") {
        QMessageBox::warning(this, "Erro", "Selecione na lista o produto que deseja alterar!");
    } else {
        int linha = ui->tw_ge_produtos->currentRow();
        QString produto = ui->tw_ge_produtos->item(linha, 1)->text();
        QMessageBox::StandardButton opcao = QMessageBox::question(this, "Exclusão", "Deseja realmente excluir o produto "+produto+" ?", QMessageBox::Yes|QMessageBox::No);
        if (opcao == QMessageBox::Yes) {
            int id = ui->tw_ge_produtos->item(linha, 0)->text().toInt();

            QSqlQuery query;
            query.prepare("DELETE FROM tb_produtos WHERE id_produto = "+QString::number(id));
            if (query.exec()) {
                ui->tw_ge_produtos->removeRow(linha);
                QMessageBox::information(this, "Removido", "Produto removido.");
            } else {
                QMessageBox::warning(this, "Erro", "Falha ao remover o produto "+produto);
            }
        }
    }
}


void fm_gestaoEstoque::on_btn_ge_filtrar_clicked()
{
    QString busca;
    QString filtro = ui->txt_ge_filtrar->text();
    funcoes_globais::removerLinha(ui->tw_ge_produtos);

    if (filtro == "") {
        if (ui->rb_ge_codigo->isChecked()) {
            busca = "SELECT id_produto, produto FROM tb_produtos ORDER BY id_produto";
        } else {
            busca = "SELECT id_produto, produto FROM tb_produtos ORDER BY produto";
        }
    } else {
        if (ui->rb_ge_codigo->isChecked()) {
            busca = "SELECT id_produto, produto FROM tb_produtos WHERE id_produto = "+filtro+" ORDER BY id_produto";
        } else {
            busca = "SELECT id_produto, produto FROM tb_produtos WHERE produto LIKE '%"+filtro+"%' ORDER BY produto";

        }
    }

    int numLinha = 0;
    QSqlQuery query;
    query.prepare(busca);

    if (query.exec()) {
        while(query.next()) {
            ui->tw_ge_produtos->insertRow(numLinha);
            ui->tw_ge_produtos->setItem(numLinha, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tw_ge_produtos->setItem(numLinha, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tw_ge_produtos->setRowHeight(numLinha, 20);
            numLinha++;
        }
    } else {
        QMessageBox::warning(this, "Erro", "Erro ao listar produtos na tabela!");
    }

    ui->txt_ge_filtrar->clear();
    ui->txt_ge_filtrar->setFocus();

}

