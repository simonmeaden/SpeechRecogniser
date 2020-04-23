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
#ifndef SPEECHRECOGNISER_H
#define SPEECHRECOGNISER_H

#include <QObject>
#include <QtDebug>

#include "SpeechRecogniser_global.h"
#include "microphonereader.h"
#include "portaudio.h"
#include "snowboy-detect.h"

namespace SpeechRecognition {

class SpeechRecogniser : public QObject
{
  Q_OBJECT

public:
  explicit SpeechRecogniser(QObject* parent = nullptr);

  void stop();
  bool isRunning();
  //  void operate();

  void receiveData(QVector<float> data);

signals:
  void sendData(QVector<float>);
  void finished();

private:
  MicrophoneReader* m_reader;
  bool m_running;
};

} // end of namespace SpeechRecognition

#endif // SPEECHRECOGNISER_H
