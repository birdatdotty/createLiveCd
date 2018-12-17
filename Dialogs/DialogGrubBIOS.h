#ifndef DIALOGGRUBBIOS_H
#define DIALOGGRUBBIOS_H

#include <QDialog>
#include <QJsonObject>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <QProcess>

class DialogGrubBIOS : public QDialog
{
public:
  DialogGrubBIOS(QJsonObject, QString*);

private:
  QGridLayout mainLayout;
  QLabel label;

  QStringList cmds;
  QPushButton start;

  QString strStep0, strStep1, strStep2, strStep3, sGrubgCfg, file;
  QLabel *lbStep0, *lbStep1, *lbStep2, *lbStep3;
  QString* ret;

  QProcess proc;

private slots:
    void run();
};

#endif // DIALOGGRUBBIOS_H
