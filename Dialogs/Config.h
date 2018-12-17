#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QSettings>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Config : public QDialog
{
public:
  Config(QSettings*);

private:
  QSettings* settings;
  QLabel label;
  QGridLayout mainLayout;
  QLabel *lGrubMkstandalone,
                *lCdboot,
                *lBuildDir;

  QLineEdit *eGrubMkstandalone,
                      *eCdboot,
                      *eBuildDir;

  QPushButton *bGrubMkstandalone,
                             *bCdboot,
                             *bBuildDir,
                             *bOK;

  QString sGrubMkstandalone,
                  sCdboot,
                  sBuildDir;



private slots:
  void selectMkstandalone();
  void selectCdboot();
  void selectBuildDir();
  void apply();

};

#endif // CONFIG_H
