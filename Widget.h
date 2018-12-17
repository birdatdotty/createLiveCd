#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>

#include <QSettings>
#include <QProcess>

class Widget : public QWidget
{
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private:
  QLabel label,
                lSquashfs,
                lInitrd,
                lVmlinuz,
                lGrub,
                lGrubUEFI,
                lGrubBIOS,
                lOutputDisk;

  QLineEdit eSquashfs,
                      eInitrd,
                      eVmlinuz,
                      eGrub,
                      eGrubUEFI,
                      eGrubBIOS,
                      eOutputDisk;
  QGridLayout mainLayout;

  QPushButton bSelectSquashfs,
                            bSelectInitrd,
                            bSelectVmlinuz,
                            bSelectGrub,
                            bSelectGrubUEFI,
                            bCreateGrubUEFI,
                            bSelectGrubBIOS,
                            bCreateGrubBIOS,
                            bSelectOutputDisk,
                            bConfig,
                            bOK;

  QSettings *settings;

  QString sSquashfs,
                  sInitrd,
                  sVmlinuz,
                  sGrub,
                  sGrubUEFI,
                  sGrubBIOS;

  // status filds:
  bool boolESquashfs,
           boolEInitrd,
           boolEInitd,
           boolEVmlinuz,
           boolEGrub,
           boolEGrubUEFI,
           boolEGrubBIOS,
           boolEOutputDisk;


private slots:
 void createGrubUEFI();
 void createGrubBIOS();
 void config();
 void saveAs();
 void selectSquashfs();
 void selectGrub();
 void selectGrubUEFI();
 void selectGrubBIOS();
 void selectInitrd();
 void selectVmlinuz();
 void Start();

 void testBOK(QString);
};

#endif // WIDGET_H
