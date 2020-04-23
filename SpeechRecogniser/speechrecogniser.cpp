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
#include <QThread>

#include "speechrecogniser.h"

namespace SpeechRecognition {

SpeechRecogniser::SpeechRecogniser(QObject* parent)
  : QObject(parent)
  , m_running(true)
{
  QThread* reader_thread = new QThread;
  m_reader = new MicrophoneReader();
  connect(
    reader_thread, &QThread::started, m_reader, &MicrophoneReader::record);
  connect(m_reader, &MicrophoneReader::finished, reader_thread, &QThread::quit);
  connect(
    m_reader, &MicrophoneReader::finished, m_reader, &QObject::deleteLater);
  connect(m_reader,
          &MicrophoneReader::finished,
          reader_thread,
          &QObject::deleteLater);

  /* This sends the recorded data on to the application in case it wants it for
   * something else. A plot maybe?*/
  connect(
    m_reader, &MicrophoneReader::sendData, this, &SpeechRecogniser::sendData);
  connect(m_reader,
          &MicrophoneReader::sendData,
          this,
          &SpeechRecogniser::receiveData);

  m_reader->moveToThread(reader_thread);
  reader_thread->start();
}

void
SpeechRecogniser::receiveData(QVector<float> data)
{
  qWarning() << tr("received %1 values.").arg(data.size());
}

bool
SpeechRecogniser::isRunning()
{
  return m_reader->isRunning();
}

// void
// SpeechRecogniser::operate()
//{
//  while (m_running) {
//    // TODO something ??
//  }
//}

void
SpeechRecogniser::stop()
{
  if (m_reader->isRunning()) {
    m_reader->stop();
  }
  m_running = false;
}

} // end of namespace SpeechRecognition
