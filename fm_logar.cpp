#include "fm_logar.h"
#include "ui_fm_logar.h"
#include <QMessageBox>
#include "fm_principal.h"
#include "variaveis_globais.h"

Fm_logar::Fm_logar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fm_logar)
{
    ui->setupUi(this);
    logado = false;
    ui->btn_login->setFocus();
}

Fm_logar::~Fm_logar()
{
    delete ui;
}

void Fm_logar::on_btn_login_clicked()
{
    if (!con.abrir()) {
        QMessageBox::warning(this, "Erro", "Erro ao abrir banco de dados");
    } else {
        QString username, password;
        username = ui->txt_user->text();
        password = ui->txt_pass->text();

        QSqlQuery query;
        query.prepare("SELECT * FROM tb_colaboradores WHERE "
                    "username_colab = '"+username+"' AND password_colab = '"+password+"'"
        );
        if (query.exec()) {
            query.first();
            if (query.value(1).toString() != "") {
                variaveis_globais::logado = true;
                variaveis_globais::nome_colab = query.value(1).toString();
                variaveis_globais::id_colab = query.value(0).toInt();
                variaveis_globais::acesso_colab = query.value(5).toString();

                con.fechar();
                close();
            } else {
                QMessageBox::warning(this, "Erro", "Colaborador não encontrado");
            }
        } else {
            QMessageBox::warning(this, "Erro", "Falha no login");
        }
    }
    // Login
    con.fechar();
}


void Fm_logar::on_btn_cancel_clicked()
{
    logado = false;
    close();
}

