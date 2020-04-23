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
#ifndef MICROPHONEREADER_H
#define MICROPHONEREADER_H

#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QThread>
#include <QtDebug>

#include "circularbuffer.h"
#include "portaudio.h"

typedef float SAMPLE;
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define NUM_SECONDS 5
#define NUM_CHANNELS 1
#define DITHER_FLAG 0
#define PA_SAMPLE_TYPE paFloat32
#define SAMPLE_SILENCE 0.0f
#define PRINTF_S_FORMAT "%.8f"

namespace SpeechRecognition {

class MicrophoneReader : public QThread
{
  Q_OBJECT

public:
  explicit MicrophoneReader(QObject* parent = nullptr);
  ~MicrophoneReader();

  bool startRecording();
  bool stopRecording();
  void stop();
  void quit();
  void emitData(QVector<float> data);

  bool closedDown() const;

signals:
  void sendData(QVector<float>);

protected:
  bool m_running;
  QMutex m_mutex;

  PaStream* m_stream;
  void initialise();
  void run() override;
};

} // end of namespace SpeechRecognition

#endif // MICROPHONEREADER_H
