#include "fm_gestaocolaboradores.h"
#include "ui_fm_gestaocolaboradores.h"
#include <QMessageBox>
#include <QtSql>
#include "funcoes_globais.h"

// debug console #include <QDebug>

fm_gestaoColaboradores::fm_gestaoColaboradores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_gestaoColaboradores)
{
    ui->setupUi(this);

    if (!con.abrir()) {
        QMessageBox::warning(this, "Erro", "Erro ao abrir banco de dados");
    } else {
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_colaboradores");
        if (query.exec()) {

        } else {
            QMessageBox::warning(this, "Erro", "Erro ao listar os colaboradores");
        }
    }

    ui->cb_ge_acessoColab->addItem("Escolha: ");
    ui->cb_ge_acessoColab->addItem("A");
    ui->cb_ge_acessoColab->addItem("B");
    ui->txt_ge_nomeColab->setFocus();

    ui->tw_ge_listarColab->setColumnCount(2);
    ui->tabWidget->setCurrentIndex(1);

    ui->tw_ge_listarColab->setColumnWidth(0, 50);
    ui->tw_ge_listarColab->setColumnWidth(1, 200);
    QStringList cabecalho = {"ID", "Nome"};
    ui->tw_ge_listarColab->setHorizontalHeaderLabels(cabecalho);
    // cor da linha selecionada e personalizações
    ui->tw_ge_listarColab->setStyleSheet("QTableView {selection-background-color:red}");
    ui->tw_ge_listarColab->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_ge_listarColab->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_ge_listarColab->verticalHeader()->setVisible(false);

    on_tabWidget_currentChanged(1);

}

fm_gestaoColaboradores::~fm_gestaoColaboradores()
{
    delete ui;
    con.fechar();
}

void fm_gestaoColaboradores::limparCampos() {
    ui->txt_ge_idColab->clear();
    ui->txt_ge_nomeColab->clear();
    ui->txt_ge_userColab->clear();
    ui->txt_ge_passColab->clear();
    ui->txt_ge_telefoneColab->clear();
    ui->cb_ge_acessoColab->setCurrentIndex(0);

    ui->txt_ge_nomeColab->setFocus();

}


void fm_gestaoColaboradores::on_btn_cancelar_novoColab_clicked()
{

}


void fm_gestaoColaboradores::on_btn_novo_novoColab_clicked()
{
    // informar o index a ter os campos limpos
    limparCampos();
}


void fm_gestaoColaboradores::on_btn_gravar_novoColab_clicked()
{

}


void fm_gestaoColaboradores::on_btn_ge_cancelar_clicked()
{

}


void fm_gestaoColaboradores::on_btn_ge_novo_clicked()
{
    limparCampos();
}

void fm_gestaoColaboradores::on_btn_ge_gravar_clicked()
{
    QSqlQuery query;
    QString nome   = ui->txt_ge_nomeColab->text();
    QString user   = ui->txt_ge_userColab->text();
    QString pass   = ui->txt_ge_passColab->text();
    QString fone   = ui->txt_ge_telefoneColab->text();
    QString acesso = ui->cb_ge_acessoColab->currentText();

    QString sql = "";

    // UPDATE
    if (ui->txt_ge_idColab->text() != "") {
        QString id = ui->tw_ge_listarColab->item(ui->tw_ge_listarColab->currentRow(), 0)->text();

        sql = "UPDATE tb_colaboradores SET "
              "nome_colab = '"+nome+
              "', username_colab = '"+user+
              "', password_colab = '"+pass+
              "', fone_colab = '"+fone+
              "', acesso_colab = '"+acesso+
          "' WHERE id_colab = "+id;
        // debug console qInfo() << sql;
        query.prepare(sql);
    // CREATE
    } else {
        query.prepare("INSERT INTO tb_colaboradores"
                        "(nome_colab, username_colab, password_colab, fone_colab, acesso_colab)"
                      "VALUES("
                        "'"+nome+"', '"+user+"', '"+pass+"', '"+fone+"', '"+acesso+"'"
                      ")");
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Erro", "Erro ao salvar os dados do(a) colaborador(a) "+nome+"!");
    } else {
        limparCampos();
        on_tabWidget_currentChanged(1);
        QMessageBox::information(this, "Sucesso", "Os dados do(a) colaborador(a) "+nome+" foram salvos com sucesso.");
    }

}

