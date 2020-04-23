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
#include "microphonereader.h"

namespace SpeechRecognition {

static int
recordCallback(const void* inputBuffer,
               void* /*outputBuffer*/,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo* /*timeInfo*/,
               PaStreamCallbackFlags /*statusFlags*/,
               void* sender)
{
  MicrophoneReader* reader = static_cast<MicrophoneReader*>(sender);

  if (reader->closedDown()) {
    return paComplete;
  }

  const SAMPLE* rptr = (const SAMPLE*)inputBuffer;
  long framesToCalc = framesPerBuffer;
  long i;
  int finished;
  QVector<float> out_data;

  finished = paContinue;

  if (inputBuffer == nullptr) {
    out_data.clear();

  } else {
    for (i = 0; i < framesToCalc; i++) {
      out_data.append(*rptr++);
    }
  }

  reader->emitData(out_data);
  return finished;
}

MicrophoneReader::MicrophoneReader(QObject* parent)
  : QThread(parent)
  , m_running(true)
  , m_stream(nullptr)
{
  initialise();
}

MicrophoneReader::~MicrophoneReader()
{
  stop();
  PaError err = Pa_CloseStream(m_stream);

  if (err != paNoError) {
    qWarning() << tr("Error closing stream");
  }

  Pa_Terminate();
}

/* Initialise the microphone reader and returns true if successful.

   Sets up the recorder to record data. Use startRecording() to actually start
   the recorder running.
*/
void
MicrophoneReader::initialise()
{
  QMutexLocker locker(&m_mutex);
  PaError err = paNoError;
  PaStreamParameters inputParameters /*, outputParameters*/;

  err = Pa_Initialize();

  if (err != paNoError) {
    qWarning() << tr("unable to intialise PortAudio.");
    return;
  }

  inputParameters.device =
    Pa_GetDefaultInputDevice(); /* default input device */

  if (inputParameters.device == paNoDevice) {
    qWarning() << tr("Error: No default input device.");
    return;
  }

  inputParameters.channelCount = 1; /* mono input */
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency =
    Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = nullptr;

  /*
    Passing a pointer to MicrophoneReader as the final parameter instead of a
    custom data object allows us to access the Qt signals via a custom method.
    Also nullptr is passed as an output parameter as we are not outputting
    anything directly instead we are using the Qt signal.
  */
  err = Pa_OpenStream(&m_stream,
                      &inputParameters,
                      nullptr, /* &outputParameters, No output in this case*/
                      SAMPLE_RATE,
                      FRAMES_PER_BUFFER,
                      paClipOff, /* we won't output out of range samples so
                                    don't bother clipping them */
                      recordCallback,
                      this);

  if (err != paNoError) {
    qWarning() << tr("unable to open input stream for microphone");
  }

  if (m_stream) {
    err = Pa_StartStream(m_stream);

    if (err != paNoError) {
      qWarning() << tr("Unable to start stream");
      return;
    }

    m_running = true;
  }
}

/*!
   \brief Stops and closes down the recorder.
*/
void
MicrophoneReader::stop()
{
  QMutexLocker locker(&m_mutex);
  m_running = false;
}

bool
MicrophoneReader::closedDown() const
{
  return !m_running;
}

/*

*/
void
MicrophoneReader::run()
{
  PaError err = paNoError;

  while (m_running) {
    while ((err = Pa_IsStreamActive(m_stream)) == 1) {
      Pa_Sleep(100);
    }

    if (err != paNoError) {
      Pa_Terminate();
      return;
    }
  }

  //  emit finished();
}

/*!
   \brief Custom method wrapper for the output signal which takes as a parameter
   a QVector<float>.
*/
void
MicrophoneReader::emitData(QVector<float> data)
{
  emit sendData(data);
}

} // end of namespace SpeechRecognition
