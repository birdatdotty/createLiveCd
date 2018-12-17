#include "DialogGrubBIOS.h"

#include <QDebug>

DialogGrubBIOS::DialogGrubBIOS(QJsonObject obj, QString* ret)
  : label("Настройка Grub BIOS"),
    start("выполнить"),
    ret(ret)
{
  sGrubgCfg = obj["grub.cfg"].toString("");
  qInfo() << obj;
  mainLayout.addWidget(&label,0,0,1,4);

  QString mkstandalone = obj["grub-mkstandalone"].toString("/usr/bin/grub-mkstandalone");
  QString buildDir = obj["buildDir"].toString("/tmp");
  QString cdboot = obj["cdboot.img"].toString("/tmp");

  strStep0 = "mkdir -p " + buildDir + "/scratch/";
  strStep1 = mkstandalone + " --format=i386-pc "
                     "--output=" + buildDir+"/scratch/core.img "
                     "--install-modules=\"linux normal iso9660 biosdisk memdisk search tar ls\" "
                     "--modules=\"linux normal iso9660 biosdisk search\" "
                     "--locales=\"\" --fonts=\"\" boot/grub/grub.cfg=" + sGrubgCfg;
/*
  grub-mkstandalone \
      --format=i386-pc \
      --output=$HOME/LIVE_BOOT/scratch/core.img \
      --install-modules="linux normal iso9660 biosdisk memdisk search tar ls" \
      --modules="linux normal iso9660 biosdisk search" \
      --locales="" \
      --fonts="" \
      "boot/grub/grub.cfg=$HOME/LIVE_BOOT/scratch/grub.cfg"
  */
  lbStep0 = new QLabel(strStep0);
  lbStep1 = new QLabel(strStep1);


  /*
   * cat \
    /usr/lib/grub/i386-pc/cdboot.img \
    $HOME/LIVE_BOOT/scratch/core.img > $HOME/LIVE_BOOT/scratch/bios.img
   */

  file = buildDir + "/scratch/bios.img";
  strStep2 = "dd if=" + cdboot + " of=" + file;
  strStep3 = "dd if=" + buildDir + "/scratch/core.img of=" + file + " oflag=append conv=notrunc";
  lbStep2 = new QLabel(strStep2);
  lbStep3 = new QLabel(strStep3);


  mainLayout.addWidget(lbStep0, 1, 0, 1, 4);
  mainLayout.addWidget(lbStep1, 2, 0, 1, 4);
  mainLayout.addWidget(lbStep2, 3, 0, 1, 4);
  mainLayout.addWidget(lbStep3, 4, 0, 1, 4);

  mainLayout.addWidget(&start, 5, 1, 1, 2);

  setLayout(&mainLayout);
  connect(&start, &QPushButton::released, this, &DialogGrubBIOS::run);
}

void DialogGrubBIOS::run () {
  proc.execute(strStep0);
  proc.waitForFinished(-1);

  proc.execute(strStep1);
  proc.waitForFinished(-1);

  proc.execute(strStep2);
  proc.waitForFinished(-1);

  proc.execute(strStep3);
  proc.waitForFinished(-1);

  *ret = file;
  accept();
}
