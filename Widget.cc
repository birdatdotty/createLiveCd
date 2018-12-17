#include "Widget.h"

#include <QDebug>

#include "Dialogs/DialogGrubUEFI.h"
#include "Dialogs/DialogGrubBIOS.h"
#include "Dialogs/Config.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent)
  : QWidget(parent),
    label("Создание диска:"),
    lSquashfs("выбрать squashfs:"),
    lInitrd("initrd"),
    lVmlinuz("vmlinuz"),
    lGrub("конфиг grub:"),
    lGrubUEFI("образ grub UEFI:"),
    lGrubBIOS("образ grub BIOS:"),
    lOutputDisk("сохранять как:"),

    bSelectSquashfs("Выбрать"),
    bSelectInitrd("Выбрать"),
    bSelectVmlinuz("Выбрать"),
    bSelectGrub("Выбрать"),
    bSelectGrubUEFI("выбрать"),
    bCreateGrubUEFI("создать"),
    bSelectGrubBIOS("выбрать"),
    bCreateGrubBIOS("создать"),
    bSelectOutputDisk("выбрать"),
    bConfig("Настройки"),
    bOK("Подготовить диск")
{
  mainLayout.setAlignment(Qt::AlignTop);

  settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                                  "dotty.su", "createIso");

  int row(0);
  mainLayout.addWidget(&label,0,0,1,2);
  mainLayout.addWidget(&bConfig,0,3);

  mainLayout.addWidget(&lInitrd, ++row,0);
  mainLayout.addWidget(&eInitrd, row,1);
  mainLayout.addWidget(&bSelectInitrd, row,2, 1,2);

  mainLayout.addWidget(&lVmlinuz, ++row,0);
  mainLayout.addWidget(&eVmlinuz, row,1);
  mainLayout.addWidget(&bSelectVmlinuz, row,2, 1,2);

  mainLayout.addWidget(&lSquashfs, ++row,0);
  mainLayout.addWidget(&eSquashfs, row,1);
  mainLayout.addWidget(&bSelectSquashfs, row,2, 1,2);

  mainLayout.addWidget(&lGrub, ++row,0);
  eGrub.setText( settings->value("GrubCfg",  "").toString());
  mainLayout.addWidget(&eGrub, row,1);
  mainLayout.addWidget(&bSelectGrub, row,2, 1,2);

  mainLayout.addWidget(&lGrubUEFI,++row,0);
  eGrubUEFI.setText( settings->value("GrubUEFI", "").toString());
  mainLayout.addWidget(&eGrubUEFI,row,1);
  mainLayout.addWidget(&bSelectGrubUEFI,row,2);
  mainLayout.addWidget(&bCreateGrubUEFI,row,3);

  mainLayout.addWidget(&lGrubBIOS,++row,0);
  eGrubBIOS.setText( settings->value("GrubBIOS", "").toString());
  mainLayout.addWidget(&eGrubBIOS,row,1);
  mainLayout.addWidget(&bSelectGrubBIOS,row,2);
  mainLayout.addWidget(&bCreateGrubBIOS,row,3);

  mainLayout.addWidget(&lOutputDisk, ++row,0);
  mainLayout.addWidget(&eOutputDisk, row,1);
  mainLayout.addWidget(&bSelectOutputDisk, row,2, 1,2);

  mainLayout.addWidget(&bOK, ++row, 1);//, 1, 2);

  connect(&bSelectSquashfs, &QPushButton::released, this, &Widget::selectSquashfs);
  connect(&bSelectInitrd, &QPushButton::released, this, &Widget::selectInitrd);
  connect(&bSelectVmlinuz, &QPushButton::released, this, &Widget::selectVmlinuz);
  connect(&bConfig, &QPushButton::released, this, &Widget::config);
  connect(&bSelectGrub, &QPushButton::released, this, &Widget::selectGrub);
  connect(&bCreateGrubUEFI, &QPushButton::released, this, &Widget::createGrubUEFI);
  connect(&bSelectGrubUEFI, &QPushButton::released, this, &Widget::selectGrubUEFI);
  connect(&bCreateGrubBIOS, &QPushButton::released, this, &Widget::createGrubBIOS);
  connect(&bSelectGrubUEFI, &QPushButton::released, this, &Widget::selectGrubBIOS);
  connect(&bSelectOutputDisk, &QPushButton::released,
                  this, &Widget::saveAs);
  connect(&bOK, &QPushButton::released, this, &Widget::Start);

  // active bOk
  bOK.setEnabled(false);

  boolESquashfs = (eSquashfs.text().size()>0);
  boolEInitrd = (eInitrd.text().size()>0);
  boolEVmlinuz = (eInitrd.text().size()>0);
  boolEGrub = (eGrub.text().size()>0);
  boolEGrubUEFI = (eGrubUEFI.text().size()>0);
  boolEGrubBIOS = (eGrubBIOS.text().size()>0);
  boolEOutputDisk = (eOutputDisk.text().size()>0);

  connect (&eSquashfs, &QLineEdit::textChanged, [=](QString txt) {boolESquashfs = (txt.size()>0);});
  connect (&eInitrd, &QLineEdit::textChanged, [=](QString txt) {boolEInitrd = (txt.size()>0);});
  connect (&eVmlinuz, &QLineEdit::textChanged, [=](QString txt) {boolEVmlinuz = (txt.size()>0);});
  connect (&eGrub, &QLineEdit::textChanged, [=](QString txt) {boolEGrub = (txt.size()>0);});
  connect (&eGrubUEFI, &QLineEdit::textChanged, [=](QString txt) {boolEGrubUEFI = (txt.size()>0);});
  connect (&eGrubBIOS, &QLineEdit::textChanged, [=](QString txt) {boolEGrubBIOS = (txt.size()>0);});
  connect (&eOutputDisk, &QLineEdit::textChanged, [=](QString txt) {boolEOutputDisk = (txt.size()>0);});

  connect (&eSquashfs, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eInitrd, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eVmlinuz, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eGrub, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eGrubUEFI, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eGrubBIOS, &QLineEdit::textChanged, this, &Widget::testBOK);
  connect (&eOutputDisk, &QLineEdit::textChanged, this, &Widget::testBOK);



  setLayout(&mainLayout);
}

