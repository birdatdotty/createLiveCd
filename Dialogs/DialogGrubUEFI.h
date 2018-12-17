#ifndef DIALOGGRUBUEFI_H
#define DIALOGGRUBUEFI_H

#include <QDialog>
#include <QJsonObject>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <QProcess>

class DialogGrubUEFI : public QDialog
{
public:
  DialogGrubUEFI(QJsonObject, QString*);

signals:
  void runCmds(QStringList);
  void returnStr(QString);


private:
  QGridLayout mainLayout;
  QLabel label;

  QStringList cmds;
  QPushButton start;

  // cmds:
  QString efiboot, strStep0, strStep1, strStep2, strStep3, strStep4, strStep5;
  QString* ret;

  // qlabel of cmds:
  QLabel *lbStep0, *lbStep1, *lbStep2, *lbStep3, *lbStep4, *lbStep5;

  QProcess proc;

private slots:
  void run();
};

#endif // DIALOGGRUBUEFI_H
