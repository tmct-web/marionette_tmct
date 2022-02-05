# marionette_tmct


## What is "Marionette" ?
Marionettes are so-called puppets.We created this to remotely control multiple remote devices via USB from a single PC.

This system contains two Arduino Leonardo, the controlling side (master side) and the controlled side (slave side).

For example, the USB port connected to the controlling side (master side) is USB-COM, and the USB port connected to the controlled side (slave side) is an HID keyboard (= the slave side looks like a normal HID keyboard). Then you can control the slave keyboard with USB-COM on the master.

マリオネッテと書いてマリオネットと読みます。マリオネットはいわゆる操り人形のことですね。
もともと1台のパソコンからUSB経由で複数の相手機器を遠隔コントロールするために作ったものです。

このシステムは、制御する側（マスタ側）・制御される側（スレーブ側）の2つ分のArduino Leonardoが入っている、ちょっと変わったハードウェアになっています。

たとえば制御する側（マスタ側）に繋がるUSBポートはUSB-COM、制御される側（スレーブ側）につながるUSBポートはHIDキーボード（＝スレーブ側は普通のHIDキーボードに見える）、という構成にすると、マスタ側のUSB-COMでスレーブ側をキーボード制御する、なんてことができます。

![Example](https://ss1.xrea.com/tmct.s1009.xrea.com/img/work/td-xx-7e2g01/marionette_hw_exdiag.svg)


## Hardware specifications
Marionette has two circuits of Arduino Leonardo.

The two circuits are called a master (MASTER) and a slave (SLAVE), respectively, and can be operated completely differently by different programs. The hardware block diagram is shown below.

MarionetteはArduino Leonardoの2枚分の回路が実装されています。

2枚分の回路はそれぞれマスタ(MASTER)とスレーブ(SLAVE)と呼び、各々別のプログラムでまったく別の動作をさせることができます。ハードウェアのブロック図を以下に示します。

![Hardware block diagram](https://ss1.xrea.com/tmct.s1009.xrea.com/img/work/td-xx-7e2g01/marionette_hw_diag.svg)

See our [homepage](https://ss1.xrea.com/tmct.s1009.xrea.com/work/td-ja-7e2g01.html) for more information.(* Japanese only)

詳細は [ホームページ](https://ss1.xrea.com/tmct.s1009.xrea.com/work/td-ja-7e2g01.html) を参照してください。


## Recommended projects
We recommend this project. Compatibility with Marionette is outstanding.

Marionetteで使用すると、良い結果が得られるでしょう。

[NicoHood/HID](https://github.com/NicoHood/HID)
