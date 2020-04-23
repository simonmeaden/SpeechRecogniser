#ifndef MICROPHONEPLOT_H
#define MICROPHONEPLOT_H

#include <QFrame>
#include <QTimer>
#include <QWidget>

#include "circularbuffer.h"
//#include "portaudiowrapper.h"

namespace SpeechRecognition {

/*!
  \class MicrophonePlot
  \brief The MicrophonePlot class is a display widget for microphone amplitude.

  The MicrophonePlot class allows the user to modify the sample format, sample
  rate and display length through it's own methods and store the result in an
  internal buffer.
*/
class MicrophonePlot : public QFrame
{
  Q_OBJECT

public:
  MicrophonePlot(/*double updateInterval,*/
                 int sampleRate,
                 int displayTime,
                 //                 SampleFormat format,
                 QWidget* parent = nullptr);

  void addData(QVector<float> data);

  void setSampleRate(int sampleRate);
  double displayTime() const;
  void setDisplayTime(double displayTime);

  //  void startSampling();
  //  void stopSampling();

  //  double updateInterval() const;
  //  void setUpdateInterval(double updateInterval);

  QBrush background() const;
  void setBackground(const QBrush& background);

  QColor lineColor() const;
  void setLineColor(const QColor& lineColor);

  QColor sampleColor() const;
  void setSampleColor(const QColor& sampleColor);

  double gridTime() const;
  void setGridTime(double gridTime);

protected:
  void paintEvent(QPaintEvent*) override;

private:
  QTimer m_updateTimer;
  //  double m_interval;
  int m_displayTime;
  int m_sampleRate;
  int m_bufferSize;
  double m_gridTime;
  int m_penWidth;
  CircularBuffer<float>* m_buffer;
  QBrush m_background;
  QColor m_lineColor;
  QColor m_sampleColor;

  void alignScales(QWidget* canvas);
  void updateDisplay();
};

} // end of namespace SpeechRecognition

#endif // MICROPHONEPLOT_H