void Widget::testBOK(QString) {
  bOK.setEnabled( boolESquashfs &  boolEInitrd & boolEVmlinuz &
                                  boolEGrub & boolEGrubUEFI & boolEGrubBIOS & boolEOutputDisk);
}

Widget::~Widget()
{

}

void Widget::config() {
  Config* pDlg = new Config(settings);
  if (pDlg->exec() == QDialog::Accepted)
  {
    // Пользователь выбрал Accepted
    // Получить данные для дальнейшего анализа и обработки
//    Data data = pdlg->getData();
//    ...
      qInfo("QDialog::Accepted");
  }
//  delete pDlg;
}

void Widget::createGrubUEFI() {
  QString ret;
  QJsonObject obj;
  obj["grub-mkstandalone"] = settings->value("grub-mkstandalone","").toString();
  obj["grub.cfg"] = sGrub;
  DialogGrubUEFI* pDlg = new DialogGrubUEFI(obj, &ret);
  if (pDlg->exec() == QDialog::Accepted)
  {
    // Пользователь выбрал Accepted
    // Получить данные для дальнейшего анализа и обработки
//    Data data = pdlg->getData();
//    ...
      qInfo() << "ret:" << ret;
      sGrubUEFI = ret;
      eGrubUEFI.setText(ret);
      settings->setValue("GrubUEFI", ret);
      settings->sync();
  }
  delete pDlg;
}

void Widget::createGrubBIOS() {
  QString ret;
  QJsonObject obj;
  obj["grub-mkstandalone"] = settings->value("grub-mkstandalone","").toString();
  obj["grub.cfg"] = settings->value("GrubCfg","").toString();
  obj["cdboot.img"] = settings->value("cdboot-img","").toString();

//  obj["grub.cfg"] = sGrub;
  DialogGrubBIOS* pDlg = new DialogGrubBIOS(obj, &ret);
  if (pDlg->exec() == QDialog::Accepted)
  {
    // Пользователь выбрал Accepted
    // Получить данные для дальнейшего анализа и обработки
//    Data data = pdlg->getData();
//    ...
      qInfo() << "ret:" << ret;
      sGrubBIOS = ret;
      eGrubBIOS.setText(ret);
      settings->setValue("GrubBIOS", ret);
      settings->sync();
  }
  delete pDlg;
}