void fm_gestaoColaboradores::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        funcoes_globais::removerLinha(ui->tw_ge_listarColab);
        int numLinha = 0;
        // Remover os produtos do TW
        QSqlQuery query;
        query.prepare("SELECT id_colab, nome_colab FROM tb_colaboradores ORDER BY nome_colab ASC");
        if (query.exec()) {

            while (query.next()) {
                ui->tw_ge_listarColab->insertRow(numLinha);
                ui->tw_ge_listarColab->setItem(numLinha, 0, new QTableWidgetItem(query.value(0).toString()));
                ui->tw_ge_listarColab->setItem(numLinha, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->tw_ge_listarColab->setRowHeight(numLinha, 20);
                numLinha++;
            }

        } else {
            QMessageBox::warning(this, "Erro", "Erro ao listar os colaboradores");
        }
    }
}


void fm_gestaoColaboradores::on_tw_ge_listarColab_itemSelectionChanged()
{
    int id = ui->tw_ge_listarColab->item(ui->tw_ge_listarColab->currentRow(), 0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_colaboradores WHERE id_colab = "+QString::number(id));
    if (query.exec()) {
        query.first();
        ui->txt_ge_idColab->setText(query.value(0).toString());
        ui->txt_ge_nomeColab->setText(query.value(1).toString());
        ui->txt_ge_userColab->setText(query.value(2).toString());
        ui->txt_ge_passColab->setText(query.value(3).toString());
        ui->txt_ge_telefoneColab->setText(query.value(4).toString());
        ui->cb_ge_acessoColab->setCurrentText(query.value(5).toString());
    }
}


void fm_gestaoColaboradores::on_btn_ge_filtrar_clicked()
{
    QString busca;
    QString filtro = ui->txt_ge_filtro->text();
    funcoes_globais::removerLinha(ui->tw_ge_listarColab);

    if (filtro == "") {
        if (ui->rb_ge_idColab->isChecked()) {
            busca = "SELECT id_colab, nome_colab FROM tb_colaboradores ORDER BY id_colab";
        } else {
            busca = "SELECT id_colab, nome_colab FROM tb_colaboradores ORDER BY nome_colab";
        }
    } else {
        if (ui->rb_ge_idColab->isChecked()) {
            busca = "SELECT id_colab, nome_colab FROM tb_colaboradores WHERE id_colab = "+filtro+" ORDER BY id_colab";
        } else {
            busca = "SELECT id_colab, nome_colab FROM tb_colaboradores WHERE nome_colab LIKE '%"+filtro+"%' ORDER BY nome_colab";

        }
    }

    int numLinha = 0;
    QSqlQuery query;
    query.prepare(busca);

    if (query.exec()) {
        while(query.next()) {
            ui->tw_ge_listarColab->insertRow(numLinha);
            ui->tw_ge_listarColab->setItem(numLinha, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tw_ge_listarColab->setItem(numLinha, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tw_ge_listarColab->setRowHeight(numLinha, 20);
            numLinha++;
        }
    } else {
        QMessageBox::warning(this, "Erro", "Erro ao listar produtos na tabela!");
    }

    ui->txt_ge_filtro->clear();
    ui->txt_ge_filtro->setFocus();
}

void fm_gestaoColaboradores::on_pushButton_clicked()
{
    if (ui->tw_ge_listarColab->currentRow() == -1) {
        QMessageBox::information(this, "Informação", "Escolha na tabela, o usuário que deseja excluir!");
        return;
    } else {
        QString nome = ui->txt_ge_nomeColab->text();

        QMessageBox::StandardButton opc = QMessageBox::question(this, "Exclusão", "Confirma exclusão do colaborador "+nome+"?", QMessageBox::Yes|QMessageBox::No);
        if (opc == QMessageBox::Yes) {
            int linha = ui->tw_ge_listarColab->currentRow();
            QString id = ui->tw_ge_listarColab->item(linha, 0)->text();
            QSqlQuery query;
            query.prepare("DELETE FROM tb_colaboradores WHERE id_colab = "+id);

            if(query.exec()) {
                ui->tw_ge_listarColab->removeRow(linha);
                QMessageBox::information(this, "Sucesso", "O(a) colaborador(a) foi excluído com sucesso.");
            } else {
                QMessageBox::information(this, "Sucesso", "O(a) colaborador(a) foi excluído com sucesso.");
            }
        }
    }
}

