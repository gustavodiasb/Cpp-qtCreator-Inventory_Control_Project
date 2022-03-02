#ifndef FM_EDITARPRODUTOVENDA_H
#define FM_EDITARPRODUTOVENDA_H

#include <QDialog>

namespace Ui {
class fm_editarProdutoVenda;
}

class fm_editarProdutoVenda : public QDialog
{
    Q_OBJECT

public:
    explicit fm_editarProdutoVenda(QWidget *parent = nullptr);
    ~fm_editarProdutoVenda();

private slots:
    void on_btn_editarConfirmar_clicked();

    void on_btn_editarCancelar_clicked();

private:
    Ui::fm_editarProdutoVenda *ui;
};

#endif // FM_EDITARPRODUTOVENDA_H
