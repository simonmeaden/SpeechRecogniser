/**
  Copyright 2020 Simon Meaden

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "mainwindow.h"

#define _WIDTH = 500
#define _HEIGHT = 400

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_x(0)
  , m_y(0)
  , m_w(500)
  , m_h(400)
  , m_sampleRate(44100)
  , m_displayTime(500)
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QSize size = screen->size();
  m_x = int((size.width() - m_w) / 2);
  m_y = int((size.height() - m_h) / 2);
  setGeometry(m_x, m_y, m_w, m_h);

  initGui();

  m_reader = new MicrophoneReader(this);
  connect(
    m_reader, &MicrophoneReader::finished, m_reader, &QObject::deleteLater);
  connect(
    m_reader, &MicrophoneReader::sendData, m_plot, &MicrophonePlot::addData);
  m_reader->start();
}

MainWindow::~MainWindow() {}

void
MainWindow::initGui()
{
  QFrame* frm = new QFrame(this);
  QGridLayout* main_layout = new QGridLayout();
  frm->setLayout(main_layout);

  m_plot = new MicrophonePlot(m_sampleRate, m_displayTime, this);
  main_layout->addWidget(m_plot, 0, 0);

  setCentralWidget(frm);
}

void
MainWindow::closeEvent(QCloseEvent* event)
{
  if (m_reader->isRunning()) {
    m_reader->stop();
    event->accept();
  } else {
    event->ignore();
  }
}
