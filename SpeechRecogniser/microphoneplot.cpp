
#include <QPainter>
#include <QWidget>
#include <qglobal.h>

#include "microphoneplot.h"

namespace SpeechRecognition {

/*!
  \brief Display plot for microphone amplitude.



  \param displayTime - the length of times to display in milliseconds.
  \param sampleRate - the sample rate in samples per second.
  \param parent - the parent QWidget.
*/
MicrophonePlot::MicrophonePlot(
  int sampleRate,
  int displayTime,
  //                               SampleFormat format,
  QWidget* parent)
  : QFrame(parent)
  //  , m_interval(interval)
  , m_displayTime(displayTime)
  , m_sampleRate(sampleRate)
  , m_bufferSize(int((qreal(displayTime) / qreal(1000)) * qreal(sampleRate)))
  , m_gridTime(200)
  , m_buffer(new CircularBuffer<float>(m_bufferSize))
  , m_background(QColor("white"))
  , m_lineColor(QColor("black"))
  , m_sampleColor(QColor("blue"))
{
  //  setFrameStyle(QFrame::Box);
  setMinimumSize(QSize(100, 100));
  //  setMaximumSize(QSize(1000, 100));
  m_updateTimer.start(50);
  connect(
    &m_updateTimer, &QTimer::timeout, this, &MicrophonePlot::updateDisplay);
}

/*!
  \brief Adds samples to the data set.

  The data supplied is added to the end of the data set.

  \param data - a QVector of double amplitude values.
*/
void
MicrophonePlot::addData(QVector<float> data)
{
  *m_buffer << data;
}

void
MicrophonePlot::setSampleRate(int sampleRate)
{
  if (sampleRate != m_sampleRate) {
    m_sampleRate = sampleRate;
    m_buffer->resize(int(m_displayTime * m_sampleRate));
  }
}

/*!
   \brief Returns the display time in seconds.
   \return the display time.
*/
double
MicrophonePlot::displayTime() const
{
  return m_displayTime;
}

/*!
   \brief Sets the display time in seconds.
   \param displayTime - the display time.
*/
void
MicrophonePlot::setDisplayTime(double displayTime)
{
  if (displayTime != m_displayTime) {
    m_displayTime = displayTime;
    m_buffer->resize(int(m_displayTime * m_sampleRate));
  }
}

void
MicrophonePlot::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);

  auto r = rect();
  int h = r.height();
  int h2 = int(h / 2);
  int w = r.width();
  qreal x_Width = qreal(w) / qreal(m_bufferSize);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(r, m_background);

  auto pen = painter.pen();

  // paint the grid lines.
  pen.setWidth(1);
  pen.setColor(m_lineColor);
  painter.setPen(pen);
  painter.drawLine(0, h2, w, h2);

  double gridSections = int((m_displayTime) / m_gridTime);
  int wSec = int(w / gridSections);
  int xSec;

  for (int i = 0; i < gridSections + 1; i++) {
    xSec = (i * wSec) + 1;
    painter.drawLine(xSec, 0, xSec, h);
  }

  // NOTE This assumes a maximum value of 1???
  painter.translate(0, h2);
  pen.setColor(m_sampleColor);
  painter.setPen(pen);
  qreal xPos = 0, yPos;
  QPainterPath path;
  yPos = h2 * m_buffer->get(0);
  path.moveTo(xPos, yPos);
  for (int i = 1; i < m_buffer->size(); i++) {
    yPos = h2 * m_buffer->get(i);
    xPos += x_Width;
    path.lineTo(xPos, yPos);
  }
  painter.drawPath(path);
}

/*!
   \brief Gets the time between vertical grid lines in milliseconds.. Defaults
   to 200mS. \return the time in milliseconds.
*/
double
MicrophonePlot::gridTime() const
{
  return m_gridTime;
}

/*!
   \brief Sets the time between vertical grid lines in milliseconds. Defaults to
   200mS. \param gridTime - the time in milliseconds.
*/
void
MicrophonePlot::setGridTime(double gridTime)
{
  m_gridTime = gridTime;
}

/*!
   \brief Gets the color of the amplitude line. Defaults to blue.
   \return a QColor object.
*/
QColor
MicrophonePlot::sampleColor() const
{
  return m_sampleColor;
}

/*!
   \brief Sets the color of the amplitude line.. Defaults to blue.
   \param sampleColor - a QColor object.
*/
void
MicrophonePlot::setSampleColor(const QColor& sampleColor)
{
  m_sampleColor = sampleColor;
}

/*!
   \brief Gets the color of the grid lines. Defaults to black.
   \return a QColor object.
*/
QColor
MicrophonePlot::lineColor() const
{
  return m_lineColor;
}

/*!
   \brief Sets the color of the grid lines. Defaults to black.
   \param sampleColor - a QColor object.
*/
void
MicrophonePlot::setLineColor(const QColor& lineColor)
{
  m_lineColor = lineColor;
}

/*!
   \brief Gets the color of the background.. Defaults to white.
   \return a QColor object.
*/
QBrush
MicrophonePlot::background() const
{
  return m_background;
}

/*!
   \brief Sets the color of the background. Defaults to white.
   \param sampleColor - a QColor object.
*/
void
MicrophonePlot::setBackground(const QBrush& background)
{
  m_background = background;
}

///*!
//     \brief Returns the update timer interval in milliseconds.
//     \return the update interval.
//*/
// double
// MicrophonePlot::updateInterval() const
//{
//  return m_interval;
//}

///*!
//   \brief Sets the update interval in milliseconds.

//   \param interval - the update interval.
//*/
// void
// MicrophonePlot::setUpdateInterval(double interval)
//{
//  if (interval != m_interval) {
//    m_interval = interval;

//    if (m_updateTimer.isActive()) {
//      m_updateTimer.stop();
//      m_updateTimer.start(m_interval);
//    }
//  }
//}

void
MicrophonePlot::alignScales(QWidget* /*canvas*/)
{}

void
MicrophonePlot::updateDisplay()
{
  update();
}

} // end of namespace SpeechRecognition
