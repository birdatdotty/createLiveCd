#include "DialogGrubUEFI.h"

#include <QDebug>
#include <iostream>

DialogGrubUEFI::DialogGrubUEFI(QJsonObject obj, QString* ret)
  : label("Настройка Grub UEFI"),
    start("выполнить"),
    ret(ret)
{
  mainLayout.addWidget(&label,0,0,1,4);

  QString mkstandalone = obj["grub-mkstandalone"].toString("/usr/bin/grub-mkstandalone");
  QString buildDir = obj["buildDir"].toString("/tmp");
//  grub-mkstandalone
//      output
//      grub.cfg
//      ............................
  strStep0 = "mkdir -p " + buildDir + "/scratch/";
  strStep1 = mkstandalone +
                        " --format=x86_64-efi "
                        "--output=" + buildDir+"/scratch/bootx64.efi "
                        " --locales=\"\" --fonts=\"\" "
                        "boot/grub/grub.cfg=" + buildDir + "/scratch/grub.cfg";

mainLayout.addWidget(new QLabel(strStep0),1,1);
mainLayout.addWidget(new QLabel(strStep1),2,1);
//////////////////////////////////////////////
  efiboot = buildDir + "/scratch/efiboot.img";
  strStep2 = "dd if=/dev/zero of=" +  efiboot + " bs=1M count=10";
  strStep3 = "mkfs.vfat " + buildDir + "/scratch/efiboot.img";
  strStep4 = "mmd -i " + buildDir + "/scratch/efiboot.img efi efi/boot";
  strStep5 = "mcopy -i " + buildDir + "/scratch/efiboot.img " + buildDir + "/scratch/bootx64.efi ::efi/boot/";

  lbStep0 = new QLabel(strStep0);
  lbStep1 = new QLabel(strStep1);
  lbStep2 = new QLabel(strStep2);
  lbStep3 = new QLabel(strStep3);
  lbStep4 = new QLabel(strStep4);
  lbStep5 = new QLabel(strStep5);


  mainLayout.addWidget(lbStep0, 1,1);
  mainLayout.addWidget(lbStep1, 2,1);
  mainLayout.addWidget(lbStep2, 3,1);
  mainLayout.addWidget(lbStep3, 4,1);
  mainLayout.addWidget(lbStep4, 5,1);
  mainLayout.addWidget(lbStep5, 6,1);
  mainLayout.addWidget(&start,7,1);
  cmds  << strStep0 << strStep1 << strStep2
              << strStep3 << strStep4 << strStep5;



//  runCmds(cmds);

//  std::cout << strStep0.toStdString() << std::endl;
//  std::cout  << strStep1.toStdString() << std::endl;
//  std::cout  << strStep2.toStdString() << std::endl;
//  std::cout  << strStep3.toStdString() << std::endl;
//  std::cout  << strStep4.toStdString() << std::endl;
//  std::cout  << strStep5.toStdString() << std::endl;

  setLayout(&mainLayout);
  connect(&start, &QPushButton::released, this, &DialogGrubUEFI::run);
}

void DialogGrubUEFI::run() {
  QPalette sample_palette;
//  sample_palette.setColor(QPalette::Window, Qt::white);
  sample_palette.setColor(QPalette::WindowText, Qt::blue);

  auto p = lbStep1->palette();
  p.setColor(QPalette::Text, QColor(255,255,100));
  qInfo("void DialogGrubUEFI::run()");
  proc.execute(strStep0);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep0->setPalette(sample_palette);
//    lbStep0->setText("<s>"+strStep0+"</s>");

  proc.execute(strStep1);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep1->setPalette(sample_palette);

  proc.execute(strStep2);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep2->setPalette(sample_palette);
//    lbStep2->setText("<s>"+strStep2+"</s>");

  proc.execute(strStep3);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep3->setPalette(sample_palette);

  proc.execute(strStep4);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep4->setPalette(sample_palette);

  proc.execute(strStep5);
  proc.waitForFinished(-1);
  if (proc.exitStatus() == QProcess::NormalExit)
    lbStep5->setPalette(sample_palette);
  *ret = efiboot;
  accept();
}