void Widget::saveAs() {
//  QString fileName=QFileDialog::getOpenFileName(this,tr("OpenFile"),sCdboot,tr("cdboot.img"));
//  if (fileName !=""){
////      settings->setValue("cdboot-img", fileName);
//      sCdboot = fileName;
//      eCdboot->setText(fileName);
//  }
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                              "disk.iso",
                              tr("Images (*.iso)"));
  qInfo() << "QFileDialog::getSaveFileName:" << fileName;
  if (fileName != "")
    eOutputDisk.setText(fileName);
}

void Widget::selectSquashfs() {
    sSquashfs = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("*"));
    if (sSquashfs !=""){
  //      settings->setValue("cdboot-img", fileName);
//        sCdboot = fileName;
//        eCdboot->setText(fileName);
    }

//    qInfo() << "fileName:" << fileName;
    eSquashfs.setText(sSquashfs);

}

void Widget::selectGrubUEFI() {
    sGrubUEFI = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("efiboot.img"));
    if (sGrubUEFI !=""){
  //      settings->setValue("cdboot-img", fileName);
//        sCdboot = fileName;
//        eCdboot->setText(fileName);
    }

//    qInfo() << "fileName:" << fileName;
   eGrubUEFI.setText(sGrubUEFI);
}

void Widget::selectGrubBIOS() {
  sGrubBIOS = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("efiboot.img"));
  if (sGrubBIOS !=""){
//      settings->setValue("cdboot-img", fileName);
//        sCdboot = fileName;
//        eCdboot->setText(fileName);
  }

//    qInfo() << "fileName:" << fileName;
 eGrubBIOS.setText(sGrubBIOS);
}

void Widget::selectGrub() {
  sGrub = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("grub.cfg"));
  if (sGrub != "") {
    settings->setValue("GrubCfg", sGrub);
    settings->sync();
  }
  eGrub.setText(sGrub);
}

void Widget::Start() {
  QProcess *proc = new QProcess();
  QString buildDir = settings->value("BuildDir").toString();
  qInfo("void Widget::Start()");
//  proc->start("mkdir", QStringList() << "-p" << buildDir + "/image/");
  proc->execute("mkdir", QStringList() << "-p" << buildDir + "/image/live");
  proc->execute("touch", QStringList() << buildDir + "/image/DEBIAN_CUSTOM");
  proc->execute("touch", QStringList() << buildDir + "/image/DEBIAN_CUSTOM");

  proc->execute("mkdir", QStringList() << "-p" << buildDir + "/scratch/");
  proc->execute("cp", QStringList() << eGrub.text() << buildDir + "/scratch/");

  proc->execute("cp", QStringList() << sInitrd << buildDir + "/image/initrd");
  proc->execute("cp", QStringList() << sVmlinuz << buildDir + "/image/vmlinuz");
  proc->execute("cp", QStringList() << sSquashfs << buildDir + "/image/live/filesystem.squashfs");
  qInfo() << eSquashfs.text() << eGrub.text() << eGrubUEFI.text() << eGrubBIOS.text() << eOutputDisk.text();


//  "xorriso"
  QStringList args;
  args << "-as" << "mkisofs" << "-iso-level" << "3" << "-full-iso9660-filenames" << "-volid" << "DEBIAN_CUSTOM" << "-eltorito-boot" << "boot/grub/bios.img" << "-no-emul-boot" << "-boot-load-size" << "4" << "-boot-info-table" << "--eltorito-catalog" << "boot/grub/boot.cat"
                    << "--grub2-boot-info" << "--grub2-mbr" << "/usr/lib/grub/i386-pc/boot_hybrid.img" << "-eltorito-alt-boot" << "-e" << "EFI/efiboot.img" << "-no-emul-boot" << "-append_partition" << "2" << "0xef"
                    << eGrubUEFI.text() << "-output"  << eOutputDisk.text() << "-graft-points" << buildDir+"/image" << "/boot/grub/bios.img=" + eGrubBIOS.text() << "/EFI/efiboot.img=" + eGrubUEFI.text();

qInfo() << "xorriso" <<  args;
  proc->start("xorriso", args);
  bOK.setEnabled(false);
  connect(proc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=]() {
      bOK.setEnabled(true);
  });
}

void Widget::selectInitrd() {
  sInitrd = QFileDialog::getOpenFileName(this,tr("initrd"),"",tr("*"));
  eInitrd.setText(sInitrd);
}
void Widget::selectVmlinuz() {
  sVmlinuz = QFileDialog::getOpenFileName(this,tr("vmlinuz"),"",tr("*"));
  eVmlinuz.setText(sVmlinuz);
}
