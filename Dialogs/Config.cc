#include "Config.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

Config::Config(QSettings* settings)
  : settings(settings),
    label("Настройки:"),
    lGrubMkstandalone(new QLabel("grub-mkstandalone:")),
    lCdboot(new QLabel("cdboot.img:")),
    lBuildDir(new QLabel("Директория для сборки:"))
{
  mainLayout.setAlignment(Qt::AlignTop);
  mainLayout.addWidget(&label, 0, 0);

  // grub-mkstandalone
  mainLayout.addWidget(lGrubMkstandalone, 1, 0);

  sGrubMkstandalone = settings->value("grub-mkstandalone","").toString();
  eGrubMkstandalone = new QLineEdit(sGrubMkstandalone);
  mainLayout.addWidget(eGrubMkstandalone, 1, 1);

  bGrubMkstandalone = new QPushButton("...");
  mainLayout.addWidget(bGrubMkstandalone, 1, 2);
  connect(bGrubMkstandalone, &QPushButton::released, this, &Config::selectMkstandalone);

  mainLayout.addWidget(lGrubMkstandalone, 1, 0);
  // end grub-mkstandalone

  // cdboot-img
  mainLayout.addWidget(lCdboot,2,0);

  sCdboot = settings->value("cdboot-img","").toString();
  eCdboot = new QLineEdit(sCdboot);
  mainLayout.addWidget(eCdboot, 2, 1);

  bCdboot = new QPushButton("...");
  mainLayout.addWidget(bCdboot, 2, 2);
  connect(bCdboot, &QPushButton::released, this, &Config::selectCdboot);
  // end cdboot-img

  // buildDir
  mainLayout.addWidget(lBuildDir,3,0);

  sBuildDir = settings->value("BuildDir","").toString();
  eBuildDir = new QLineEdit(sBuildDir);
  mainLayout.addWidget(eBuildDir, 3, 1);

  bBuildDir = new QPushButton("...");
  mainLayout.addWidget(bBuildDir, 3, 2);
  connect(bBuildDir, &QPushButton::released, this, &Config::selectBuildDir);
  //end buildDir

  bOK = new QPushButton("Применить!");
  mainLayout.addWidget(bOK, 4, 1);
  connect(bOK, &QPushButton::released,
                  this, &Config::apply);


  setLayout(&mainLayout);
}

void Config::selectMkstandalone() {
  QString fileName=QFileDialog::getOpenFileName(this,tr("OpenFile"),sGrubMkstandalone,tr("*"));
  if (fileName !=""){
      sGrubMkstandalone = fileName;
      eGrubMkstandalone->setText(fileName);
//      settings->setValue("grub-mkstandalone", fileName);
  }
}

void Config::selectCdboot() {
  QString fileName=QFileDialog::getOpenFileName(this,tr("OpenFile"),sCdboot,tr("cdboot.img"));
  if (fileName !=""){
//      settings->setValue("cdboot-img", fileName);
      sCdboot = fileName;
      eCdboot->setText(fileName);
  }
}

void Config::selectBuildDir() {
  QString dirName=QFileDialog::getExistingDirectory(this,tr("OpenFile"),sBuildDir,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
  if (dirName !=""){
//      settings->setValue("cdboot-img", fileName);
      sBuildDir = dirName;
      eBuildDir->setText(dirName);
  }
}

void Config::apply() {
  settings->setValue("grub-mkstandalone", sGrubMkstandalone);
  settings->setValue("cdboot-img", sCdboot);
  settings->setValue("BuildDir", sBuildDir);

  settings->sync();

  accept();
}
