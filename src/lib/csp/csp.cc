#include "csp.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

void Channel::Write(int data) {
  std::unique_lock<std::mutex> ul(_mutex);
  _req = _handshake_phase;
  _data = data;
  _status = ChannelStatus::w_pend;
  ul.unlock();
  _cl.notify_all();
  ul.lock();

  _cl.wait(ul, [=]() { return _ack == _handshake_phase; });
  _status = ChannelStatus::idle;
  _handshake_phase = !_handshake_phase;
}

void Channel::Write() {
  std::unique_lock<std::mutex> ul(_mutex);
  _req = _handshake_phase;
  _status = ChannelStatus::w_pend;
  ul.unlock();
  _cl.notify_all();
  ul.lock();

  _cl.wait(ul, [=]() { return _ack == _handshake_phase; });
  _status = ChannelStatus::idle;
  _handshake_phase = !_handshake_phase;
}

int Channel::Read() {
  std::unique_lock<std::mutex> ul(_mutex);
  _status = ChannelStatus::r_pend;
  // if blocked, ul.unlock() is automatically called.
  _cl.wait(ul, [=]() { return _req == _handshake_phase; });
  // if unblocks, ul.lock() is automatically called
  int data = _data;

  if (_receive_counter == _number_of_receivers - 1) {
    _ack = _handshake_phase;
    _status = ChannelStatus::idle;
    _receive_counter = 0;
    ul.unlock();
    _cl.notify_all();
    ul.lock();
  } else {
    _status = ChannelStatus::s12m_pend;
    _receive_counter++;
    _cl.wait(ul, [=]() { return _receive_counter == 0; });
  }
  return data;
}
