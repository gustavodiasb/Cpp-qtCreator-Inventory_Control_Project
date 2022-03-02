#include "fm_editarprodutovenda.h"
#include "ui_fm_editarprodutovenda.h"
#include "fm_novavenda.h"

fm_editarProdutoVenda::fm_editarProdutoVenda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_editarProdutoVenda)
{
    ui->setupUi(this);

    ui->txt_editarProduto->setText(fm_novaVenda::g_produto);
    ui->txt_editarPrecoUnitario->setText(fm_novaVenda::g_valorUnitario);
    ui->txt_editarQuantidade->setText(fm_novaVenda::g_quantidade);
}

fm_editarProdutoVenda::~fm_editarProdutoVenda()
{
    delete ui;
}

void fm_editarProdutoVenda::on_btn_editarConfirmar_clicked()
{
    QString aux;
    fm_novaVenda::alteracao = true;

    fm_novaVenda::g_quantidade = ui->txt_editarQuantidade->text();

    aux = ui->txt_editarPrecoUnitario->text();
    std::replace(aux.begin(), aux.end(), ',', '.');
    fm_novaVenda::g_valorUnitario = aux;

    fm_novaVenda::g_valorTotal = QString::number(
                ui->txt_editarQuantidade->text().toDouble() * aux.toDouble());
    close();

}

void fm_editarProdutoVenda::on_btn_editarCancelar_clicked()
{
    fm_novaVenda::alteracao = false;
    close();
}

