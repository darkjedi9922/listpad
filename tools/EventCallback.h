#include <QObject>
#include <QEvent>
#include <functional>

/**
 * Allows to setup a function via event filter on Qt events to run specified one-shot callback after the event
 * processing
 */
class EventCallback : public QObject
{
  Q_OBJECT

public:
  /**
   * Setups an event callback on a Qt object
   * @param target target qt object
   * @param event event type
   * @param function callback function
   */
  static void setup(QObject *target, QEvent::Type event, std::function<void(QEvent *)> const &function)
  {
    target->installEventFilter(new EventCallback(target, event, function));
  }

protected:
  bool eventFilter(QObject *obj, QEvent *event) override
  {
    auto result = QObject::eventFilter(obj, event);
    if (event->type() == this->event) {
      this->function(event);
      target->removeEventFilter(this);
    }
    return result;
  }

private:
  EventCallback(QObject *target, QEvent::Type event, std::function<void(QEvent *)> const &function) : QObject()
  {
    this->function = function;
    this->target = target;
    this->event = event;
  }

  std::function<void(QEvent *)> function;
  QEvent::Type event;
  QObject *target;
};